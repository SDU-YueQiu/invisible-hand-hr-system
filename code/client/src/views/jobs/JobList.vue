<template>
  <div class="job-list-container">
    <h1 class="page-title">职位列表</h1>
    
    <div class="filter-section">
      <el-form :inline="true" :model="filterForm" class="search-form">
        <el-form-item class="search-item">
          <el-input 
            v-model="filterForm.keyword" 
            placeholder="职位名称、公司、技能..." 
            prefix-icon="el-icon-search"
            @keyup.enter.native="loadJobs"
            class="keyword-input"
          ></el-input>
        </el-form-item>
        
        <el-form-item class="select-item">
          <el-select 
            v-model="filterForm.location" 
            placeholder="工作地点" 
            clearable
            class="filter-select"
          >
            <el-option label="北京" value="北京"></el-option>
            <el-option label="上海" value="上海"></el-option>
            <el-option label="广州" value="广州"></el-option>
            <el-option label="深圳" value="深圳"></el-option>
            <el-option label="杭州" value="杭州"></el-option>
          </el-select>
        </el-form-item>
        
        <el-form-item class="select-item">
          <el-select 
            v-model="filterForm.experience" 
            placeholder="工作经验" 
            clearable
            class="filter-select"
          >
            <el-option label="应届毕业生" value="应届毕业生"></el-option>
            <el-option label="1-3年" value="1-3年"></el-option>
            <el-option label="3-5年" value="3-5年"></el-option>
            <el-option label="5-10年" value="5-10年"></el-option>
            <el-option label="10年以上" value="10年以上"></el-option>
          </el-select>
        </el-form-item>
        
        <el-form-item class="select-item">
          <el-select 
            v-model="filterForm.education" 
            placeholder="学历要求" 
            clearable
            class="filter-select"
          >
            <el-option label="不限" value="不限"></el-option>
            <el-option label="大专" value="大专"></el-option>
            <el-option label="本科" value="本科"></el-option>
            <el-option label="硕士" value="硕士"></el-option>
            <el-option label="博士" value="博士"></el-option>
          </el-select>
        </el-form-item>
        
        <el-form-item class="button-item">
          <el-button type="primary" @click="loadJobs">搜索</el-button>
          <el-button @click="resetFilter">重置</el-button>
        </el-form-item>
      </el-form>
    </div>
    
    <div v-loading="loading" class="job-list">
      <el-card v-for="job in jobs" :key="job.jobId" class="job-card" @click.native="goToJobDetail(job.jobId)">
        <div class="job-card-header">
          <div class="job-title-wrapper">
            <h2 class="job-title">{{ job.jobTitle }}</h2>
            <span class="job-salary">{{ formatSalary(job.minSalary, job.maxSalary) }}</span>
          </div>
          <div class="company-info">
            <img v-if="job.logoURL" :src="job.logoURL" class="company-logo" alt="公司logo">
            <span class="company-name">{{ getEnterpriseName(job.enterpriseId) }}</span>
          </div>
        </div>
        
        <div class="job-card-body">
          <div class="job-tags">
            <el-tag size="small">{{ job.workLocation }}</el-tag>
            <el-tag size="small" type="info">{{ job.jobCategory }}</el-tag>
            <el-tag size="small" type="success" v-if="job.jobStatus === 'Recruiting'">招聘中</el-tag>
          </div>
          
          <div class="job-publish-date">
            发布时间: {{ formatDate(job.publishDate) }}
          </div>
        </div>
      </el-card>
      
      <div v-if="jobs.length === 0 && !loading" class="no-data">
        <el-empty description="暂无职位数据"></el-empty>
      </div>
    </div>
    
    <div class="pagination-container" v-if="pagination.totalItems > 0">
      <el-pagination
        background
        @current-change="handleCurrentChange"
        :current-page.sync="pagination.currentPage"
        :page-size="pagination.itemsPerPage"
        layout="prev, pager, next, jumper"
        :total="pagination.totalItems">
      </el-pagination>
    </div>
  </div>
</template>

<script setup>
import { ref, reactive, onMounted } from 'vue';
import { useRouter } from 'vue-router';
import { ElMessage } from 'element-plus';
import axios from 'axios';
import { useUserStore } from '../../stores'; // 根据你的store路径调整

const router = useRouter();
const userStore = useUserStore();

const jobs = ref([]);
const loading = ref(false);

const filterForm = reactive({
  keyword: '',
  location: '',
  experience: '',
  education: '',
  salaryMin: '',
  salaryMax: ''
});

const pagination = reactive({
  currentPage: 1,
  totalPages: 0,
  totalItems: 0,
  itemsPerPage: 10
});

const enterpriseNames = {
  1: '企业A',
  2: '企业B',
  3: '企业C',
  4: '企业D'
};

const loadJobs = async () => {
  try {
    loading.value = true;
    
    // 构建查询参数
    const params = {
      page: pagination.currentPage,
      limit: pagination.itemsPerPage,
      sortBy: 'publishDate',
      sortOrder: 'desc'
    };
    
    // 添加筛选条件
    if (filterForm.keyword) params.keyword = filterForm.keyword;
    if (filterForm.location) params.location = filterForm.location;
    if (filterForm.experience) params.experience = filterForm.experience;
    if (filterForm.education) params.education = filterForm.education;
    if (filterForm.salaryMin) params.salaryMin = filterForm.salaryMin;
    if (filterForm.salaryMax) params.salaryMax = filterForm.salaryMax;
    
    // 配置请求头
    const config = {
      params,
      headers: {}
    };
    
    // 如果有token，添加到请求头
    if (userStore.token) {
      config.headers['Authorization'] = `Bearer ${userStore.token}`;
    }
    
    const response = await axios.get('http://localhost:8080/api/v1/jobs', config);
    
    console.log('获取职位列表响应:', response);
    
    if (response.data) {
      // 处理不同的响应格式
      if (response.data.success && response.data.data) {
        jobs.value = Array.isArray(response.data.data.jobs) ? response.data.data.jobs : 
                     Array.isArray(response.data.data) ? response.data.data : [];
        
        if (response.data.data.pagination) {
          Object.assign(pagination, response.data.data.pagination);
        }
      } else if (response.data.message && Array.isArray(response.data.message)) {
        jobs.value = response.data.message;
        pagination.totalItems = jobs.value.length;
        pagination.totalPages = Math.ceil(jobs.value.length / pagination.itemsPerPage);
      } else {
        jobs.value = [];
      }
    }
  } catch (error) {
    console.error('获取职位列表出错:', error);
    
    if (error.response && error.response.status === 401) {
      ElMessage.error('未登录或登录已过期，请重新登录');
      userStore.clearUser();
      router.push('/login');
    } else {
      ElMessage.error('获取职位列表失败，请稍后重试');
    }
    
    jobs.value = [];
  } finally {
    loading.value = false;
  }
};

// 其他方法...
const resetFilter = () => {
  Object.assign(filterForm, {
    keyword: '',
    location: '',
    experience: '',
    education: '',
    salaryMin: '',
    salaryMax: ''
  });
  pagination.currentPage = 1;
  loadJobs();
};

const handleCurrentChange = (page) => {
  pagination.currentPage = page;
  loadJobs();
};

const goToJobDetail = (jobId) => {
  router.push({ name: 'JobDetail', params: { id: jobId } });
};

const formatDate = (dateString) => {
  if (!dateString) return '未知';
  try {
    const date = new Date(dateString);
    if (isNaN(date.getTime())) return '未知';
    return `${date.getFullYear()}-${(date.getMonth() + 1).toString().padStart(2, '0')}-${date.getDate().toString().padStart(2, '0')}`;
  } catch (e) {
    return '未知';
  }
};

const formatSalary = (min, max) => {
  if (!min && !max) return '薪资面议';
  
  const formatNumber = (num) => {
    if (!num) return '?';
    return num >= 10000 ? (num / 10000).toFixed(1) + 'w' : num + '';
  };
  
  return `${formatNumber(min)}-${formatNumber(max)}`;
};

const getEnterpriseName = (enterpriseId) => {
  return enterpriseNames[enterpriseId] || `企业${enterpriseId}`;
};

onMounted(() => {
  loadJobs();
});
</script>


<style scoped>
.job-list-container {
  max-width: 1200px;
  margin: 0 auto;
  padding: 20px;
}

.page-title {
  margin-bottom: 24px;
  color: #303133;
}

.filter-section {
  background-color: #fff;
  padding: 20px;
  border-radius: 4px;
  box-shadow: 0 2px 12px 0 rgba(0,0,0,0.1);
  margin-bottom: 20px;
}

.search-form {
  display: flex;
  flex-wrap: wrap;
  align-items: flex-start;
}

/* 调整搜索表单项宽度 */
.search-item {
  margin-right: 10px;
  margin-bottom: 10px;
}

.keyword-input {
  width: 280px;
}

.select-item {
  margin-right: 10px;
  margin-bottom: 10px;
}

/* 确保下拉框有足够宽度显示placeholder */
.filter-select {
  width: 130px;
}

.button-item {
  margin-bottom: 10px;
}

.job-list {
  margin-top: 20px;
  min-height: 200px;
}

.job-card {
  margin-bottom: 16px;
  transition: transform 0.3s;
  cursor: pointer;
}

.job-card:hover {
  transform: translateY(-5px);
  box-shadow: 0 5px 15px rgba(0,0,0,0.1);
}

.job-card-header {
  display: flex;
  justify-content: space-between;
  margin-bottom: 12px;
}

.job-title-wrapper {
  display: flex;
  align-items: center;
}

.job-title {
  margin: 0;
  font-size: 18px;
  color: #303133;
  margin-right: 10px;
}

.job-salary {
  color: #f56c6c;
  font-weight: bold;
}

.company-info {
  display: flex;
  align-items: center;
}

.company-logo {
  width: 30px;
  height: 30px;
  border-radius: 50%;
  margin-right: 8px;
  object-fit: cover;
}

.company-name {
  color: #606266;
}

.job-card-body {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.job-tags {
  display: flex;
  gap: 8px;
}

.job-publish-date {
  color: #909399;
  font-size: 14px;
}

.pagination-container {
  text-align: center;
  margin-top: 30px;
}

.no-data {
  margin-top: 40px;
  text-align: center;
}

/* 响应式调整 */
@media screen and (max-width: 768px) {
  .search-form {
    flex-direction: column;
  }
  
  .keyword-input,
  .filter-select {
    width: 100%;
  }
  
  .search-item,
  .select-item,
  .button-item {
    margin-right: 0;
    width: 100%;
  }
}
</style>


