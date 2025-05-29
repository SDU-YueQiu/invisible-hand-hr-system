<!-- src/views/user/Dashboard.vue-->
<template>
  <div class="dashboard-container">
    <el-container>
      <el-aside width="200px" class="aside">
        <div class="logo">
          <h3>人才招聘系统</h3>
        </div>
        <el-menu 
          default-active="dashboard"
          class="el-menu-vertical"
          router
        >
          <el-menu-item index="/user/dashboard">
            <el-icon><icon-menu /></el-icon>
            <span>个人中心</span>
          </el-menu-item>
          <el-menu-item index="/user/profile">
            <el-icon><user /></el-icon>
            <span>个人资料</span>
          </el-menu-item>
          <el-menu-item index="/user/resumes">
            <el-icon><document /></el-icon>
            <span>我的简历</span>
          </el-menu-item>
          <el-menu-item index="/user/applications">
            <el-icon><connection /></el-icon>
            <span>申请记录</span>
          </el-menu-item>
          <el-menu-item index="/user/settings">
            <el-icon><setting /></el-icon>
            <span>账号设置</span>
          </el-menu-item>
        </el-menu>
        <div class="logout-btn">
          <el-button type="danger" plain @click="handleLogout">退出登录</el-button>
        </div>
      </el-aside>
      
      <el-container>
        <el-header height="60px" class="header">
          <div class="header-title">个人中心</div>
          <div class="user-info">
            <el-dropdown>
              <span class="el-dropdown-link">
                {{ username }}<el-icon class="el-icon--right"><arrow-down /></el-icon>
              </span>
              <template #dropdown>
                <el-dropdown-menu>
                  <el-dropdown-item @click="router.push('/user/profile')">个人资料</el-dropdown-item>
                  <el-dropdown-item @click="router.push('/user/settings')">账号设置</el-dropdown-item>
                  <el-dropdown-item divided @click="handleLogout">退出登录</el-dropdown-item>
                </el-dropdown-menu>
              </template>
            </el-dropdown>
          </div>
        </el-header>
        
        <el-main class="main">
          <div class="dashboard-summary">
            <el-row :gutter="20">
              <el-col :span="8">
                <el-card class="box-card">
                  <template #header>
                    <div class="card-header">
                      <span>简历统计</span>
                    </div>
                  </template>
                  <div class="card-content">
                    <div class="stat-number">{{ resumeCount }}</div>
                    <div class="stat-label">我的简历数</div>
                  </div>
                </el-card>
              </el-col>
              
              <el-col :span="8">
                <el-card class="box-card">
                  <template #header>
                    <div class="card-header">
                      <span>职位申请</span>
                    </div>
                  </template>
                  <div class="card-content">
                    <div class="stat-number">{{ applicationCount }}</div>
                    <div class="stat-label">申请的职位数</div>
                  </div>
                </el-card>
              </el-col>
              
              <el-col :span="8">
                <el-card class="box-card">
                  <template #header>
                    <div class="card-header">
                      <span>邀请面试</span>
                    </div>
                  </template>
                  <div class="card-content">
                    <div class="stat-number">{{ interviewCount }}</div>
                    <div class="stat-label">面试邀请数</div>
                  </div>
                </el-card>
              </el-col>
            </el-row>
          </div>
          
          <!-- 改进的申请记录部分 -->
          <div class="recent-applications">
            <div class="applications-header">
              <h3>最近申请</h3>
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
              <el-tab-pane label="最近申请" name="recent">
                <el-table 
                  v-loading="loading"
                  :data="recentApplications"
                  style="width: 100%"
                  stripe
                >
                  <el-table-column label="职位信息" min-width="220">
                    <template #default="scope">
                      <div class="job-info">
                        <div class="job-title">{{ getJobTitle(scope.row) }}</div>
                        <div class="company-name">{{ getCompanyName(scope.row) }}</div>
                      </div>
                    </template>
                  </el-table-column>
                  
                  <el-table-column label="使用简历" min-width="150" width="150">
                    <template #default="scope">
                      {{ getResumeTitle(scope.row) }}
                    </template>
                  </el-table-column>
                  
                  <el-table-column prop="currentStatus" label="状态" min-width="100" width="100">
                    <template #default="scope">
                      <el-tag :type="getStatusType(scope.row.currentStatus)">
                        {{ getStatusText(scope.row.currentStatus) }}
                      </el-tag>
                    </template>
                  </el-table-column>
                  
                  <el-table-column label="操作" min-width="220" width="220">
                    <template #default="scope">
                      <div class="action-buttons-container">
                        <el-button type="primary" link @click="viewJobDetail(scope.row.jobId)">
                          查看职位
                        </el-button>
                        <el-button type="success" link @click="viewResume(scope.row.resumeId)">
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
                
                <div class="view-more-link" v-if="recentApplications.length > 0">
                  <el-button type="primary" text @click="router.push('/user/applications')">
                    查看全部申请 <el-icon><arrow-right /></el-icon>
                  </el-button>
                </div>
                
                <el-empty v-if="recentApplications.length === 0 && !loading" description="暂无申请记录" />
              </el-tab-pane>
              
              <el-tab-pane label="面试邀请" name="interview">
                <div class="interview-list" v-if="interviewInvitations.length > 0">
                  <el-card v-for="invite in interviewInvitations" :key="invite.applicationId" class="interview-card">
                    <div class="interview-header">
                      <div class="job-info">
                        <div class="job-title">{{ getJobTitle(invite) }}</div>
                        <div class="company-name">{{ getCompanyName(invite) }}</div>
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
                      <el-button type="primary" @click="viewJobDetail(invite.jobId)">查看职位</el-button>
                      <el-button type="info" @click="confirmInterview(invite.applicationId)">确认参加</el-button>
                    </div>
                  </el-card>
                </div>
                
                <el-empty v-if="interviewInvitations.length === 0 && !loading" description="暂无面试邀请" />
              </el-tab-pane>
            </el-tabs>
          </div>
        </el-main>
      </el-container>
    </el-container>
  </div>
</template>

<script setup>
import { ref, computed, onMounted } from 'vue'
import { useRouter } from 'vue-router'
import { ElMessage, ElMessageBox } from 'element-plus'
import { 
  Menu as IconMenu, 
  User, 
  Document, 
  Connection, 
  Setting, 
  ArrowDown,
  ArrowRight,
  Refresh,
  Close,
  Calendar,
  Location,
  Phone
} from '@element-plus/icons-vue'
import { useLocalStorage } from '@vueuse/core'
import { useUserStore } from '../../stores'
import request from '../../utils/request'

const router = useRouter()
const userStore = useUserStore()

const token = useLocalStorage('token', '')
const userType = useLocalStorage('userType', '')

const userInfo = ref({})
const username = ref('')

// 申请相关数据
const loading = ref(false)
const applications = ref([])
const filterStatus = ref('')
const activeTab = ref('recent')
const jobCache = ref({}) // 缓存职位信息
const resumeCache = ref({}) // 缓存简历信息

// 统计数据
const resumeCount = ref(0)
const applicationCount = ref(0)
const interviewCount = ref(0)

const statusOptions = [
  { label: '待处理', value: 'Pending' },
  { label: '已投递', value: 'Submitted' },
  { label: '已查看', value: 'ViewedByHR' },
  { label: '面试邀请', value: 'InterviewInvited' },
  { label: '已面试', value: 'Interviewed' },
  { label: '录用', value: 'Hired' },
  { label: '未录用', value: 'Rejected' },
  { label: '已撤销', value: 'Cancelled' }
]

// 处理API返回的应用数据
const processedApplications = computed(() => {
  return applications.value.map(app => {
    // 确保所有必要字段都存在
    return {
      ...app,
      // 如果应用中没有这些信息，尝试从缓存中获取
      jobInfo: app.jobInfo || jobCache.value[app.jobId] || null,
      resumeInfo: app.resumeInfo || resumeCache.value[app.resumeId] || null
    }
  })
})

// 最近申请列表 - 只显示最近的5条
const recentApplications = computed(() => {
  return processedApplications.value.slice(0, 5)
})

// 计算面试邀请列表 - 只显示最近的3条
const interviewInvitations = computed(() => {
  return processedApplications.value
    .filter(app => app.currentStatus === 'InterviewInvited')
    .slice(0, 3)
})

onMounted(() => {
  fetchUserInfo()
  fetchApplications()
  fetchResumeCount()
})

const fetchUserInfo = async () => {
  try {
    if (!token.value) {
      ElMessage.warning('未登录')
      router.push('/login')
      return
    }
    
    const response = await request.get('/users/me')
    
    if (response.success) {
      userInfo.value = response.data
      username.value = userInfo.value.username
      
      // 更新 store
      userStore.setUser({
        token: token.value,
        userType: userType.value,
        userId: userInfo.value.userId,
        username: userInfo.value.username
      })
    } else {
      ElMessage.error(response.message || '获取用户信息失败')
    }
  } catch (error) {
    console.error('获取用户信息失败:', error)
    if (error.response?.status === 401) {
      ElMessage.error('登录已过期，请重新登录')
      // 清除token
      token.value = ''
      userType.value = ''
      router.push('/login')
    } else {
      ElMessage.error('获取用户信息失败')
    }
  }
}

const fetchApplications = async () => {
  try {
    loading.value = true
    
    const params = {
      page: 1,
      limit: 10
    }
    
    if (filterStatus.value) {
      params.status = filterStatus.value
    }
    
    const res = await request.get('/users/me/applications', { params })
    
    if (res.success) {
      // 兼容不同的API返回结构
      applications.value = res.data?.applications || []
      
      // 如果返回的数据格式与示例中的简单格式相符
      if (applications.value.length > 0 && applications.value[0].jobId && !applications.value[0].jobInfo) {
        // 需要获取更多职位和简历信息
        await fetchAdditionalInfo(applications.value)
      }
      
      // 更新统计数据
      applicationCount.value = res.data?.pagination?.totalItems || applications.value.length
      interviewCount.value = applications.value.filter(app => app.currentStatus === 'InterviewInvited').length
    }
  } catch (error) {
    ElMessage.error('获取申请记录失败')
    console.error('Error fetching applications:', error)
  } finally {
    loading.value = false
  }
}

const fetchResumeCount = async () => {
  try {
    const res = await request.get('/users/me/resumes')
    if (res.success) {
      // If the response data is an array, get its length
      // Otherwise, try to access the data from the expected structure
      if (Array.isArray(res.data)) {
        resumeCount.value = res.data.length
      } else if (res.data && Array.isArray(res.data.resumes)) {
        resumeCount.value = res.data.resumes.length
      } else {
        resumeCount.value = 0
      }
    } else {
      console.error('获取简历列表失败:', res.message)
      resumeCount.value = 0
    }
  } catch (error) {
    console.error('获取简历列表失败:', error)
    resumeCount.value = 0
  }
}


const parseResumeData = (data) => {
  // Make a copy to avoid modifying the original
  const processedData = { ...data }
  
  // Fields that should be parsed as JSON
  const jsonFields = [
    'basicInfo',
    'jobIntent',
    'educationExperience',
    'workExperience',
    'projectExperience',
    'skillsCertificates'
  ]
  
  // Process only JSON fields
  jsonFields.forEach(field => {
    if (processedData[field] && typeof processedData[field] === 'string') {
      try {
        // Only try to parse if it looks like JSON
        if (processedData[field].trim().startsWith('{') || processedData[field].trim().startsWith('[')) {
          processedData[field] = JSON.parse(processedData[field])
        }
      } catch (e) {
        console.error(`Failed to parse ${field}:`, e)
        // Keep original value if parsing fails
      }
    }
  })
  
  return processedData
}

// 获取额外的职位和简历信息
const fetchAdditionalInfo = async (applicationsList) => {
  // 提取所有需要获取详情的职位ID和简历ID
  const jobIds = [...new Set(applicationsList.map(app => app.jobId))]
  const resumeIds = [...new Set(applicationsList.map(app => app.resumeId))]
  
  try {
    // 逐个获取职位信息 (一次获取一个职位)
    if (jobIds.length > 0) {
      // 对每个职位ID单独请求详细信息
      for (const jobId of jobIds) {
        const jobRes = await request.get(`/jobs/${jobId}`)
        
        if (jobRes.data && jobRes.data.message) {
          // 从正确的路径获取职位信息
          const jobData = jobRes.data.message
          
          // 更新职位缓存
          jobCache.value[jobId] = {
            jobId: jobData.jobId,
            jobTitle: jobData.jobTitle,
            enterpriseName: jobData.enterpriseInfo?.enterpriseName || '未知企业',
            // 可以添加其他需要的职位信息字段
            workLocation: jobData.workLocation,
            minSalary: jobData.minSalary,
            maxSalary: jobData.maxSalary,
            jobCategory: jobData.jobCategory
          }
        }
      }
    }
    
    // 批量获取简历信息
    if (resumeIds.length > 0) {
      // 对每个简历ID单独请求
      for (const resumeId of resumeIds) {
        const resumeRes = await request.get(`/users/me/resumes/${resumeId}`)
        
        if (resumeRes.success && resumeRes.data) {
          // 处理简历数据
          const resumeData = parseResumeData(resumeRes.data)
          
          // 更新简历缓存
          resumeCache.value[resumeId] = {
            resumeId: resumeId,
            resumeTitle: resumeData.resumeTitle || '未知简历',
            // 可以添加其他需要的简历信息字段
          }
        }
      }
    }
  } catch (error) {
    console.error('Error fetching additional info:', error)
  }
}

// 获取职位标题，处理不同数据结构
const getJobTitle = (application) => {
  if (application.jobInfo?.jobTitle) {
    return application.jobInfo.jobTitle
  }
  
  // 尝试从缓存获取
  if (application.jobId && jobCache.value[application.jobId]?.jobTitle) {
    return jobCache.value[application.jobId].jobTitle
  }
  
  return '未知职位'
}

// 获取企业名称，处理不同数据结构
const getCompanyName = (application) => {
  if (application.jobInfo?.enterpriseName) {
    return application.jobInfo.enterpriseName
  }
  
  // 尝试从缓存获取
  if (application.jobId && jobCache.value[application.jobId]?.enterpriseName) {
    return jobCache.value[application.jobId].enterpriseName
  }
  
  return '未知企业'
}

// 获取简历标题，处理不同数据结构
const getResumeTitle = (application) => {
  if (application.resumeInfo?.resumeTitle) {
    return application.resumeInfo.resumeTitle
  }
  
  // 尝试从缓存获取
  if (application.resumeId && resumeCache.value[application.resumeId]?.resumeTitle) {
    return resumeCache.value[application.resumeId].resumeTitle
  }
  
  return '未知简历'
}

const getStatusText = (status) => {
  const statusMap = {
    'Pending': '待处理',
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
    'Pending': 'info',
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

const refreshApplications = () => {
  fetchApplications()
}

const handleFilterChange = () => {
  fetchApplications()
}

const viewJobDetail = (jobId) => {
  if (!jobId) {
    ElMessage.warning('无效的职位ID')
    return
  }
  router.push(`/jobs/${jobId}`)
}

const viewResume = (resumeId) => {
  if (!resumeId) {
    ElMessage.warning('无效的简历ID')
    return
  }
  router.push(`/user/resume/view/${resumeId}`)
}

const canCancelApplication = (application) => {
  // 只有特定状态的申请可以撤销，例如已投递、已查看、待处理
  const cancelableStatus = ['Submitted', 'ViewedByHR', 'Pending']
  return cancelableStatus.includes(application.currentStatus)
}

const cancelApplication = (applicationId) => {
  if (!applicationId) {
    ElMessage.warning('无效的申请ID')
    return
  }
  
  ElMessageBox.confirm('确定要撤销该申请吗?', '提示', {
    confirmButtonText: '确定',
    cancelButtonText: '取消',
    type: 'warning'
  }).then(async () => {
    try {
      const res = await request.put(`/users/me/applications/${applicationId}/cancel`)
      
      if (res.success) {
        ElMessage.success('申请已成功撤销')
        refreshApplications()
      } else {
        ElMessage.error(res.message || '撤销申请失败')
      }
    } catch (error) {
      ElMessage.error('撤销申请失败')
      console.error(error)
    }
  }).catch(() => {})
}

const confirmInterview = (applicationId) => {
  if (!applicationId) {
    ElMessage.warning('无效的申请ID')
    return
  }
  
  ElMessageBox.confirm('确认参加此次面试吗?', '面试确认', {
    confirmButtonText: '确认参加',
    cancelButtonText: '取消',
    type: 'info'
  }).then(async () => {
    try {
      const res = await request.put(`/users/me/applications/${applicationId}/confirm-interview`)
      
      if (res.success) {
        ElMessage.success('已确认参加面试')
        refreshApplications()
      } else {
        ElMessage.error(res.message || '确认失败')
      }
    } catch (error) {
      ElMessage.error('确认失败')
      console.error(error)
    }
  }).catch(() => {})
}

const handleLogout = () => {
  ElMessageBox.confirm('确定要退出登录吗?', '提示', {
    confirmButtonText: '确定',
    cancelButtonText: '取消',
    type: 'warning',
  }).then(async () => {
    try {
      // 清除登录状态 (useLocalStorage 响应式清除)
      token.value = ''
      userType.value = ''
      userStore.clearUser()
      
      const response = await request.post('/auth/logout')
      
      if (response.success) {
        router.push('/login')
        ElMessage.success('已成功退出登录')
      } else {
        ElMessage.error('退出登录失败，请稍后再试')
      }
    } catch (error) {
      console.error('退出登录失败:', error)
      ElMessage.error('退出登录失败，请稍后再试')
      
      // 即使API调用失败也跳转到登录页面
      router.push('/login')
    }
  }).catch(() => {})
}
</script>

<style scoped>
.dashboard-container {
  height: 100vh;
  display: flex;
}

.aside {
  background-color: #304156;
  color: white;
  height: 100vh;
  position: relative;
}

.logo {
  height: 60px;
  display: flex;
  justify-content: center;
  align-items: center;
  border-bottom: 1px solid #1f2d3d;
}

.logo h3 {
  color: #fff;
  margin: 0;
}

.el-menu-vertical {
  border-right: none;
}

.header {
  background-color: white;
  border-bottom: 1px solid #e6e6e6;
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.header-title {
  font-size: 18px;
  font-weight: bold;
}

.user-info {
  display: flex;
  align-items: center;
}

.el-dropdown-link {
  cursor: pointer;
  display: flex;
  align-items: center;
}

.main {
  background-color: #f0f2f5;
  padding: 20px;
  overflow-y: auto;
}

.dashboard-summary {
  margin-bottom: 30px;
}

.box-card {
  height: 100%;
}

.card-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.card-content {
  text-align: center;
  padding: 10px 0;
}

.stat-number {
  font-size: 28px;
  font-weight: bold;
  color: #409EFF;
  margin-bottom: 10px;
}

.stat-label {
  color: #909399;
}

/* 申请记录相关样式 */
.recent-applications {
  background-color: white;
  padding: 20px;
  border-radius: 4px;
  box-shadow: 0 2px 12px 0 rgba(0,0,0,0.1);
}

.applications-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20px;
}

.applications-header h3 {
  margin: 0;
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

/* Action buttons styles */
.action-buttons-container {
  display: flex;
  flex-wrap: wrap;
  gap: 8px;
  justify-content: flex-start;
}

.action-buttons-container .el-button {
  margin-left: 0;
  margin-right: 0;
  padding: 4px 8px;
}

.view-more-link {
  margin-top: 15px;
  text-align: center;
}

/* 面试卡片样式 */
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

.logout-btn {
  position: absolute;
  bottom: 20px;
  width: 100%;
  text-align: center;
}
</style>

