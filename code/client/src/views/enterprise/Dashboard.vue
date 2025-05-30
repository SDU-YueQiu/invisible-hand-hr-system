<template>
  <div class="dashboard-container">
    <el-container>
      <!-- 主内容区 -->
      <el-container>
        <!-- 主内容 -->
        <el-main class="main">
          <!-- 数据统计卡片 -->
          <div class="dashboard-summary">
            <el-row :gutter="20">
              <el-col :span="24">
                <el-card class="box-card">
                  <template #header>
                    <div class="card-header">
                      <span>申请统计</span>
                    </div>
                  </template>
                  <div class="card-content">
                    <div class="stat-number">{{ applicationCount }}</div>
                    <div class="stat-label">收到的申请数</div>
                  </div>
                </el-card>
              </el-col>
            </el-row>
          </div>
          
          <!-- 最近申请表格 -->
          <div class="recent-applications">
            <h3>最近申请记录</h3>
            <el-table :data="recentApplications" style="width: 100%">
              <el-table-column prop="jobTitle" label="职位名称" width="180" />
              <el-table-column prop="applicantName" label="申请人" />
              <el-table-column prop="applyDate" label="申请日期" width="120" />
              <el-table-column label="操作" width="120">
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
import { ref } from 'vue'
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
import { useEnterpriseStore } from '../../stores'

const router = useRouter()
const enterpriseStore = useEnterpriseStore()

// 企业名称
const enterpriseName = ref(enterpriseStore.name || '企业名称')

// 模拟数据
const applicationCount = ref(15)
const recentApplications = ref([
  {
    id: 1,
    jobTitle: '前端开发工程师',
    applicantName: '张小明',
    applyDate: '2023-05-15'
  },
  {
    id: 2,
    jobTitle: 'UI设计师',
    applicantName: '李华',
    applyDate: '2023-05-14'
  },
  {
    id: 3,
    jobTitle: 'Java开发',
    applicantName: '王强',
    applyDate: '2023-05-13'
  },
])

const viewDetail = (row) => {
  router.push(`/enterprise/applications/${row.id}`)
}

const handleLogout = () => {
  ElMessageBox.confirm('确定要退出登录吗?', '提示', {
    confirmButtonText: '确定',
    cancelButtonText: '取消',
    type: 'warning',
  }).then(() => {
    localStorage.removeItem('token')
    localStorage.removeItem('userType')
    enterpriseStore.clearData()
    router.push('/login')
    ElMessage.success('已成功退出登录')
  }).catch(() => {})
}
</script>

<style scoped>
/* 保持与个人中心相同的样式 */
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
