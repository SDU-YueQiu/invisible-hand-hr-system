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
          <el-menu-item index="/admin/jobs">
            <el-icon><connection /></el-icon>
            <span>职位管理</span>
          </el-menu-item>
          <el-menu-item index="/admin/feedback">
            <el-icon><message /></el-icon>
            <span>反馈系统</span>
          </el-menu-item>
          <el-menu-item index="/admin/announcements">
            <el-icon><notification /></el-icon>
            <span>公告系统</span>
          </el-menu-item>
          <el-menu-item index="/admin/settings">
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
          <div class="header-title">管理员中心</div>
          <div class="user-info">
            <el-dropdown>
              <span class="el-dropdown-link">
                {{ username }}<el-icon class="el-icon--right"><arrow-down /></el-icon>
              </span>
              <template #dropdown>
                <el-dropdown-menu>
                  <el-dropdown-item @click="router.push('/admin/settings')">账号设置</el-dropdown-item>
                  <el-dropdown-item divided @click="handleLogout">退出登录</el-dropdown-item>
                </el-dropdown-menu>
              </template>
            </el-dropdown>
          </div>
        </el-header>

        <el-main class="main">
          <el-row :gutter="20" class="stats-cards">
            <el-col :span="8">
              <el-card class="stat-card">
                <div class="card-content">
                  <div class="stat-number">{{ stats.userCount }}</div>
                  <div class="stat-label">个人用户数</div>
                </div>
              </el-card>
            </el-col>
            <el-col :span="8">
              <el-card class="stat-card">
                <div class="card-content">
                  <div class="stat-number">{{ stats.enterpriseCount }}</div>
                  <div class="stat-label">企业用户数</div>
                </div>
              </el-card>
            </el-col>
            <el-col :span="8">
              <el-card class="stat-card">
                <div class="card-content">
                  <div class="stat-number">{{ stats.jobCount }}</div>
                  <div class="stat-label">职位数量</div>
                </div>
              </el-card>
            </el-col>
          </el-row>

          <el-row :gutter="20" class="recent-activity">
            <el-col :span="12">
              <el-card class="recent-card">
                <template #header>
                  <div class="card-header">
                    <span>最近反馈</span>
                    <el-button @click="router.push('/admin/feedback')" link>查看全部</el-button>
                  </div>
                </template>
                <div v-if="recentFeedbacks.length">
                  <div v-for="feedback in recentFeedbacks" :key="feedback.feedbackId" class="feedback-item">
                    <div class="feedback-header">
                      <span class="type-tag">{{ feedback.feedbackType }}</span>
                      <span class="time">{{ formatDate(feedback.createTime) }}</span>
                    </div>
                    <div class="feedback-content">{{ feedback.content }}</div>
                    <div class="feedback-user">来自：{{ feedback.userName }}</div>
                  </div>
                </div>
                <el-empty v-else description="暂无反馈记录" />
              </el-card>
            </el-col>

            <el-col :span="12">
              <el-card class="recent-card">
                <template #header>
                  <div class="card-header">
                    <span>最近公告</span>
                    <el-button @click="router.push('/admin/announcements')" link>查看全部</el-button>
                  </div>
                </template>
                <div v-if="recentAnnouncements.length">
                  <div v-for="announcement in recentAnnouncements" :key="announcement.announcementId" class="announcement-item">
                    <div class="announcement-title">{{ announcement.title }}</div>
                    <div class="announcement-time">{{ formatDate(announcement.publishTime) }}</div>
                    <div class="announcement-summary">{{ announcement.contentSummary }}</div>
                  </div>
                </div>
                <el-empty v-else description="暂无公告记录" />
              </el-card>
            </el-col>
          </el-row>
        </el-main>
      </el-container>
    </el-container>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'
import { useRouter } from 'vue-router'
import { ElMessage } from 'element-plus'
import request from '../../utils/request'

// 统计数据
const stats = ref({
  userCount: 0,
  enterpriseCount: 0,
  jobCount: 0
})

// 最近反馈
const recentFeedbacks = ref([])

// 最近公告
const recentAnnouncements = ref([])

// 时间格式化
const formatDate = (dateString) => {
  if (!dateString) return ''
  const date = new Date(dateString)
  return `${date.getFullYear()}-${String(date.getMonth()+1).padStart(2, '0')}-${String(date.getDate()).padStart(2, '0')}`
}

// 获取统计数据
const fetchStats = async () => {
  try {
    const [userRes, enterpriseRes, jobRes] = await Promise.all([
      request.get('/admin/users/count'),
      request.get('/admin/enterprises/count'),
      request.get('/admin/jobs/count')
    ])
    
    stats.value.userCount = userRes.data || 0
    stats.value.enterpriseCount = enterpriseRes.data || 0
    stats.value.jobCount = jobRes.data || 0
  } catch (error) {
    ElMessage.error('获取统计数据失败')
    console.error('获取统计数据失败:', error)
  }
}

// 获取最近反馈
const fetchRecentFeedbacks = async () => {
  try {
    const res = await request.get('/admin/feedbacks', { 
      params: { page: 1, limit: 5 } 
    })
    recentFeedbacks.value = res.data?.feedbacks || []
  } catch (error) {
    ElMessage.error('获取反馈列表失败')
    console.error('获取反馈列表失败:', error)
  }
}

// 获取最近公告
const fetchRecentAnnouncements = async () => {
  try {
    const res = await request.get('/announcements', { 
      params: { page: 1, limit: 5 } 
    })
    recentAnnouncements.value = res.data?.announcements || []
  } catch (error) {
    ElMessage.error('获取公告列表失败')
    console.error('获取公告列表失败:', error)
  }
}

onMounted(async () => {
  await Promise.all([
    fetchStats(),
    fetchRecentFeedbacks(),
    fetchRecentAnnouncements()
  ])
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
  overflow-y: auto;
}

.stats-cards {
  margin-bottom: 30px;
}

.stat-card {
  height: 100%;
}

.card-content {
  text-align: center;
  padding: 20px 0;
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

.recent-activity {
  margin-bottom: 30px;
}

.recent-card {
  height: 100%;
}

.card-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.feedback-item {
  padding: 10px 0;
  border-bottom: 1px solid #eee;
}

.feedback-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 5px;
}

.type-tag {
  font-size: 12px;
  color: #fff;
  background-color: #409EFF;
  padding: 2px 6px;
  border-radius: 4px;
}

.time {
  font-size: 12px;
  color: #999;
}

.feedback-content {
  font-size: 14px;
  margin-bottom: 5px;
}

.feedback-user {
  font-size: 12px;
  color: #666;
}

.announcement-item {
  padding: 10px 0;
  border-bottom: 1px solid #eee;
}

.announcement-title {
  font-weight: bold;
  margin-bottom: 5px;
}

.announcement-time {
  font-size: 12px;
  color: #999;
  margin-bottom: 5px;
}

.announcement-summary {
  font-size: 14px;
  color: #666;
  overflow: hidden;
  text-overflow: ellipsis;
  display: -webkit-box;
  -webkit-line-clamp: 2;
  line-clamp:2;
  -webkit-box-orient: vertical;
}

.logout-btn {
  position: absolute;
  bottom: 20px;
  width: 100%;
  text-align: center;
}
</style>