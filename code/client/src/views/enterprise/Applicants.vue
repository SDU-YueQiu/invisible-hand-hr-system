<template>
  <div class="application-container">
    <div class="header">
      <h2>岗位申请管理</h2>
      <div class="filters">
        <el-select
          v-model="selectedJobId"
          placeholder="按岗位名称筛选"
          filterable
          clearable
          @change="handleJobSelect"
        >
          <el-option
            v-for="job in jobList"
            :key="job.jobId"
            :label="job.jobTitle"
            :value="job.jobId"
          />
        </el-select>
        <el-button 
          type="primary" 
          :icon="Refresh" 
          @click="fetchApplications"
          :disabled="!selectedJobId"
        >
          刷新
        </el-button>
      </div>
    </div>

    <div v-if="!selectedJobId" class="select-job-prompt">
      <el-empty description="请先选择岗位"></el-empty>
    </div>

    <div v-else>
      <el-table
        v-loading="loading"
        :data="applications"
        style="width: 100%"
        stripe
        empty-text="该岗位暂无申请记录"
      >
        <el-table-column prop="applicationId" label="申请ID" width="200" />
        
        <el-table-column label="申请人信息" min-width="220">
          <template #default="scope">
            <div class="applicant-info">
              <!-- <div class="name">{{ scope.row.userName || '匿名用户' }}</div> -->
              <div class="name">用户Id: {{ scope.row.userId || 'else' }}</div>
              <div class="resume">简历: {{ scope.row.resumeTitle }}</div>
              <!-- <div class="contact">联系方式: {{ scope.row.contactInfo || '未提供' }}</div> -->
            </div>
          </template>
        </el-table-column>
        
        <!-- <el-table-column label="申请时间" width="180">
          <template #default="scope">
            {{ formatDateTime(scope.row.applicationTime) }}
          </template>
        </el-table-column> -->
        
      </el-table>
       <div class="pagination-container" v-if="applications.length > 0">
        <el-pagination
          v-model:current-page="currentPage"
          v-model:page-size="pageSize"
          :page-sizes="[10, 20, 30, 50]"
          layout="total, sizes, prev, pager, next, jumper"
          :total="totalItems"
          @size-change="handleSizeChange"
          @current-change="handleCurrentChange"
        />

      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, computed, onMounted } from 'vue'
import { useRouter } from 'vue-router'
import { ElMessage } from 'element-plus'
import { 
  Refresh, 
  ArrowDown, 
  Document,
  View,
  ChatDotRound,
  Close,
  CircleCheck
} from '@element-plus/icons-vue'
import { useEnterpriseStore } from '../../stores'
import axios from 'axios'

const router = useRouter()
const enterpriseStore = useEnterpriseStore()
const totalItems = ref(0)
const loading = ref(false)
const applications = ref([])
const jobList = ref([])
const selectedJobTitle = ref('')
const selectedJobId = ref('')
const currentJobInfo = ref(null)
const detailDialogVisible = ref(false)
const selectedApplication = ref(null)
const currentPage = ref(1)
const pageSize = ref(10)

const status= [
  { label: '全部', value: '' },
  { label: '已提交', value: 'Submitted' },
  { label: '已查看', value: 'Viewed' },
  { label: '面试中', value: 'Interviewing' },
  { label: '已拒绝', value: 'Rejected' },
  { label: '已录用', value: 'Hired' }
]



// 初始化加载岗位列表
onMounted(async () => {
  await fetchJobList()
})
// 获取企业所有岗位
const fetchJobList = async () => {
  try {
    loading.value = true
    const res = await axios.get('http://localhost:8080/api/v1/enterprises/me/jobs', {
      headers: {
        'Authorization': `Bearer ${enterpriseStore.token}`
      }
    })
    console.log("response", res)
    let data = []
    if (res.status === 200&& res.data) {
      if (Array.isArray(res.data)) {
        data = res.data
      } else if (Array.isArray(res.data.items)) {
        data = res.data.items
      } else if (Array.isArray(res.data.data)) {
        data = res.data.data
      } else {
        console.warn('API返回的数据结构不符合预期:', res.data)
      }
    }
    
    jobList.value = data
  } catch (error) {
    console.error('获取岗位列表失败:', error)
    jobList.value = [] // 确保出错时也是数组
    ElMessage.error('加载岗位列表失败')
  } finally {
    loading.value = false
  }
}
// 处理岗位选择
const handleJobSelect = (jobId) => {
  selectedJobId.value = jobId
  const selectedJob = jobList.value.find(job => job.jobId === jobId)
  if (selectedJob) {
    selectedJobTitle.value = selectedJob.JobTitle
    console.log("jobtiltle ", selectedJobTitle)
    currentJobInfo.value = selectedJob
    fetchApplications()
  }
}
onMounted(() => {

  fetchApplications()

})
// 获取特定岗位的申请记录
const fetchApplications = async () => {
  console.log("selectedJobId.value",selectedJobId.value)
  if (!selectedJobId.value) return
  
  try {
    loading.value = true
    const params = {
      page: currentPage.value,
      pageSize: pageSize.value
    }
    const baseURL = "http://localhost:8080/api/v1"
    const res = await axios.get(
      `${baseURL}/enterprises/me/jobs/${selectedJobId.value}/applicants`,{
      headers: {
        'Authorization': `Bearer ${enterpriseStore.token}`
      },params
      }
    )
    console.log("response", res)//.data.data.applicants)
    if (res.status === 200&& res.data && Array.isArray(res.data.data.applicants)) {
    //app = res.data.data.applicants;
    //app.userName = 
    applications.value = res.data.data.applicants;
      totalItems.value = res.data.totalItems || applications.value.length;
    } else {
      console.warn('API返回的数据结构不符合预期:', res.data);
      applications.value = []; // 确保赋值为空数组
    }
  } catch (error) {
    console.error('获取申请记录失败:', error)
  } finally {
    loading.value = false
  }
}

// 更新申请状态
const updateApplicationStatus = async (application, status) => {
  try {
    const res = await axios.put(
      `/api/v1/enterprises/me/jobs/${selectedJobId.value}/applicants/${application.applicationId}/status`,
      { status }
    );
    if (res.status === 200) {
      application.status = status;
      ElMessage.success('状态已更新');
    } else {
      ElMessage.error('更新状态失败');
    }
  } catch (error) {
    console.error('更新状态失败:', error);
    ElMessage.error('网络错误，请重试');
  }
};
const scheduleInterview = async (application) => {
  try {
    const res = await axios.post(
      `/api/v1/enterprises/me/jobs/${selectedJobId.value}/applicants/${application.applicationId}/schedule-interview`,
      {}
    );
    if (res.status === 200) {
      ElMessage.success('面试已安排');
      application.status = 'Interviewing';
    } else {
      ElMessage.error('安排面试失败');
    }
  } catch (error) {
    console.error('安排面试失败:', error);
    ElMessage.error('网络错误，请重试');
  }
};
// 查看简历
const viewResume = (resumeId) => {
  if (resumeId) {
    router.push(`/enterprise/resumes/${resumeId}`)
  } else {
    ElMessage.warning('简历ID无效')
  }
}
// 显示申请详情
const showApplicationDetail = (application) => {
  console.log(application)
  selectedApplication.value = application
  console .log("selectedApplication", selectedApplication)
  detailDialogVisible.value = true
}

// 辅助函数
const formatDateTime = (datetime) => {
  return new Date(datetime).toLocaleString('zh-CN', {
    year: 'numeric',
    month: '2-digit',
    day: '2-digit',
    hour: '2-digit',
    minute: '2-digit'
  })
}

const getStatusText = (status) => {
  const map = {
    'Submitted': '已提交',
    'Viewed': '已查看',
    'Interviewing': '面试中',
    'Rejected': '已拒绝',
    'Hired': '已录用'
  }
  return map[status] || status
}

const getStatusStyle = (status) => {
  const typeMap = {
    'Submitted': 'info',
    'Viewed': 'primary',
    'Interviewing': 'warning',
    'Rejected': 'danger',
    'Hired': 'success'
  }
  return typeMap[status] || ''
}
// 刷新申请列表

const refreshApplications = () => {
  fetchApplications()
}



// 处理筛选条件变化
const handleFilterChange = () => {
  currentPage.value = 1 // 重置为第一页
  fetchApplications()
}

// 处理分页大小变化
const handleSizeChange = (newSize) => {
  pageSize.value = newSize
  fetchApplications()
}

// 处理页码变化

const handleCurrentChange = (newPage) => {
  currentPage.value = newPage
  fetchApplications()
}
</script>

<style scoped>
.application-container {
  padding: 20px;
  background: #fff;
  border-radius: 4px;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
}

.header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20px;
}

.filters {
  display: flex;
  gap: 10px;
}

.select-job-prompt {
  margin-top: 50px;
  text-align: center;
}

.current-job-info {
  margin-bottom: 20px;
  padding: 15px;
  background-color: #f5f7fa;
  border-radius: 4px;
}

.current-job-info h3 {
  margin: 0 0 10px 0;
  color: #303133;
}

.applicant-info .name {
  font-weight: bold;
  margin-bottom: 5px;
}

.applicant-info .resume,
.applicant-info .contact {
  font-size: 13px;
  color: #606266;
  margin-top: 3px;
}

.application-detail {
  padding: 10px;
}

.action-buttons {
  margin-top: 20px;
  text-align: center;
}
</style>