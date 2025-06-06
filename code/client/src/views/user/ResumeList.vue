<template>
  <div class="resume-container">
    <div class="resume-header">
      <h2>我的简历</h2>
      <el-button type="primary" @click="createNewResume">新建简历</el-button>
    </div>
    
    <div v-if="loading" class="loading-container">
      <el-skeleton :rows="5" animated />
    </div>
    
    <div v-else-if="resumes.length === 0" class="empty-container">
      <el-empty description="您还没有创建过简历">
        <el-button type="primary" @click="createNewResume">立即创建</el-button>
      </el-empty>
    </div>
    
    <div v-else class="resume-list">
      <el-card v-for="resume in resumes" :key="resume.resumeId" class="resume-card">
        <div class="resume-card-content">
          <div class="resume-card-title">{{ resume.resumeTitle }}</div>
          <div class="resume-card-meta">
            <!-- <div>更新时间：{{ formatDate(resume.lastUpdateTime) }}</div> -->
            <div>可见性：{{ getVisibilityText(resume.visibilityStatus) }}</div>
          </div>
        </div>
        
        <div class="resume-card-actions">
          <el-button type="primary" text @click="viewResume(resume.resumeId)">查看</el-button>
          <el-button type="success" text @click="editResume(resume.resumeId)">编辑</el-button>
          <el-dropdown>
            <span class="el-dropdown-link">
              <el-button type="info" text>更多<el-icon class="el-icon--right"><ArrowDown /></el-icon></el-button>
            </span>
            <template #dropdown>
              <el-dropdown-menu>
                <el-dropdown-item @click="toggleVisibility(resume)">
                  {{ resume.visibilityStatus === 'Private' ? '设置为公开' : '设置为私密' }}
                </el-dropdown-item>
                <el-dropdown-item @click="duplicateResume(resume.resumeId)">复制简历</el-dropdown-item>
                <el-dropdown-item divided @click="confirmDelete(resume.resumeId)">删除简历</el-dropdown-item>
              </el-dropdown-menu>
            </template>
          </el-dropdown>
        </div>
      </el-card>
    </div>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import { useRouter } from 'vue-router'
import { ArrowDown } from '@element-plus/icons-vue'
import request from '../../utils/request'

const router = useRouter()
const loading = ref(true)
const resumes = ref([])

onMounted(async () => {
  await fetchResumes()
})

const parseResumeData = (data) => {
  const fieldsToParse = [
    'basicInfo',
    'jobIntent',
    'educationExperience',
    'workExperience',
    'projectExperience',
    'skillsCertificates',
    'lastUpdateTime',
    'resumeTitle',
    'selfDescription'
  ]
  fieldsToParse.forEach(field => {
    if (data[field] && typeof data[field] === 'string') {
      try {
        data[field] = JSON.parse(data[field])
      } catch (e) {
        console.error(`failed to parse ${field}:`, e)
      }
    }
  })
  return data
}

const fetchResumes = async () => {
  try {
    loading.value = true
    const res = await request.get('/users/me/resumes')
    if (res.success) {
      resumes.value = parseResumeData(res.data) || []
    } else {
      ElMessage.error(res.message || '获取简历列表失败')
    }
  } catch (error) {
    ElMessage.error('获取简历列表失败')
    console.error(error)
  } finally {
    loading.value = false
  }
}

const formatDate = (dateString) => {
  if (!dateString) return '未知时间'
  
  const date = new Date(dateString)
  return date.toLocaleDateString('zh-CN', {
    year: 'numeric',
    month: '2-digit',
    day: '2-digit',
    hour: '2-digit',
    minute: '2-digit'
  })
}

const getVisibilityText = (status) => {
  const statusMap = {
    'Public': '公开给所有企业',
    'PublicToApplied': '仅对申请企业公开',
    'Private': '私密'
  }
  return statusMap[status] || status
}

const createNewResume = () => {
  router.push('/user/resume/create')
}

const viewResume = (resumeId) => {
  router.push(`/user/resume/view/${resumeId}`)
}

const editResume = (resumeId) => {
  router.push(`/user/resume/edit/${resumeId}`)
}

const toggleVisibility = async (resume) => {
  try {
    const newStatus = resume.visibilityStatus === 'Private' ? 'Public' : 'Private'
    const res = await request.put(`/users/me/resumes/${resume.resumeId}`, {
      visibilityStatus: newStatus
    })
    
    if (res.success) {
      resume.visibilityStatus = newStatus
      ElMessage.success('简历可见性已更新')
    } else {
      ElMessage.error(res.message || '更新简历可见性失败')
    }
  } catch (error) {
    ElMessage.error('更新简历可见性失败')
    console.error(error)
  }
}

const duplicateResume = async (resumeId) => {
  try {
    // 获取简历详情
    const getRes = await request.get(`/users/me/resumes/${resumeId}`)
    if (getRes.success) {
      const resumeData = parseResumeData(getRes.data)
      // 创建新简历（复制）
      const postData = {
        ...resumeData,
        resumeTitle: `${resumeData.resumeTitle} (副本)`,
      }
      delete postData.resumeId // 删除ID以创建新简历
      
      const postRes = await request.post('/users/me/resumes', postData)
      if (postRes.success) {
        ElMessage.success('简历复制成功')
        fetchResumes() // 刷新列表
      } else {
        ElMessage.error(postRes.message || '复制简历失败')
      }
    }
  } catch (error) {
    ElMessage.error('复制简历失败')
    console.error(error)
  }
}

const confirmDelete = (resumeId) => {
  ElMessageBox.confirm('确定要删除该简历吗？此操作不可撤销。', '提示', {
    confirmButtonText: '确定',
    cancelButtonText: '取消',
    type: 'warning',
  }).then(async () => {
    try {
      const res = await request.delete(`/users/me/resumes/${resumeId}`)
      if (res.success) {
        ElMessage.success('简历删除成功')
        // 从列表中移除该简历
        resumes.value = resumes.value.filter(resume => resume.resumeId !== resumeId)
      } else {
        ElMessage.error(res.message || '删除简历失败')
      }
    } catch (error) {
      ElMessage.error('删除简历失败')
      console.error(error)
    }
  }).catch(() => {})
}
</script>

<style scoped>
.resume-container {
  padding: 20px;
  background-color: white;
  border-radius: 4px;
  box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
}

.resume-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20px;
}

.loading-container, .empty-container {
  margin: 40px 0;
  text-align: center;
}

.resume-list {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(300px, 1fr));
  gap: 20px;
}

.resume-card {
  margin-bottom: 0;
  height: 100%;
  display: flex;
  flex-direction: column;
}

.resume-card-content {
  flex-grow: 1;
}

.resume-card-title {
  font-size: 18px;
  font-weight: bold;
  margin-bottom: 10px;
}

.resume-card-meta {
  color: #606266;
  font-size: 14px;
  margin-bottom: 15px;
}

.resume-card-actions {
  display: flex;
  justify-content: flex-end;
  border-top: 1px solid #EBEEF5;
  padding-top: 10px;
  margin-top: auto;
}
</style>
