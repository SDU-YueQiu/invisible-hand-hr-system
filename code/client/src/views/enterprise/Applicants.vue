<template>
  <div class="application-container">
    <div class="header">
      <h2>申请记录管理</h2>
      <div class="filters">
        <el-select 
          v-model="filterStatus" 
          placeholder="筛选状态"
          clearable
          @change="handleFilterChange"
        >
          <el-option
            v-for="status in statusOptions"
            :key="status.value"
            :label="status.label"
            :value="status.value"
          />
        </el-select>
        <el-button 
          type="primary" 
          :icon="Refresh" 
          @click="refreshApplications"
        >
          刷新
        </el-button>
      </div>
    </div>

    <el-table
      v-loading="loading"
      :data="applications"
      style="width: 100%"
      stripe
      empty-text="暂无申请记录"
    >
      <el-table-column prop="ApplicationID" label="申请ID" width="120" />
      
      <el-table-column label="关联信息" width="220">
        <template #default="scope">
          <div class="relation-info">
            <div>用户ID: {{ scope.row.UserID }}</div>
            <div>企业ID: {{ scope.row.EnterpriseID }}</div>
            <div>职位ID: {{ scope.row.JobID }}</div>
          </div>
        </template>
      </el-table-column>

      <el-table-column label="时间信息" width="180">
        <template #default="scope">
          {{ formatDateTime(scope.row.ApplicationTime) }}
        </template>
      </el-table-column>

      <el-table-column label="状态" width="120">
        <template #default="scope">
          <el-tag :type="getStatusStyle(scope.row.CurrentStatus)">
            {{ getStatusText(scope.row.CurrentStatus) }}
          </el-tag>
        </template>
      </el-table-column>

      <el-table-column label="企业备注">
        <template #default="scope">
          <el-tooltip 
            v-if="scope.row.EnterpriseNotes" 
            :content="scope.row.EnterpriseNotes"
          >
            <span class="notes-preview">
              {{ previewNotes(scope.row.EnterpriseNotes) }}
            </span>
          </el-tooltip>
          <span v-else>--</span>
        </template>
      </el-table-column>

      <el-table-column label="操作" width="150">
        <template #default="scope">
          <el-dropdown>
            <el-button type="primary" link>
              操作 <el-icon><arrow-down /></el-icon>
            </el-button>
            <template #dropdown>
              <el-dropdown-menu>
                <el-dropdown-item @click="viewDetail(scope.row)">
                  <el-icon><View /></el-icon>
                  查看详情
                </el-dropdown-item>
                <el-dropdown-item 
                  v-if="canCancel(scope.row)" 
                  @click="cancelApplication(scope.row.ApplicationID)"
                >
                  <el-icon><Close /></el-icon>
                  撤销申请
                </el-dropdown-item>
                <el-dropdown-item 
                  divided 
                  @click="viewResume(scope.row.ResumeID)"
                >
                  <el-icon><Document /></el-icon>
                  查看简历
                </el-dropdown-item>
              </el-dropdown-menu>
            </template>
          </el-dropdown>
        </template>
      </el-table-column>
    </el-table>

    <div class="pagination" v-if="applications.length > 0">
      <el-pagination
        v-model:current-page="currentPage"
        v-model:page-size="pageSize"
        :page-sizes="[10, 20, 50, 100]"
        layout="total, sizes, prev, pager, next"
        :total="total"
        @size-change="handleSizeChange"
        @current-change="handleCurrentChange"
      />
    </div>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'
import { 
  Refresh, 
  ArrowDown, 
  View, 
  Close, 
  Document 
} from '@element-plus/icons-vue'
import { useEnterpriseStore } from '../../stores'

// 状态定义
const loading = ref(false)
const applications = ref([])
const total = ref(0)
const currentPage = ref(1)
const pageSize = ref(10)
const filterStatus = ref('')

// 状态选项配置
const statusOptions = [
  { label: '已提交', value: 'Submitted' },
  { label: '已受理', value: 'Accepted' },
  { label: '处理中', value: 'Processing' },
  { label: '已完成', value: 'Completed' },
  { label: '已取消', value: 'Cancelled' }
]

// 获取申请列表
const fetchApplications = async () => {
  try {
    loading.value = true
    const params = {
      page: currentPage.value,
      size: pageSize.value,
      status: filterStatus.value
    }

    const res = await request.get('http://localhost:8080/api/v1/enterprises/me/applicants', { params })
    if (res.success) {
      applications.value = res.data.items
      total.value = res.data.total
    }
  } catch (error) {
    console.error('获取申请记录失败:', error)
  } finally {
    loading.value = false
  }
}

// 状态显示转换
const getStatusText = (status) => {
  const map = {
    'Submitted': '已提交',
    'Accepted': '已受理',
    'Processing': '处理中',
    'Completed': '已完成',
    'Cancelled': '已取消'
  }
  return map[status] || status
}

// 状态样式映射
const getStatusStyle = (status) => {
  const typeMap = {
    'Submitted': 'info',
    'Accepted': 'primary',
    'Processing': 'warning',
    'Completed': 'success',
    'Cancelled': 'danger'
  }
  return typeMap[status] || ''
}

// 操作权限判断
const canCancel = (application) => {
  return ['Submitted', 'Accepted'].includes(application.CurrentStatus)
}

// 时间格式化
const formatDateTime = (datetime) => {
  return new Date(datetime).toLocaleString('zh-CN', {
    year: 'numeric',
    month: '2-digit',
    day: '2-digit',
    hour: '2-digit',
    minute: '2-digit'
  })
}

// 备注预览处理
const previewNotes = (notes) => {
  return notes.length > 15 ? notes.substring(0, 15) + '...' : notes
}

// 分页处理
const handleSizeChange = (val) => {
  pageSize.value = val
  fetchApplications()
}

const handleCurrentChange = (val) => {
  currentPage.value = val
  fetchApplications()
}

// 筛选条件变化
const handleFilterChange = () => {
  currentPage.value = 1
  fetchApplications()
}

// 刷新数据
const refreshApplications = () => {
  currentPage.value = 1
  fetchApplications()
}

// 业务操作
const viewJobDetail = (jobId) => {
  router.push(`/jobs/${jobId}`)
}

const viewResume = (resumeId) => {
  router.push(`http://localhost:8080/api/v1/enterprises/me/resumes/view${resumeId}`)
}

const cancelApplication = async (id) => {
  try {
    await request.put(`http://localhost:8080/api/v1/enterprises/me/applications/${applicationId}/cancel`)
    ElMessage.success('撤销申请成功')
    fetchApplications()
  } catch (error) {
    ElMessage.error('撤销申请失败')
  }
}
// 初始化加载
onMounted(() => {
  fetchApplications()
})
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
  align-items: center;
}

.relation-info {
  font-size: 13px;
  line-height: 1.5;
  color: #666;
}

.notes-preview {
  color: #888;
  cursor: help;
}

.pagination {
  margin-top: 20px;
  display: flex;
  justify-content: flex-end;
}
</style>