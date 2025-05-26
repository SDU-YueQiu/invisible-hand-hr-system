<template>
  <div class="resume-form-container">
    <div class="resume-form-header">
      <h2>{{ isEdit ? '编辑简历' : '创建简历' }}</h2>
    </div>
    
    <el-form 
      :model="resumeForm" 
      :rules="rules" 
      ref="resumeFormRef" 
      label-width="120px" 
      class="resume-form"
      v-loading="loading"
    >
      <!-- 简历基本信息 -->
      <h3>基本信息</h3>
      <el-form-item label="简历标题" prop="resumeTitle">
        <el-input v-model="resumeForm.resumeTitle" placeholder="给简历起一个名称" />
      </el-form-item>
      
      <!-- 个人基础信息 -->
      <el-form-item label="姓名" prop="basicInfo.name">
        <el-input v-model="resumeForm.basicInfo.name" placeholder="您的真实姓名" />
      </el-form-item>
      
      <el-form-item label="性别">
        <el-radio-group v-model="resumeForm.basicInfo.gender">
          <el-radio label="男">男</el-radio>
          <el-radio label="女">女</el-radio>
          <el-radio label="其他">其他</el-radio>
        </el-radio-group>
      </el-form-item>
      
      <el-form-item label="出生日期">
        <el-date-picker
          v-model="resumeForm.basicInfo.birthDate"
          type="date"
          placeholder="选择出生日期"
        />
      </el-form-item>
      
      <!-- 求职意向 -->
      <h3>求职意向</h3>
      <el-form-item label="期望行业">
        <el-input v-model="resumeForm.jobIntent.industry" placeholder="您期望就职的行业" />
      </el-form-item>
      
      <el-form-item label="期望职位">
        <el-input v-model="resumeForm.jobIntent.position" placeholder="您期望的职位" />
      </el-form-item>
      
      <el-form-item label="期望薪资">
        <el-input v-model="resumeForm.jobIntent.salary" placeholder="例如：8k-12k" />
      </el-form-item>
      
      <!-- 教育经历 -->
      <h3>教育经历 
        <el-button type="primary" size="small" circle @click="addEducation">
          <el-icon><Plus /></el-icon>
        </el-button>
      </h3>
      
      <el-card v-for="(edu, index) in resumeForm.educationExperience" :key="index" class="mb-20">
        <template #header>
          <div class="card-header">
            <span>教育经历 #{{ index + 1 }}</span>
            <el-button type="danger" text @click="removeEducation(index)">删除</el-button>
          </div>
        </template>
        
        <el-form-item :label="'学校名称'">
          <el-input v-model="edu.school" placeholder="学校名称" />
        </el-form-item>
        
        <el-form-item :label="'学历'">
          <el-select v-model="edu.degree" placeholder="选择学历">
            <el-option label="高中" value="高中" />
            <el-option label="大专" value="大专" />
            <el-option label="本科" value="本科" />
            <el-option label="硕士" value="硕士" />
            <el-option label="博士" value="博士" />
          </el-select>
        </el-form-item>
        
        <el-form-item :label="'专业'">
          <el-input v-model="edu.major" placeholder="所学专业" />
        </el-form-item>
        
        <el-form-item :label="'起止时间'">
          <el-date-picker
            v-model="edu.dateRange"
            type="daterange"
            range-separator="至"
            start-placeholder="开始日期"
            end-placeholder="结束日期"
          />
        </el-form-item>
      </el-card>
      
      <!-- 工作经历 -->
      <h3>工作经历
        <el-button type="primary" size="small" circle @click="addWork">
          <el-icon><Plus /></el-icon>
        </el-button>
      </h3>
      
      <el-card v-for="(work, index) in resumeForm.workExperience" :key="index" class="mb-20">
        <template #header>
          <div class="card-header">
            <span>工作经历 #{{ index + 1 }}</span>
            <el-button type="danger" text @click="removeWork(index)">删除</el-button>
          </div>
        </template>
        
        <el-form-item :label="'公司名称'">
          <el-input v-model="work.company" placeholder="公司名称" />
        </el-form-item>
        
        <el-form-item :label="'职位'">
          <el-input v-model="work.role" placeholder="担任职位" />
        </el-form-item>
        
        <el-form-item :label="'起止时间'">
          <el-date-picker
            v-model="work.dateRange"
            type="daterange"
            range-separator="至"
            start-placeholder="开始日期"
            end-placeholder="结束日期"
          />
        </el-form-item>
        
        <el-form-item :label="'工作描述'">
          <el-input type="textarea" v-model="work.description" rows="4" placeholder="简要描述您的工作内容和成就" />
        </el-form-item>
      </el-card>
      
      <!-- 项目经历 -->
      <h3>项目经历
        <el-button type="primary" size="small" circle @click="addProject">
          <el-icon><Plus /></el-icon>
        </el-button>
      </h3>
      
      <el-card v-for="(project, index) in resumeForm.projectExperience" :key="index" class="mb-20">
        <template #header>
          <div class="card-header">
            <span>项目经历 #{{ index + 1 }}</span>
            <el-button type="danger" text @click="removeProject(index)">删除</el-button>
          </div>
        </template>
        
        <el-form-item :label="'项目名称'">
          <el-input v-model="project.projectName" placeholder="项目名称" />
        </el-form-item>
        
        <el-form-item :label="'项目描述'">
          <el-input type="textarea" v-model="project.description" rows="4" placeholder="描述项目内容和您的角色职责" />
        </el-form-item>
      </el-card>
      
      <!-- 技能证书 -->
      <h3>技能证书
        <el-button type="primary" size="small" circle @click="addSkill">
          <el-icon><Plus /></el-icon>
        </el-button>
      </h3>
      
      <div class="skills-wrapper">
        <el-tag
          v-for="(skill, index) in resumeForm.skillsCertificates"
          :key="index"
          class="skill-tag"
          closable
          @close="removeSkill(index)"
        >
          {{ skill.skillName }}{{ skill.level ? ' (' + skill.level + ')' : '' }}
        </el-tag>
      </div>
      
      <el-dialog v-model="skillDialogVisible" title="添加技能或证书" width="30%">
        <el-form :model="newSkill" label-width="80px">
          <el-form-item label="技能名称">
            <el-input v-model="newSkill.skillName" placeholder="技能或证书名称" />
          </el-form-item>
          <el-form-item label="掌握程度">
            <el-select v-model="newSkill.level" placeholder="选择掌握程度">
              <el-option label="入门" value="入门" />
              <el-option label="熟练" value="熟练" />
              <el-option label="精通" value="精通" />
            </el-select>
          </el-form-item>
        </el-form>
        <template #footer>
          <span class="dialog-footer">
            <el-button @click="skillDialogVisible = false">取消</el-button>
            <el-button type="primary" @click="confirmAddSkill">确定</el-button>
          </span>
        </template>
      </el-dialog>
      
      <!-- 自我描述 -->
      <h3>自我描述</h3>
      <el-form-item>
        <el-input 
          type="textarea" 
          v-model="resumeForm.selfDescription" 
          rows="5" 
          placeholder="用简洁的语言描述您的性格特点、职业规划等" 
        />
      </el-form-item>
      
      <!-- 简历附件 -->
      <h3>简历附件</h3>
      <el-form-item>
        <el-upload
          action="/api/v1/upload"
          :on-success="handleAttachmentSuccess"
          :on-remove="handleAttachmentRemove"
          :before-upload="beforeAttachmentUpload"
          :limit="1"
        >
          <el-button type="primary">上传附件</el-button>
          <template #tip>
            <div class="el-upload__tip">支持的格式：PDF、Word文档（.docx, .pdf）</div>
          </template>
        </el-upload>
      </el-form-item>
      
      <!-- 简历可见性 -->
      <h3>简历可见性</h3>
      <el-form-item>
        <el-radio-group v-model="resumeForm.visibilityStatus">
          <el-radio label="PublicToAll">公开给所有企业</el-radio>
          <el-radio label="PublicToApplied">仅对申请企业公开</el-radio>
          <el-radio label="Private">完全私密</el-radio>
        </el-radio-group>
      </el-form-item>
      
      <!-- 操作按钮 -->
      <el-form-item class="form-actions">
        <el-button type="primary" @click="submitForm(resumeFormRef)" :loading="submitting">
          {{ isEdit ? '保存修改' : '创建简历' }}
        </el-button>
        <el-button @click="cancel">取消</el-button>
      </el-form-item>
    </el-form>
  </div>
</template>

<script setup>
import { ref, reactive, onMounted, computed } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import { ElMessage } from 'element-plus'
import { Plus } from '@element-plus/icons-vue'
import request from '../../utils/request'

const route = useRoute()
const router = useRouter()
const resumeId = computed(() => route.params.id)
const isEdit = computed(() => !!resumeId.value)

const resumeFormRef = ref()
const loading = ref(false)
const submitting = ref(false)
const skillDialogVisible = ref(false)

const resumeForm = reactive({
  resumeTitle: '',
  basicInfo: {
    name: '',
    gender: '男',
    birthDate: ''
  },
  jobIntent: {
    industry: '',
    position: '',
    salary: ''
  },
  educationExperience: [],
  workExperience: [],
  projectExperience: [],
  skillsCertificates: [],
  selfDescription: '',
  visibilityStatus: 'Private',
  attachmentPath: ''
})

const newSkill = reactive({
  skillName: '',
  level: ''
})

const rules = reactive({
  resumeTitle: [
    { required: true, message: '请输入简历标题', trigger: 'blur' }
  ],
  'basicInfo.name': [
    { required: true, message: '请输入您的姓名', trigger: 'blur' }
  ]
})

onMounted(async () => {
  if (isEdit.value) {
    await fetchResumeDetails()
  } else {
    // 初始化一个空的教育经历
    addEducation()
  }
})

const fetchResumeDetails = async () => {
  try {
    loading.value = true
    const res = await request.get(`/users/me/resumes/${resumeId.value}`)
    if (res.success) {
      Object.assign(resumeForm, res.data)
    } else {
      ElMessage.error(res.message || '获取简历详情失败')
    }
  } catch (error) {
    ElMessage.error('获取简历详情失败')
    console.error(error)
  } finally {
    loading.value = false
  }
}

// 教育经历相关操作
const addEducation = () => {
  resumeForm.educationExperience.push({
    school: '',
    degree: '',
    major: '',
    dateRange: []
  })
}

const removeEducation = (index) => {
  resumeForm.educationExperience.splice(index, 1)
}

// 工作经历相关操作
const addWork = () => {
  resumeForm.workExperience.push({
    company: '',
    role: '',
    dateRange: [],
    description: ''
  })
}

const removeWork = (index) => {
  resumeForm.workExperience.splice(index, 1)
}

// 项目经历相关操作
const addProject = () => {
  resumeForm.projectExperience.push({
    projectName: '',
    description: ''
  })
}

const removeProject = (index) => {
  resumeForm.projectExperience.splice(index, 1)
}

// 技能证书相关操作
const addSkill = () => {
  skillDialogVisible.value = true
  newSkill.skillName = ''
  newSkill.level = ''
}

const confirmAddSkill = () => {
  if (newSkill.skillName) {
    resumeForm.skillsCertificates.push({
      skillName: newSkill.skillName,
      level: newSkill.level
    })
    skillDialogVisible.value = false
  } else {
    ElMessage.warning('请输入技能名称')
  }
}

const removeSkill = (index) => {
  resumeForm.skillsCertificates.splice(index, 1)
}

// 附件上传相关操作
const handleAttachmentSuccess = (res) => {
  if (res.success) {
    resumeForm.attachmentPath = res.data.url
    ElMessage.success('附件上传成功')
  }
}

const handleAttachmentRemove = () => {
  resumeForm.attachmentPath = ''
}

const beforeAttachmentUpload = (file) => {
  const isValidType = file.type === 'application/pdf' || 
                      file.type === 'application/vnd.openxmlformats-officedocument.wordprocessingml.document'
  const isLt10M = file.size / 1024 / 1024 < 10

  if (!isValidType) {
    ElMessage.error('只能上传PDF或DOCX格式文件!')
  }
  if (!isLt10M) {
    ElMessage.error('文件大小不能超过10MB!')
  }
  
  return isValidType && isLt10M
}

// 表单提交
const submitForm = async (formEl) => {
  if (!formEl) return
  
  await formEl.validate(async (valid) => {
    if (valid) {
      submitting.value = true
      
      try {
        // 格式化表单数据
        const formattedData = formatResumeData()
        
        let res
        if (isEdit.value) {
          res = await request.put(`/users/me/resumes/${resumeId.value}`, formattedData)
        } else {
          res = await request.post('/users/me/resumes', formattedData)
        }
        
        if (res.success) {
          ElMessage.success(isEdit.value ? '简历更新成功' : '简历创建成功')
          router.push('/user/resumes')
        } else {
          ElMessage.error(res.message || (isEdit.value ? '更新简历失败' : '创建简历失败'))
        }
      } catch (error) {
        console.error('提交简历失败:', error)
        ElMessage.error(isEdit.value ? '更新简历失败' : '创建简历失败')
      } finally {
        submitting.value = false
      }
    }
  })
}

// 格式化数据用于API提交
const formatResumeData = () => {
  // 创建对象副本以避免修改原始对象
  const formattedData = JSON.parse(JSON.stringify(resumeForm))
  
  // 处理日期范围数据格式
  if (formattedData.educationExperience) {
    formattedData.educationExperience.forEach(edu => {
      if (edu.dateRange && edu.dateRange.length === 2) {
        edu.startDate = edu.dateRange[0]
        edu.endDate = edu.dateRange[1]
        delete edu.dateRange
      }
    })
  }
  
  if (formattedData.workExperience) {
    formattedData.workExperience.forEach(work => {
      if (work.dateRange && work.dateRange.length === 2) {
        work.startDate = work.dateRange[0]
        work.endDate = work.dateRange[1]
        delete work.dateRange
      }
    })
  }
  
  return formattedData
}

const cancel = () => {
  router.push('/user/resumes')
}
</script>

<style scoped>
.resume-form-container {
  padding: 20px;
  background-color: white;
  border-radius: 4px;
  box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
}

.resume-form-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20px;
}

.resume-form {
  margin-top: 20px;
}

h3 {
  margin-top: 30px;
  margin-bottom: 15px;
  border-bottom: 1px solid #EBEEF5;
  padding-bottom: 10px;
  display: flex;
  align-items: center;
  gap: 10px;
}

.mb-20 {
  margin-bottom: 20px;
}

.card-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.skills-wrapper {
  display: flex;
  flex-wrap: wrap;
  gap: 10px;
  margin-bottom: 20px;
}

.skill-tag {
  margin-right: 0;
}

.form-actions {
  margin-top: 40px;
}
</style>
