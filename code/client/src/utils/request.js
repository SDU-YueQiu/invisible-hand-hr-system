// src/utils/request.js
import axios from 'axios'

const request = axios.create({
  baseURL: 'http://localhost:8080/api/v1', 
  timeout: 5000
})

// 请求拦截器
request.interceptors.request.use(
  config => {
    // 从localStorage获取token
    const token = localStorage.getItem('token')
    if (token) {
      config.headers['Authorization'] = `Bearer ${token}`
    }
    return config
  },
  error => {
    return Promise.reject(error)
  }
)

// 响应拦截器
request.interceptors.response.use(
  response => {
    return response.data
  },
  error => {
    console.error('API error:', error)
    return Promise.reject(error)
  }
)

export default request
