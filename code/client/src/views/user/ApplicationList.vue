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
          :data="processedApplications"
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
          
          <!-- Increased width for operations column -->
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
        
        <div class="pagination-container" v-if="processedApplications.length > 0">
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
        
        <el-empty v-if="processedApplications.length === 0 && !loading" description="暂无申请记录" />
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
const jobCache = ref({}) // 缓存职位信息
const resumeCache = ref({}) // 缓存简历信息
const totalItems = ref(0)
const currentPage = ref(1)
const pageSize = ref(10)
const filterStatus = ref('')
const activeTab = ref('all')

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

// 计算面试邀请列表
const interviewInvitations = computed(() => {
  return processedApplications.value.filter(app => app.currentStatus === 'InterviewInvited')
})

onMounted(() => {
  fetchApplications()
})

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
    
    console.log('API response:', res)
    
    if (res.success) {
      // 兼容不同的API返回结构
      applications.value = res.data?.applications || [];
      
      // 如果返回的数据格式与示例中的简单格式相符
      if (applications.value.length > 0 && applications.value[0].jobId && !applications.value[0].jobInfo) {
        // 需要获取更多职位和简历信息
        await fetchAdditionalInfo(applications.value);
      }
      
      totalItems.value = res.data?.pagination?.totalItems || applications.value.length;
    }
  } catch (error) {
    ElMessage.error('获取申请记录失败')
    console.error('Error fetching applications:', error)
  } finally {
    loading.value = false
  }
}

const parseResumeData = (data) => {
  // Make a copy to avoid modifying the original
  const processedData = { ...data };
  
  // Fields that should be parsed as JSON
  const jsonFields = [
    'basicInfo',
    'jobIntent',
    'educationExperience',
    'workExperience',
    'projectExperience',
    'skillsCertificates'
  ];
  
  // Process only JSON fields
  jsonFields.forEach(field => {
    if (processedData[field] && typeof processedData[field] === 'string') {
      try {
        // Only try to parse if it looks like JSON
        if (processedData[field].trim().startsWith('{') || processedData[field].trim().startsWith('[')) {
          processedData[field] = JSON.parse(processedData[field]);
        }
      } catch (e) {
        console.error(`Failed to parse ${field}:`, e);
        // Keep original value if parsing fails
      }
    }
  });
  
  return processedData;
}

// 获取额外的职位和简历信息
const fetchAdditionalInfo = async (applicationsList) => {
  // 提取所有需要获取详情的职位ID和简历ID
  const jobIds = [...new Set(applicationsList.map(app => app.jobId))];
  const resumeIds = [...new Set(applicationsList.map(app => app.resumeId))];
  
  try {
    // 逐个获取职位信息 (一次获取一个职位)
    if (jobIds.length > 0) {
      console.log('Fetching job details for IDs:', jobIds);
      
      // 对每个职位ID单独请求详细信息
      for (const jobId of jobIds) {
        const jobRes = await request.get(`/jobs/${jobId}`);
        console.log(`Job API response for ID ${jobId}:`, jobRes);
        
        if (jobRes.data && jobRes.data.message) {
          // 从正确的路径获取职位信息
          const jobData = jobRes.data.message;
          
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
          };
        }
      }
      
      console.log('Updated job cache:', jobCache.value);
    }
    
    // 批量获取简历信息
    if (resumeIds.length > 0) {
      console.log('Fetching resume details for IDs:', resumeIds);
      
      // 对每个简历ID单独请求
      for (const resumeId of resumeIds) {
        const resumeRes = await request.get(`/users/me/resumes/${resumeId}`);
        console.log(`Resume API response for ID ${resumeId}:`, resumeRes);
        
        if (resumeRes.success && resumeRes.data) {
          // 处理简历数据
          const resumeData = parseResumeData(resumeRes.data);
          
          // 更新简历缓存
          resumeCache.value[resumeId] = {
            resumeId: resumeId,
            resumeTitle: resumeData.resumeTitle || '未知简历',
            // 可以添加其他需要的简历信息字段
          };
        }
      }
      
      console.log('Updated resume cache:', resumeCache.value);
    }
  } catch (error) {
    console.error('Error fetching additional info:', error);
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

const viewJobDetail = (jobId) => {
  if (!jobId) {
    ElMessage.warning('无效的职位ID');
    return;
  }
  router.push(`/jobs/${jobId}`)
}

const viewResume = (resumeId) => {
  if (!resumeId) {
    ElMessage.warning('无效的简历ID');
    return;
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
    ElMessage.warning('无效的申请ID');
    return;
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
    ElMessage.warning('无效的申请ID');
    return;
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

/* Updated action buttons styles */
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
