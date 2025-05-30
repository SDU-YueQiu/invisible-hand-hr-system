<template>
  <div class="job-container">
    <div class="job-header">
      <h2>岗位管理</h2>
      <el-button type="primary" @click="createNewJob">新建岗位</el-button>
    </div>
    
    <div v-if="loading" class="loading-container">
      <el-skeleton :rows="5" animated />
    </div>
    
    <div v-else-if="jobs.length === 0" class="empty-container">
      <el-empty description="尚未发布任何岗位">
        <el-button type="primary" @click="createNewJob">立即发布</el-button>
      </el-empty>
    </div>
    
    <div v-else class="job-list">
      <el-card v-for="job in jobs" :key="job.jobId" class="job-card">
        <div class="job-card-content">
          <div class="job-card-header">
            <h3 class="job-title">{{ job.jobTitle }}</h3>
            <el-tag :type="getStatusType(job.jobStatus)" effect="plain">
              {{ getStatusText(job.jobStatus) }}
            </el-tag>
          </div>
          
          <div class="job-meta">
            <div class="meta-item">
              <el-icon><Location /></el-icon>
              {{ job.workLocation }}
            </div>
            <div class="meta-item">
              <el-icon><Clock /></el-icon>
              截止：{{ formatDate(job.deadlineDate) }}
            </div>
            <div class="meta-item">
              <el-icon><Collection /></el-icon>
              {{ job.jobCategory }}
            </div>
          </div>
        </div>
        
        <div class="job-card-actions">
          <el-button type="primary" text @click="viewJob(job.jobId)">查看</el-button>
          <el-button type="success" text @click="editJob(job.jobId)">编辑</el-button>
          <el-dropdown>
            <span class="el-dropdown-link">
              <el-button type="info" text>更多<el-icon><ArrowDown /></el-icon></el-button>
            </span>
            <template #dropdown>
              <el-dropdown-menu>
                <el-dropdown-item @click="toggleJobStatus(job)">
                  {{ job.jobStatus === 'Recruiting' ? '关闭岗位' : '重新开放' }}
                </el-dropdown-item>
                <el-dropdown-item divided @click="confirmDelete(job.jobId)">删除岗位</el-dropdown-item>
              </el-dropdown-menu>
            </template>
          </el-dropdown>
        </div>
      </el-card>
    </div>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import { useRouter } from 'vue-router'
import { 
  Location,
  Clock,
  Collection,
  ArrowDown 
} from '@element-plus/icons-vue'
import { useEnterpriseStore } from '../../stores'
import axios from 'axios'

const enterpriseStore = useEnterpriseStore()
const router = useRouter()
const loading = ref(true)
const jobs = ref([])

onMounted(async () => {
  await fetchJobs()
})

const fetchJobs = async () => {
  try {
    loading.value = true
    const res = await axios.get('http://localhost:8080/api/v1/enterprises/me/jobs', {
      headers: {
        'Authorization': `Bearer ${enterpriseStore.token}`
      }
    })
    console.log("response", res)
    if (res.status === 200) {
      jobs.value = res.data.map(parseJobData) || []
    } else {
      ElMessage.error(res.data.message || '获取岗位列表失败')
    }
  } catch (error) {
    ElMessage.error('获取岗位列表失败')
    console.error(error)
  } finally {
    loading.value = false
  }
}

const parseJobData = (job) => {
  // 处理需要JSON解析的字段
  const jsonFields = ['Responsibilities', 'Requirements', 'Benefits']
  jsonFields.forEach(field => {
    if (job[field] && typeof job[field] === 'string') {
      try {
        job[field] = JSON.parse(job[field])
      } catch (e) {
        console.error(`解析字段 ${field} 失败:`, e)
      }
    }
  })
  return job
}

const formatDate = (dateString) => {
  if (!dateString) return '--'
  const date = new Date(dateString)
  return date.toLocaleDateString('zh-CN', {
    year: 'numeric',
    month: '2-digit',
    day: '2-digit'
  })
}

const getStatusText = (status) => {
  const statusMap = {
    'Recruiting': '招聘中',
    'Closed': '已关闭',
    'Pending': '审核中'
  }
  return statusMap[status] || status
}

const getStatusType = (status) => {
  const typeMap = {
    'Recruiting': 'success',
    'Closed': 'info',
    'Pending': 'warning'
  }
  return typeMap[status] || ''
}

const createNewJob = () => {
  router.push('/enterprise/job/create')
}

const viewJob = (jobId) => {
  router.push(`/jobs/${jobId}`)
}

const editJob = (jobId) => {
  router.push(`/enterprise/job/edit/${jobId}`)
}

const toggleJobStatus = async (job) => {
  try {
    const newStatus = job.jobStatus === 'Recruiting' ? 'Closed' : 'Recruiting'
    const res = await request.patch(`/enterprises/me/jobs/${job.jobId}`, {
      jobStatus: newStatus
    })
    
    if (res.success) {
      job.jobStatus = newStatus
      ElMessage.success(`岗位状态已更新为${getStatusText(newStatus)}`)
    } else {
      ElMessage.error(res.message || '更新岗位状态失败')
    }
  } catch (error) {
    ElMessage.error('更新岗位状态失败')
    console.error(error)
  }
}

const confirmDelete = (jobId) => {
  ElMessageBox.confirm('确定永久删除该岗位？此操作不可撤销', '警告', {
    confirmButtonText: '确认删除',
    cancelButtonText: '取消',
    type: 'warning',
    confirmButtonClass: 'el-button--danger'
  }).then(async () => {
    try {
      const token = enterpriseStore.token;
      const res = await axios.delete(`http://localhost:8080/api/v1/enterprises/me/jobs/${jobId}`,{
        headers: {
        Authorization: `Bearer ${token}`
      }
      });
      if (res.status === 200) {
        jobs.value = jobs.value.filter(job => job.jobId !== jobId)
        ElMessage.success('岗位已删除')
      } else {
        ElMessage.error(res.message || '删除失败')
      }
    } catch (error) {
      ElMessage.error('删除岗位失败')
      console.error(error)
    }
  }).catch(() => {})
}
</script>

<style scoped>
.job-container {
  padding: 24px;
  background: #fff;
  border-radius: 8px;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
}

.job-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 24px;
}

.job-list {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(320px, 1fr));
  gap: 16px;
}

.job-card {
  transition: box-shadow 0.2s;
  &:hover {
    box-shadow: 0 4px 12px rgba(0, 0, 0, 0.15);
  }
}

.job-card-content {
  padding: 16px;
}

.job-card-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 12px;
}

.job-title {
  margin: 0;
  font-size: 16px;
  color: #303133;
}

.job-meta {
  display: grid;
  gap: 8px;
  margin: 12px 0;
  color: #606266;
  font-size: 14px;
}

.meta-item {
  display: flex;
  align-items: center;
  gap: 6px;
}

.job-dates {
  display: flex;
  justify-content: space-between;
  font-size: 12px;
  color: #909399;
  margin-top: 12px;
}

.job-card-actions {
  border-top: 1px solid #ebeef5;
  padding-top: 12px;
  display: flex;
  justify-content: flex-end;
  gap: 8px;
}
</style>

