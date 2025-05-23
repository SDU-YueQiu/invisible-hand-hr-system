<!-- <template>
  <div class="job-list-container">
    <h1 class="page-title">职位列表</h1>
    
    <div class="filter-section">
      <el-form :inline="true" :model="filterForm" class="search-form">
        <el-form-item>
          <el-input 
            v-model="filterForm.keyword" 
            placeholder="职位名称、公司、技能..." 
            prefix-icon="el-icon-search"
            @keyup.enter.native="loadJobs"
          ></el-input>
        </el-form-item>
        
        <el-form-item>
          <el-select v-model="filterForm.location" placeholder="工作地点" clearable>
            <el-option label="北京" value="北京"></el-option>
            <el-option label="上海" value="上海"></el-option>
            <el-option label="广州" value="广州"></el-option>
            <el-option label="深圳" value="深圳"></el-option>
            <el-option label="杭州" value="杭州"></el-option>
          </el-select>
        </el-form-item>
        
        <el-form-item>
          <el-select v-model="filterForm.experience" placeholder="工作经验" clearable>
            <el-option label="应届毕业生" value="应届毕业生"></el-option>
            <el-option label="1-3年" value="1-3年"></el-option>
            <el-option label="3-5年" value="3-5年"></el-option>
            <el-option label="5-10年" value="5-10年"></el-option>
            <el-option label="10年以上" value="10年以上"></el-option>
          </el-select>
        </el-form-item>
        
        <el-form-item>
          <el-select v-model="filterForm.education" placeholder="学历要求" clearable>
            <el-option label="不限" value="不限"></el-option>
            <el-option label="大专" value="大专"></el-option>
            <el-option label="本科" value="本科"></el-option>
            <el-option label="硕士" value="硕士"></el-option>
            <el-option label="博士" value="博士"></el-option>
          </el-select>
        </el-form-item>
        
        <el-form-item>
          <el-button type="primary" @click="loadJobs">搜索</el-button>
          <el-button @click="resetFilter">重置</el-button>
        </el-form-item>
      </el-form>
    </div>
    
    <div class="job-list">
      <el-card v-for="job in jobs" :key="job.jobId" class="job-card" @click.native="goToJobDetail(job.jobId)">
        <div class="job-card-header">
          <div class="job-title-wrapper">
            <h2 class="job-title">{{ job.jobTitle }}</h2>
            <span class="job-salary">{{ job.salaryMin }}-{{ job.salaryMax }}K</span>
          </div>
          <div class="company-info">
            <img v-if="job.logoURL" :src="job.logoURL" class="company-logo" alt="公司logo">
            <span class="company-name">{{ job.enterpriseName }}</span>
          </div>
        </div>
        
        <div class="job-card-body">
          <div class="job-tags">
            <el-tag size="small">{{ job.workLocation }}</el-tag>
            <el-tag size="small" type="info">{{ job.experienceRequired }}</el-tag>
            <el-tag size="small" type="info">{{ job.educationRequired }}</el-tag>
          </div>
          
          <div class="job-publish-date">
            发布时间: {{ formatDate(job.publishDate) }}
          </div>
        </div>
      </el-card>
      
      <div v-if="jobs.length === 0" class="no-data">
        <el-empty description="暂无职位数据"></el-empty>
      </div>
    </div>
    
    <div class="pagination-container">
      <el-pagination
        background
        @current-change="handleCurrentChange"
        :current-page.sync="pagination.currentPage"
        :page-size="pagination.itemsPerPage"
        layout="prev, pager, next, jumper"
        :total="pagination.totalItems">
      </el-pagination>
    </div>
  </div>
</template>

<script>
import axios from 'axios';

//为了测试，应该删掉
import { JobAPI } from '@/api'
//为了测试，应该不用的

export default {
  name: 'JobList',
  data() {
    return {
      jobs: [],
      filterForm: {
        keyword: '',
        location: '',
        experience: '',
        education: '',
        salaryMin: '',
        salaryMax: ''
      },
      pagination: {
        currentPage: 1,
        totalPages: 0,
        totalItems: 0,
        itemsPerPage: 10
      },
      loading: false
    };
  },
  created() {
    this.loadJobs();
  },
  methods: {
    async loadJobs() {
      try {
        this.loading = true;
        
        // 构建查询参数
        const params = {
          page: this.pagination.currentPage,
          limit: this.pagination.itemsPerPage,
          sortBy: 'publishDate',
          sortOrder: 'desc'
        };
        
        // 添加筛选条件
        if (this.filterForm.keyword) params.keyword = this.filterForm.keyword;
        if (this.filterForm.location) params.location = this.filterForm.location;
        if (this.filterForm.experience) params.experience = this.filterForm.experience;
        if (this.filterForm.education) params.education = this.filterForm.education;
        if (this.filterForm.salaryMin) params.salaryMin = this.filterForm.salaryMin;
        if (this.filterForm.salaryMax) params.salaryMax = this.filterForm.salaryMax;
        
        //const response = await axios.get('/api/v1/jobs', { params });
        /*测试添加*/ 
        const response = await JobAPI.getJobList(params);
        
        if (response.success) {
          this.jobs = response.data.jobs || response.data;
          // 如果是json-server直接返回的数组，需要手动设置分页信息
          if (Array.isArray(response.data)) {
            this.pagination.totalItems = response.data.length;
            this.pagination.totalPages = Math.ceil(response.data.length / this.pagination.itemsPerPage);
          } else if (response.data.pagination) {
            this.pagination = response.data.pagination;
          }
        } else {
          this.$message.error(response.message || '获取职位列表失败');
        }
        // if (response.data.success) {
        //   this.jobs = response.data.data.jobs;
        //   this.pagination = response.data.data.pagination;
        // } else {
        //   this.$message.error(response.data.message || '获取职位列表失败');
        // }
      } catch (error) {
        console.error('获取职位列表出错:', error);
        this.$message.error('获取职位列表失败，请稍后重试');
      } finally {
        this.loading = false;
      }
    },
    resetFilter() {
      this.filterForm = {
        keyword: '',
        location: '',
        experience: '',
        education: '',
        salaryMin: '',
        salaryMax: ''
      };
      this.pagination.currentPage = 1;
      this.loadJobs();
    },
    handleCurrentChange(page) {
      this.pagination.currentPage = page;
      this.loadJobs();
    },
    goToJobDetail(jobId) {
      this.$router.push({ name: 'JobDetail', params: { id: jobId } });
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
.job-list-container {
  max-width: 1200px;
  margin: 0 auto;
  padding: 20px;
}

.page-title {
  margin-bottom: 24px;
  color: #303133;
}

.filter-section {
  background-color: #fff;
  padding: 20px;
  border-radius: 4px;
  box-shadow: 0 2px 12px 0 rgba(0,0,0,0.1);
  margin-bottom: 20px;
}

.search-form {
  display: flex;
  flex-wrap: wrap;
}

.job-list {
  margin-top: 20px;
}

.job-card {
  margin-bottom: 16px;
  transition: transform 0.3s;
  cursor: pointer;
}

.job-card:hover {
  transform: translateY(-5px);
  box-shadow: 0 5px 15px rgba(0,0,0,0.1);
}

.job-card-header {
  display: flex;
  justify-content: space-between;
  margin-bottom: 12px;
}

.job-title-wrapper {
  display: flex;
  align-items: center;
}

.job-title {
  margin: 0;
  font-size: 18px;
  color: #303133;
  margin-right: 10px;
}

.job-salary {
  color: #f56c6c;
  font-weight: bold;
}

.company-info {
  display: flex;
  align-items: center;
}

.company-logo {
  width: 30px;
  height: 30px;
  border-radius: 50%;
  margin-right: 8px;
}

.company-name {
  color: #606266;
}

.job-card-body {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.job-tags {
  display: flex;
  gap: 8px;
}

.job-publish-date {
  color: #909399;
  font-size: 14px;
}

.pagination-container {
  text-align: center;
  margin-top: 30px;
}

.no-data {
  margin-top: 40px;
  text-align: center;
}
</style> -->

<template>
  <div class="job-list-container">
    <h1 class="page-title">职位列表</h1>
    
    <div class="filter-section">
      <el-form :inline="true" :model="filterForm" class="search-form">
        <el-form-item class="search-item">
          <el-input 
            v-model="filterForm.keyword" 
            placeholder="职位名称、公司、技能..." 
            prefix-icon="el-icon-search"
            @keyup.enter.native="loadJobs"
            class="keyword-input"
          ></el-input>
        </el-form-item>
        
        <el-form-item class="select-item">
          <el-select 
            v-model="filterForm.location" 
            placeholder="工作地点" 
            clearable
            class="filter-select"
          >
            <el-option label="北京" value="北京"></el-option>
            <el-option label="上海" value="上海"></el-option>
            <el-option label="广州" value="广州"></el-option>
            <el-option label="深圳" value="深圳"></el-option>
            <el-option label="杭州" value="杭州"></el-option>
          </el-select>
        </el-form-item>
        
        <el-form-item class="select-item">
          <el-select 
            v-model="filterForm.experience" 
            placeholder="工作经验" 
            clearable
            class="filter-select"
          >
            <el-option label="应届毕业生" value="应届毕业生"></el-option>
            <el-option label="1-3年" value="1-3年"></el-option>
            <el-option label="3-5年" value="3-5年"></el-option>
            <el-option label="5-10年" value="5-10年"></el-option>
            <el-option label="10年以上" value="10年以上"></el-option>
          </el-select>
        </el-form-item>
        
        <el-form-item class="select-item">
          <el-select 
            v-model="filterForm.education" 
            placeholder="学历要求" 
            clearable
            class="filter-select"
          >
            <el-option label="不限" value="不限"></el-option>
            <el-option label="大专" value="大专"></el-option>
            <el-option label="本科" value="本科"></el-option>
            <el-option label="硕士" value="硕士"></el-option>
            <el-option label="博士" value="博士"></el-option>
          </el-select>
        </el-form-item>
        
        <el-form-item class="button-item">
          <el-button type="primary" @click="loadJobs">搜索</el-button>
          <el-button @click="resetFilter">重置</el-button>
        </el-form-item>
      </el-form>
    </div>
    
    <div v-loading="loading" class="job-list">
      <el-card v-for="job in jobs" :key="job.jobId" class="job-card" @click.native="goToJobDetail(job.jobId)">
        <div class="job-card-header">
          <div class="job-title-wrapper">
            <h2 class="job-title">{{ job.jobTitle }}</h2>
            <span class="job-salary">{{ job.salaryMin }}-{{ job.salaryMax }}K</span>
          </div>
          <div class="company-info">
            <img v-if="job.logoURL" :src="job.logoURL" class="company-logo" alt="公司logo">
            <span class="company-name">{{ job.enterpriseName }}</span>
          </div>
        </div>
        
        <div class="job-card-body">
          <div class="job-tags">
            <el-tag size="small">{{ job.workLocation }}</el-tag>
            <el-tag size="small" type="info">{{ job.experienceRequired }}</el-tag>
            <el-tag size="small" type="info">{{ job.educationRequired }}</el-tag>
          </div>
          
          <div class="job-publish-date">
            发布时间: {{ formatDate(job.publishDate) }}
          </div>
        </div>
      </el-card>
      
      <div v-if="jobs.length === 0 && !loading" class="no-data">
        <el-empty description="暂无职位数据"></el-empty>
      </div>
    </div>
    
    <div class="pagination-container" v-if="pagination.totalItems > 0">
      <el-pagination
        background
        @current-change="handleCurrentChange"
        :current-page.sync="pagination.currentPage"
        :page-size="pagination.itemsPerPage"
        layout="prev, pager, next, jumper"
        :total="pagination.totalItems">
      </el-pagination>
    </div>
  </div>
</template>

<script>
import axios from 'axios';

//为了测试，应该删掉
import { JobAPI } from '@/api'
//为了测试，应该不用的

export default {
  name: 'JobList',
  data() {
    return {
      jobs: [],
      filterForm: {
        keyword: '',
        location: '',
        experience: '',
        education: '',
        salaryMin: '',
        salaryMax: ''
      },
      pagination: {
        currentPage: 1,
        totalPages: 0,
        totalItems: 0,
        itemsPerPage: 10
      },
      loading: false
    };
  },
  created() {
    this.loadJobs();
  },
  methods: {
    async loadJobs() {
      try {
        this.loading = true;
        
        // 构建查询参数
        const params = {
          page: this.pagination.currentPage,
          limit: this.pagination.itemsPerPage,
          sortBy: 'publishDate',
          sortOrder: 'desc'
        };
        
        // 添加筛选条件
        if (this.filterForm.keyword) params.keyword = this.filterForm.keyword;
        if (this.filterForm.location) params.location = this.filterForm.location;
        if (this.filterForm.experience) params.experience = this.filterForm.experience;
        if (this.filterForm.education) params.education = this.filterForm.education;
        if (this.filterForm.salaryMin) params.salaryMin = this.filterForm.salaryMin;
        if (this.filterForm.salaryMax) params.salaryMax = this.filterForm.salaryMax;
        
        //const response = await axios.get('/api/v1/jobs', { params });
        /*测试添加*/ 
        const response = await JobAPI.getJobList(params);
        
        if (response.success) {
          this.jobs = response.data.jobs || response.data;
          // 如果是json-server直接返回的数组，需要手动设置分页信息
          if (Array.isArray(response.data)) {
            this.pagination.totalItems = response.data.length;
            this.pagination.totalPages = Math.ceil(response.data.length / this.pagination.itemsPerPage);
          } else if (response.data.pagination) {
            this.pagination = response.data.pagination;
          }
        } else {
          this.$message.error(response.message || '获取职位列表失败');
        }
      } catch (error) {
        console.error('获取职位列表出错:', error);
        this.$message.error('获取职位列表失败，请稍后重试');
      } finally {
        this.loading = false;
      }
    },
    resetFilter() {
      this.filterForm = {
        keyword: '',
        location: '',
        experience: '',
        education: '',
        salaryMin: '',
        salaryMax: ''
      };
      this.pagination.currentPage = 1;
      this.loadJobs();
    },
    handleCurrentChange(page) {
      this.pagination.currentPage = page;
      this.loadJobs();
    },
    goToJobDetail(jobId) {
      this.$router.push({ name: 'JobDetail', params: { id: jobId } });
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
.job-list-container {
  max-width: 1200px;
  margin: 0 auto;
  padding: 20px;
}

.page-title {
  margin-bottom: 24px;
  color: #303133;
}

.filter-section {
  background-color: #fff;
  padding: 20px;
  border-radius: 4px;
  box-shadow: 0 2px 12px 0 rgba(0,0,0,0.1);
  margin-bottom: 20px;
}

.search-form {
  display: flex;
  flex-wrap: wrap;
  align-items: flex-start;
}

/* 调整搜索表单项宽度 */
.search-item {
  margin-right: 10px;
  margin-bottom: 10px;
}

.keyword-input {
  width: 280px;
}

.select-item {
  margin-right: 10px;
  margin-bottom: 10px;
}

/* 确保下拉框有足够宽度显示placeholder */
.filter-select {
  width: 130px;
}

.button-item {
  margin-bottom: 10px;
}

.job-list {
  margin-top: 20px;
  min-height: 200px;
}

.job-card {
  margin-bottom: 16px;
  transition: transform 0.3s;
  cursor: pointer;
}

.job-card:hover {
  transform: translateY(-5px);
  box-shadow: 0 5px 15px rgba(0,0,0,0.1);
}

.job-card-header {
  display: flex;
  justify-content: space-between;
  margin-bottom: 12px;
}

.job-title-wrapper {
  display: flex;
  align-items: center;
}

.job-title {
  margin: 0;
  font-size: 18px;
  color: #303133;
  margin-right: 10px;
}

.job-salary {
  color: #f56c6c;
  font-weight: bold;
}

.company-info {
  display: flex;
  align-items: center;
}

.company-logo {
  width: 30px;
  height: 30px;
  border-radius: 50%;
  margin-right: 8px;
  object-fit: cover;
}

.company-name {
  color: #606266;
}

.job-card-body {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.job-tags {
  display: flex;
  gap: 8px;
}

.job-publish-date {
  color: #909399;
  font-size: 14px;
}

.pagination-container {
  text-align: center;
  margin-top: 30px;
}

.no-data {
  margin-top: 40px;
  text-align: center;
}

/* 响应式调整 */
@media screen and (max-width: 768px) {
  .search-form {
    flex-direction: column;
  }
  
  .keyword-input,
  .filter-select {
    width: 100%;
  }
  
  .search-item,
  .select-item,
  .button-item {
    margin-right: 0;
    width: 100%;
  }
}
</style>

