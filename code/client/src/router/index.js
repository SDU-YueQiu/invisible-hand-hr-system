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
  {
    path: '/register',
    name: 'Register',
    component: () => import('../views/public/Register.vue'),
  },
  {
    path: '/register/enterprise',
    name: 'EnterpriseRegister',
    component: () => import('../views/public/EnterpriseRegister.vue'),
  },
//   {
//     path: '/jobs',
//     name: 'JobList',
//     component: () => import('../views/jobs/JobList.vue'),
//   },
//   {
//     path: '/jobs/:id',
//     name: 'JobDetail',
//     component: () => import('../views/jobs/JobDetail.vue'),
//   }
]

const router = createRouter({
  history: createWebHistory(),
  routes
})

export default router
