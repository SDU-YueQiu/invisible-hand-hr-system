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
              placeholder="至少6个字符，包含字母和数字"
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
            <label for="licenseImage">营业执照</label>
            <input 
              type="file" 
              id="licenseImage" 
              @change="handleFileUpload" 
              accept="image/*"
              required
            >
            <p class="upload-tip">请上传清晰的营业执照照片或扫描件</p>
          </div>
          
          <button type="submit" class="submit-btn">提交注册申请</button>
        </form>
        
        <div class="login-link">
          已有账号？<router-link to="/login">立即登录</router-link>
        </div>
      </div>
    </main>
    
    <footer class="footer">
      <p>© 2023 无形大手人才招聘系统</p>
    </footer>
  </div>
</template>

<script>
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
      licenseFile: null
    };
  },
  methods: {
    handleFileUpload(event) {
      this.licenseFile = event.target.files[0];
      // 实际项目中应该上传文件到服务器，获取URL
      // 这里仅做模拟
      if (this.licenseFile) {
        console.log('文件已选择:', this.licenseFile.name);
        // 模拟上传成功后获取URL
        this.form.licenseImageURL = 'temp-url-for-license-image';
      }
    },
    handleRegister() {
      // 验证表单
      if (this.form.password !== this.form.confirmPassword) {
        alert('两次输入的密码不一致');
        return;
      }
      
      // 用户名验证
      const usernameRegex = /^[a-zA-Z0-9_]{4,20}$/;
      if (!usernameRegex.test(this.form.loginUsername)) {
        alert('用户名必须为4-20个字符，只能包含字母、数字和下划线');
        return;
      }
      
      // 密码验证
      const passwordRegex = /^(?=.*[A-Za-z])(?=.*\d)[A-Za-z\d]{6,}$/;
      if (!passwordRegex.test(this.form.password)) {
        alert('密码至少6个字符，必须包含字母和数字');
        return;
      }
      
      // 手机号验证
      if (!/^1[3-9]\d{9}$/.test(this.form.contactPhone)) {
        alert('请输入有效的手机号码');
        return;
      }
      
      // 邮箱验证
      const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
      if (!emailRegex.test(this.form.contactEmail)) {
        alert('请输入有效的邮箱地址');
        return;
      }
      
      // 检查执照是否上传
      if (!this.form.licenseImageURL) {
        alert('请上传营业执照');
        return;
      }
      
      // TODO: 发送注册请求到 API
      console.log('企业注册表单提交', this.form);
      
      // 注册逻辑：
      // const registerData = {
      //   loginUsername: this.form.loginUsername,
      //   password: this.form.password,
      //   enterpriseName: this.form.enterpriseName,
      //   creditCode: this.form.creditCode,
      //   description: this.form.description,
      //   industry: this.form.industry,
      //   scale: this.form.scale,
      //   address: this.form.address,
      //   contactPerson: this.form.contactPerson,
      //   contactPhone: this.form.contactPhone,
      //   contactEmail: this.form.contactEmail,
      //   licenseImageURL: this.form.licenseImageURL
      // };
      
      // 调用 API: POST /auth/enterprise/register
      // 成功后提示用户等待审核并跳转到登录页面
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
