// src/router/index.js
import { createRouter, createWebHistory } from 'vue-router'

const routes = [
  {
    path: '/',
    name: 'Home',
    component: () => import('../views/public/Home.vue'),
  },
  {
    path: '/login',
    name: 'Login',
    component: () => import('../views/public/Login.vue'),
  },
  // 定义更多路由...
]

const router = createRouter({
  history: createWebHistory(),
  routes
})

export default router
