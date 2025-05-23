// src/utils/request.js
import axios from 'axios'

const request = axios.create({
  baseURL: 'http://localhost:8080', // json-server的基础URL
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

// 响应拦截器 - 适配json-server返回格式
request.interceptors.response.use(
  response => {
    const data = response.data
    
    // 检查是否已经是API预期的格式
    if (data && data.hasOwnProperty('success')) {
      return data
    }
    
    // 转换json-server返回的数据为API预期格式
    return {
      success: true,
      data: data,
      message: '操作成功'
    }
  },
  error => {
    // 处理错误响应
    if (error.response) {
      // 服务器返回了错误状态码
      const status = error.response.status
      let message = '请求失败'
      
      switch (status) {
        case 400:
          message = '请求参数错误'
          break
        case 401:
          message = '未授权，请登录'
          // 可以在这里处理登录过期的情况，比如清除token并跳转到登录页
          localStorage.removeItem('token')
          // 如果使用router，可以添加: router.push('/login')
          break
        case 403:
          message = '拒绝访问'
          break
        case 404:
          message = '请求的资源不存在'
          break
        case 409:
          message = '资源冲突，可能已存在'
          break
        case 500:
          message = '服务器内部错误'
          break
        default:
          message = `请求错误(${status})`
      }
      
      // 返回统一的错误格式
      return Promise.reject({
        success: false,
        data: null,
        message: error.response.data?.message || message,
        errorCode: error.response.data?.errorCode || status.toString()
      })
    }
    
    // 处理请求错误（如网络错误）
    return Promise.reject({
      success: false,
      data: null,
      message: error.message || '网络异常，请检查网络连接',
      errorCode: 'NETWORK_ERROR'
    })
  }
)

export default request
