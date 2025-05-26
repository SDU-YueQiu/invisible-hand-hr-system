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
          <el-menu-item index="/enterprise/dashboard">
            <el-icon><icon-menu /></el-icon>
            <span>企业中心</span>
          </el-menu-item>
          <el-menu-item index="enterprise/profile">
            <el-icon><user /></el-icon>
            <span>企业资料</span>
          </el-menu-item>
          <el-menu-item index="/enterprise/jobs">
            <el-icon><document /></el-icon>
            <span>在招岗位</span>
          </el-menu-item>
          <el-menu-item index="job/create">
            <el-icon><connection /></el-icon>
            <span>增加岗位</span>
          </el-menu-item>
          <el-menu-item index="job/edit/:id">
            <el-icon><search /></el-icon>
            <span>岗位变更</span>
          </el-menu-item>
          <el-menu-item index="/enterprise/jobs/:id/applicants">
            <el-icon><search /></el-icon>
            <span>查看求职申请</span>
          </el-menu-item>
          <el-menu-item index="/enterprise/talents">
            <el-icon><search /></el-icon>
            <span>人才</span>
          </el-menu-item>
          <el-menu-item index="/enterprise/settings">
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
                  <el-dropdown-item @click="$router.push('/enterprise/profile')">企业资料</el-dropdown-item>
                  <el-dropdown-item @click="$router.push('/enterprise/settings')">账号设置</el-dropdown-item>
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
    '/enterprise/dashboard': '企业中心',
    '/enterprise/profile': '企业资料',
    '/enterprise/jobs': '在招岗位',
    '/enterprise/jobs/create': '增加岗位',
    '/enterprise/jobs/edit/:id': '岗位变更',
    '/enterprise/jobs/:id/applicants': '查看求职申请',
    '/enterprise/talents': '人才',
    '/enterprise/settings': '账号设置'
  }
  return pathMap[route.path] || '企业中心'
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
