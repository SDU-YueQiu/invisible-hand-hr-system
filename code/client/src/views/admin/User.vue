<template>
  <div>
    <h2>个人用户管理</h2>
    <el-table :data="users" v-loading="loading">
      <el-table-column prop="id" label="ID" width="80" />
      <el-table-column prop="username" label="用户名" />
      <el-table-column label="状态">
        <template #default="{row}">
          <el-tag :type="row.status === 'Active' ? 'success' : 'danger'">
            {{ row.status }}
          </el-tag>
        </template>
      </el-table-column>
      <el-table-column label="操作">
        <template #default="{row}">
          <el-select 
            v-model="row.status" 
            @change="updateUserStatus(row.id, row.status)"
            placeholder="更改状态"
          >
            <el-option label="启用" value="Active" />
            <el-option label="禁用" value="Disabled" />
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
    />
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue';
import axios from 'axios';

const users = ref([]);
const loading = ref(false);
const currentPage = ref(1);
const pageSize = ref(10);
const total = ref(0);

// 获取用户列表
const fetchUsers = async () => {
  loading.value = true;
  try {
    const res = await axios.get('/admin/users', {
      params: { 
        page: currentPage.value,
        limit: pageSize.value 
      }
    });
    users.value = res.data.data;
    total.value = res.data.pagination.total;
  } finally {
    loading.value = false;
  }
};

// 更新用户状态
const updateUserStatus = async (userId, newStatus) => {
  try {
    await axios.put(`/admin/users/${userId}/status`, {
      newStatus
    });
    ElMessage.success('状态更新成功');
  } catch (error) {
    ElMessage.error('更新失败');
  }
};

onMounted(fetchUsers);
</script>