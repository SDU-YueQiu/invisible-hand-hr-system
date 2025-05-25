<template>
  <div class="layout-container">
    <el-container>
      <el-aside width="200px" class="aside">
        <div class="logo">
          <h3>人才招聘系统</h3>
        </div>
        <el-menu 
          :default-active="activeMenu"
          class="el-menu-vertical"
          router
          background-color="#304156"
          text-color="#bfcbd9"
          active-text-color="#409EFF"
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
          <el-menu-item index="/jobs">
            <el-icon><search /></el-icon>
            <span>找工作</span>
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
          <div class="header-title">{{ pageTitle }}</div>
          <div class="user-info">
            <el-dropdown>
              <span class="el-dropdown-link">
                {{ username }}<el-icon class="el-icon--right"><arrow-down /></el-icon>
              </span>
              <template #dropdown>
                <el-dropdown-menu>
                  <el-dropdown-item @click="$router.push('/user/profile')">个人资料</el-dropdown-item>
                  <el-dropdown-item @click="$router.push('/user/settings')">账号设置</el-dropdown-item>
                  <el-dropdown-item divided @click="handleLogout">退出登录</el-dropdown-item>
                </el-dropdown-menu>
              </template>
            </el-dropdown>
          </div>
        </el-header>
        
        <el-main class="main">
          <router-view></router-view>
        </el-main>
      </el-container>
    </el-container>
  </div>
</template>

<script setup>
import { ref, computed } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import { ElMessageBox, ElMessage } from 'element-plus'
import { 
  Menu as IconMenu, 
  User, 
  Document, 
  Connection, 
  Setting, 
  Search,
  ArrowDown 
} from '@element-plus/icons-vue'
import { useUserStore } from '../../stores'

const route = useRoute()
const router = useRouter()
const userStore = useUserStore()

const username = computed(() => userStore.username || '用户名')

const activeMenu = computed(() => route.path)

const pageTitle = computed(() => {
  const pathMap = {
    '/user/dashboard': '个人中心',
    '/user/profile': '个人资料',
    '/user/resumes': '我的简历',
    '/user/resume/create': '创建简历',
    '/user/resume/edit': '编辑简历',
    '/user/resume/view': '查看简历',
    '/user/applications': '求职申请',
    '/user/settings': '账号设置'
  }
  
  // 处理动态路由
  if (route.path.includes('/user/resume/edit/')) {
    return '编辑简历'
  } else if (route.path.includes('/user/resume/view/')) {
    return '查看简历'
  }
  
  return pathMap[route.path] || '个人中心'
})

const handleLogout = () => {
  ElMessageBox.confirm('确定要退出登录吗?', '提示', {
    confirmButtonText: '确定',
    cancelButtonText: '取消',
    type: 'warning',
  }).then(() => {
    // 清除登录状态
    localStorage.removeItem('token')
    localStorage.removeItem('userType')
    userStore.clearUser()
    
    router.push('/login')
    ElMessage.success('已成功退出登录')
  }).catch(() => {})
}
</script>

<style scoped>
.layout-container {
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
  height: calc(100vh - 60px);
  overflow-y: auto;
}

.logout-btn {
  position: absolute;
  bottom: 20px;
  width: 100%;
  text-align: center;
}
</style>
