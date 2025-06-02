<template>
  <div class="admin-dashboard">
    <el-container>
      <el-aside width="200px" class="aside">
        <!-- 侧边栏导航 -->
        <div class="logo">
          <h2>人才管理系统</h2>
        </div>
        <el-menu
          default-active="dashboard"
          class="el-menu-vertical"
          router
        >
          <el-menu-item index="/admin/dashboard">
            <el-icon><icon-menu /></el-icon>
            <span>管理员中心</span>
          </el-menu-item>
          <el-menu-item index="/admin/users">
            <el-icon><user /></el-icon>
            <span>个人用户管理</span>
          </el-menu-item>
          <el-menu-item index="/admin/enterprises">
            <el-icon><document /></el-icon>
            <span>企业用户管理</span>
          </el-menu-item>
        </el-menu>
        <div class="logout-btn">
          <el-button type="danger" plain @click="handleLogout">退出登录</el-button>
        </div>
      </el-aside>

      <el-container>
        <el-header height="60px" class="header">
          <div class="header-title">管理员中心</div>
          <div class="user-info">
            <el-dropdown>
              <span class="el-dropdown-link">
                {{ username }}<el-icon class="el-icon--right"><arrow-down /></el-icon>
              </span>
              <template #dropdown>
                <el-dropdown-menu>
                  <el-dropdown-item @click="router.push('/admin/settings')">账号设置</el-dropdown-item>
                  <el-dropdown-item divided>退出登录</el-dropdown-item>
                </el-dropdown-menu>
              </template>
            </el-dropdown>
          </div>
        </el-header>

        <el-main class="main">
          <!-- 空白主内容区 -->
          <div class="empty-content">
            <el-empty description="暂无数据" />
          </div>
        </el-main>
      </el-container>
    </el-container>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'
import { useRouter } from 'vue-router'
import { useAdminStore } from '../../stores'

const router = useRouter()
const adminStore = useAdminStore()

// 用户状态
const username = ref(adminStore.username || '管理员')

// 退出登录
const handleLogout = () => {
  // 退出登录逻辑
}

onMounted(() => {
  // 页面初始化逻辑
})
</script>

<style scoped>
.admin-dashboard {
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
  overflow-y: auto;
}

.empty-content {
  display: flex;
  justify-content: center;
  align-items: center;
  height: 100%;
}

.logout-btn {
  position: absolute;
  bottom: 20px;
  width: 100%;
  text-align: center;
}
</style>