<template>
  <div class="resume-view-container">
    <div class="resume-view-header">
      <div>
        <h2>{{ resume.resumeTitle }}</h2>
        <el-tag v-if="resume.visibilityStatus" :type="getVisibilityTagType()">
          {{ getVisibilityText(resume.visibilityStatus) }}
        </el-tag>
      </div>
      <div class="resume-actions">
        <el-button type="primary" @click="editResume">编辑简历</el-button>
        <el-button @click="downloadPdf">导出PDF</el-button>
      </div>
    </div>
    
    <div v-if="loading" class="loading-container">
      <el-skeleton :rows="15" animated />
    </div>
    
    <div v-else class="resume-content">
      <!-- 基本信息 -->
      <div class="resume-section">
        <h3 class="section-title">基本信息</h3>
        <div class="section-content basic-info">
          <div class="info-item">
            <span class="info-label">姓名：</span>
            <span class="info-value">{{ resume.basicInfo?.name }}</span>
          </div>
          <div class="info-item">
            <span class="info-label">性别：</span>
            <span class="info-value">{{ resume.basicInfo?.gender }}</span>
          </div>
          <div class="info-item">
            <span class="info-label">出生日期：</span>
            <span class="info-value">{{ formatDate(resume.basicInfo?.birthDate) }}</span>
          </div>
        </div>
      </div>
      
      <!-- 求职意向 -->
      <div class="resume-section">
        <h3 class="section-title">求职意向</h3>
        <div class="section-content">
          <div class="info-item">
            <span class="info-label">期望行业：</span>
            <span class="info-value">{{ resume.jobIntent?.industry }}</span>
          </div>
          <div class="info-item">
            <span class="info-label">期望职位：</span>
            <span class="info-value">{{ resume.jobIntent?.position }}</span>
          </div>
          <div class="info-item">
            <span class="info-label">期望薪资：</span>
            <span class="info-value">{{ resume.jobIntent?.salary }}</span>
          </div>
        </div>
      </div>
      
      <!-- 教育经历 -->
      <div class="resume-section">
        <h3 class="section-title">教育经历</h3>
        <div class="section-content" v-if="resume.educationExperience?.length">
          <el-timeline>
            <el-timeline-item
              v-for="(edu, index) in resume.educationExperience"
              :key="index"
              :timestamp="formatDateRange(edu.dateRange)"
              placement="top"
            >
              <div class="timeline-title">{{ edu.school }} - {{ edu.major }}</div>
              <div>{{ edu.degree }}</div>
            </el-timeline-item>
          </el-timeline>
        </div>
        <div class="section-content" v-else>
          <el-empty description="暂无教育经历" />
        </div>
      </div>
      
      <!-- 工作经历 -->
      <div class="resume-section">
        <h3 class="section-title">工作经历</h3>
        <div class="section-content" v-if="resume.workExperience?.length">
          <el-timeline>
            <el-timeline-item
              v-for="(work, index) in resume.workExperience"
              :key="index"
              :timestamp="formatDateRange(work.dateRange)"
              placement="top"
            >
              <div class="timeline-title">{{ work.company }} - {{ work.role }}</div>
              <div class="timeline-description">{{ work.description }}</div>
            </el-timeline-item>
          </el-timeline>
        </div>
        <div class="section-content" v-else>
          <el-empty description="暂无工作经历" />
        </div>
      </div>
      
      <!-- 项目经历 -->
      <div class="resume-section">
        <h3 class="section-title">项目经历</h3>
        <div class="section-content" v-if="resume.projectExperience?.length">
          <el-card v-for="(project, index) in resume.projectExperience" :key="index" class="project-card">
            <template #header>
              <div>
                {{ project.projectName }}
              </div>
            </template>
            <div class="project-description">{{ project.description }}</div>
          </el-card>
        </div>
        <div class="section-content" v-else>
          <el-empty description="暂无项目经历" />
        </div>
      </div>
      
      <!-- 技能证书 -->
      <div class="resume-section">
        <h3 class="section-title">技能证书</h3>
        <div class="section-content" v-if="resume.skillsCertificates?.length">
          <el-tag
            v-for="(skill, index) in resume.skillsCertificates"
            :key="index"
            class="skill-tag"
            :type="getSkillTagType(skill.level)"
          >
            {{ skill.skillName }}{{ skill.level ? ' (' + skill.level + ')' : '' }}
          </el-tag>
        </div>
        <div class="section-content" v-else>
          <el-empty description="暂无技能证书" />
        </div>
      </div>
      
      <!-- 自我描述 -->
      <div class="resume-section" v-if="resume.selfDescription">
        <h3 class="section-title">自我描述</h3>
        <div class="section-content self-description">
          {{ resume.selfDescription }}
        </div>
      </div>
      
      <!-- 简历附件 -->
      <div class="resume-section" v-if="resume.attachmentPath">
        <h3 class="section-title">简历附件</h3>
        <div class="section-content">
          <el-button type="primary" @click="downloadAttachment">
            <el-icon><download /></el-icon>
            下载附件
          </el-button>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import { ElMessage } from 'element-plus'
import { Download } from '@element-plus/icons-vue'
import request from '../../utils/request'

const route = useRoute()
const router = useRouter()
const resumeId = route.params.id
const loading = ref(true)
const resume = ref({})

onMounted(async () => {
  await fetchResumeDetails()
})
const parseResumeData = (data) => {
  const jsonFields = [
    'basicInfo',
    'jobIntent',
    'educationExperience',
    'workExperience',
    'projectExperience',
    'skillsCertificates'
  ];
  
  const plainFields = [
    'resumeTitle',
    'selfDescription',
    'lastUpdateTime'
  ];
  
  Object.keys(data).forEach(field => {
    if (!data[field]) return;
    
    if (jsonFields.includes(field) && typeof data[field] === 'string') {
      try {
        if (data[field].trim().startsWith('{') || data[field].trim().startsWith('[')) {
          data[field] = JSON.parse(data[field]);
        }
      } catch (e) {
        console.error(`Failed to parse ${field}:`, e);
      }
    }
  });
  
  return data;
}


const fetchResumeDetails = async () => {
  try {
    loading.value = true
    const res = await request.get(`/users/me/resumes/${resumeId}`)
    if (res.success) {
      console.log('简历详情:', res.data)
      resume.value = parseResumeData(res.data)
    }
  } catch (error) {
    ElMessage.error('获取简历详情失败')
    console.error(error)
  } finally {
    loading.value = false
  }
}

const formatDate = (dateString) => {
  if (!dateString) return ''
  
  const date = new Date(dateString)
  return date.toLocaleDateString('zh-CN', {
    year: 'numeric',
    month: '2-digit',
    day: '2-digit'
  })
}

const formatDateRange = (dateRange) => {
  if (!dateRange || !Array.isArray(dateRange) || dateRange.length !== 2) return ''
  
  const startDate = dateRange[0] ? new Date(dateRange[0]) : null
  const endDate = dateRange[1] ? new Date(dateRange[1]) : null
  
  if (!startDate) return ''
  
  const startText = startDate.toLocaleDateString('zh-CN', { year: 'numeric', month: '2-digit' })
  const endText = endDate ? endDate.toLocaleDateString('zh-CN', { year: 'numeric', month: '2-digit' }) : '至今'
  
  return `${startText} 至 ${endText}`
}

const getVisibilityText = (status) => {
  const statusMap = {
    'Public': '公开给所有企业',
    'PublicToApplied': '仅对申请企业公开',
    'Private': '私密'
  }
  return statusMap[status] || status
}

const getVisibilityTagType = () => {
  const statusMap = {
    'Public': 'success',
    'PublicToApplied': 'warning',
    'Private': 'info'
  }
  return statusMap[resume.value.visibilityStatus] || ''
}

const getSkillTagType = (level) => {
  const levelMap = {
    '入门': 'info',
    '熟练': 'success',
    '精通': 'danger'
  }
  return levelMap[level] || ''
}

const editResume = () => {
  router.push(`/user/resume/edit/${resumeId}`)
}

const downloadPdf = () => {
  // 实际项目中，这里需要调用接口生成PDF并下载
  ElMessage.info('PDF导出功能开发中')
}

const downloadAttachment = () => {
  // 在实际项目中，可能需要处理文件下载
  if (resume.value.attachmentPath) {
    window.open(resume.value.attachmentPath, '_blank')
  }
}
</script>

<style scoped>
.resume-view-container {
  padding: 20px;
  background-color: white;
  border-radius: 4px;
  box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
}

.resume-view-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 30px;
  padding-bottom: 15px;
  border-bottom: 1px solid #EBEEF5;
}

.resume-view-header h2 {
  margin: 0 0 10px 0;
}

.loading-container {
  padding: 20px;
}

.resume-section {
  margin-bottom: 30px;
}

.section-title {
  position: relative;
  padding-left: 15px;
  margin-bottom: 15px;
  font-size: 18px;
}

.section-title::before {
  content: '';
  position: absolute;
  left: 0;
  top: 50%;
  transform: translateY(-50%);
  width: 4px;
  height: 18px;
  background-color: #409EFF;
  border-radius: 2px;
}

.section-content {
  padding: 0 10px;
}

.basic-info {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(200px, 1fr));
  gap: 15px;
}

.info-item {
  margin-bottom: 10px;
}

.info-label {
  font-weight: bold;
  color: #606266;
  margin-right: 5px;
}

.info-value {
  color: #303133;
}

.timeline-title {
  font-weight: bold;
  margin-bottom: 5px;
}

.timeline-description {
  color: #606266;
  white-space: pre-line;
}

.project-card {
  margin-bottom: 15px;
}

.project-description {
  white-space: pre-line;
  color: #606266;
}

.skill-tag {
  margin-right: 10px;
  margin-bottom: 10px;
}

.self-description {
  white-space: pre-line;
  color: #606266;
  line-height: 1.6;
}
</style>
