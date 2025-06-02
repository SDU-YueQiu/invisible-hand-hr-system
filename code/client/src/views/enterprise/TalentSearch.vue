<template>
  <div class="resume-search-container">
    <div class="header">
      <h2>简历搜索</h2>
      <el-button type="primary" :icon="Refresh" @click="searchResumes">刷新</el-button>
    </div>

    <div class="search-form">
      <div class="form-row">
        <div class="form-item">
          <label>关键词搜索</label>
          <el-input
            v-model="searchParams.keyword"
            placeholder="姓名、职位、技能等"
            clearable
          ></el-input>
        </div>

        <div class="form-item">
          <label>技能要求</label>
          <el-select
            v-model="searchParams.skills"
            multiple
            filterable
            allow-create
            placeholder="请选择技能"
            style="width: 100%"
          >
            <el-option
              v-for="skill in commonSkills"
              :key="skill"
              :label="skill"
              :value="skill"
            ></el-option>
          </el-select>
        </div>

        <div class="form-item">
          <label>工作经验</label>
          <div style="display: flex; gap: 10px">
            <el-input-number
              v-model="searchParams.minExperience"
              :min="0"
              :max="50"
              placeholder="最小年限"
              controls-position="right"
            ></el-input-number>
            <span style="line-height: 32px">至</span>
            <el-input-number
              v-model="searchParams.maxExperience"
              :min="0"
              :max="50"
              placeholder="最大年限"
              controls-position="right"
            ></el-input-number>
          </div>
        </div>

        <div class="form-item">
          <label>学历要求</label>
          <el-select
            v-model="searchParams.education"
            placeholder="请选择学历"
            clearable
            style="width: 100%"
          >
            <el-option
              v-for="edu in educationOptions"
              :key="edu.value"
              :label="edu.label"
              :value="edu.value"
            ></el-option>
          </el-select>
        </div>

        <div class="form-item">
          <label>简历状态</label>
          <el-select
            v-model="searchParams.status"
            placeholder="请选择状态"
            clearable
            style="width: 100%"
          >
            <el-option
              v-for="status in statusOptions"
              :key="status.value"
              :label="status.label"
              :value="status.value"
            ></el-option>
          </el-select>
        </div>
      </div>

      <div class="actions">
        <el-button @click="resetSearch">重置</el-button>
        <el-button type="primary" @click="searchResumes">搜索</el-button>
      </div>
    </div>

    <div v-if="loading" class="loading-container">
      <el-icon size="24" class="is-loading"><Loading /></el-icon>
      <p style="margin-top: 10px">正在搜索简历...</p>
    </div>

    <div v-else>
      <div v-if="resumes.length > 0" class="result-stats">
        共搜索到 <strong>{{ totalItems }}</strong> 份符合条件的简历
      </div>

      <div v-for="resume in resumes" :key="resume.resumeId" class="resume-card">
        <div class="resume-header">
          <div class="resume-title">{{ resume.resumeTitle }}</div>
          <div class="resume-meta">最后更新: {{ formatDate(resume.lastUpdated) }}</div>
        </div>

        <div class="resume-content">
          <div class="resume-avatar">
            <el-icon size="24"><User /></el-icon>
          </div>

          <div class="resume-info">
            <div class="resume-info-row">
              <div class="info-label">姓名</div>
              <div class="info-value">{{ resume.name }}</div>
            </div>

            <div class="resume-info-row">
              <div class="info-label">工作经验</div>
              <div class="info-value">{{ resume.experience }} 年</div>
            </div>

            <div class="resume-info-row">
              <div class="info-label">学历</div>
              <div class="info-value">{{ resume.education }}</div>
            </div>

            <div class="resume-info-row">
              <div class="info-label">状态</div>
              <div class="info-value">
                <el-tag :type="getStatusStyle(resume.status)" size="small">
                  {{ getStatusText(resume.status) }}
                </el-tag>
              </div>
            </div>

            <div class="skills">
              <div class="skill-tag" v-for="skill in resume.skills" :key="skill">
                {{ skill }}
              </div>
            </div>
          </div>
        </div>
      </div>

      <div v-if="resumes.length === 0 && !loading" class="empty-container">
        <el-empty description="未找到符合条件的简历">
          <el-button type="primary" @click="resetSearch">重置搜索条件</el-button>
        </el-empty>
      </div>

      <div class="pagination-container" v-if="resumes.length > 0">
        <el-pagination
          v-model:current-page="currentPage"
          v-model:page-size="pageSize"
          :page-sizes="[10, 20, 30, 50]"
          layout="total, sizes, prev, pager, next, jumper"
          :total="totalItems"
          @size-change="handleSizeChange"
          @current-change="handleCurrentChange"
        ></el-pagination>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, computed, onMounted } from 'vue';
import { ElMessage, ElPagination, ElEmpty, ElTag, ElButton, ElInput, ElSelect, ElOption, ElInputNumber } from 'element-plus';
import { Refresh, Loading, User, ChatDotRound } from '@element-plus/icons-vue';
import axios from 'axios';
import { useEnterpriseStore } from '../../stores'


// 搜索参数
const searchParams = ref({
  keyword: '',
  skills: [],
  minExperience: null,
  maxExperience: null,
  education: '',
  status: ''
});

// 简历列表
const enterpriseStore = useEnterpriseStore()
const resumes = ref([]);
const loading = ref(false);
const totalItems = ref(0);
const currentPage = ref(1);
const pageSize = ref(10);

// 常用技能选项
const commonSkills = ref([
  'Java', 'Python', 'JavaScript', 'React', 'Vue.js',
  'Spring Boot', 'Node.js', 'MySQL', 'MongoDB', 'Docker',
  'Kubernetes', 'AWS', '微服务', 'Git', 'CI/CD'
]);

// 学历选项
const educationOptions = ref([
  { label: '不限', value: '' },
  { label: '高中', value: '高中' },
  { label: '大专', value: '大专' },
  { label: '本科', value: '本科' },
  { label: '硕士', value: '硕士' },
  { label: '博士', value: '博士' }
]);

// 状态选项
const statusOptions = ref([
  { label: '不限', value: '' },
  { label: '活跃', value: 'active' },
  { label: '已联系', value: 'contacted' },
  { label: '已归档', value: 'archived' }
]);

// 获取状态文本
const getStatusText = (status) => {
  const map = {
    'active': '活跃',
    'contacted': '已联系',
    'archived': '已归档'
  };
  return map[status] || status;
};

// 获取状态样式
const getStatusStyle = (status) => {
  const typeMap = {
    'active': 'success',
    'contacted': 'primary',
    'archived': 'info'
  };
  return typeMap[status] || '';
};

// 格式化日期
const formatDate = (dateString) => {
  if (!dateString) return '';
  const date = new Date(dateString);
  return date.toLocaleDateString('zh-CN', {
    year: 'numeric',
    month: '2-digit',
    day: '2-digit'
  });
};

// 搜索简历
const searchResumes = async () => {
  try {
    loading.value = true;
    const params = {
      ...searchParams.value,
      page: currentPage.value,
      pageSize: pageSize.value
    };
     const baseURL = "http://localhost:8080/api/v1"
    const res = await axios.get(
      `${baseURL}/enterprises/me/resumes/search`,{
      headers: {
        'Authorization': `Bearer ${enterpriseStore.token}`
      },params
      }
    )
    console.log("respone",res)
    if (res.status === 200 ) {
      resumes.value = res.data || [];
      totalItems.value = res.data.totalItems || resumes.value.length;
      console.log("resume",res.data )
    } else {
      ElMessage.error(res.message || '搜索失败');
    }
  } catch (error) {
    console.error('搜索简历失败:', error);
    ElMessage.error('搜索简历失败');
  } finally {
    loading.value = false;
  }
};

// 重置搜索条件
const resetSearch = () => {
  searchParams.value = {
    keyword: '',
    skills: [],
    minExperience: null,
    maxExperience: null,
    education: '',
    status: ''
  };
  currentPage.value = 1;
  searchResumes();
};

// 查看简历
const viewResume = (resumeId) => {
  router.push(`/user/resume/view/${resumeId}`)
};

// 分页大小变化
const handleSizeChange = (newSize) => {
  pageSize.value = newSize;
  searchResumes();
};

// 页码变化
const handleCurrentChange = (newPage) => {
  currentPage.value = newPage;
  searchResumes();
};

// 初始化加载
onMounted(() => {
  searchResumes();
});
</script>

<style scoped>
/* 注意添加 scoped 避免样式污染 */
.resume-search-container {
  max-width: 1200px;
  margin: 20px auto;
  padding: 20px;
  background: #fff;
  border-radius: 8px;
  box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
}

.header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20px;
  padding-bottom: 15px;
  border-bottom: 1px solid #ebeef5;
}

.header h2 {
  color: #303133;
  font-size: 20px;
  font-weight: 600;
}

.search-form {
  padding: 20px;
  background: #f8fafc;
  border-radius: 4px;
  margin-bottom: 20px;
}

.form-row {
  display: flex;
  flex-wrap: wrap;
  margin: 0 -10px;
}

.form-item {
  flex: 1;
  min-width: 200px;
  padding: 0 10px;
  margin-bottom: 15px;
}

.form-item label {
  display: block;
  margin-bottom: 8px;
  color: #606266;
  font-size: 14px;
}

.actions {
  display: flex;
  justify-content: flex-end;
  padding-top: 10px;
  gap: 10px;
}

.resume-card {
  margin-bottom: 15px;
  border: 1px solid #ebeef5;
  border-radius: 4px;
  padding: 15px;
  transition: all 0.3s;
}

.resume-card:hover {
  box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
  transform: translateY(-2px);
}

.resume-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 10px;
}

.resume-title {
  font-size: 16px;
  font-weight: 600;
  color: #303133;
}

.resume-meta {
  font-size: 12px;
  color: #909399;
}

.resume-content {
  display: flex;
  margin-top: 15px;
}

.resume-avatar {
  width: 80px;
  height: 80px;
  border-radius: 4px;
  background: #f0f2f5;
  display: flex;
  align-items: center;
  justify-content: center;
  color: #909399;
  margin-right: 15px;
  flex-shrink: 0;
}

.resume-info {
  flex: 1;
}

.resume-info-row {
  display: flex;
  margin-bottom: 8px;
}

.info-label {
  width: 80px;
  color: #909399;
  font-size: 13px;
}

.info-value {
  flex: 1;
  font-size: 13px;
}

.skills {
  display: flex;
  flex-wrap: wrap;
  margin-top: 10px;
}

.skill-tag {
  background-color: #ecf5ff;
  color: #409eff;
  border-radius: 4px;
  padding: 2px 8px;
  margin: 0 5px 5px 0;
  font-size: 12px;
}

.actions-container {
  display: flex;
  justify-content: flex-end;
  margin-top: 10px;
  gap: 8px;
}

.pagination-container {
  margin-top: 20px;
  display: flex;
  justify-content: center;
}

.empty-container {
  padding: 40px 0;
  text-align: center;
  color: #909399;
}

.loading-container {
  padding: 30px 0;
  text-align: center;
}

.result-stats {
  margin-bottom: 15px;
  color: #606266;
  font-size: 14px;
}

.result-stats strong {
  color: #409eff;
}
</style>    