<template>
  <div class="enterprise-register-container">
    <header class="header">
      <h1>无形大手人才招聘系统</h1>
      <div class="nav-links">
        <router-link to="/">首页</router-link>
        <router-link to="/jobs">职位列表</router-link>
        <router-link to="/login">登录</router-link>
        <router-link to="/register">注册</router-link>
      </div>
    </header>
    
    <main class="main-content">
      <div class="register-box">
        <h2>企业用户注册</h2>
        <div class="register-type-links">
          <router-link to="/register">个人用户</router-link>
          <router-link to="/register/enterprise" class="active">企业用户</router-link>
        </div>
        
        <form @submit.prevent="handleRegister" class="register-form">
          <h3>账号信息</h3>
          <div class="form-group">
            <label for="loginUsername">登录用户名</label>
            <input 
              type="text" 
              id="loginUsername" 
              v-model="form.loginUsername" 
              placeholder="4-20个字符，只能包含字母、数字和下划线"
              required
            >
          </div>
          
          <div class="form-group">
            <label for="password">密码</label>
            <input 
              type="password" 
              id="password" 
              v-model="form.password" 
              placeholder="至少6个字符,包含字母和数字"
              required
            >
          </div>
          
          <div class="form-group">
            <label for="confirmPassword">确认密码</label>
            <input 
              type="password" 
              id="confirmPassword" 
              v-model="form.confirmPassword" 
              placeholder="再次输入密码"
              required
            >
          </div>
          
          <h3>企业信息</h3>
          <div class="form-group">
            <label for="enterpriseName">企业名称</label>
            <input 
              type="text" 
              id="enterpriseName" 
              v-model="form.enterpriseName" 
              required
            >
          </div>
          
          <div class="form-group">
            <label for="creditCode">统一社会信用代码</label>
            <input 
              type="text" 
              id="creditCode" 
              v-model="form.creditCode" 
              required
            >
          </div>
          
          <div class="form-group">
            <label for="description">企业描述</label>
            <textarea 
              id="description" 
              v-model="form.description" 
              rows="3"
            ></textarea>
          </div>
          
          <div class="form-group">
            <label for="industry">所属行业</label>
            <input 
              type="text" 
              id="industry" 
              v-model="form.industry"
            >
          </div>
          
          <div class="form-group">
            <label for="scale">企业规模</label>
            <select id="scale" v-model="form.scale">
              <option value="">请选择</option>
              <option value="0-50人">0-50人</option>
              <option value="50-200人">50-200人</option>
              <option value="200-500人">200-500人</option>
              <option value="500-1000人">500-1000人</option>
              <option value="1000人以上">1000人以上</option>
            </select>
          </div>
          
          <div class="form-group">
            <label for="address">企业地址</label>
            <input 
              type="text" 
              id="address" 
              v-model="form.address"
            >
          </div>
          
          <h3>联系人信息</h3>
          <div class="form-group">
            <label for="contactPerson">联系人姓名</label>
            <input 
              type="text" 
              id="contactPerson" 
              v-model="form.contactPerson" 
              required
            >
          </div>
          
          <div class="form-group">
            <label for="contactPhone">联系电话</label>
            <input 
              type="tel" 
              id="contactPhone" 
              v-model="form.contactPhone" 
              required
            >
          </div>
          
          <div class="form-group">
            <label for="contactEmail">联系邮箱</label>
            <input 
              type="email" 
              id="contactEmail" 
              v-model="form.contactEmail" 
              required
            >
          </div>
          
          <div class="form-group">
            <label for="licenseImage">营业执照 (选填)</label>
            <input 
              type="file" 
              id="licenseImage" 
              @change="handleFileUpload" 
              accept="image/*"
            >
            <p class="upload-tip">请上传清晰的营业执照照片或扫描件（非必须）</p>
          </div>
          
          <button type="submit" class="submit-btn" :disabled="isSubmitting">
            {{ isSubmitting ? '提交中...' : '提交注册申请' }}
          </button>
        </form>
        
        <div class="login-link">
          已有账号？<router-link to="/login">立即登录</router-link>
        </div>
      </div>
    </main>
    
    <footer class="footer">
      <p>© 2025 无形大手人才招聘系统</p>
    </footer>
  </div>
</template>

<script>
import axios from 'axios';

export default {
  name: 'EnterpriseRegister',
  data() {
    return {
      form: {
        loginUsername: '',
        password: '',
        confirmPassword: '',
        enterpriseName: '',
        creditCode: '',
        description: '',
        industry: '',
        scale: '',
        address: '',
        contactPerson: '',
        contactPhone: '',
        contactEmail: '',
        licenseImageURL: ''
      },
      licenseFile: null,
      isSubmitting: false,
      baseURL: 'http://frp-pet.com:36041/api/v1'
    };
  },
  methods: {
    async handleFileUpload(event) {
      this.licenseFile = event.target.files[0];
      if (this.licenseFile) {
        try {
          // 创建FormData对象上传文件
          const formData = new FormData();
          formData.append('file', this.licenseFile);
          
          // 假设有一个文件上传API
          const response = await axios.post(`${this.baseURL}/upload`, formData, {
            headers: {
              'Content-Type': 'multipart/form-data'
            }
          });
          
          if (response.data.success) {
            this.form.licenseImageURL = response.data.data.fileUrl;
            this.$message ? this.$message.success('文件上传成功') : alert('文件上传成功');
          } else {
            this.$message ? this.$message.error('文件上传失败') : alert('文件上传失败');
          }
        } catch (error) {
          console.error('文件上传错误:', error);
          this.$message ? this.$message.error('文件上传失败，请重试') : alert('文件上传失败，请重试');
        }
      }
    },
    async handleRegister() {
      // 验证表单
      if (this.form.password !== this.form.confirmPassword) {
        this.$message ? this.$message.error('两次输入的密码不一致') : alert('两次输入的密码不一致');
        return;
      }
      
      // 用户名验证
      const usernameRegex = /^[a-zA-Z0-9_]{4,20}$/;
      if (!usernameRegex.test(this.form.loginUsername)) {
        this.$message ? this.$message.error('用户名必须为4-20个字符，只能包含字母、数字和下划线') : 
          alert('用户名必须为4-20个字符,只能包含字母、数字和下划线');
        return;
      }
      
      // 密码验证
      const passwordRegex = /^(?=.*[A-Za-z])(?=.*\d)[A-Za-z\d]{6,}$/;
      if (!passwordRegex.test(this.form.password)) {
        this.$message ? this.$message.error('密码至少6个字符,必须包含字母和数字') : 
          alert('密码至少6个字符,必须包含字母和数字');
        return;
      }
      
      // 手机号验证
      if (!/^1[3-9]\d{9}$/.test(this.form.contactPhone)) {
        this.$message ? this.$message.error('请输入有效的手机号码') : alert('请输入有效的手机号码');
        return;
      }
      
      // 邮箱验证
      const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
      if (!emailRegex.test(this.form.contactEmail)) {
        this.$message ? this.$message.error('请输入有效的邮箱地址') : alert('请输入有效的邮箱地址');
        return;
      }
      
      try {
        this.isSubmitting = true;
        
        // 构建符合API要求的数据对象
        const registerData = {
          loginUsername: this.form.loginUsername,
          password: this.form.password,
          enterpriseName: this.form.enterpriseName,
          creditCode: this.form.creditCode,
          description: this.form.description || undefined,
          industry: this.form.industry || undefined,
          scale: this.form.scale || undefined,
          address: this.form.address || undefined,
          contactPerson: this.form.contactPerson,
          contactPhone: this.form.contactPhone,
          contactEmail: this.form.contactEmail
        };
        
        // 如果有上传营业执照，添加到请求数据中
        if (this.form.licenseImageURL) {
          registerData.licenseImageURL = this.form.licenseImageURL;
        }
        
        // 发送企业注册请求
        const response = await axios.post(`${this.baseURL}/auth/enterprise/register`, registerData);
        
        if (response.data.success) {
          this.$message ? this.$message.success('企业注册申请已提交，请等待审核') : 
            alert('企业注册申请已提交，请等待审核');
          this.$router.push('/login');
        } else {
          this.$message ? this.$message.error(response.data.message || '注册失败，请重试') : 
            alert(response.data.message || '注册失败，请重试');
        }
      } catch (error) {
        let errorMsg = '注册失败，请稍后重试';
        
        if (error.response) {
          // 处理不同错误状态码
          switch (error.response.status) {
            case 400:
              errorMsg = '请求参数有误，请检查输入';
              break;
            case 409:
              errorMsg = '用户名或统一社会信用代码已存在';
              break;
            default:
              errorMsg = error.response.data.message || errorMsg;
          }
        }
        
        this.$message ? this.$message.error(errorMsg) : alert(errorMsg);
        console.error('注册错误:', error);
      } finally {
        this.isSubmitting = false;
      }
    }
  }
}
</script>

<style scoped>
.enterprise-register-container {
  display: flex;
  flex-direction: column;
  min-height: 100vh;
}

.header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 20px;
  background-color: #fff;
  box-shadow: 0 2px 4px rgba(0,0,0,0.1);
}

.header h1 {
  margin: 0;
  color: #409EFF;
  font-size: 24px;
}

.nav-links {
  display: flex;
  gap: 20px;
}

.nav-links a {
  color: #333;
  text-decoration: none;
  font-weight: 500;
}

.nav-links a:hover {
  color: #409EFF;
}

.main-content {
  flex: 1;
  padding: 20px;
  display: flex;
  justify-content: center;
}

.register-box {
  width: 100%;
  max-width: 700px;
  padding: 30px;
  background-color: #fff;
  border-radius: 8px;
  box-shadow: 0 2px 12px 0 rgba(0,0,0,0.1);
  margin: 20px 0;
}

.register-box h2 {
  text-align: center;
  margin-bottom: 20px;
}

.register-box h3 {
  margin-top: 25px;
  margin-bottom: 15px;
  padding-bottom: 8px;
  border-bottom: 1px solid #ebeef5;
  color: #303133;
}

.register-type-links {
  display: flex;
  margin-bottom: 20px;
  border-bottom: 1px solid #dcdfe6;
}

.register-type-links a {
  flex: 1;
  text-align: center;
  padding: 12px;
  color: #606266;
  text-decoration: none;
}

.register-type-links a.active {
  color: #409EFF;
  border-bottom: 2px solid #409EFF;
}

.form-group {
  margin-bottom: 20px;
}

.form-group label {
  display: block;
  margin-bottom: 8px;
  font-weight: 500;
}

.form-group input,
.form-group select,
.form-group textarea {
  width: 100%;
  padding: 12px;
  border: 1px solid #dcdfe6;
  border-radius: 4px;
  font-size: 14px;
}

.form-group textarea {
  resize: vertical;
}

.upload-tip {
  margin-top: 5px;
  font-size: 12px;
  color: #909399;
}

.submit-btn {
  width: 100%;
  padding: 12px;
  background-color: #409EFF;
  color: white;
  border: none;
  border-radius: 4px;
  cursor: pointer;
  font-size: 16px;
  margin-top: 20px;
}

.login-link {
  text-align: center;
  margin-top: 20px;
}

.login-link a {
  color: #409EFF;
  text-decoration: none;
}

.footer {
  padding: 20px;
  background-color: #f5f7fa;
  text-align: center;
  margin-top: 40px;
}
</style>
