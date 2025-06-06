<template>
  <div class="settings-container">
    <h2>管理员设置</h2>
    
    <el-tabs v-model="activeTab">
      <el-tab-pane label="修改密码" name="password">
        <el-form 
          :model="passwordForm" 
          :rules="passwordRules" 
          ref="passwordFormRef" 
          label-width="120px"
          class="settings-form"
        >
          <el-form-item label="当前密码" prop="oldPassword">
            <el-input 
              v-model="passwordForm.oldPassword" 
              type="password" 
              placeholder="请输入当前密码"
              show-password
            />
          </el-form-item>
          
          <el-form-item label="新密码" prop="newPassword">
            <el-input 
              v-model="passwordForm.newPassword" 
              type="password" 
              placeholder="请输入新密码（至少6位，包含字母和数字）"
              show-password
            />
          </el-form-item>
          
          <el-form-item label="确认新密码" prop="confirmPassword">
            <el-input 
              v-model="passwordForm.confirmPassword" 
              type="password" 
              placeholder="请再次输入新密码"
              show-password
            />
          </el-form-item>
          
          <el-form-item>
            <el-button 
              type="primary" 
              @click="submitPasswordForm(passwordFormRef)" 
              :loading="passwordLoading"
            >
              保存修改
            </el-button>
          </el-form-item>
        </el-form>
      </el-tab-pane>
      
      <el-tab-pane label="账号信息" name="security">
        <div class="security-info">
          <div class="info-item">
            <span class="label">管理员ID：</span>
            <span>{{ adminInfo.adminId }}</span>
          </div>
          <div class="info-item">
            <span class="label">用户名：</span>
            <span>{{ adminInfo.adminUsername }}</span>
          </div>
          <div class="info-item">
            <span class="label">注册时间：</span>
            <span>{{ formatDate(adminInfo.registrationDate) }}</span>
          </div>
          <div class="info-item">
            <span class="label">最近登录：</span>
            <span>{{ formatDate(adminInfo.lastLoginDate) }}</span>
          </div>
          <div class="info-item">
            <span class="label">账号状态：</span>
            <el-tag type="success">正常</el-tag>
          </div>
        </div>
        
        <div class="danger-zone">
          <h3>安全操作</h3>
          <el-button type="danger" @click="logout">退出登录</el-button>
        </div>
      </el-tab-pane>
    </el-tabs>
  </div>
</template>

<script setup>
import { ref, reactive, onMounted } from 'vue'
import { useRouter } from 'vue-router'
import { ElMessage, ElMessageBox } from 'element-plus'
import { useAdminStore } from '../../stores'
import axios from 'axios' // 新增axios导入

const router = useRouter()
const adminStore = useAdminStore()

// 当前激活的标签页
const activeTab = ref('password')

// 修改密码相关
const passwordFormRef = ref()
const passwordLoading = ref(false)

const passwordForm = reactive({
  oldPassword: '',
  newPassword: '',
  confirmPassword: ''
})

// ...（验证函数保持不变）

// 管理员信息相关
const adminInfo = reactive({
  adminId: '',
  adminUsername: '',
  registrationDate: '',
  lastLoginDate: '',
  accountStatus: 'Active'
})

// 在组件挂载时获取管理员信息
onMounted(async () => {
  await fetchAdminInfo()
})

// 获取管理员基本信息 - 改写为axios方式
const fetchAdminInfo = async () => {
  loading.value = true
  try {
    const token = adminStore.token
    const headers = { Authorization: `Bearer ${token}` }
    const baseURL = "http://frp-pet.com:36041/api/v1"
    
    const res = await axios.get(`${baseURL}/admin/me`, { headers })
    
    // 假设接口返回结构为 { success: true, data: { ... } }
    if (res.data.success) {
      Object.assign(adminInfo, res.data.data)
    }
  } catch (error) {
    console.error('获取管理员信息失败', error)
    ElMessage.error('获取管理员信息失败')
  } finally {
    loading.value = false
  }
}

// 修改密码 - 改写为axios方式
const submitPasswordForm = async (formEl) => {
  if (!formEl) return
  
  await formEl.validate(async (valid) => {
    if (valid) {
      try {
        passwordLoading.value = true
        
        const token = adminStore.token
        const headers = { Authorization: `Bearer ${token}` }
        const baseURL = "http://frp-pet.com:36041/api/v1"
        
        const res = await axios.put(
          `${baseURL}/admin/me/password`,
          {
            oldPassword: passwordForm.oldPassword,
            newPassword: passwordForm.newPassword
          },
          { headers }
        )
        
        if (res.data.success) {
          ElMessage.success('密码修改成功，请重新登录')
          
          // 清除登录状态
          localStorage.removeItem('adminToken')
          adminStore.clearAdmin()
          
          // 跳转到登录页
          router.push('/admin/login')
        }
      } catch (error) {
        console.error('密码修改失败', error)
        if (error.response?.data) {
          ElMessage.error(error.response.data.message || '密码修改失败')
        } else {
          ElMessage.error('密码修改失败，请检查当前密码是否正确')
        }
      } finally {
        passwordLoading.value = false
      }
    }
  })
}

// 退出登录 - 改写为axios方式
const logout = async () => {
  try {
    await ElMessageBox.confirm(
      '确认退出登录？',
      '提示',
      {
        confirmButtonText: '确认',
        cancelButtonText: '取消',
        type: 'warning'
      }
    )
    
    const token = adminStore.token
    const headers = { Authorization: `Bearer ${token}` }
    const baseURL = "http://frp-pet.com:36041/api/v1"
    
    await axios.post(`${baseURL}/auth/logout`, {}, { headers })
    
    // 清除本地状态
    localStorage.removeItem('adminToken')
    adminStore.clearAdmin()
    
    ElMessage.success('退出成功')
    router.push('/admin/login')
  } catch (error) {
    if (error !== 'cancel') {
      console.error('退出登录失败', error)
      ElMessage.error('退出登录失败')
    }
  }
}

// 格式化日期 - 保持不变
const formatDate = (dateString) => {
  if (!dateString) return '未知'
  const date = new Date(dateString)
  return date.toLocaleString('zh-CN', {
    year: 'numeric',
    month: '2-digit',
    day: '2-digit',
    hour: '2-digit',
    minute: '2-digit'
  })
}
</script>

<style scoped>
.settings-container {
  padding: 20px;
  background-color: white;
  border-radius: 4px;
  box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
}

.settings-form {
  max-width: 500px;
  margin-top: 20px;
}

.security-info {
  margin-bottom: 30px;
}

.info-item {
  margin-bottom: 15px;
  font-size: 14px;
}

.label {
  font-weight: bold;
  margin-right: 10px;
  color: #606266;
}

.danger-zone {
  margin-top: 40px;
  padding-top: 20px;
  border-top: 1px solid #EBEEF5;
}

.danger-zone h3 {
  color: #F56C6C;
  margin-bottom: 15px;
}
</style>