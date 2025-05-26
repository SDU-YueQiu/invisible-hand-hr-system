<template>
  <div class="profile-container">
    <h2>个人资料</h2>
    
    <el-form 
      :model="userForm" 
      :rules="rules" 
      ref="userFormRef" 
      label-width="100px" 
      class="user-form"
      v-loading="loading"
    >
      <el-form-item label="用户名">
        <div>{{ userForm.username }}</div>
      </el-form-item>
      
      <el-form-item label="手机号码" prop="phoneNumber">
        <el-input v-model="userForm.phoneNumber" placeholder="请输入手机号码" />
      </el-form-item>
      
      <el-form-item label="电子邮箱" prop="email">
        <el-input v-model="userForm.email" placeholder="请输入电子邮箱" />
      </el-form-item>
      
      <el-form-item label="头像">
        <el-upload
          class="avatar-uploader"
          action="/api/v1/upload"
          :show-file-list="false"
          :on-success="handleAvatarSuccess"
          :before-upload="beforeAvatarUpload"
        >
          <img v-if="userForm.avatarURL" :src="userForm.avatarURL" class="avatar" />
          <el-icon v-else class="avatar-uploader-icon"><Plus /></el-icon>
        </el-upload>
      </el-form-item>
      
      <el-form-item>
        <el-button type="primary" @click="submitForm(userFormRef)" :loading="submitting">保存修改</el-button>
      </el-form-item>
    </el-form>
  </div>
</template>

<script setup>
import { ref, reactive, onMounted } from 'vue'
import { ElMessage } from 'element-plus'
import { Plus } from '@element-plus/icons-vue'
import request from '../../utils/request'

const userFormRef = ref()
const loading = ref(false)
const submitting = ref(false)

const userForm = reactive({
  username: '',
  phoneNumber: '',
  email: '',
  avatarURL: '',
  registrationDate: '',
  lastLoginDate: '',
  accountStatus: ''
})

const rules = reactive({
  phoneNumber: [
    { pattern: /^1[3-9]\d{9}$/, message: '请输入正确的手机号码', trigger: 'blur' }
  ],
  email: [
    { type: 'email', message: '请输入正确的邮箱地址', trigger: 'blur' }
  ]
})

onMounted(async () => {
  await fetchUserProfile()
})

const fetchUserProfile = async () => {
  try {
    loading.value = true
    const res = await request.get('/users/me')
    if (res.success) {
      Object.assign(userForm, res.data)
    } else {
      ElMessage.error(res.message || '获取用户信息失败')
    }
  } catch (error) {
    ElMessage.error('获取用户信息失败')
    console.error(error)
  } finally {
    loading.value = false
  }
}

const handleAvatarSuccess = (res) => {
  if (res.success) {
    userForm.avatarURL = res.data.url
    ElMessage.success('头像上传成功')
  }
}

const beforeAvatarUpload = (file) => {
  const isJPG = file.type === 'image/jpeg' || file.type === 'image/png'
  const isLt2M = file.size / 1024 / 1024 < 2

  if (!isJPG) {
    ElMessage.error('头像只能上传JPG或PNG格式图片!')
  }
  if (!isLt2M) {
    ElMessage.error('头像图片大小不能超过2MB!')
  }
  
  return isJPG && isLt2M
}

const submitForm = async (formEl) => {
  if (!formEl) return
  
  await formEl.validate(async (valid) => {
    if (valid) {
      try {
        submitting.value = true
        const updatedData = {
          phoneNumber: userForm.phoneNumber,
          email: userForm.email,
          avatarURL: userForm.avatarURL
        }
        
        const res = await request.put('/users/me', updatedData)
        if (res.success) {
          ElMessage.success('个人资料更新成功')
        } else {
          ElMessage.error(res.message || '更新个人资料失败')
        }
      } catch (error) {
        ElMessage.error('更新个人资料失败')
        console.error(error)
      } finally {
        submitting.value = false
      }
    }
  })
}
</script>

<style scoped>
.profile-container {
  padding: 20px;
  background-color: white;
  border-radius: 4px;
  box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
}

.user-form {
  max-width: 600px;
  margin-top: 20px;
}

.avatar-uploader {
  width: 100px;
  height: 100px;
  border: 1px dashed #d9d9d9;
  border-radius: 6px;
  cursor: pointer;
  overflow: hidden;
}

.avatar-uploader:hover {
  border-color: #409EFF;
}

.avatar-uploader-icon {
  font-size: 28px;
  color: #8c939d;
  width: 100px;
  height: 100px;
  line-height: 100px;
  text-align: center;
}

.avatar {
  width: 100%;
  height: 100%;
  display: block;
  object-fit: cover;
}
</style>
