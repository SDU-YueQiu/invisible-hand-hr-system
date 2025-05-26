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
      
      <el-form-item label="工作地点">
        <el-input v-model="jobForm.location" placeholder="请输入工作地点" />
      </el-form-item>
      
      <!-- 岗位需求 -->
      <h3>岗位需求</h3>
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
      
      <el-form-item label="岗位描述">
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
import request from '../../utils/request'

const route = useRoute()
const router = useRouter()
const jobId = computed(() => route.params.id)
const isEdit = computed(() => !!jobId.value)

const jobFormRef = ref()
const loading = ref(false)
const submitting = ref(false)
const inputSkillVisible = ref(false)
const inputSkillValue = ref('')

const jobForm = reactive({
  jobTitle: '',
  jobType: '',
  location: '',
  salaryRange: '',
  requiredSkills: [],
  jobDescription: '',
  attachmentPath: ''
})

const rules = reactive({
  jobTitle: [
    { required: true, message: '请输入岗位名称', trigger: 'blur' }
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
    const res = await request.get(`/jobs/${jobId.value}`)
    if (res.success) {
      Object.assign(jobForm, res.data)
    } else {
      ElMessage.error(res.message || '获取岗位详情失败')
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

// 表单提交
const submitForm = async (formEl) => {
  if (!formEl) return
  
  await formEl.validate(async (valid) => {
    if (valid) {
      submitting.value = true
      
      try {
        let res
        if (isEdit.value) {
          res = await request.put(`/jobs/${jobId.value}`, jobForm)
        } else {
          res = await request.post('/jobs', jobForm)
        }
        
        if (res.success) {
          ElMessage.success(isEdit.value ? '岗位更新成功' : '岗位创建成功')
          router.push('/enterprise/jobs')
        } else {
          ElMessage.error(res.message || (isEdit.value ? '更新岗位失败' : '创建岗位失败'))
        }
      } catch (error) {
        console.error('提交岗位失败:', error)
        ElMessage.error(isEdit.value ? '更新岗位失败' : '创建岗位失败')
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