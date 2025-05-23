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
  {
    path: '/jobs',
    name: 'JobList',
    component: () => import('../views/jobs/JobList.vue'),
  },
  {
    path: '/jobs/:id',
    name: 'JobDetail',
    component: () => import('../views/jobs/JobDetail.vue'),
  },
  // 个人用户路由
  {
    path: '/user',
    component: () => import('../views/layouts/UserLayout.vue'),
    // children: [
    //   {
    //     path: 'dashboard',
    //     name: 'UserDashboard',
    //     component: () => import('../views/user/Dashboard.vue'),
    //     meta: { requiresAuth: true, userType: 'individual' }
    //   },
    //   {
    //     path: 'profile',
    //     name: 'UserProfile',
    //     component: () => import('../views/user/Profile.vue'),
    //     meta: { requiresAuth: true, userType: 'individual' }
    //   },
    //   {
    //     path: 'resumes',
    //     name: 'UserResumes',
    //     component: () => import('../views/user/ResumeList.vue'),
    //     meta: { requiresAuth: true, userType: 'individual' }
    //   },
    //   {
    //     path: 'resume/create',
    //     name: 'CreateResume',
    //     component: () => import('../views/user/ResumeForm.vue'),
    //     meta: { requiresAuth: true, userType: 'individual' }
    //   },
    //   {
    //     path: 'resume/edit/:id',
    //     name: 'EditResume',
    //     component: () => import('../views/user/ResumeForm.vue'),
    //     meta: { requiresAuth: true, userType: 'individual' }
    //   },
    //   {
    //     path: 'resume/view/:id',
    //     name: 'ViewResume',
    //     component: () => import('../views/user/ResumeView.vue'),
    //     meta: { requiresAuth: true, userType: 'individual' }
    //   },
    //   {
    //     path: 'applications',
    //     name: 'UserApplications',
    //     component: () => import('../views/user/ApplicationList.vue'),
    //     meta: { requiresAuth: true, userType: 'individual' }
    //   },
    //   {
    //     path: 'settings',
    //     name: 'UserSettings',
    //     component: () => import('../views/user/Settings.vue'),
    //     meta: { requiresAuth: true, userType: 'individual' }
    //   }
    // ]
  }
]

const router = createRouter({
  history: createWebHistory(),
  routes
})

export default router
