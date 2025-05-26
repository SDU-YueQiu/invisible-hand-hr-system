<!-- src/views/user/Dashboard.vue -->
<template>
  <div class="dashboard-container">
    <el-container>
      <el-aside width="200px" class="aside">
        <div class="logo">
          <h3>人才招聘系统</h3>
        </div>
        <el-menu 
          default-active="dashboard"
          class="el-menu-vertical"
          router
        >
          <el-menu-item index="/user/dashboard">
            <el-icon><icon-menu /></el-icon>
            <span>个人中心</span>
          </el-menu-item>
          <el-menu-item index="/user/profile">
            <el-icon><user /></el-icon>
            <span>个人资料</span>
          </el-menu-item>
          <el-menu-item index="/user/resumes">
            <el-icon><document /></el-icon>
            <span>我的简历</span>
          </el-menu-item>
          <el-menu-item index="/user/applications">
            <el-icon><connection /></el-icon>
            <span>申请记录</span>
          </el-menu-item>
          <el-menu-item index="/user/settings">
            <el-icon><setting /></el-icon>
            <span>账号设置</span>
          </el-menu-item>
        </el-menu>
        <div class="logout-btn">
          <el-button type="danger" plain @click="handleLogout">退出登录</el-button>
        </div>
      </el-aside>
      
      <el-container>
        <el-header height="60px" class="header">
          <div class="header-title">个人中心</div>
          <div class="user-info">
            <el-dropdown>
              <span class="el-dropdown-link">
                {{ username }}<el-icon class="el-icon--right"><arrow-down /></el-icon>
              </span>
              <template #dropdown>
                <el-dropdown-menu>
                  <el-dropdown-item @click="router.push('/user/profile')">个人资料</el-dropdown-item>
                  <el-dropdown-item @click="router.push('/user/settings')">账号设置</el-dropdown-item>
                  <el-dropdown-item divided @click="handleLogout">退出登录</el-dropdown-item>
                </el-dropdown-menu>
              </template>
            </el-dropdown>
          </div>
        </el-header>
        
        <el-main class="main">
          <div class="dashboard-summary">
            <el-row :gutter="20">
              <el-col :span="8">
                <el-card class="box-card">
                  <template #header>
                    <div class="card-header">
                      <span>简历统计</span>
                    </div>
                  </template>
                  <div class="card-content">
                    <div class="stat-number">2</div>
                    <div class="stat-label">我的简历数</div>
                  </div>
                </el-card>
              </el-col>
              
              <el-col :span="8">
                <el-card class="box-card">
                  <template #header>
                    <div class="card-header">
                      <span>职位申请</span>
                    </div>
                  </template>
                  <div class="card-content">
                    <div class="stat-number">5</div>
                    <div class="stat-label">申请的职位数</div>
                  </div>
                </el-card>
              </el-col>
              
              <el-col :span="8">
                <el-card class="box-card">
                  <template #header>
                    <div class="card-header">
                      <span>邀请面试</span>
                    </div>
                  </template>
                  <div class="card-content">
                    <div class="stat-number">2</div>
                    <div class="stat-label">面试邀请数</div>
                  </div>
                </el-card>
              </el-col>
            </el-row>
          </div>
          
          <div class="recent-applications">
            <h3>最近申请</h3>
            <el-table :data="recentApplications" style="width: 100%">
              <el-table-column prop="jobTitle" label="职位名称" />
              <el-table-column prop="companyName" label="公司名称" />
              <el-table-column prop="applyDate" label="申请日期" />
              <el-table-column prop="status" label="状态">
                <template #default="scope">
                  <el-tag :type="getStatusType(scope.row.status)">
                    {{ scope.row.status }}
                  </el-tag>
                </template>
              </el-table-column>
              <el-table-column label="操作">
                <template #default="scope">
                  <el-button type="text" size="small" @click="viewDetail(scope.row)">
                    查看详情
                  </el-button>
                </template>
              </el-table-column>
            </el-table>
          </div>
        </el-main>
      </el-container>
    </el-container>
  </div>
</template>

<script setup>
import { ref, onMounted} from 'vue'
import { useRouter } from 'vue-router'
import { ElMessage, ElMessageBox } from 'element-plus'
import { 
  Menu as IconMenu, 
  User, 
  Document, 
  Connection, 
  Setting, 
  ArrowDown 
} from '@element-plus/icons-vue'
import { useLocalStorage } from '@vueuse/core'
import axios from 'axios'
import { useUserStore } from '../../stores'

const router = useRouter()
const userStore = useUserStore()

const token = useLocalStorage('token', '')
const userType = useLocalStorage('userType', '')

const userInfo = ref({})
const username = ref('')

const fetchUserInfo = async () => {
  try {
    if (!token.value) {
      ElMessage.warning('未登录')
      router.push('/login')
      return
    }
    
    const baseURL = 'http://localhost:8080/api/v1'
    const response = await axios.get(`${baseURL}/users/me`, {
      headers: { 
        'Authorization': `Bearer ${token.value}`
      }
    })
    console.log('全部：',response)
    console.log("data:",response.data);

    if (response.status ===200) {
      userInfo.value = response.data
      username.value = userInfo.value.username
      
      // 更新 store
      userStore.setUser({
        token: token.value,
        userType: userType.value,
        userId: userInfo.value.userId,
        username: userInfo.value.username
      })
    } else {
      ElMessage.error(response.data.message || '获取用户信息失败')
    }
  } catch (error) {
    console.error('获取用户信息失败:', error)
    if (error.response?.status === 401) {
      ElMessage.error('登录已过期，请重新登录')
      // 清除token
      token.value = ''
      userType.value = ''
      router.push('/login')
    } else {
      ElMessage.error('获取用户信息失败')
    }
  }
}

onMounted(() => {
  fetchUserInfo()
})
// // 模拟数据
// const username = ref(userStore.username || '用户名')
// const recentApplications = ref([
//   {
//     id: 1,
//     jobTitle: '前端开发工程师',
//     companyName: '科技有限公司',
//     applyDate: '2023-05-15',
//     status: '已投递'
//   },
//   {
//     id: 2,
//     jobTitle: 'UI设计师',
//     companyName: '创意科技',
//     applyDate: '2023-05-10',
//     status: '面试邀请'
//   },
//   {
//     id: 3,
//     jobTitle: 'Web全栈开发',
//     companyName: '互联网科技',
//     applyDate: '2023-05-05',
//     status: '已查看'
//   },
//   {
//     id: 4,
//     jobTitle: '产品经理',
//     companyName: '软件科技',
//     applyDate: '2023-04-28',
//     status: '不合适'
//   },
// ])

const getStatusType = (status) => {
  const statusMap = {
    '已投递': 'info',
    '已查看': 'success',
    '面试邀请': 'warning',
    '不合适': 'danger',
  }
  return statusMap[status] || 'info'
}

const viewDetail = (row) => {
  ElMessage.info(`查看申请详情：${row.jobTitle}`)
}

const handleLogout = () => {
  ElMessageBox.confirm('确定要退出登录吗?', '提示', {
    confirmButtonText: '确定',
    cancelButtonText: '取消',
    type: 'warning',
  }).then(() => {
    // 清除登录状态 (useLocalStorage 响应式清除)
    token.value = ''
    userType.value = ''
    userStore.clearUser()
    const baseURL = 'http://localhost:8080/api/v1'
    const response = axios.post(`${baseURL}/auth/logout`, {})
    if(response.status === 200) {
      router.push('/login')
      ElMessage.success('已成功退出登录')
    } else {
      ElMessage.error('退出登录失败，请稍后再试')
    }
  }).catch(() => {})
}
</script>

<style scoped>
.dashboard-container {
  height: 100vh;
  display: flex;
}

.aside {
  background-color: #304156;
  color: white;
  height: 100vh;
  position: relative;
}

.logo {
  height: 60px;
  display: flex;
  justify-content: center;
  align-items: center;
  border-bottom: 1px solid #1f2d3d;
}

.logo h3 {
  color: #fff;
  margin: 0;
}

.el-menu-vertical {
  border-right: none;
}

.header {
  background-color: white;
  border-bottom: 1px solid #e6e6e6;
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.header-title {
  font-size: 18px;
  font-weight: bold;
}

.user-info {
  display: flex;
  align-items: center;
}

.el-dropdown-link {
  cursor: pointer;
  display: flex;
  align-items: center;
}

.main {
  background-color: #f0f2f5;
  padding: 20px;
}

.dashboard-summary {
  margin-bottom: 30px;
}

.box-card {
  height: 100%;
}

.card-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.card-content {
  text-align: center;
  padding: 10px 0;
}

.stat-number {
  font-size: 28px;
  font-weight: bold;
  color: #409EFF;
  margin-bottom: 10px;
}

.stat-label {
  color: #909399;
}

.recent-applications {
  background-color: white;
  padding: 20px;
  border-radius: 4px;
  box-shadow: 0 2px 12px 0 rgba(0,0,0,0.1);
}

.recent-applications h3 {
  margin-top: 0;
  margin-bottom: 20px;
}

.logout-btn {
  position: absolute;
  bottom: 20px;
  width: 100%;
  text-align: center;
}
</style>
