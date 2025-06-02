<template>
  <div>
    <h2>企业用户管理</h2>
    <el-table :data="enterprises">
      <el-table-column prop="id" label="ID" />
      <el-table-column prop="name" label="企业名称" />
      <el-table-column prop="status" label="审核状态" />
      <el-table-column label="操作">
        <template #default="{row}">
          <el-button 
            v-if="row.status === 'Pending'"
            type="success" 
            @click="approveEnterprise(row.id)"
          >批准</el-button>
          <el-button 
            v-if="row.status === 'Pending'"
            type="danger"
            @click="rejectEnterprise(row.id)"
          >拒绝</el-button>
        </template>
      </el-table-column>
    </el-table>

    <!-- 拒绝对话框 -->
    <el-dialog v-model="rejectDialogVisible" title="拒绝理由">
      <el-input v-model="rejectReason" placeholder="请输入拒绝理由" />
      <template #footer>
        <el-button @click="rejectDialogVisible = false">取消</el-button>
        <el-button type="primary" @click="confirmReject">确定</el-button>
      </template>
    </el-dialog>
  </div>
</template>

<script setup>
import { ref } from 'vue';

const enterprises = ref([]);
const rejectDialogVisible = ref(false);
const rejectReason = ref('');
const currentEnterpriseId = ref(null);

// 获取企业列表
const fetchEnterprises = async () => {
  const res = await axios.get('/admin/enterprises');
  enterprises.value = res.data;
};

// 批准企业
const approveEnterprise = async (id) => {
  await axios.put(`/admin/enterprises/${id}/approve`);
  ElMessage.success('企业已批准');
  fetchEnterprises();
};

// 拒绝企业（打开对话框）
const rejectEnterprise = (id) => {
  currentEnterpriseId.value = id;
  rejectDialogVisible.value = true;
};

// 确认拒绝
const confirmReject = async () => {
  await axios.put(`/admin/enterprises/${currentEnterpriseId.value}/reject`, {
    auditOpinion: rejectReason.value
  });
  rejectDialogVisible.value = false;
  ElMessage.success('企业已拒绝');
  fetchEnterprises();
};

onMounted(fetchEnterprises);
</script>