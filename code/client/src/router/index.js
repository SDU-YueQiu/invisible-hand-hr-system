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
    children: [
      {
        path: 'dashboard',
        name: 'UserDashboard',
        component: () => import('../views/user/Dashboard.vue'),
        meta: { requiresAuth: true, userType: 'individual' }
      },
      {
        path: 'profile',
        name: 'UserProfile',
        component: () => import('../views/user/Profile.vue'),
        meta: { requiresAuth: true, userType: 'individual' }
      },
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
      {
        path: 'settings',
        name: 'UserSettings',
        component: () => import('../views/user/Settings.vue'),
        meta: { requiresAuth: true, userType: 'individual' }
      }
    ]
  },
    // 企业用户路由
  {
    path: '/enterprise',
    component: () => import('../views/layouts/EnterpriseLayout.vue'),
    children: [
      {
        path: 'dashboard',
        name: 'EnterpriseDashboard',
        component: () => import('../views/enterprise/Dashboard.vue'),
        meta: { requiresAuth: true, userType: 'enterprise' }
      },
//       {
//         path: 'profile',
//         name: 'EnterpriseProfile',
//         component: () => import('../views/enterprise/Profile.vue'),
//         meta: { requiresAuth: true, userType: 'enterprise' }
//       },
//       {
//         path: 'jobs',
//         name: 'EnterpriseJobs',
//         component: () => import('../views/enterprise/JobList.vue'),
//         meta: { requiresAuth: true, userType: 'enterprise' }
//       },
//       {
//         path: 'job/create',
//         name: 'CreateJob',
//         component: () => import('../views/enterprise/JobForm.vue'),
//         meta: { requiresAuth: true, userType: 'enterprise' }
//       },
//       {
//         path: 'job/edit/:id',
//         name: 'EditJob',
//         component: () => import('../views/enterprise/JobForm.vue'),
//         meta: { requiresAuth: true, userType: 'enterprise' }
//       },
//       {
//         path: 'job/:id/applicants',
//         name: 'JobApplicants',
//         component: () => import('../views/enterprise/Applicants.vue'),
//         meta: { requiresAuth: true, userType: 'enterprise' }
//       },
//       {
//         path: 'talents',
//         name: 'TalentSearch',
//         component: () => import('../views/enterprise/TalentSearch.vue'),
//         meta: { requiresAuth: true, userType: 'enterprise' }
//       },
//       {
//         path: 'settings',
//         name: 'EnterpriseSettings',
//         component: () => import('../views/enterprise/Settings.vue'),
//         meta: { requiresAuth: true, userType: 'enterprise' }
//       }
     ]
  },
  // 管理员路由
//   {
//     path: '/admin',
//     component: () => import('../views/layouts/AdminLayout.vue'),
//     children: [
//       {
//         path: 'dashboard',
//         name: 'AdminDashboard',
//         component: () => import('../views/admin/Dashboard.vue'),
//         meta: { requiresAuth: true, userType: 'admin' }
//       },
//       {
//         path: 'users',
//         name: 'UserManagement',
//         component: () => import('../views/admin/UserManagement.vue'),
//         meta: { requiresAuth: true, userType: 'admin' }
//       },
//       {
//         path: 'enterprises',
//         name: 'EnterpriseManagement',
//         component: () => import('../views/admin/EnterpriseManagement.vue'),
//         meta: { requiresAuth: true, userType: 'admin' }
//       },
//       {
//         path: 'jobs',
//         name: 'JobManagement',
//         component: () => import('../views/admin/JobManagement.vue'),
//         meta: { requiresAuth: true, userType: 'admin' }
//       },
//       {
//         path: 'feedback',
//         name: 'FeedbackManagement',
//         component: () => import('../views/admin/FeedbackManagement.vue'),
//         meta: { requiresAuth: true, userType: 'admin' }
//       },
//       {
//         path: 'announcements',
//         name: 'AnnouncementManagement',
//         component: () => import('../views/admin/AnnouncementManagement.vue'),
//         meta: { requiresAuth: true, userType: 'admin' }
//       },
//       {
//         path: 'settings',
//         name: 'AdminSettings',
//         component: () => import('../views/admin/Settings.vue'),
//         meta: { requiresAuth: true, userType: 'admin' }
//       }
//     ]
//   }
//]
// }
]

const router = createRouter({
  history: createWebHistory(),
  routes
})

export default router
