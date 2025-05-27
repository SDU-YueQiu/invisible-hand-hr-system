<template>
  <div class="talent-container">
    <!-- 筛选控制区 -->
    <div class="control-header">
      <h3>职位申请人查询 (职位ID: {{ jobId }})</h3>
      <div class="control-actions">
        <el-select 
          v-model="filterStatus" 
          placeholder="申请状态" 
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
        <el-button type="primary" :icon="Refresh" @click="refreshData" />
      </div>
    </div>

    <!-- 申请人列表 -->
    <el-table 
      v-loading="loading"
      :data="applications"
      style="width: 100%"
      stripe
      highlight-current-row
    >
      <el-table-column prop="ApplicationID" label="申请ID" width="120" />
      
      <el-table-column label="申请人信息" width="220">
        <template #default="scope">
          <div class="applicant-info">
            <div class="user-id">用户ID: {{ scope.row.UserID }}</div>
            <div class="resume-id">简历ID: {{ scope.row.ResumeID }}</div>
          </div>
        </template>
      </el-table-column>

      <el-table-column label="申请时间" width="180">
        <template #default="scope">
          {{ formatDateTime(scope.row.ApplicationTime) }}
        </template>
      </el-table-column>

      <el-table-column label="当前状态" width="120">
        <template #default="scope">
          <el-tag :type="getStatusStyle(scope.row.CurrentStatus)">
            {{ getStatusText(scope.row.CurrentStatus) }}
          </el-tag>
        </template>
      </el-table-column>

      <el-table-column label="企业备注">
        <template #default="scope">
          <editable-notes 
            :notes="scope.row.EnterpriseNotes"
            @update="handleUpdateNotes(scope.row.ApplicationID, $event)"
          />
        </template>
      </el-table-column>

      <el-table-column label="操作" width="240">
        <template #default="scope">
          <div class="action-buttons">
            <el-button 
              size="small" 
              @click="viewResume(scope.row.ResumeID)"
            >
              查看简历
            </el-button>
            <status-actions 
              :status="scope.row.CurrentStatus"
              @change="handleStatusChange(scope.row.ApplicationID, $event)"
            />
          </div>
        </template>
      </el-table-column>
    </el-table>

    <!-- 分页 -->
    <div class="pagination" v-if="total > 0">
      <el-pagination
        v-model:current-page="currentPage"
        v-model:page-size="pageSize"
        :page-sizes="[10, 20, 50]"
        layout="total, sizes, prev, pager, next"
        :total="total"
        @size-change="handleSizeChange"
        @current-change="handleCurrentChange"
      />
    </div>

    <!-- 空状态 -->
    <el-empty v-if="!loading && applications.length === 0" description="暂无申请记录" />
  </div>
</template>

<script setup>
import { ref, watch, computed } from 'vue'
import { useRoute } from 'vue-router'
import { ElMessage } from 'element-plus'
import { Refresh } from '@element-plus/icons-vue'
import { useEnterpriseStore } from '../../stores'

const route = useRoute()

// 响应式数据
const jobId = computed(() => route.params.jobId)
const loading = ref(false)
const applications = ref([])
const total = ref(0)
const currentPage = ref(1)
const pageSize = ref(10)
const filterStatus = ref('')

// 状态选项配置
const statusOptions = [
  { label: '全部', value: '' },
  { label: '已申请', value: 'Submitted' },
  { label: '已查看', value: 'Viewed' },
  { label: '面试中', value: 'Interviewing' },
  { label: '已录用', value: 'Hired' },
  { label: '已拒绝', value: 'Rejected' }
]

// 监听职位ID变化
watch(jobId, (newVal) => {
  if (newVal) {
    resetFilters()
    fetchApplications()
  }
}, { immediate: true })

// 获取申请数据
const fetchApplications = async () => {
  try {
    loading.value = true
    const params = {
      page: currentPage.value,
      size: pageSize.value,
      status: filterStatus.value
    }

    const res = await request.get(`http://localhost:8080/api/v1/enterprises/me/applications`, { params })
    if (res.success) {
      applications.value = res.data.items
      total.value = res.data.total
    }
  } catch (error) {
    ElMessage.error('获取申请数据失败')
    console.error('Error fetching applications:', error)
  } finally {
    loading.value = false
  }
}

// 重置筛选条件
const resetFilters = () => {
  currentPage.value = 1
  filterStatus.value = ''
}

// 状态显示转换
const getStatusText = (status) => {
  const statusMap = {
    Submitted: '已申请',
    Viewed: '已查看',
    Interviewing: '面试中',
    Hired: '已录用',
    Rejected: '已拒绝'
  }
  return statusMap[status] || status
}

// 状态样式映射
const getStatusStyle = (status) => {
  const styleMap = {
    Submitted: 'info',
    Viewed: 'primary',
    Interviewing: 'warning',
    Hired: 'success',
    Rejected: 'danger'
  }
  return styleMap[status] || ''
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

// 筛选处理
const handleFilterChange = () => {
  currentPage.value = 1
  fetchApplications()
}

// 刷新数据
const refreshData = () => {
  resetFilters()
  fetchApplications()
}

// 查看简历
const viewResume = (resumeId) => {
  router.push(`http://localhost:8080/api/v1/enterprises/me/resumes/view${resumeId}`)
}

// 处理状态变更
const handleStatusChange = async (applicationId, newStatus) => {
  try {
    await request.put(`http://localhost:8080/api/v1/enterprises/me/applications/${applicationId}/status`, { status: newStatus })
    ElMessage.success('状态更新成功')
    fetchApplications()
  } catch (error) {
    ElMessage.error('状态更新失败')
  }
}

</script>

<style scoped>
.talent-container {
  padding: 20px;
  background: #fff;
  border-radius: 8px;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
}

.control-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20px;
  padding: 10px 0;
  border-bottom: 1px solid #eee;
}

.control-actions {
  display: flex;
  gap: 10px;
  align-items: center;
}

.applicant-info {
  font-size: 13px;
  line-height: 1.5;
}

.user-id {
  color: #666;
}

.resume-id {
  color: #999;
  font-size: 12px;
}

.action-buttons {
  display: flex;
  gap: 8px;
  flex-wrap: wrap;
}

.pagination {
  margin-top: 20px;
  display: flex;
  justify-content: flex-end;
}
</style>
