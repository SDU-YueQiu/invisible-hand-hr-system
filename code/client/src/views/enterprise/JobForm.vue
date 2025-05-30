<template>
  <div class="job-form-container">
    <div class="job-form-header">
      <h2>{{ isEdit ? '岗位变更' : '创建岗位' }}</h2>
    </div>
    
    <el-form 
      :model="jobForm" 
      :rules="rules" 
      ref="jobFormRef" 
      label-width="120px" 
      class="job-form"
      v-loading="loading"
    >
      <!-- 岗位基本信息 -->
      <h3>基本信息</h3>
      <el-form-item label="岗位名称" prop="jobTitle">
        <el-input v-model="jobForm.jobTitle" placeholder="请输入岗位名称" />
      </el-form-item>
      
      <el-form-item label="岗位类型">
        <el-select v-model="jobForm.jobType" placeholder="请选择岗位类型">
          <el-option label="技术类" value="技术类" />
          <el-option label="产品类" value="产品类" />
          <el-option label="设计类" value="设计类" />
          <el-option label="运营类" value="运营类" />
          <el-option label="市场类" value="市场类" />
        </el-select>
      </el-form-item>
      
      <el-form-item label="工作地点" prop="location">
        <el-input v-model="jobForm.location" placeholder="请输入工作地点" />
      </el-form-item>
      
      <!-- 岗位需求 -->
      <h3>岗位需求</h3>
      <el-form-item label="招聘人数">
        <el-input-number v-model="jobForm.recruitmentCount" :min="1" placeholder="招聘人数" />
      </el-form-item>
      
      <el-form-item label="工作要求" prop="requirements">
        <el-input 
          type="textarea" 
          v-model="jobForm.requirements" 
          rows="3" 
          placeholder="请输入工作要求" 
        />
      </el-form-item>
      
      <el-form-item label="经验要求">
        <el-select v-model="jobForm.experienceRequired" placeholder="请选择经验要求">
          <el-option label="不限" value="不限" />
          <el-option label="1年以下" value="1年以下" />
          <el-option label="1-3年" value="1-3年" />
          <el-option label="3-5年" value="3-5年" />
          <el-option label="5年以上" value="5年以上" />
        </el-select>
      </el-form-item>
      
      <el-form-item label="学历要求">
        <el-select v-model="jobForm.educationRequired" placeholder="请选择学历要求">
          <el-option label="不限" value="不限" />
          <el-option label="大专" value="大专" />
          <el-option label="本科" value="本科" />
          <el-option label="硕士" value="硕士" />
          <el-option label="博士" value="博士" />
        </el-select>
      </el-form-item>
      
      <el-form-item label="福利待遇">
        <el-input 
          type="textarea" 
          v-model="jobForm.benefits" 
          rows="3" 
          placeholder="请输入福利待遇" 
        />
      </el-form-item>
      
      <el-form-item label="截止日期">
        <el-date-picker
          v-model="jobForm.deadlineDate"
          type="date"
          placeholder="选择截止日期"
          format="YYYY-MM-DD"
          value-format="YYYY-MM-DD"
        />
      </el-form-item>
      
      <el-form-item label="预期薪资">
        <el-input v-model="jobForm.salaryRange" placeholder="例如：8k-12k" />
      </el-form-item>
      
      <el-form-item label="所需技能">
        <el-tag
          v-for="(skill, index) in jobForm.requiredSkills"
          :key="index"
          class="skill-tag"
          closable
          @close="removeSkill(index)"
        >
          {{ skill }}
        </el-tag>
        <el-input
          class="input-new-tag"
          v-if="inputSkillVisible"
          v-model="inputSkillValue"
          ref="saveTagInput"
          size="small"
          @keyup.enter="handleSkillInputConfirm"
          @blur="handleSkillInputConfirm"
        />
        <el-button v-else class="button-new-tag" size="small" @click="showInput">
          + 添加技能
        </el-button>
      </el-form-item>
      
      <el-form-item label="岗位描述" prop="jobDescription">
        <el-input 
          type="textarea" 
          v-model="jobForm.jobDescription" 
          rows="5" 
          placeholder="请输入岗位职责和要求" 
        />
      </el-form-item>
    
      <!-- 操作按钮 -->
      <el-form-item class="form-actions">
        <el-button type="primary" @click="submitForm(jobFormRef)" :loading="submitting">
          {{ isEdit ? '保存修改' : '创建岗位' }}
        </el-button>
        <el-button @click="cancel">取消</el-button>
      </el-form-item>
    </el-form>
  </div>
</template>

<script setup>
import { ref, reactive, onMounted, computed, nextTick } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import { ElMessage } from 'element-plus'
import { useEnterpriseStore } from '../../stores'
import axios from 'axios'

const route = useRoute()
const router = useRouter()
const jobId = computed(() => route.params.id)
const isEdit = computed(() => !!jobId.value)

const jobFormRef = ref()
const saveTagInput = ref()
const loading = ref(false)
const submitting = ref(false)
const inputSkillVisible = ref(false)
const inputSkillValue = ref('')
const enterpriseStore = useEnterpriseStore()

const jobForm = reactive({
  jobTitle: '',
  jobType: '', // 在提交时映射为 jobCategory
  location: '', // 在提交时映射为 workLocation
  salaryRange: '', // 需要解析为 minSalary 和 maxSalary
  requirements: '',
  experienceRequired: '',
  educationRequired: '',
  benefits: '',
  recruitmentCount: null,
  deadlineDate: '',
  requiredSkills: [], // 合并到 requirements 中
  jobDescription: '' // 在提交时映射为 responsibilities
})

const rules = reactive({
  jobTitle: [
    { required: true, message: '请输入岗位名称', trigger: 'blur' }
  ],
  location: [
    { required: true, message: '请输入工作地点', trigger: 'blur' }
  ],
  jobDescription: [
    { required: true, message: '请输入岗位职责', trigger: 'blur' }
  ],
  requirements: [
    { required: true, message: '请输入工作要求', trigger: 'blur' }
  ]
})

onMounted(async () => {
  if (isEdit.value) {
    await fetchJobDetails()
  }
})

const fetchJobDetails = async () => {
  try {
    loading.value = true
    
    // 检查是否有 token
    if (!enterpriseStore.token) {
      ElMessage.error('未登录或登录已过期，请重新登录')
      router.push('/login')
      return
    }
    
    const res = await axios.get(`http://localhost:8080/api/v1/enterprises/me/jobs/${jobId.value}`, {
      headers: {
        'Authorization': `Bearer ${enterpriseStore.token}`
      }
    })
    
    if (res.data.success) {
      const data = res.data.data
      // 将API返回的数据映射到表单字段
      Object.assign(jobForm, {
        jobTitle: data.jobTitle || '',
        jobType: data.jobCategory || '',
        location: data.workLocation || '',
        salaryRange: data.minSalary && data.maxSalary ? `${data.minSalary}-${data.maxSalary}` : '',
        requirements: data.requirements || '',
        experienceRequired: data.experienceRequired || '',
        educationRequired: data.educationRequired || '',
        benefits: data.benefits || '',
        recruitmentCount: data.recruitmentCount || null,
        deadlineDate: data.deadlineDate || '',
        requiredSkills: [], // 如果API返回技能数组，在这里处理
        jobDescription: data.responsibilities || ''
      })
    } else {
      ElMessage.error(res.data.message || '获取岗位详情失败')
    }
  } catch (error) {
    ElMessage.error('获取岗位详情失败')
    console.error(error)
  } finally {
    loading.value = false
  }
}

// 技能标签相关操作
const removeSkill = (index) => {
  jobForm.requiredSkills.splice(index, 1)
}

const showInput = () => {
  inputSkillVisible.value = true
  nextTick(() => {
    saveTagInput.value.input.focus()
  })
}

const handleSkillInputConfirm = () => {
  if (inputSkillValue.value) {
    jobForm.requiredSkills.push(inputSkillValue.value)
  }
  inputSkillVisible.value = false
  inputSkillValue.value = ''
}

// 解析薪资范围的辅助函数
const parseSalaryRange = (salaryRange) => {
  if (!salaryRange) return {}
  
  const match = salaryRange.match(/(\d+)k?-(\d+)k?/i)
  if (match) {
    const minValue = parseInt(match[1])
    const maxValue = parseInt(match[2])
    return {
      minSalary: salaryRange.toLowerCase().includes('k') ? minValue * 1000 : minValue,
      maxSalary: salaryRange.toLowerCase().includes('k') ? maxValue * 1000 : maxValue
    }
  }
  return {}
}

// 表单提交
const submitForm = async (formEl) => {
  if (!formEl) return
  
  await formEl.validate(async (valid) => {
    if (valid) {
      submitting.value = true
      
      try {
        // 检查是否有 token
        if (!enterpriseStore.token) {
          ElMessage.error('未登录或登录已过期，请重新登录')
          router.push('/login')
          return
        }
        
        // 将表单数据映射为 API 期望的格式
        const apiData = {
          jobTitle: jobForm.jobTitle,
          jobCategory: jobForm.jobType,
          workLocation: jobForm.location,
          responsibilities: jobForm.jobDescription,
          requirements: jobForm.requirements + (jobForm.requiredSkills.length > 0 ? '\n所需技能: ' + jobForm.requiredSkills.join('、') : ''),
          experienceRequired: jobForm.experienceRequired,
          educationRequired: jobForm.educationRequired,
          benefits: jobForm.benefits,
          recruitmentCount: jobForm.recruitmentCount,
          deadlineDate: jobForm.deadlineDate,
          ...parseSalaryRange(jobForm.salaryRange)
        }
        
        const config = {
          headers: {
            'Authorization': `Bearer ${enterpriseStore.token}`,
            'Content-Type': 'application/json'
          }
        }
        
        let res
        const baseURL = 'http://localhost:8080/api/v1'
        if (isEdit.value) {
          res = await axios.put(`${baseURL}/enterprises/me/jobs/${jobId.value}`, apiData, config)
        } else {
          res = await axios.post(`${baseURL}/enterprises/me/jobs`, apiData, config)
        }
        
        if (res.status === 200 || res.status === 201) { 
          console.log("response",res)
          ElMessage.success(isEdit.value ? '岗位更新成功' : '岗位创建成功')
          router.push('/enterprise/jobs')
        } else {
          ElMessage.error(res.data.message || (isEdit.value ? '更新岗位失败' : '创建岗位失败'))
        }
      } catch (error) {
        console.error('提交岗位失败:', error)
        // 打印详细错误信息
        if (error.response) {
          console.error('错误响应:', error.response.data)
          ElMessage.error(error.response.data.message || '请求失败')
        } else {
          ElMessage.error(isEdit.value ? '更新岗位失败' : '创建岗位失败')
        }
      } finally {
        submitting.value = false
      }
    }
  })
}

const cancel = () => {
  router.push('/enterprise/jobs')
}
</script>

<style scoped>
.job-form-container {
  padding: 20px;
  background-color: white;
  border-radius: 4px;
  box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
}

.job-form-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20px;
}

.job-form {
  margin-top: 20px;
}

h3 {
  margin-top: 30px;
  margin-bottom: 15px;
  border-bottom: 1px solid #EBEEF5;
  padding-bottom: 10px;
}

.skill-tag {
  margin-right: 10px;
}

.button-new-tag {
  margin-left: 10px;
  height: 32px;
  line-height: 30px;
  padding-top: 0;
  padding-bottom: 0;
}

.input-new-tag {
  width: 90px;
  margin-left: 10px;
  vertical-align: bottom;
}

.form-actions {
  margin-top: 40px;
}
</style>
