<!-- views/public/Register.vue -->
<template>
  <div class="register-container">
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
        <h2>个人用户注册</h2>
        <div class="register-type-links">
          <router-link to="/register" class="active">个人用户</router-link>
          <router-link to="/register/enterprise">企业用户</router-link>
        </div>
        
        <div v-if="errorMessage" class="error-message">
          {{ errorMessage }}
        </div>
        
        <form @submit.prevent="handleRegister" class="register-form">
          <div class="form-group">
            <label for="username">用户名</label>
            <input 
              type="text" 
              id="username" 
              v-model="form.username" 
              placeholder="4-20个字符，只能包含字母、数字和下划线"
              required
            >
            <div v-if="errors.username" class="field-error">{{ errors.username }}</div>
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
            <div v-if="errors.password" class="field-error">{{ errors.password }}</div>
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
            <div v-if="errors.confirmPassword" class="field-error">{{ errors.confirmPassword }}</div>
          </div>
          
          <div class="form-group">
            <label for="phoneNumber">手机号</label>
            <input 
              type="tel" 
              id="phoneNumber" 
              v-model="form.phoneNumber" 
              placeholder="选填，请输入有效的手机号"
            >
            <div v-if="errors.phoneNumber" class="field-error">{{ errors.phoneNumber }}</div>
          </div>
          
          <div class="form-group">
            <label for="email">邮箱</label>
            <input 
              type="email" 
              id="email" 
              v-model="form.email" 
              placeholder="选填，请输入有效的邮箱"
            >
            <div v-if="errors.email" class="field-error">{{ errors.email }}</div>
          </div>
          
          <button type="submit" class="submit-btn" :disabled="isSubmitting">
            {{ isSubmitting ? '注册中...' : '注册' }}
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
  name: 'Register',
  data() {
    return {
      form: {
        username: '',
        password: '',
        confirmPassword: '',
        phoneNumber: '',
        email: ''
      },
      errors: {
        username: '',
        password: '',
        confirmPassword: '',
        phoneNumber: '',
        email: ''
      },
      errorMessage: '',
      isSubmitting: false
    };
  },
  methods: {
    validateForm() {
      // 重置错误信息
      this.errors = {
        username: '',
        password: '',
        confirmPassword: '',
        phoneNumber: '',
        email: ''
      };
      this.errorMessage = '';
      
      let isValid = true;
      
      // 用户名验证：4-20个字符，只能包含字母、数字和下划线
      const usernameRegex = /^[a-zA-Z0-9_]{4,20}$/;
      if (!usernameRegex.test(this.form.username)) {
        this.errors.username = '用户名必须为4-20个字符,只能包含字母、数字和下划线';
        isValid = false;
      }
      
      // 密码验证：至少6个字符，包含字母和数字
      const passwordRegex = /^(?=.*[A-Za-z])(?=.*\d)[A-Za-z\d]{6,}$/;
      if (!passwordRegex.test(this.form.password)) {
        this.errors.password = '密码至少6个字符,必须包含字母和数字';
        isValid = false;
      }
      
      // 确认密码验证
      if (this.form.password !== this.form.confirmPassword) {
        this.errors.confirmPassword = '两次输入的密码不一致';
        isValid = false;
      }
      
      // 手机号验证（如果有）
      if (this.form.phoneNumber && !/^1[3-9]\d{9}$/.test(this.form.phoneNumber)) {
        this.errors.phoneNumber = '请输入有效的手机号码';
        isValid = false;
      }
      
      // 邮箱验证（如果有）
      if (this.form.email && !/^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(this.form.email)) {
        this.errors.email = '请输入有效的邮箱地址';
        isValid = false;
      }
      
      return isValid;
    },
    
    async handleRegister() {
      // 验证表单
      if (!this.validateForm()) {
        return;
      }
      
      this.isSubmitting = true;
      
      try {
        // 准备请求数据
        const registerData = {
          username: this.form.username,
          password: this.form.password
        };
        
        // 只有在有值时才添加可选字段
        if (this.form.phoneNumber) {
          registerData.phoneNumber = this.form.phoneNumber;
        }
        
        if (this.form.email) {
          registerData.email = this.form.email;
        }
        const baseURL = 'http://frp-pet.com:36041/api/v1';
        
        // 发送注册请求
        const response = await axios.post(`${baseURL}/auth/individual/register`, registerData);
        console.log(response.status)
        if (response.status === 201) {
          // 显示成功消息
          this.$notify({
            title: '成功',
            message: '注册成功！',
            type: 'success'
          });
          
          // 跳转到首页或个人中心
          this.$router.push('/login');
        }
      } catch (error) {
        console.error('注册失败:', error);
        
        if (error.response) {
          // 服务器响应错误
          if (error.response.status === 409) {
            this.errorMessage = '用户名、手机号或邮箱已存在';
          } else if (error.response.status === 400) {
            this.errorMessage = error.response.data.message || '请求参数错误';
          } else {
            this.errorMessage = '注册失败，请稍后再试';
          }
        } else if (error.request) {
          // 请求发送但没有收到响应
          this.errorMessage = '服务器无响应，请检查网络连接';
        } else {
          // 请求设置时出错
          this.errorMessage = '请求错误，请稍后再试';
        }
      } finally {
        this.isSubmitting = false;
      }
    }
  }
}
</script>

<style scoped>
.register-container {
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
  align-items: center;
}

.register-box {
  width: 100%;
  max-width: 500px;
  padding: 30px;
  background-color: #fff;
  border-radius: 8px;
  box-shadow: 0 2px 12px 0 rgba(0,0,0,0.1);
}

.register-box h2 {
  text-align: center;
  margin-bottom: 20px;
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

.form-group input {
  width: 100%;
  padding: 12px;
  border: 1px solid #dcdfe6;
  border-radius: 4px;
  font-size: 14px;
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
  margin-top: 10px;
}

.submit-btn:disabled {
  background-color: #a0cfff;
  cursor: not-allowed;
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

.error-message {
  padding: 10px;
  margin-bottom: 20px;
  background-color: #fef0f0;
  color: #f56c6c;
  border-radius: 4px;
  text-align: center;
}

.field-error {
  color: #f56c6c;
  font-size: 12px;
  margin-top: 5px;
}
</style>
