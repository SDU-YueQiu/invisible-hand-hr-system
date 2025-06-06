<template>
  <div class="settings-container">
    <h2>账号设置</h2>
    
    <el-tabs v-model="activeTab">
      <el-tab-pane label="修改密码" name="password">
        <el-form 
          :model="passwordForm" 
          :rules="passwordRules" 
          ref="passwordFormRef" 
          label-width="100px"
          class="settings-form"
        >
          <el-form-item label="当前密码" prop="oldPassword">
            <el-input 
              v-model="passwordForm.oldPassword" 
              type="password" 
              placeholder="请输入当前密码"
              show-password
            />
          </el-form-item>
          
          <el-form-item label="新密码" prop="newPassword">
            <el-input 
              v-model="passwordForm.newPassword" 
              type="password" 
              placeholder="请输入新密码（至少6位，包含字母和数字）"
              show-password
            />
          </el-form-item>
          
          <el-form-item label="确认新密码" prop="confirmPassword">
            <el-input 
              v-model="passwordForm.confirmPassword" 
              type="password" 
              placeholder="请再次输入新密码"
              show-password
            />
          </el-form-item>
          
          <el-form-item>
            <el-button 
              type="primary" 
              @click="submitPasswordForm(passwordFormRef)" 
              :loading="passwordLoading"
            >
              保存修改
            </el-button>
          </el-form-item>
        </el-form>
      </el-tab-pane>
      
      <el-tab-pane label="隐私设置" name="privacy">
        <el-form class="settings-form">
          <el-form-item label="简历默认可见性">
            <el-select v-model="privacySettings.defaultResumeVisibility">
              <el-option label="公开给所有企业" value="Public" />
              <el-option label="仅对申请企业公开" value="PublicToApplied" />
              <el-option label="完全私密" value="Private" />
            </el-select>
          </el-form-item>
          
          <el-form-item label="求职状态">
            <el-select v-model="privacySettings.jobSeekingStatus">
              <el-option label="在找工作" value="Active" />
              <el-option label="暂不找工作" value="Inactive" />
              <el-option label="仅查看机会" value="Passive" />
            </el-select>
          </el-form-item>
          
          <el-form-item>
            <el-button 
              type="primary" 
              @click="savePrivacySettings" 
              :loading="privacyLoading"
            >
              保存设置
            </el-button>
          </el-form-item>
        </el-form>
      </el-tab-pane>
      
      <el-tab-pane label="通知设置" name="notifications">
        <el-form class="settings-form">
          <el-form-item label="邮件通知">
            <el-switch v-model="notificationSettings.emailEnabled" />
          </el-form-item>
          
          <el-form-item label="接收内容">
            <el-checkbox-group v-model="notificationSettings.emailTypes">
              <el-checkbox label="application">职位申请状态变更</el-checkbox>
              <el-checkbox label="interview">面试邀请</el-checkbox>
              <el-checkbox label="system">系统公告</el-checkbox>
            </el-checkbox-group>
          </el-form-item>
          
          <el-form-item>
            <el-button 
              type="primary" 
              @click="saveNotificationSettings" 
              :loading="notificationLoading"
            >
              保存设置
            </el-button>
          </el-form-item>
        </el-form>
      </el-tab-pane>
      
      <el-tab-pane label="账号安全" name="security">
        <div class="security-info">
          <div class="info-item">
            <span class="label">账号创建时间：</span>
            <span>{{ formatDate(userInfo.registrationDate) }}</span>
          </div>
          <div class="info-item">
            <span class="label">最近登录时间：</span>
            <span>{{ formatDate(userInfo.lastLoginDate) }}</span>
          </div>
          <div class="info-item">
            <span class="label">账号状态：</span>
            <el-tag type="success" v-if="userInfo.accountStatus === 'Active'">正常</el-tag>
            <el-tag type="danger" v-else>异常</el-tag>
          </div>
        </div>
        
        <div class="danger-zone">
          <h3>危险操作区</h3>
          <el-button type="danger" @click="showDeactivateDialog">注销账号</el-button>
          <el-dialog
            v-model="deactivateDialogVisible"
            title="账号注销确认"
            width="30%"
          >
            <span>注销账号将删除您的所有数据，此操作不可撤销，确认继续吗？</span>
            <template #footer>
              <span class="dialog-footer">
                <el-button @click="deactivateDialogVisible = false">取消</el-button>
                <el-button type="danger" @click="deactivateAccount" :loading="deactivateLoading">
                  确认注销
                </el-button>
              </span>
            </template>
          </el-dialog>
        </div>
      </el-tab-pane>
    </el-tabs>
  </div>
</template>

<script setup>
import { ref, reactive, onMounted } from 'vue'
import { useRouter } from 'vue-router'
import { ElMessage, ElMessageBox } from 'element-plus'
import request from '../../utils/request'
import { useUserStore } from '../../stores'

const router = useRouter()
const userStore = useUserStore()

// 当前激活的标签页
const activeTab = ref('password')

// 修改密码相关
const passwordFormRef = ref()
const passwordLoading = ref(false)

const passwordForm = reactive({
  oldPassword: '',
  newPassword: '',
  confirmPassword: ''
})

const validatePass = (rule, value, callback) => {
  // 密码规则：至少6位，包含字母和数字
  const pattern = /^(?=.*[A-Za-z])(?=.*\d)[A-Za-z\d]{6,}$/
  if (!pattern.test(value)) {
    callback(new Error('密码至少为6位，且必须包含字母和数字'))
  } else {
    callback()
  }
}

const validateConfirmPass = (rule, value, callback) => {
  if (value !== passwordForm.newPassword) {
    callback(new Error('两次输入密码不一致'))
  } else {
    callback()
  }
}

const passwordRules = reactive({
  oldPassword: [
    { required: true, message: '请输入当前密码', trigger: 'blur' }
  ],
  newPassword: [
    { required: true, message: '请输入新密码', trigger: 'blur' },
    { validator: validatePass, trigger: 'blur' }
  ],
  confirmPassword: [
    { required: true, message: '请再次输入新密码', trigger: 'blur' },
    { validator: validateConfirmPass, trigger: 'blur' }
  ]
})

// 隐私设置相关
const privacyLoading = ref(false)
const privacySettings = reactive({
  defaultResumeVisibility: 'Private',
  jobSeekingStatus: 'Active'
})

// 通知设置相关
const notificationLoading = ref(false)
const notificationSettings = reactive({
  emailEnabled: true,
  emailTypes: ['application', 'interview', 'system']
})

// 账号安全相关
const userInfo = reactive({
  registrationDate: '',
  lastLoginDate: '',
  accountStatus: 'Active'
})

const deactivateDialogVisible = ref(false)
const deactivateLoading = ref(false)

// 在组件挂载时获取用户信息
onMounted(async () => {
  await fetchUserInfo()
  await fetchUserSettings()
})

// 获取用户基本信息
const fetchUserInfo = async () => {
  try {
    const res = await request.get('/users/me')
    if (res.success) {
      Object.assign(userInfo, res.data)
    }
  } catch (error) {
    console.error('获取用户信息失败', error)
    ElMessage.error('获取用户信息失败')
  }
}

// 获取用户设置信息
const fetchUserSettings = async () => {
  try {
    // 这里可以添加获取用户设置的API调用
    // 由于API文档中没有明确的设置获取接口，所以这里模拟获取
    
    // 模拟服务器响应
    setTimeout(() => {
      // 这里可以根据实际API响应设置值
    }, 500)
  } catch (error) {
    console.error('获取用户设置失败', error)
  }
}

// 修改密码
const submitPasswordForm = async (formEl) => {
  if (!formEl) return
  
  await formEl.validate(async (valid) => {
    if (valid) {
      try {
        passwordLoading.value = true
        
        const res = await request.put('/users/me/password', {
          oldPassword: passwordForm.oldPassword,
          newPassword: passwordForm.newPassword
        })
        
        if (res.success) {
          ElMessage.success('密码修改成功，请重新登录')
          
          // 清除登录状态
          localStorage.removeItem('token')
          localStorage.removeItem('userType')
          userStore.clearUser()
          
          // 跳转到登录页
          router.push('/login')
        }
      } catch (error) {
        console.error('密码修改失败', error)
        if (error.response && error.response.data) {
          ElMessage.error(error.response.data.message || '密码修改失败')
        } else {
          ElMessage.error('密码修改失败，请检查当前密码是否正确')
        }
      } finally {
        passwordLoading.value = false
      }
    }
  })
}

// 保存隐私设置
const savePrivacySettings = async () => {
  try {
    privacyLoading.value = true
    
    // 这里模拟API调用，实际项目中应该替换为真实的API调用
    await new Promise(resolve => setTimeout(resolve, 500))
    
    ElMessage.success('隐私设置保存成功')
  } catch (error) {
    console.error('保存隐私设置失败', error)
    ElMessage.error('保存隐私设置失败')
  } finally {
    privacyLoading.value = false
  }
}

// 保存通知设置
const saveNotificationSettings = async () => {
  try {
    notificationLoading.value = true
    
    // 这里模拟API调用，实际项目中应该替换为真实的API调用
    await new Promise(resolve => setTimeout(resolve, 500))
    
    ElMessage.success('通知设置保存成功')
  } catch (error) {
    console.error('保存通知设置失败', error)
    ElMessage.error('保存通知设置失败')
  } finally {
    notificationLoading.value = false
  }
}

// 显示注销账号对话框
const showDeactivateDialog = () => {
  deactivateDialogVisible.value = true
}

// 注销账号
const deactivateAccount = async () => {
  try {
    deactivateLoading.value = true
    
    // 二次确认
    await ElMessageBox.confirm(
      '此操作将永久删除您的账号及所有相关数据，确认继续吗？',
      '警告',
      {
        confirmButtonText: '确认注销',
        cancelButtonText: '取消',
        type: 'warning'
      }
    )
    
    // 这里应该调用注销账号的API
    // 注意：API文档中并未提供注销账号的接口，这里仅作为示例
    // 在实际项目中，可能是发送请求到特定的注销接口
    
    // 模拟API调用
    await new Promise(resolve => setTimeout(resolve, 1000))
    
    // 清除登录状态
    localStorage.removeItem('token')
    localStorage.removeItem('userType')
    userStore.clearUser()
    
    ElMessage.success('账号已注销')
    router.push('/login')
  } catch (error) {
    if (error !== 'cancel') {
      console.error('账号注销失败', error)
      ElMessage.error('账号注销失败')
    }
  } finally {
    deactivateLoading.value = false
    deactivateDialogVisible.value = false
  }
}

// 格式化日期
const formatDate = (dateString) => {
  if (!dateString) return '未知'
  
  const date = new Date(dateString)
  return date.toLocaleString('zh-CN', {
    year: 'numeric',
    month: '2-digit',
    day: '2-digit',
    hour: '2-digit',
    minute: '2-digit'
  })
}
</script>

<style scoped>
.settings-container {
  padding: 20px;
  background-color: white;
  border-radius: 4px;
  box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
}

.settings-form {
  max-width: 500px;
  margin-top: 20px;
}

.security-info {
  margin-bottom: 30px;
}

.info-item {
  margin-bottom: 15px;
  font-size: 14px;
}

.label {
  font-weight: bold;
  margin-right: 10px;
  color: #606266;
}

.danger-zone {
  margin-top: 40px;
  padding-top: 20px;
  border-top: 1px solid #EBEEF5;
}

.danger-zone h3 {
  color: #F56C6C;
  margin-bottom: 15px;
}
</style>
