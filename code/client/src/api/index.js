// src/api/index.js
import request from '@/utils/request'

// 职位相关API
export const JobAPI = {
  // 获取职位列表
  getJobList(params) {
    return request.get('/jobs', { params })
  },
  
  // 获取职位详情
  getJobDetail(jobId) {
    return request.get(`/jobs/${jobId}`)
  },
  
  // 申请职位
  applyJob(jobId, resumeId) {
    return request.post(`/jobs/${jobId}/apply`, { resumeId })
  }
}

// 用户认证相关API
export const AuthAPI = {
  // 个人用户登录
  login(username, password) {
    return request.post('/auth/individual/login', { username, password })
  },
  
  // 个人用户注册
  register(userData) {
    return request.post('/auth/individual/register', userData)
  },
  
  // 企业用户登录
  enterpriseLogin(loginUsername, password) {
    return request.post('/auth/enterprise/login', { loginUsername, password })
  },
  
  // 企业用户注册
  enterpriseRegister(enterpriseData) {
    return request.post('/auth/enterprise/register', enterpriseData)
  },
  
  // 退出登录
  logout() {
    return request.post('/auth/logout')
  }
}

// 个人用户相关API
export const UserAPI = {
  // 获取当前用户信息
  getCurrentUser() {
    return request.get('/users/me')
  },
  
  // 更新当前用户信息
  updateUserInfo(userData) {
    return request.put('/users/me', userData)
  },
  
  // 修改密码
  changePassword(oldPassword, newPassword) {
    return request.put('/users/me/password', { oldPassword, newPassword })
  },
  
  // 获取用户简历列表
  getResumes() {
    return request.get('/users/me/resumes')
  },
  
  // 创建简历
  createResume(resumeData) {
    return request.post('/users/me/resumes', resumeData)
  },
  
  // 获取简历详情
  getResumeDetail(resumeId) {
    return request.get(`/users/me/resumes/${resumeId}`)
  },
  
  // 更新简历
  updateResume(resumeId, resumeData) {
    return request.put(`/users/me/resumes/${resumeId}`, resumeData)
  },
  
  // 删除简历
  deleteResume(resumeId) {
    return request.delete(`/users/me/resumes/${resumeId}`)
  },
  
  // 获取申请记录
  getApplications(params) {
    return request.get('/users/me/applications', { params })
  }
}

// 导出所有API
export default {
  JobAPI,
  AuthAPI,
  UserAPI
}
