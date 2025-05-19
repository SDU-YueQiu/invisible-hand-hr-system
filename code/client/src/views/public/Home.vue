<!-- src/views/public/Home.vue -->
<template>
  <div class="home-container">
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
      <div class="hero-section">
        <h2>找到您理想的工作</h2>
        <p>海量职位，等您来选</p>
        <div class="search-box">
          <input type="text" placeholder="搜索职位关键词...">
          <button>搜索</button>
        </div>
      </div>
      
      <div class="features">
        <div class="feature">
          <h3>个人用户</h3>
          <p>创建简历、搜索职位、跟踪申请</p>
        </div>
        <div class="feature">
          <h3>企业用户</h3>
          <p>发布职位、管理招聘、寻找人才</p>
        </div>
        <div class="feature">
          <h3>便捷服务</h3>
          <p>智能匹配、实时通知、专业支持</p>
        </div>
      </div>
    </main>
    
    <footer class="footer">
      <p>© 2023 无形大手人才招聘系统</p>
    </footer>
  </div>
</template>

<style scoped>
.home-container {
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
}

.hero-section {
  text-align: center;
  padding: 60px 0;
  background-color: #f5f7fa;
  border-radius: 8px;
  margin-bottom: 40px;
}

.hero-section h2 {
  font-size: 32px;
  margin-bottom: 10px;
}

.search-box {
  display: flex;
  max-width: 600px;
  margin: 30px auto 0;
}

.search-box input {
  flex: 1;
  padding: 12px;
  border: 1px solid #dcdfe6;
  border-radius: 4px 0 0 4px;
  font-size: 16px;
}

.search-box button {
  padding: 12px 24px;
  background-color: #409EFF;
  color: white;
  border: none;
  border-radius: 0 4px 4px 0;
  cursor: pointer;
  font-size: 16px;
}

.features {
  display: flex;
  justify-content: space-between;
  gap: 20px;
  margin-top: 40px;
}

.feature {
  flex: 1;
  padding: 20px;
  background-color: #fff;
  border-radius: 8px;
  box-shadow: 0 2px 12px 0 rgba(0,0,0,0.1);
  text-align: center;
}

.footer {
  padding: 20px;
  background-color: #f5f7fa;
  text-align: center;
  margin-top: 40px;
}
</style>
