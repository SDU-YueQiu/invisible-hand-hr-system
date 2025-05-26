<template>
  <div class="applications-container">
    <div class="applications-header">
      <h2>求职申请记录</h2>
      <div class="filter-actions">
        <el-select v-model="filterStatus" placeholder="筛选状态" clearable @change="handleFilterChange">
          <el-option
            v-for="status in statusOptions"
            :key="status.value"
            :label="status.label"
            :value="status.value"
          />
        </el-select>
        <el-button @click="refreshApplications" :icon="Refresh">刷新</el-button>
      </div>
    </div>
    
    <el-tabs v-model="activeTab" class="applications-tabs">
      <el-tab-pane label="所有申请" name="all">
        <el-table 
          v-loading="loading"
          :data="applications"
          style="width: 100%"
          stripe
        >
          <el-table-column prop="applicationTime" label="申请时间" width="180">
            <template #default="scope">
              {{ formatDate(scope.row.applicationTime) }}
            </template>
          </el-table-column>
          
          <el-table-column label="职位信息">
            <template #default="scope">
              <div class="job-info">
                <div class="job-title">{{ scope.row.jobInfo.jobTitle }}</div>
                <div class="company-name">{{ scope.row.jobInfo.enterpriseName }}</div>
              </div>
            </template>
          </el-table-column>
          
          <el-table-column prop="resumeInfo.resumeTitle" label="使用简历" width="180" />
          
          <el-table-column prop="currentStatus" label="状态" width="120">
            <template #default="scope">
              <el-tag :type="getStatusType(scope.row.currentStatus)">
                {{ getStatusText(scope.row.currentStatus) }}
              </el-tag>
            </template>
          </el-table-column>
          
          <el-table-column label="操作" width="180">
            <template #default="scope">
              <div class="action-buttons">
                <el-button type="primary" link @click="viewJobDetail(scope.row.jobInfo.jobId)">
                  查看职位
                </el-button>
                <el-button type="success" link @click="viewResume(scope.row.resumeInfo.resumeId)">
                  查看简历
                </el-button>
                <el-dropdown v-if="canCancelApplication(scope.row)">
                  <el-button type="info" link>
                    更多<el-icon class="el-icon--right"><arrow-down /></el-icon>
                  </el-button>
                  <template #dropdown>
                    <el-dropdown-menu>
                      <el-dropdown-item @click="cancelApplication(scope.row.applicationId)">
                        <el-icon color="#F56C6C"><close /></el-icon>
                        撤销申请
                      </el-dropdown-item>
                    </el-dropdown-menu>
                  </template>
                </el-dropdown>
              </div>
            </template>
          </el-table-column>
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
        
        <el-empty v-if="applications.length === 0 && !loading" description="暂无申请记录" />
      </el-tab-pane>
      
      <el-tab-pane label="面试邀请" name="interview">
        <div class="interview-list" v-if="interviewInvitations.length > 0">
          <el-card v-for="invite in interviewInvitations" :key="invite.applicationId" class="interview-card">
            <div class="interview-header">
              <div class="job-info">
                <div class="job-title">{{ invite.jobInfo.jobTitle }}</div>
                <div class="company-name">{{ invite.jobInfo.enterpriseName }}</div>
              </div>
              <div class="status-tag">
                <el-tag effect="dark" type="warning">面试邀请</el-tag>
              </div>
            </div>
            
            <div class="interview-body">
              <div class="interview-time">
                <el-icon><calendar /></el-icon>
                面试时间：{{ invite.interviewTime || '待定' }}
              </div>
              <div class="interview-location">
                <el-icon><location /></el-icon>
                面试地点：{{ invite.interviewLocation || '待定' }}
              </div>
              <div class="interview-contact">
                <el-icon><phone /></el-icon>
                联系人：{{ invite.contactPerson || '待定' }}
              </div>
              <div class="interview-note" v-if="invite.note">
                <el-icon><document /></el-icon>
                备注：{{ invite.note }}
              </div>
            </div>
            
            <div class="interview-footer">
              <el-button type="primary" @click="viewJobDetail(invite.jobInfo.jobId)">查看职位</el-button>
              <el-button type="info" @click="confirmInterview(invite.applicationId)">确认参加</el-button>
            </div>
          </el-card>
        </div>
        
        <el-empty v-if="interviewInvitations.length === 0 && !loading" description="暂无面试邀请" />
      </el-tab-pane>
    </el-tabs>
  </div>
</template>

<script setup>
import { ref, computed, onMounted } from 'vue'
import { useRouter } from 'vue-router'
import { ElMessage, ElMessageBox } from 'element-plus'
import { 
  Refresh, 
  ArrowDown, 
  Close, 
  Calendar, 
  Location, 
  Phone, 
  Document 
} from '@element-plus/icons-vue'
import request from '../../utils/request'

const router = useRouter()
const loading = ref(false)
const applications = ref([])
const totalItems = ref(0)
const currentPage = ref(1)
const pageSize = ref(10)
const filterStatus = ref('')
const activeTab = ref('all')

const statusOptions = [
  { label: '已投递', value: 'Submitted' },
  { label: '已查看', value: 'ViewedByHR' },
  { label: '面试邀请', value: 'InterviewInvited' },
  { label: '已面试', value: 'Interviewed' },
  { label: '录用', value: 'Hired' },
  { label: '未录用', value: 'Rejected' },
  { label: '已撤销', value: 'Cancelled' }
]

// 计算面试邀请列表
const interviewInvitations = computed(() => {
  return applications.value.filter(app => app.currentStatus === 'InterviewInvited')
})

onMounted(() => {
  fetchApplications()
})

const fetchApplications = async () => {
  try {
    loading.value = true
    
    const params = {
      page: currentPage.value,
      limit: pageSize.value
    }
    
    if (filterStatus.value) {
      params.status = filterStatus.value
    }
    
    const res = await request.get('/users/me/applications', { params })
    
    if (res.success) {
      applications.value = res.data.applications
      totalItems.value = res.data.pagination.totalItems
    }
  } catch (error) {
    ElMessage.error('获取申请记录失败')
    console.error(error)
  } finally {
    loading.value = false
  }
}

const refreshApplications = () => {
  fetchApplications()
}

const handleFilterChange = () => {
  currentPage.value = 1
  fetchApplications()
}

const handleSizeChange = (val) => {
  pageSize.value = val
  fetchApplications()
}

const handleCurrentChange = (val) => {
  currentPage.value = val
  fetchApplications()
}

const formatDate = (dateString) => {
  if (!dateString) return ''
  
  const date = new Date(dateString)
  return date.toLocaleDateString('zh-CN', {
    year: 'numeric',
    month: '2-digit',
    day: '2-digit',
    hour: '2-digit',
    minute: '2-digit'
  })
}

const getStatusText = (status) => {
  const statusMap = {
    'Submitted': '已投递',
    'ViewedByHR': '已查看',
    'InterviewInvited': '面试邀请',
    'Interviewed': '已面试',
    'Hired': '录用',
    'Rejected': '未录用',
    'Cancelled': '已撤销'
  }
  return statusMap[status] || status
}

const getStatusType = (status) => {
  const statusMap = {
    'Submitted': 'info',
    'ViewedByHR': 'success',
    'InterviewInvited': 'warning',
    'Interviewed': 'success',
    'Hired': 'danger',
    'Rejected': '',
    'Cancelled': 'info'
  }
  return statusMap[status] || ''
}

const viewJobDetail = (jobId) => {
  router.push(`/jobs/${jobId}`)
}

const viewResume = (resumeId) => {
  router.push(`/user/resume/view/${resumeId}`)
}

const canCancelApplication = (application) => {
  // 只有特定状态的申请可以撤销，例如已投递、已查看
  const cancelableStatus = ['Submitted', 'ViewedByHR']
  return cancelableStatus.includes(application.currentStatus)
}

const cancelApplication = (applicationId) => {
  ElMessageBox.confirm('确定要撤销该申请吗?', '提示', {
    confirmButtonText: '确定',
    cancelButtonText: '取消',
    type: 'warning'
  }).then(async () => {
    try {
      // 这里应该调用撤销申请的接口
      const res = await request.put(`/users/me/applications/${applicationId}/cancel`)
      
      if (res.success) {
        ElMessage.success('申请已成功撤销')
        refreshApplications()
      }
    } catch (error) {
      ElMessage.error('撤销申请失败')
      console.error(error)
    }
  }).catch(() => {})
}

const confirmInterview = (applicationId) => {
  ElMessageBox.confirm('确认参加此次面试吗?', '面试确认', {
    confirmButtonText: '确认参加',
    cancelButtonText: '取消',
    type: 'info'
  }).then(async () => {
    try {
      // 此处应该调用确认面试的接口
      const res = await request.put(`/users/me/applications/${applicationId}/confirm-interview`)
      
      if (res.success) {
        ElMessage.success('已确认参加面试')
        refreshApplications()
      }
    } catch (error) {
      ElMessage.error('确认失败')
      console.error(error)
    }
  }).catch(() => {})
}
</script>

<style scoped>
.applications-container {
  padding: 20px;
  background-color: white;
  border-radius: 4px;
  box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
}

.applications-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20px;
}

.filter-actions {
  display: flex;
  gap: 10px;
}

.applications-tabs {
  margin-top: 20px;
}

.job-info {
  display: flex;
  flex-direction: column;
}

.job-title {
  font-weight: bold;
  margin-bottom: 5px;
}

.company-name {
  font-size: 12px;
  color: #909399;
}

.action-buttons {
  display: flex;
  gap: 10px;
}

.pagination-container {
  margin-top: 20px;
  display: flex;
  justify-content: flex-end;
}

.interview-list {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(350px, 1fr));
  gap: 20px;
  margin-top: 20px;
}

.interview-card {
  margin-bottom: 0;
}

.interview-header {
  display: flex;
  justify-content: space-between;
  margin-bottom: 15px;
}

.status-tag {
  align-self: flex-start;
}

.interview-body {
  margin-bottom: 20px;
}

.interview-body > div {
  margin-bottom: 10px;
  display: flex;
  align-items: center;
  gap: 5px;
}

.interview-footer {
  display: flex;
  justify-content: flex-end;
  gap: 10px;
  margin-top: 15px;
}
</style>
