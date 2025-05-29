<template>
  <div class="job-detail-container">
    <el-breadcrumb separator="/">
      <el-breadcrumb-item :to="{ path: '/' }">首页</el-breadcrumb-item>
      <el-breadcrumb-item :to="{ path: '/jobs' }">职位列表</el-breadcrumb-item>
      <el-breadcrumb-item>职位详情</el-breadcrumb-item>
    </el-breadcrumb>
    
    <div v-if="loading" class="loading-container">
      <el-skeleton :rows="10" animated />
    </div>
    
    <div v-else-if="job" class="job-container">
      <div class="job-header">
        <div class="job-basic-info">
          <h1 class="job-title">{{ job.jobTitle }}</h1>
          <div class="job-highlights">
            <span class="job-salary">{{ job.maxSalary ? `${job.minSalary}-${job.maxSalary}K` : '薪资面议' }}</span>
            <span class="job-location">{{ job.workLocation }}</span>
            <span class="job-experience">{{ job.experienceRequired || '经验不限' }}</span>
            <span class="job-education">{{ job.educationRequired || '学历不限' }}</span>
          </div>
          <div class="job-publish-info">
            发布时间: {{ formatDate(job.publishDate) }}
            <span v-if="job.deadlineDate">
              | 截止日期: {{ formatDate(job.deadlineDate) }}
            </span>
          </div>
        </div>
      </div>
      
      <div class="job-content">
        <div class="job-detail-section">
          <el-tabs type="border-card">
            <el-tab-pane label="职位详情">
              <div class="job-description">
                <div class="section-block" v-if="job.responsibilities">
                  <h3>职位职责</h3>
                  <div class="detail-content">{{ job.responsibilities }}</div>
                </div>
                
                <div class="section-block" v-if="job.requirements">
                  <h3>任职要求</h3>
                  <div class="detail-content">{{ job.requirements }}</div>
                </div>
                
                <div class="section-block" v-if="job.benefits">
                  <h3>福利待遇</h3>
                  <div class="detail-content">{{ job.benefits }}</div>
                </div>
              </div>
            </el-tab-pane>
            
            <el-tab-pane label="公司信息">
              <div class="company-info">
                <div class="company-header">
                  <img v-if="job.enterpriseInfo.logoURL" :src="job.enterpriseInfo.logoURL" class="company-logo" alt="公司logo">
                  <div class="company-basic">
                    <h3 class="company-name">{{ job.enterpriseInfo.enterpriseName }}</h3>
                    <div class="company-tags">
                      <el-tag size="small">{{ job.enterpriseInfo.industry || '未知行业' }}</el-tag>
                      <el-tag size="small">{{ job.enterpriseInfo.scale || '规模未知' }}</el-tag>
                    </div>
                  </div>
                </div>
                
                <div class="company-description" v-if="job.enterpriseInfo.description">
                  <h3>公司简介</h3>
                  <p>{{ job.enterpriseInfo.description }}</p>
                </div>
              </div>
            </el-tab-pane>
          </el-tabs>
        </div>
        
        <div class="job-apply-section">
          <el-card class="apply-card">
            <div class="apply-header">
              <h3>申请该职位</h3>
            </div>
            
            <div class="apply-content" v-if="isLoggedIn">
              <div v-if="hasResumes">
                <p>请选择一份简历投递</p>
                <el-select v-model="selectedResumeId" placeholder="选择简历">
                  <el-option
                    v-for="resume in userResumes"
                    :key="resume.resumeId"
                    :label="resume.resumeTitle"
                    :value="resume.resumeId">
                  </el-option>
                </el-select>
                
                <div class="apply-actions">
                  <el-button type="primary" @click="applyJob" :loading="applyLoading">立即申请</el-button>
                </div>
              </div>
              
              <div v-else>
                <p>您还没有创建简历</p>
                <el-button type="primary" @click="goToCreateResume">创建简历</el-button>
              </div>
            </div>
            
            <div class="apply-content" v-else>
              <p>请先登录再申请职位</p>
              <el-button type="primary" @click="goToLogin">立即登录</el-button>
              <el-button @click="goToRegister">注册新账号</el-button>
            </div>
          </el-card>
          
          <el-card class="company-card" v-if="job.enterpriseInfo">
            <div class="recruit-count" v-if="job.recruitmentCount">
              <h4>招聘人数</h4>
              <p>{{ job.recruitmentCount }}人</p>
            </div>
            
            <div class="contact-info" v-if="isLoggedIn">
              <h4>联系方式</h4>
              <p>请申请后通过系统消息查看</p>
            </div>
          </el-card>
        </div>
      </div>
    </div>
    
    <div v-else class="not-found">
      <el-result
        icon="error"
        title="未找到职位"
        sub-title="该职位可能已被删除或下架">
        <template #extra>
          <el-button type="primary" @click="$router.push('/jobs')">返回职位列表</el-button>
        </template>
      </el-result>
    </div>
  </div>
</template>

<script>
import axios from 'axios';
import { useRouter } from 'vue-router';
import { ElMessage } from 'element-plus';
import { useUserStore } from '../../stores'; 
import request from '../../utils/request';

export default {
  name: 'JobDetail',
  data() {
    return {
      job: null,
      loading: true,
      isLoggedIn: false, // 默认未登录状态，实际应用中需要从vuex或localStorage判断
      userResumes: [], // 用户简历列表
      selectedResumeId: null,
      applyLoading: false,
      baseURL: "http://localhost:8080/api/v1" 
    };
  },
  setup() {
    const userStore = useUserStore(); // 正确的位置，在setup钩子中
    const router = useRouter();
    
    return { userStore, router };
  },
  computed: {
    hasResumes() {
      return this.userResumes.length > 0;
    }
  },
  created() {
    this.loadJobDetail();
    this.checkLoginStatus();
  },
  methods: {
    async loadJobDetail() {
      try {
        this.loading = true;
        const jobId = this.$route.params.id;
        
        const token = localStorage.getItem('token');
        const config = token ? { headers: { 'Authorization': `Bearer ${token}` } } : {};
        
        const response = await axios.get(`${this.baseURL}/jobs/${jobId}`, config);
        console.log('API Response:', response.data);
        
        if (response.data && response.data.message) {
          this.job = response.data.message;
          
          // 确保enterpriseInfo存在，避免渲染错误
          if (!this.job.enterpriseInfo) {
            this.job.enterpriseInfo = {
              enterpriseName: '未知企业',
              industry: '未知行业',
              scale: '规模未知'
            };
          }
          
          console.log('Job data processed:', this.job);
        } else {
          throw new Error('获取职位详情失败：数据结构不正确');
        }
      } catch (error) {
        console.error('获取职位详情出错:', error);
        ElMessage.error('获取职位详情失败，请稍后重试');
      } finally {
        this.loading = false;
      }
    },

     
    checkLoginStatus() {
      // 检查用户登录状态的逻辑
      const token = localStorage.getItem('token');
      this.isLoggedIn = !!token;
      
      if (this.isLoggedIn) {
        this.loadUserResumes();
      }
    },
    
    async loadUserResumes() {
      try {
        const token = localStorage.getItem('token');
        const config = { headers: { 'Authorization': `Bearer ${token}` } };
        
        const response = await axios.get(`${this.baseURL}/users/me/resumes`, config);
        
        if (response.data && response.data.data) {
          this.userResumes = response.data.data;
          if (this.userResumes.length > 0) {
            this.selectedResumeId = this.userResumes[0].resumeId;
          }
        }
      } catch (error) {
        console.error('获取简历列表出错:', error);
      }
    },

    async applyJob() {
      if (!this.selectedResumeId) {
        ElMessage.warning('请选择要投递的简历');
        return;
      }
      
      try {
        this.applyLoading = true;
        const jobId = this.$route.params.id;
        const token = localStorage.getItem('token');
        
        const numericJobId = parseInt(jobId, 10);
        const numericResumeId = parseInt(this.selectedResumeId, 10);
        
        console.log('申请职位参数:', { 
          jobId: numericJobId, 
          resumeId: numericResumeId 
        });
        
        const config = {
          headers: {
            'Authorization': `Bearer ${token}`,
            'Content-Type': 'application/json'
          }
        };
        
        const requestBody = {
          resumeId: numericResumeId
        };
        
        const response = await axios.post(
          `${this.baseURL}/jobs/${numericJobId}/apply`, 
          requestBody,
          config
        );
        
        console.log('申请响应:', response.data);
        
        if (response.data) {
          ElMessage.success('职位申请成功');
        } else {
          ElMessage.error('职位申请失败');
        }
      } catch (error) {
        console.error('申请职位出错:', error);
        
        if (error.response) {
          console.log('错误响应数据:', error.response.data);
          
          const errorMsg = error.response.data.message || error.response.data || '申请职位失败';
          ElMessage.error(`申请失败: ${errorMsg}`);
        } else if (error.response && error.response.status === 409) {
          ElMessage.warning('您已经申请过该职位');
        } else {
          ElMessage.error('申请职位失败，请稍后重试');
        }
      } finally {
        this.applyLoading = false;
      }
    },

    goToLogin() {
      this.router.push('/login');
    },
    
    goToRegister() {
      this.router.push('/register');
    },
    
    goToCreateResume() {
      this.router.push('/user/resumes/create');
    },
    
    formatDate(dateString) {
      if (!dateString) return '';
      const date = new Date(dateString);
      return `${date.getFullYear()}-${(date.getMonth() + 1).toString().padStart(2, '0')}-${date.getDate().toString().padStart(2, '0')}`;
    }
  }
}
</script>

<style scoped>
.job-detail-container {
  max-width: 1200px;
  margin: 0 auto;
  padding: 20px;
}

.loading-container {
  margin-top: 30px;
}

.job-container {
  margin-top: 20px;
}

.job-header {
  background-color: #fff;
  padding: 24px;
  border-radius: 4px;
  box-shadow: 0 2px 12px 0 rgba(0,0,0,0.1);
  margin-bottom: 20px;
}

.job-title {
  margin: 0 0 16px 0;
  font-size: 24px;
  color: #303133;
}

.job-highlights {
  display: flex;
  flex-wrap: wrap;
  gap: 16px;
  margin-bottom: 16px;
}

.job-salary {
  color: #f56c6c;
  font-weight: bold;
}

.job-location, .job-experience, .job-education {
  color: #606266;
}

.job-publish-info {
  color: #909399;
  font-size: 14px;
}

.job-content {
  display: flex;
  gap: 20px;
}

.job-detail-section {
  flex: 3;
}

.job-apply-section {
  flex: 1;
}

.section-block {
  margin-bottom: 24px;
}

.section-block h3 {
  margin-bottom: 12px;
  color: #303133;
  font-size: 18px;
}

.detail-content {
  white-space: pre-line;
  color: #606266;
  line-height: 1.6;
}

.apply-card, .company-card {
  margin-bottom: 20px;
}

.apply-header {
  margin-bottom: 16px;
}

.apply-content p {
  margin-bottom: 16px;
}

.apply-actions {
  margin-top: 16px;
}

.company-header {
  display: flex;
  align-items: center;
  margin-bottom: 16px;
}

.company-logo {
  width: 60px;
  height: 60px;
  border-radius: 4px;
  margin-right: 12px;
}

.company-name {
  margin: 0 0 8px 0;
  font-size: 18px;
}

.company-tags {
  display: flex;
  gap: 8px;
}

.company-description h3 {
  margin-bottom: 12px;
  color: #303133;
  font-size: 16px;
}

.company-description p {
  color: #606266;
  line-height: 1.6;
}

.recruit-count, .contact-info {
  margin-bottom: 16px;
}

.recruit-count h4, .contact-info h4 {
  margin-bottom: 8px;
  color: #303133;
}

.not-found {
  margin-top: 40px;
}
</style>
