<!-- src/views/public/Login.vue -->
<template>
  <div class="login-container">
    <div class="login-box">
      <h2>用户登录</h2>
      
      <el-tabs v-model="activeTab">
        <el-tab-pane label="个人用户" name="individual">
          <el-form :model="individualForm" label-position="top">
            <el-form-item label="用户名">
              <el-input v-model="individualForm.username" placeholder="用户名/手机号/邮箱" />
            </el-form-item>
            
            <el-form-item label="密码">
              <el-input 
                v-model="individualForm.password" 
                type="password" 
                placeholder="请输入密码"
                show-password
              />
            </el-form-item>
            
            <el-form-item>
              <el-button 
                type="primary" 
                :loading="loading" 
                @click="handleIndividualLogin" 
                style="width: 100%"
              >
                登录
              </el-button>
            </el-form-item>
          </el-form>
          
          <div class="form-footer">
            <router-link to="/register">没有账号？立即注册</router-link>
          </div>
        </el-tab-pane>
        
        <el-tab-pane label="企业用户" name="enterprise">
          <el-form :model="enterpriseForm" label-position="top">
            <el-form-item label="企业账号">
              <el-input v-model="enterpriseForm.loginUsername" placeholder="请输入企业账号" />
            </el-form-item>
            
            <el-form-item label="密码">
              <el-input 
                v-model="enterpriseForm.password" 
                type="password" 
                placeholder="请输入密码"
                show-password
              />
            </el-form-item>
            
            <el-form-item>
              <el-button 
                type="primary" 
                :loading="loading" 
                @click="handleEnterpriseLogin" 
                style="width: 100%"
              >
                登录
              </el-button>
            </el-form-item>
          </el-form>
          
          <div class="form-footer">
            <router-link to="/register/enterprise">企业注册</router-link>
          </div>
        </el-tab-pane>
        
        <el-tab-pane label="管理员" name="admin">
          <el-form :model="adminForm" label-position="top">
            <el-form-item label="管理员账号">
              <el-input v-model="adminForm.adminUsername" placeholder="请输入管理员账号" />
            </el-form-item>
            
            <el-form-item label="密码">
              <el-input 
                v-model="adminForm.password" 
                type="password" 
                placeholder="请输入密码"
                show-password
              />
            </el-form-item>
            
            <el-form-item>
              <el-button 
                type="primary" 
                :loading="loading" 
                @click="handleAdminLogin" 
                style="width: 100%"
              >
                登录
              </el-button>
            </el-form-item>
          </el-form>
        </el-tab-pane>
      </el-tabs>
    </div>
  </div>
</template>

<script setup>
import { ref, reactive } from 'vue'
import { useRouter } from 'vue-router'
import { ElMessage } from 'element-plus'
import { useUserStore } from '../../stores'
import { useLocalStorage } from '@vueuse/core'
import axios from 'axios'

const router = useRouter()
const userStore = useUserStore()

const loading = ref(false)
const activeTab = ref('individual')

const individualForm = reactive({
  username: '',
  password: ''
})

const enterpriseForm = reactive({
  loginUsername: '',
  password: ''
})

const adminForm = reactive({
  adminUsername: '',
  password: ''
})

const handleIndividualLogin = async () => {
  if (!individualForm.username || !individualForm.password) {
    ElMessage.error('请输入用户名和密码')
    return
  }
  loading.value = true
  // 使用 useLocalStorage
  const token = useLocalStorage('token', '')
  const userType = useLocalStorage('userType', '')
  
  const loginData = {
    username: individualForm.username,
    password: individualForm.password
  };
  
  try {
    const baseURL = 'http://localhost:8080/api/v1';
    const response = await axios.post(`${baseURL}/auth/individual/login`, loginData);
    
    if (response.status === 200) {
      // 从响应中获取数据
      const { token: responseToken, userId } = response.data;
      
      // 保存到 localStorage (响应式)
      token.value = responseToken
      userType.value = 'individual'
      
      // 更新 store
      userStore.setUser({
        token: responseToken,
        userType: 'individual',
        userId,
        username: individualForm.username
      })
      
      ElMessage.success('登录成功')
      router.push('/user/dashboard')
    } else {
      ElMessage.error(response.data.message)
    }
  } catch (error) {
    console.error('登录错误:', error)
    ElMessage.error('登录失败，请检查用户名和密码')
  } finally {
    loading.value = false
  }
}

const handleEnterpriseLogin = async() => {
  if (!enterpriseForm.loginUsername || !enterpriseForm.password) {
    ElMessage.error('请输入用户名和密码')
    return
  }

  loading.value = true
  const loginData = {
      username: enterpriseForm.loginUsername,
      password: enterpriseForm.password
  };
  console.log(loginData)
  try {
    const baseURL = 'http://localhost:8080/api/v1';
    console.log(baseURL);
    const response = await axios.post(`${baseURL}/auth/enterprise/login`,loginData);
    console.log(response.status)
    if (response.status==200) {
      const userData = response.data.data
      ElMessage.success('登录成功')
      router.push('/enterprise/dashboard')
    } else {
      ElMessage.error(response.data.message)
    }
  } catch (error) {
    ElMessage.error('登录失败，请检查用户名和密码')
  } finally {
    loading.value = false
  }
}

const handleAdminLogin = () => {
  // 类似个人登录...
  ElMessage.info('管理员登录功能开发中')
}
</script>

<style scoped>
.login-container {
  display: flex;
  justify-content: center;
  align-items: center;
  min-height: 100vh;
  background-color: #f5f7fa;
}

.login-box {
  width: 400px;
  padding: 30px;
  background: white;
  border-radius: 8px;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
}

h2 {
  text-align: center;
  margin-bottom: 30px;
  color: #303133;
}

.form-footer {
  margin-top: 15px;
  text-align: center;
}

.form-footer a {
  color: #409EFF;
  text-decoration: none;
}

.form-footer a:hover {
  text-decoration: underline;
}
</style>
