<template>
  <div>
    <h2>职位管理</h2>
    <el-table :data="jobs" v-loading="loading">
      <el-table-column prop="id" label="ID" width="80" />
      <el-table-column prop="title" label="职位名称" />
      <el-table-column prop="enterpriseName" label="所属企业" />
      <el-table-column label="状态">
        <template #default="{row}">
          <el-tag :type="statusTagType(row.status)">
            {{ row.status }}
          </el-tag>
        </template>
      </el-table-column>
      <el-table-column label="操作" width="220">
        <template #default="{row}">
          <el-select 
            v-model="row.status" 
            @change="updateJobStatus(row.id, row.status)"
            placeholder="更改状态"
            style="width: 160px"
          >
            <el-option label="招聘中" value="Recruiting" />
            <el-option label="已下线" value="Offline" />
            <el-option label="待审核" value="PendingReview" />
          </el-select>
        </template>
      </el-table-column>
    </el-table>

    <el-pagination
      :current-page="currentPage"
      :page-size="pageSize"
      :total="total"
      @current-change="handlePageChange"
      layout="prev, pager, next"
      class="pagination"
    />

    <!-- 审核意见对话框 -->
    <el-dialog v-model="auditDialogVisible" title="审核意见" width="500px">
      <el-input 
        v-model="auditOpinion" 
        placeholder="请输入审核意见（可选）" 
        type="textarea" 
        rows="4"
      />
      <template #footer>
        <el-button @click="auditDialogVisible = false">取消</el-button>
        <el-button type="primary" @click="confirmStatusUpdate">确定</el-button>
      </template>
    </el-dialog>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue';
import axios from 'axios';
import { ElMessage } from 'element-plus';

const jobs = ref([]);
const loading = ref(false);
const currentPage = ref(1);
const pageSize = ref(10);
const total = ref(0);
const auditDialogVisible = ref(false);
const auditOpinion = ref('');
const currentJobId = ref(null);
const newStatus = ref('');

// 获取职位列表
const fetchJobs = async () => {
  loading.value = true;
  try {
    const res = await axios.get('/admin/jobs', {
      params: { 
        page: currentPage.value,
        limit: pageSize.value 
      }
    });
    jobs.value = res.data.data;
    total.value = res.data.pagination.total;
  } finally {
    loading.value = false;
  }
};

// 状态标签样式
const statusTagType = (status) => {
  switch (status) {
    case 'Recruiting': return 'success';
    case 'Offline': return 'info';
    case 'PendingReview': return 'warning';
    default: return '';
  }
};

// 更新职位状态
const updateJobStatus = (jobId, status) => {
  currentJobId.value = jobId;
  newStatus.value = status;
  
  // 如果需要审核意见（如设置为PendingReview）
  if (status === 'PendingReview') {
    auditDialogVisible.value = true;
  } else {
    confirmStatusUpdate();
  }
};

// 确认状态更新
const confirmStatusUpdate = async () => {
  try {
    await axios.put(`/admin/jobs/${currentJobId.value}/status`, {
      newStatus: newStatus.value,
      auditOpinion: auditOpinion.value
    });
    ElMessage.success('职位状态更新成功');
    fetchJobs();
    auditDialogVisible.value = false;
    auditOpinion.value = '';
  } catch (error) {
    ElMessage.error('更新失败');
  }
};

// 分页处理
const handlePageChange = (page) => {
  currentPage.value = page;
  fetchJobs();
};

onMounted(fetchJobs);
</script>

<style scoped>
.pagination {
  margin-top: 20px;
  justify-content: center;
}
</style>