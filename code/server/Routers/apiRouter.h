/**
 * @file apiRouter.h
 * @brief API路由管理器，集中配置所有API端点
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.1
 */

#pragma once

#include "../Middleware/authMiddleware.h"
#include "../Middleware/validationMiddleware.h"
#include "../Routers/adminController.h"
#include "../Routers/authController.h"
#include "../Routers/enterpriseController.h"
#include "../Routers/feedbackController.h"
#include "../Routers/jobController.h"
#include "../Routers/resumeController.h"
#include "../Routers/userController.h"
#include <crow.h>
#include <crow/middlewares/cors.h>


namespace Router
{
    /**
      * @brief API路由管理器类，负责集中配置所有API端点
      */
    class ApiRouter
    {
    public:
        /**
          * @brief 设置所有API路由
          * @param app Crow应用实例
          */
        static void setupRoutes(crow::App<crow::CORSHandler, Middleware::AuthMiddleware, Middleware::ValidationMiddleware> &app)
        {
            // 基础URL: /api/v1

            // 2. 认证 (Auth) 模块
            CROW_ROUTE(app, "/api/v1/auth/individual/register").methods("POST"_method)(&Router::AuthController::registerIndividual);
            CROW_ROUTE(app, "/api/v1/auth/individual/login").methods("POST"_method)(&Router::AuthController::loginIndividual);
            CROW_ROUTE(app, "/api/v1/auth/enterprise/register").methods("POST"_method)(&Router::AuthController::registerEnterprise);
            CROW_ROUTE(app, "/api/v1/auth/enterprise/login").methods("POST"_method)(&Router::AuthController::loginEnterprise);
            CROW_ROUTE(app, "/api/v1/auth/admin/login").methods("POST"_method)(&Router::AuthController::loginAdmin);
            CROW_ROUTE(app, "/api/v1/auth/logout").methods("POST"_method)(&Router::AuthController::logout);

            // 3. 个人用户 (Users) 模块
            CROW_ROUTE(app, "/api/v1/users/me").methods("GET"_method)(&Router::UserController::getCurrentUserInfo);
            CROW_ROUTE(app, "/api/v1/users/me").methods("PUT"_method)(&Router::UserController::updateUserInfo);
            CROW_ROUTE(app, "/api/v1/users/me/password").methods("PUT"_method)(&Router::UserController::changePassword);
            // 6.2 获取个人用户的求职申请列表 (归属于个人用户下)
            CROW_ROUTE(app, "/api/v1/users/me/applications").methods("GET"_method)(&Router::UserController::getApplications);// 确认Controller方法

            // 4. 简历 (Resumes) 模块 (隶属于个人用户)
            CROW_ROUTE(app, "/api/v1/users/me/resumes").methods("GET"_method)(&Router::ResumeController::getResumes);
            CROW_ROUTE(app, "/api/v1/users/me/resumes").methods("POST"_method)(&Router::ResumeController::createResume);
            CROW_ROUTE(app, "/api/v1/users/me/resumes/<int>").methods("GET"_method)(&Router::ResumeController::getResume);      // {resumeId}
            CROW_ROUTE(app, "/api/v1/users/me/resumes/<int>").methods("PUT"_method)(&Router::ResumeController::updateResume);   // {resumeId}
            CROW_ROUTE(app, "/api/v1/users/me/resumes/<int>").methods("DELETE"_method)(&Router::ResumeController::deleteResume);// {resumeId}

            // 5. 职位 (Jobs) 模块 (公开查询)
            CROW_ROUTE(app, "/api/v1/jobs").methods("GET"_method)(&Router::JobController::getJobs);
            CROW_ROUTE(app, "/api/v1/jobs/<int>").methods("GET"_method)(&Router::JobController::getJobDetails);// {jobId}

            // 6. 职位申请 (Applications) 模块
            // 6.1 个人用户申请职位 (基于职位ID)
            CROW_ROUTE(app, "/api/v1/jobs/<int>/apply").methods("POST"_method)(&Router::JobController::applyForJob);// {jobId}

            // 7. 企业用户 (Enterprises) 模块
            CROW_ROUTE(app, "/api/v1/enterprises/me").methods("GET"_method)(&Router::EnterpriseController::getEnterpriseInfo);
            CROW_ROUTE(app, "/api/v1/enterprises/me").methods("PUT"_method)(&Router::EnterpriseController::updateEnterpriseInfo);
            CROW_ROUTE(app, "/api/v1/enterprises/me/password").methods("PUT"_method)(&Router::EnterpriseController::changePassword);

            // 7.4 获取企业发布的职位列表
            CROW_ROUTE(app, "/api/v1/enterprises/me/jobs").methods("GET"_method)(&Router::EnterpriseController::getPostedJobs);
            // 7.5 企业发布新职位
            CROW_ROUTE(app, "/api/v1/enterprises/me/jobs").methods("POST"_method)(&Router::EnterpriseController::postNewJob);// 新增
            // 7.6 获取企业发布的某个职位详情
            CROW_ROUTE(app, "/api/v1/enterprises/me/jobs/<int>").methods("GET"_method)(&Router::EnterpriseController::getPostedJobDetail);// {jobId} - 新增
            // 7.7 修改企业发布的职位
            CROW_ROUTE(app, "/api/v1/enterprises/me/jobs/<int>").methods("PUT"_method)(&Router::EnterpriseController::updatePostedJob);// {jobId} - 新增
            // 7.8 删除企业发布的职位
            CROW_ROUTE(app, "/api/v1/enterprises/me/jobs/<int>").methods("DELETE"_method)(&Router::EnterpriseController::deletePostedJob);// {jobId} - 新增

            // 7.9 获取某职位的申请者列表
            CROW_ROUTE(app, "/api/v1/enterprises/me/jobs/<int>/applicants").methods("GET"_method)(&Router::EnterpriseController::getJobApplicants);// {jobId} - 路径已修正, 确认Controller方法名和参数
            // 7.10 更新职位申请状态 (企业操作)
            CROW_ROUTE(app, "/api/v1/enterprises/me/applications/<int>/status").methods("PUT"_method)(&Router::EnterpriseController::updateApplicationStatus);// {applicationId}
            // 7.11 搜索符合条件的简历
            CROW_ROUTE(app, "/api/v1/enterprises/me/resumes/search").methods("GET"_method)(&Router::EnterpriseController::searchResumes);// 新增

            // 8. 管理员 (Admin) 模块
            CROW_ROUTE(app, "/api/v1/admin/users").methods("GET"_method)(&Router::AdminController::getUsers);
            CROW_ROUTE(app, "/api/v1/admin/users/<int>/status").methods("PUT"_method)(&Router::AdminController::updateUserStatus);// {userId}

            CROW_ROUTE(app, "/api/v1/admin/enterprises").methods("GET"_method)(&Router::AdminController::getEnterprises);
            // 8.4 批准企业注册
            CROW_ROUTE(app, "/api/v1/admin/enterprises/<int>/approve").methods("PUT"_method)(&Router::AdminController::approveEnterprise);// {enterpriseId} - 路径和方法已修正
            // 8.5 拒绝企业注册
            CROW_ROUTE(app, "/api/v1/admin/enterprises/<int>/reject").methods("PUT"_method)(&Router::AdminController::rejectEnterprise);// {enterpriseId} - 路径和方法已修正
            // 8.6 更新企业状态 (通用)
            CROW_ROUTE(app, "/api/v1/admin/enterprises/<int>/status").methods("PUT"_method)(&Router::AdminController::updateEnterpriseStatus);// {enterpriseId} - 新增

            // 8.7 管理所有职位
            CROW_ROUTE(app, "/api/v1/admin/jobs").methods("GET"_method)(&Router::AdminController::getAllJobs);// 新增
            // 8.8 更新职位状态 (管理员操作)
            CROW_ROUTE(app, "/api/v1/admin/jobs/<int>/status").methods("PUT"_method)(&Router::AdminController::updateJobStatus);// {jobId} - 新增

            // 9. 反馈 (Feedback) 模块
            // 9.1 用户提交反馈
            CROW_ROUTE(app, "/api/v1/feedback").methods("POST"_method)(&Router::FeedbackController::submitFeedback);// 路径已修正
            // 9.2 管理员获取反馈列表
            CROW_ROUTE(app, "/api/v1/admin/feedbacks").methods("GET"_method)(&Router::AdminController::getFeedbacks);// 移至Admin下，并使用AdminController或特定FeedbackAdminController
            // 9.3 管理员处理反馈
            CROW_ROUTE(app, "/api/v1/admin/feedbacks/<int>").methods("PUT"_method)(&Router::AdminController::processFeedback);// {feedbackId} - 新增, 移至Admin下

            // 10. 公告 (Announcements) 模块
            // 10.1 获取公告列表 (公开)
            CROW_ROUTE(app, "/api/v1/announcements").methods("GET"_method)(&Router::AnnouncementController::getAnnouncements);// 新增 (需要AnnouncementController)
            // 10.2 获取公告详情 (公开)
            CROW_ROUTE(app, "/api/v1/announcements/<int>").methods("GET"_method)(&Router::AnnouncementController::getAnnouncementDetail);// {announcementId} - 新增
            // 10.3 管理员创建公告
            CROW_ROUTE(app, "/api/v1/admin/announcements").methods("POST"_method)(&Router::AdminController::createAnnouncement);// 新增 (或AnnouncementAdminController)
            // 10.4 管理员修改公告
            CROW_ROUTE(app, "/api/v1/admin/announcements/<int>").methods("PUT"_method)(&Router::AdminController::updateAnnouncement);// {announcementId} - 新增
            // 10.5 管理员删除公告
            CROW_ROUTE(app, "/api/v1/admin/announcements/<int>").methods("DELETE"_method)(&Router::AdminController::deleteAnnouncement);// {announcementId} - 新增
        }
    };
}// namespace Router