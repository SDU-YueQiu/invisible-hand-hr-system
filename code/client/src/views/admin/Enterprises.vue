<template>
  <div>
    <h2>企业用户管理</h2>
    <el-table :data="enterprises" v-loading="loading">
      <el-table-column prop="id" label="ID" />
      <el-table-column prop="name" label="企业名称" />
      <el-table-column prop="status" label="审核状态" />
      <el-table-column label="操作">
        <template #default="{ row }">
          <el-button 
            v-if="row.status === 'Pending' || row.status === 'Rejected'"
            type="success" 
            @click="approveEnterprise(row.id)"
            :loading="row.approving"
          >批准</el-button>
          <el-button 
            v-if="row.status === 'Pending' || row.status === 'Approved'"
            type="danger"
            @click="rejectEnterprise(row.id)"
            :loading="row.rejecting"
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
import { ref, onMounted } from 'vue';
import axios from 'axios';
import { useAdminStore } from '../../stores';
import { ElMessage } from 'element-plus';

const adminStore = useAdminStore();
const enterprises = ref([]);
const loading = ref(false);
const rejectDialogVisible = ref(false);
const rejectReason = ref('');
const currentEnterpriseId = ref(null);

// 修改数据映射逻辑
const mapStatus = (status) => {
  // 根据后端实际返回值进行映射
  const statusMap = {
    'Pending': 'Pending',
    'Reviewing': 'Pending',
    'Approved': 'Approved',
    'Rejected': 'Rejected',
    default: 'Pending' // 默认值
  };
  return statusMap[status] || statusMap.default;
};
// 获取企业列表
const fetchEnterprises = async () => {
  loading.value = true;
  try {
    const token = adminStore.token;
    const headers = { Authorization: `Bearer ${token}` };
    const baseURL = "http://frp-pet.com:36041/api/v1";

    const res = await axios.get(`${baseURL}/admin/enterprises`, { headers });
    
    if (res.data.success) {
      enterprises.value = res.data.data.enterprises.map(enterprise => ({
        ...enterprise,
        id: enterprise.enterpriseId,         
        name: enterprise.enterpriseName,     
        status: mapStatus(enterprise.accountStatus),
        approving: false,
        rejecting: false
      }));
    }
  } catch (error) {
    const errorMessage = error.response?.data?.message 
      || error.message 
      || '获取企业列表失败';
    
    ElMessage.error(errorMessage);
  } finally {
    loading.value = false;
  }
};
// 批准企业
const approveEnterprise = async (id) => {
  try {
    const token = adminStore.token;
    const headers = { Authorization: `Bearer ${token}` };
    const baseURL = "http://frp-pet.com:36041/api/v1";

    const enterprise = enterprises.value.find(e => e.id === id);
    if (!enterprise) return;

    enterprise.approving = true;

    const res = await axios.put(
      `${baseURL}/admin/enterprises/${id}/approve`, 
      {}, 
      { headers }
    );
    console.log("批准响应:", res);
    if (res.status === 200) {
      ElMessage.success('企业已批准');
      await fetchEnterprises();
    }
  } catch (error) {
    const errorMessage = error.response?.data?.message 
      || error.message 
      || '批准操作失败';
    
    ElMessage.error(errorMessage);
  } finally {
    const enterprise = enterprises.value.find(e => e.id === id);
    if (enterprise) enterprise.approving = false;
  }
};

// 拒绝企业（打开对话框）
const rejectEnterprise = (id) => {
  currentEnterpriseId.value = id;
  rejectDialogVisible.value = true;
};

// 确认拒绝
const confirmReject = async () => {
  try {
    const token = adminStore.token;
    const headers = { Authorization: `Bearer ${token}` };
    const baseURL = "http://frp-pet.com:36041/api/v1";

    const enterprise = enterprises.value.find(e => e.id === currentEnterpriseId.value);
    if (!enterprise) return;

    enterprise.rejecting = true;

    const res = await axios.put(
      `${baseURL}/admin/enterprises/${currentEnterpriseId.value}/reject`, 
      { auditOpinion: rejectReason.value },
      { headers }
    );

    if (res.status === 200) {
      rejectDialogVisible.value = false;
      ElMessage.success('企业已拒绝');
      await fetchEnterprises();
    }
  } catch (error) {
    const errorMessage = error.response?.data?.message 
      || error.message 
      || '拒绝操作失败';
    
    ElMessage.error(errorMessage);
  } finally {
    const enterprise = enterprises.value.find(e => e.id === currentEnterpriseId.value);
    if (enterprise) enterprise.rejecting = false;
  }
};

onMounted(fetchEnterprises);
</script>