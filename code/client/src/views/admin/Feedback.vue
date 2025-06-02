<template>
  <div>
    <h2>反馈管理</h2>
    
    <!-- 筛选工具栏 -->
    <div class="filter-bar">
      <el-select v-model="filterType" placeholder="反馈类型" clearable @change="fetchFeedbacks">
        <el-option label="建议" value="Suggestion" />
        <el-option label="Bug报告" value="BugReport" />
        <el-option label="投诉" value="Complaint" />
      </el-select>
      
      <el-select v-model="filterStatus" placeholder="处理状态" clearable @change="fetchFeedbacks">
        <el-option label="待处理" value="Pending" />
        <el-option label="处理中" value="Processing" />
        <el-option label="已解决" value="Resolved" />
      </el-select>
    </div>

    <!-- 反馈表格 -->
    <el-table :data="feedbacks" v-loading="loading" style="width: 100%">
      <el-table-column prop="id" label="ID" width="80" />
      <el-table-column prop="feedbackType" label="类型" width="120">
        <template #default="{row}">
          <el-tag :type="typeTagType(row.feedbackType)">
            {{ formatType(row.feedbackType) }}
          </el-tag>
        </template>
      </el-table-column>
      <el-table-column prop="content" label="反馈内容" min-width="200" />
      <el-table-column prop="contactInfo" label="联系方式" width="150" />
      <el-table-column label="处理状态" width="120">
        <template #default="{row}">
          <el-tag :type="statusTagType(row.processStatus)">
            {{ formatStatus(row.processStatus) }}
          </el-tag>
        </template>
      </el-table-column>
      <el-table-column prop="createTime" label="提交时间" width="180" />
      <el-table-column label="操作" width="150">
        <template #default="{row}">
          <el-button 
            size="small" 
            @click="showProcessDialog(row)"
            :disabled="row.processStatus === 'Resolved'"
          >
            处理
          </el-button>
        </template>
      </el-table-column>
    </el-table>

    <!-- 分页 -->
    <el-pagination
      v-model:current-page="currentPage"
      v-model:page-size="pageSize"
      :total="total"
      :page-sizes="[10, 20, 50]"
      layout="total, sizes, prev, pager, next, jumper"
      @size-change="fetchFeedbacks"
      @current-change="fetchFeedbacks"
    />

    <!-- 处理反馈对话框 -->
    <el-dialog 
      v-model="processDialogVisible" 
      :title="`处理反馈 #${currentFeedback.id}`" 
      width="600px"
    >
      <div class="feedback-content">
        <h4>反馈内容：</h4>
        <p>{{ currentFeedback.content }}</p>
        
        <template v-if="currentFeedback.contactInfo">
          <h4>联系方式：</h4>
          <p>{{ currentFeedback.contactInfo }}</p>
        </template>
      </div>

      <el-divider />

      <el-form label-width="80px">
        <el-form-item label="处理状态">
          <el-radio-group v-model="processForm.processStatus">
            <el-radio label="Processing">处理中</el-radio>
            <el-radio label="Resolved">已解决</el-radio>
          </el-radio-group>
        </el-form-item>
        
        <el-form-item label="管理员回复">
          <el-input 
            v-model="processForm.adminReply" 
            type="textarea" 
            :rows="4" 
            placeholder="请输入回复内容（可选）"
          />
        </el-form-item>
      </el-form>

      <template #footer>
        <el-button @click="processDialogVisible = false">取消</el-button>
        <el-button type="primary" @click="submitProcess">提交</el-button>
      </template>
    </el-dialog>
  </div>
</template>

<script setup>
import { ref, reactive, onMounted } from 'vue';
import axios from 'axios';
import { ElMessage } from 'element-plus';

// 数据状态
const feedbacks = ref([]);
const loading = ref(false);
const currentPage = ref(1);
const pageSize = ref(10);
const total = ref(0);
const filterType = ref('');
const filterStatus = ref('');

// 对话框相关
const processDialogVisible = ref(false);
const currentFeedback = reactive({
  id: null,
  content: '',
  contactInfo: ''
});
const processForm = reactive({
  processStatus: 'Processing',
  adminReply: ''
});

// 获取反馈列表
const fetchFeedbacks = async () => {
  loading.value = true;
  try {
    const res = await axios.get('/admin/feedbacks', {
      params: {
        type: filterType.value,
        status: filterStatus.value,
        page: currentPage.value,
        limit: pageSize.value
      }
    });
    feedbacks.value = res.data.data;
    total.value = res.data.pagination.total;
  } finally {
    loading.value = false;
  }
};

// 显示处理对话框
const showProcessDialog = (feedback) => {
  currentFeedback.id = feedback.id;
  currentFeedback.content = feedback.content;
  currentFeedback.contactInfo = feedback.contactInfo;
  
  processForm.processStatus = feedback.processStatus;
  processForm.adminReply = feedback.adminReply || '';
  
  processDialogVisible.value = true;
};

// 提交处理
const submitProcess = async () => {
  try {
    await axios.put(`/admin/feedbacks/${currentFeedback.id}`, processForm);
    ElMessage.success('反馈处理成功');
    processDialogVisible.value = false;
    fetchFeedbacks();
  } catch (error) {
    ElMessage.error('处理失败');
  }
};

// 格式化显示
const formatType = (type) => {
  const map = {
    'Suggestion': '建议',
    'BugReport': 'Bug报告',
    'Complaint': '投诉'
  };
  return map[type] || type;
};

const formatStatus = (status) => {
  const map = {
    'Pending': '待处理',
    'Processing': '处理中',
    'Resolved': '已解决'
  };
  return map[status] || status;
};

// 标签样式
const typeTagType = (type) => {
  switch (type) {
    case 'Suggestion': return '';
    case 'BugReport': return 'warning';
    case 'Complaint': return 'danger';
    default: return 'info';
  }
};

const statusTagType = (status) => {
  switch (status) {
    case 'Pending': return 'danger';
    case 'Processing': return 'warning';
    case 'Resolved': return 'success';
    default: return 'info';
  }
};

onMounted(fetchFeedbacks);
</script>

<style scoped>
.filter-bar {
  margin-bottom: 20px;
}
.filter-bar > * {
  margin-right: 10px;
  width: 150px;
}

.feedback-content {
  margin-bottom: 20px;
}
.feedback-content h4 {
  margin: 10px 0 5px;
  color: #666;
}
</style>