<template>
  <div class="application-container">
    <div class="header">
      <h2>岗位申请管理</h2>
      <div class="filters">
        <el-select
          v-model="selectedJobTitle"
          placeholder="按岗位名称筛选"
          filterable
          clearable
          @change="handleJobSelect"
        >
          <el-option
            v-for="job in uniqueJobTitles"
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
        <el-table-column prop="applicationId" label="申请ID" width="100" />
        
        <el-table-column label="申请人信息" min-width="220">
          <template #default="scope">
            <div class="applicant-info">
              <div class="name">{{ scope.row.userName || '匿名用户' }}</div>
              <div class="resume">简历: {{ scope.row.resumeTitle }}</div>
              <div class="contact">联系方式: {{ scope.row.contactInfo || '未提供' }}</div>
            </div>
          </template>
        </el-table-column>

        <el-table-column label="申请时间" width="180">
          <template #default="scope">
            {{ formatDateTime(scope.row.applicationTime) }}
          </template>
        </el-table-column>

        <el-table-column label="状态" width="120">
          <template #default="scope">
            <el-tag :type="getStatusStyle(scope.row.status)">
              {{ getStatusText(scope.row.status) }}
            </el-tag>
          </template>
        </el-table-column>

        <el-table-column label="操作" width="220">
          <template #default="scope">
            <el-button-group>
              <el-tooltip content="查看完整简历" placement="top">
                <el-button
                  type="primary"
                  size="small"
                  :icon="Document"
                  @click="viewResume(scope.row.resumeId)"
                />
              </el-tooltip>
              
              <el-dropdown trigger="click">
                <el-button type="primary" size="small">
                  处理<el-icon class="el-icon--right"><arrow-down /></el-icon>
                </el-button>
                <template #dropdown>
                  <el-dropdown-menu>
                    <el-dropdown-item
                      @click="updateApplicationStatus(scope.row, 'Interviewing')"
                      :disabled="scope.row.status === 'Interviewing'"
                    >
                      <el-icon><ChatDotRound /></el-icon>
                      {{ scope.row.status === 'Interviewing' ? '已安排面试' : '安排面试' }}
                    </el-dropdown-item>
                    <el-dropdown-item
                      @click="updateApplicationStatus(scope.row, 'Rejected')"
                      :disabled="scope.row.status === 'Rejected'"
                    >
                      <el-icon><Close /></el-icon>
                      {{ scope.row.status === 'Rejected' ? '已拒绝' : '拒绝申请' }}
                    </el-dropdown-item>
                    <el-dropdown-item
                      @click="updateApplicationStatus(scope.row, 'Hired')"
                      :disabled="scope.row.status === 'Hired'"
                    >
                      <el-icon><CircleCheck /></el-icon>
                      {{ scope.row.status === 'Hired' ? '已录用' : '录用' }}
                    </el-dropdown-item>
                    <el-dropdown-item divided @click="showApplicationDetail(scope.row)">
                      <el-icon><View /></el-icon>
                      查看详情
                    </el-dropdown-item>
                  </el-dropdown-menu>
                </template>
              </el-dropdown>
            </el-button-group>
          </template>
        </el-table-column>
      </el-table>
    </div>

    <!-- 申请详情对话框 -->
    <el-dialog
      v-model="detailDialogVisible"
      :title="`申请详情 - ${selectedApplication?.userName || '匿名用户'}`"
      width="60%"
    >
      <div v-if="selectedApplication" class="application-detail">
        <el-descriptions :column="2" border>
          <el-descriptions-item label="申请ID">{{ selectedApplication.applicationId }}</el-descriptions-item>
          <el-descriptions-item label="申请时间">{{ formatDateTime(selectedApplication.applicationTime) }}</el-descriptions-item>
          <el-descriptions-item label="当前状态">
            <el-tag :type="getStatusStyle(selectedApplication.status)">
              {{ getStatusText(selectedApplication.status) }}
            </el-tag>
          </el-descriptions-item>
          <el-descriptions-item label="申请岗位">{{ selectedJobTitle }}</el-descriptions-item>
          <el-descriptions-item label="简历名称">{{ selectedApplication.resumeTitle }}</el-descriptions-item>
          <el-descriptions-item label="联系方式">{{ selectedApplication.contactInfo || '未提供' }}</el-descriptions-item>
        </el-descriptions>

        <div class="action-buttons">
          <el-button
            type="primary"
            @click="viewResume(selectedApplication.resumeId)"
          >
            查看完整简历
          </el-button>
        </div>
      </div>
    </el-dialog>
  </div>
</template>

<script setup>
import { ref, computed, onMounted } from 'vue'
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

const enterpriseStore = useEnterpriseStore()
const loading = ref(false)
const applications = ref([])
const jobList = ref([])
const selectedJobTitle = ref('')
const selectedJobId = ref('')
const currentJobInfo = ref(null)
const detailDialogVisible = ref(false)
const selectedApplication = ref(null)

// 计算去重后的岗位名称列表
const uniqueJobTitles = computed(() => {
  // 1. 确保数据是数组
  if (!Array.isArray(jobList.value)) {
    console.error('jobList 不是数组:', jobList.value)
    return []
  }
  
  // 2. 使用更安全的方法去重
  const titles = new Set()
  const result = []
  
  for (const job of jobList.value) {
    // 3. 验证对象结构
    if (!job || typeof job !== 'object' || !job.JobTitle || !job.jobId) {
      console.warn('无效的岗位数据:', job)
      continue
    }
    
    // 4. 去重逻辑
    if (!titles.has(job.JobTitle)) {
      titles.add(job.JobTitle)
      result.push({
        jobId: job.jobId,
        JobTitle: job.JobTitle
      })
    }
  }
  
  return result
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
onMounted(() => {
  fetchJobList()
})
// 处理岗位选择
const handleJobSelect = (jobId) => {
  selectedJobId.value = jobId
  const selectedJob = jobList.value.find(job => job.jobId === jobId)
  if (selectedJob) {
    selectedJobTitle.value = selectedJob.JobTitle
    currentJobInfo.value = selectedJob
    fetchApplications()
  }
}

// 获取特定岗位的申请记录
const fetchApplications = async () => {
  if (!selectedJobId.value) return
  
  try {
    loading.value = true
    const res = await axios.get(
      `/enterprises/me/jobs/${selectedJobId.value}/applicants`
    )
    console.log("response", res)
    if (res.status === 200) {
      applications.value = res.data
    }
  } catch (error) {
    console.error('获取申请记录失败:', error)
  } finally {
    loading.value = false
  }
}

// 更新申请状态
const updateApplicationStatus = async (applicationId, status) => {
  try {
    loading.value = true
    const res = await axios.put(
      `/enterprises/me/jobs/${selectedJobId.value}/applicants/${applicationId}/status`,
      { status }
    )
    if (res.status === 200) {
      fetchApplications() // 刷新列表
    }
  } catch (error) {
    console.error('更新状态失败:', error)
  }
}

// 查看简历
const viewResume = (resumeId) => {
  window.open(`/enterprises/me/resumes/${resumeId}`, '_blank')
}

// 显示申请详情
const showApplicationDetail = (application) => {
  selectedApplication.value = application
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