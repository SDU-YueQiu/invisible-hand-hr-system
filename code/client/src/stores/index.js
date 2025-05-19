// src/stores/index.js
import { defineStore } from 'pinia'
import { ref } from 'vue'

export const useUserStore = defineStore('user', () => {
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
