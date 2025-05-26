// src/stores/index.js
import { defineStore } from 'pinia'
import { ref } from 'vue'
import { useLocalStorage } from '@vueuse/core'

export const useUserStore = defineStore('user', () => {
  // 使用 useLocalStorage 让数据持久化且响应式
  const token = useLocalStorage('token', '')
  const userType = useLocalStorage('userType', '')
  const userId = useLocalStorage('userId', null)
  const username = useLocalStorage('username', '')
  
  function setUser(data) {
    token.value = data.token
    userType.value = data.userType
    userId.value = data.userId
    username.value = data.username
  }
  
  function clearUser() {
    token.value = ''
    userType.value = ''
    userId.value = null
    username.value = ''
  }
  
  return { token, userType, userId, username, setUser, clearUser }
})

export const useEnterpriseStore = defineStore('enterprise', () => {
  const token = ref('')
  const userType = ref('') // 'individual', 'enterprise', 'admin'
  const userId = ref(null)
  const username = ref('')
  
  function setUser(data) {
    token.value = data.token
    userType.value = data.userType
    userId.value = data.userId
    username.value = data.username
  }
  
  function clearUser() {
    token.value = ''
    userType.value = ''
    userId.value = null
    username.value = ''
  }
  
  return { token, userType, userId, username, setUser, clearUser }
})
