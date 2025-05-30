<template>
  <div class="enterprise-profile-container">
    <h2>企业资料</h2>
    
    <el-form 
      :model="enterpriseForm" 
      :rules="rules" 
      ref="enterpriseFormRef" 
      label-width="120px" 
      class="enterprise-form"
      v-loading="loading"
    >
      <!-- 基本信息 -->
      <h3>基本信息</h3>
      <el-form-item label="企业名称" prop="enterpriseName">
        <el-input v-model="enterpriseForm.enterpriseName" placeholder="请输入企业名称" />
      </el-form-item>
      
      <el-form-item label="统一信用代码" prop="creditCode">
        <el-input v-model="enterpriseForm.creditCode" placeholder="请输入统一信用代码" />
      </el-form-item>
      
      <el-form-item label="所属行业">
        <el-select v-model="enterpriseForm.industry" placeholder="请选择所属行业">
          <el-option label="互联网/IT" value="互联网/IT" />
          <el-option label="金融" value="金融" />
          <el-option label="制造业" value="制造业" />
          <el-option label="教育" value="教育" />
          <el-option label="医疗" value="医疗" />
          <el-option label="其他" value="其他" />
        </el-select>
      </el-form-item>
      
      <el-form-item label="企业规模">
        <el-select v-model="enterpriseForm.scale" placeholder="请选择企业规模">
          <el-option label="1-50人" value="1-50人" />
          <el-option label="50-200人" value="50-200人" />
          <el-option label="200-500人" value="200-500人" />
          <el-option label="500-1000人" value="500-1000人" />
          <el-option label="1000人以上" value="1000人以上" />
        </el-select>
      </el-form-item>
      
      <el-form-item label="企业地址">
        <el-input v-model="enterpriseForm.address" placeholder="请输入企业地址" />
      </el-form-item>
      
      <el-form-item label="企业描述">
        <el-input 
          type="textarea" 
          v-model="enterpriseForm.description" 
          rows="4" 
          placeholder="请简要描述您的企业" 
        />
      </el-form-item>
      
      <!-- 联系信息 -->
      <h3>联系信息</h3>
      <el-form-item label="联系人" prop="contactPerson">
        <el-input v-model="enterpriseForm.contactPerson" placeholder="请输入联系人姓名" />
      </el-form-item>
      
      <el-form-item label="联系电话" prop="contactPhone">
        <el-input v-model="enterpriseForm.contactPhone" placeholder="请输入联系电话" />
      </el-form-item>
      
      <el-form-item label="联系邮箱" prop="contactEmail">
        <el-input v-model="enterpriseForm.contactEmail" placeholder="请输入联系邮箱" />
      </el-form-item>
      
      <!-- 企业资质 -->
      <h3>企业资质</h3>
      <el-form-item label="企业LOGO">
        <el-upload
          class="avatar-uploader"
          action="/api/v1/upload"
          :show-file-list="false"
          :on-success="handleLogoSuccess"
          :before-upload="beforeLogoUpload"
          :headers="uploadHeaders"
        >
          <img v-if="enterpriseForm.logoURL" :src="enterpriseForm.logoURL" class="avatar" />
          <el-icon v-else class="avatar-uploader-icon"><Plus /></el-icon>
        </el-upload>
      </el-form-item>
      
      <el-form-item label="营业执照">
        <el-upload
          class="license-uploader"
          action="/api/v1/upload"
          :show-file-list="false"
          :on-success="handleLicenseSuccess"
          :before-upload="beforeLicenseUpload"
          :headers="uploadHeaders"
        >
          <img v-if="enterpriseForm.licenseImageURL" :src="enterpriseForm.licenseImageURL" class="license" />
          <el-icon v-else class="license-uploader-icon"><Plus /></el-icon>
        </el-upload>
      </el-form-item>
      
      <!-- 只读信息 -->
      <h3>系统信息</h3>
      <el-form-item label="企业ID">
        <div>{{ enterpriseForm.enterpriseId }}</div>
      </el-form-item>
      
      <el-form-item label="账户状态">
        <el-tag :type="getStatusTagType(enterpriseForm.accountStatus)">
          {{ getStatusText(enterpriseForm.accountStatus) }}
        </el-tag>
      </el-form-item>
      
      <el-form-item label="审核意见" v-if="enterpriseForm.auditOpinion">
        <div>{{ enterpriseForm.auditOpinion }}</div>
      </el-form-item>
      
      <!-- 操作按钮 -->
      <el-form-item class="form-actions">
        <el-button type="primary" @click="submitForm(enterpriseFormRef)" :loading="submitting">保存修改</el-button>
        <el-button @click="resetForm">重置</el-button>
      </el-form-item>
    </el-form>
  </div>
</template>

<script setup>
import { ref, reactive, onMounted, computed } from 'vue'
import { ElMessage } from 'element-plus'
import { Plus } from '@element-plus/icons-vue'
import { useEnterpriseStore } from '../../stores'
import axios from 'axios'
import { useRouter } from 'vue-router'

const enterpriseStore = useEnterpriseStore()
const router = useRouter()
const enterpriseFormRef = ref()
const loading = ref(false)
const submitting = ref(false)

// 上传组件的认证头
const uploadHeaders = computed(() => {
  return {
    'Authorization': `Bearer ${enterpriseStore.token}`
  }
})

const enterpriseForm = reactive({
  enterpriseId: '',
  enterpriseName: '',
  creditCode: '',
  description: '',
  industry: '',
  scale: '',
  address: '',
  contactPerson: '',
  contactPhone: '',
  contactEmail: '',
  logoURL: '',
  licenseImageURL: '',
  accountStatus: '',
  auditOpinion: ''
})

const rules = reactive({
  enterpriseName: [
    { required: true, message: '请输入企业名称', trigger: 'blur' }
  ],
  creditCode: [
    { required: true, message: '请输入统一信用代码', trigger: 'blur' },
    { pattern: /^[0-9a-z]{5}$/, message: '请输入正确的统一信用代码', trigger: 'blur' }
  ],
  contactPerson: [
    { required: true, message: '请输入联系人姓名', trigger: 'blur' }
  ],
  contactPhone: [
    { required: true, message: '请输入联系电话', trigger: 'blur' },
    { pattern: /^1[3-9]\d{9}$/, message: '请输入正确的手机号码', trigger: 'blur' }
  ],
  contactEmail: [
    { required: true, message: '请输入联系邮箱', trigger: 'blur' },
    { type: 'email', message: '请输入正确的邮箱地址', trigger: 'blur' }
  ]
})

// 设置axios默认baseURL
axios.defaults.baseURL = 'http://localhost:8080'

// 添加请求拦截器
axios.interceptors.request.use(
  config => {
    const token = enterpriseStore.token
    if (token) {
      config.headers['Authorization'] = `Bearer ${token}`
    }
    return config
  },
  error => {
    return Promise.reject(error)
  }
)

// 添加响应拦截器
axios.interceptors.response.use(
  response => response,
  error => {
    if (error.response && error.response.status === 401) {
      ElMessage.error('登录已过期，请重新登录')
      enterpriseStore.logout()
      router.push('/login')
      return Promise.reject(error)
    }
    return Promise.reject(error)
  }
)

onMounted(async () => {
  await fetchEnterpriseProfile()
})

const fetchEnterpriseProfile = async () => {
  try {
    loading.value = true
    const res = await axios.get('/api/v1/enterprises/me')
    console.log('完整响应:', res)
    console.log('数据内容:', res.data)
    
    if (res.status === 200 && res.data) {
      // 清空表单，然后设置新值
      Object.keys(enterpriseForm).forEach(key => {
        enterpriseForm[key] = ''
      })
      
      // 将后端数据复制到表单中
      for (const key in res.data) {
        if (key in enterpriseForm) {
          enterpriseForm[key] = res.data[key]
        }
      }
      
      console.log('更新后的表单数据:', enterpriseForm)
    } else {
      ElMessage.error('获取企业信息失败: 返回数据格式错误')
    }
  } catch (error) {
    handleApiError(error, '获取企业信息失败')
  } finally {
    loading.value = false
  }
}


// 通用错误处理函数
const handleApiError = (error, defaultMessage) => {
  if (error.response) {
    // 请求已发出，服务器以状态码响应
    if (error.response.status === 401) {
      ElMessage.error('您的登录已过期，请重新登录')
      enterpriseStore.logout()
      router.push('/login')
    } else if (error.response.status === 403) {
      ElMessage.error('您没有权限执行此操作')
    } else {
      ElMessage.error(error.response.data?.message || defaultMessage)
    }
  } else if (error.request) {
    // 请求已发出，但没有收到响应
    ElMessage.error('服务器无响应，请稍后再试')
  } else {
    // 在设置请求时出错
    ElMessage.error(defaultMessage)
  }
  console.error(error)
}

// 账户状态文本
const getStatusText = (status) => {
  switch(status) {
    case 'Verified': return '已认证'
    case 'PendingReview': return '待审核'
    case 'Rejected': return '未通过'
    default: return status
  }
}

// 账户状态标签样式
const getStatusTagType = (status) => {
  switch(status) {
    case 'Verified': return 'success'
    case 'PendingReview': return 'warning'
    case 'Rejected': return 'danger'
    default: return 'info'
  }
}

// 图片上传处理
const handleLogoSuccess = (res) => {
  if (res.success) {
    enterpriseForm.logoURL = res.data.url
    ElMessage.success('LOGO上传成功')
  } else {
    ElMessage.error(res.message || 'LOGO上传失败')
  }
}

const beforeLogoUpload = (file) => {
  const isImage = file.type === 'image/jpeg' || file.type === 'image/png'
  const isLt2M = file.size / 1024 / 1024 < 2

  if (!isImage) {
    ElMessage.error('LOGO只能上传JPG或PNG格式图片!')
  }
  if (!isLt2M) {
    ElMessage.error('LOGO图片大小不能超过2MB!')
  }
  
  return isImage && isLt2M
}

const handleLicenseSuccess = (res) => {
  if (res.success) {
    enterpriseForm.licenseImageURL = res.data.url
    ElMessage.success('营业执照上传成功')
  } else {
    ElMessage.error(res.message || '营业执照上传失败')
  }
}

const beforeLicenseUpload = (file) => {
  const isImage = file.type === 'image/jpeg' || file.type === 'image/png'
  const isLt5M = file.size / 1024 / 1024 < 5

  if (!isImage) {
    ElMessage.error('营业执照只能上传JPG或PNG格式图片!')
  }
  if (!isLt5M) {
    ElMessage.error('营业执照图片大小不能超过5MB!')
  }
  
  return isImage && isLt5M
}

const submitForm = async (formEl) => {
  if (!formEl) return
  
  await formEl.validate(async (valid) => {
    if (valid) {
      try {
        submitting.value = true
        const res = await axios.put(`/api/v1/enterprises/me`, enterpriseForm)
        console.log('更新响应:', res)
        if (res.status === 200) {
          ElMessage.success('企业资料更新成功')
          // 重新获取最新数据
          await fetchEnterpriseProfile()
        } else {
          ElMessage.error('更新企业资料失败')
        }
      } catch (error) {
        handleApiError(error, '更新企业资料失败')
      } finally {
        submitting.value = false
      }
    }
  })
}

const resetForm = () => {
  fetchEnterpriseProfile()
}
</script>

<style scoped>
.enterprise-profile-container {
  padding: 20px;
  background-color: white;
  border-radius: 4px;
  box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
}

.enterprise-form {
  max-width: 800px;
  margin-top: 20px;
}

h3 {
  margin-top: 30px;
  margin-bottom: 15px;
  border-bottom: 1px solid #EBEEF5;
  padding-bottom: 10px;
}

.avatar-uploader, .license-uploader {
  width: 150px;
  height: 150px;
  border: 1px dashed #d9d9d9;
  border-radius: 6px;
  cursor: pointer;
  overflow: hidden;
}

.avatar-uploader:hover, .license-uploader:hover {
  border-color: #409EFF;
}

.avatar-uploader-icon, .license-uploader-icon {
  font-size: 28px;
  color: #8c939d;
  width: 150px;
  height: 150px;
  line-height: 150px;
  text-align: center;
}

.avatar, .license {
  width: 100%;
  height: 100%;
  display: block;
  object-fit: contain;
}

.form-actions {
  margin-top: 40px;
}
</style>

