/**
 * @file apiRouter.h
 * @brief API路由管理器，集中配置所有API端点
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.0
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


namespace Routers
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
            // 认证相关路由
            CROW_ROUTE(app, "/api/v1/auth/individual/register").methods("POST"_method)(&Router::AuthController::registerIndividual);
            CROW_ROUTE(app, "/api/v1/auth/individual/login").methods("POST"_method)(&Router::AuthController::loginIndividual);
            CROW_ROUTE(app, "/api/v1/auth/enterprise/register").methods("POST"_method)(&Router::AuthController::registerEnterprise);
            CROW_ROUTE(app, "/api/v1/auth/enterprise/login").methods("POST"_method)(&Router::AuthController::loginEnterprise);
            CROW_ROUTE(app, "/api/v1/auth/admin/login").methods("POST"_method)(&Router::AuthController::loginAdmin);
            CROW_ROUTE(app, "/api/v1/auth/logout").methods("POST"_method)(&Router::AuthController::logout);

            // 个人用户相关路由
            CROW_ROUTE(app, "/api/v1/users/me").methods("GET"_method)(&Router::UserController::getCurrentUserInfo);
            CROW_ROUTE(app, "/api/v1/users/me").methods("PUT"_method)(&Router::UserController::updateUserInfo);
            CROW_ROUTE(app, "/api/v1/users/me/password").methods("PUT"_method)(&Router::UserController::changePassword);

            // 简历相关路由
            CROW_ROUTE(app, "/api/v1/users/me/resumes").methods("GET"_method)(&Router::ResumeController::getResumes);
            CROW_ROUTE(app, "/api/v1/users/me/resumes").methods("POST"_method)(&Router::ResumeController::createResume);
            CROW_ROUTE(app, "/api/v1/users/me/resumes").methods("GET"_method)(&Router::ResumeController::getResume);
            CROW_ROUTE(app, "/api/v1/users/me/resumes").methods("PUT"_method)(&Router::ResumeController::updateResume);
            CROW_ROUTE(app, "/api/v1/users/me/resumes").methods("DELETE"_method)(&Router::ResumeController::deleteResume);

            // 职位相关路由
            CROW_ROUTE(app, "/api/v1/jobs").methods("GET"_method)(&Router::JobController::getJobs);
            CROW_ROUTE(app, "/api/v1/jobs").methods("GET"_method)(&Router::JobController::getJobDetails);
            CROW_ROUTE(app, "/api/v1/jobs/apply").methods("POST"_method)(&Router::JobController::applyForJob);

            // 企业用户相关路由
            CROW_ROUTE(app, "/api/v1/enterprises/me").methods("GET"_method)(&Router::EnterpriseController::getEnterpriseInfo);
            CROW_ROUTE(app, "/api/v1/enterprises/me").methods("PUT"_method)(&Router::EnterpriseController::updateEnterpriseInfo);
            CROW_ROUTE(app, "/api/v1/enterprises/me/password").methods("PUT"_method)(&Router::EnterpriseController::changePassword);
            CROW_ROUTE(app, "/api/v1/enterprises/me/jobs").methods("GET"_method)(&Router::EnterpriseController::getPostedJobs);

            // 管理员相关路由
            CROW_ROUTE(app, "/api/v1/admin/users").methods("GET"_method)(&Router::AdminController::getUsers);
            CROW_ROUTE(app, "/api/v1/admin/users/status").methods("PUT"_method)(&Router::AdminController::updateUserStatus);
            CROW_ROUTE(app, "/api/v1/admin/enterprises").methods("GET"_method)(&Router::AdminController::getEnterprises);
            CROW_ROUTE(app, "/api/v1/admin/enterprises/approve").methods("POST"_method)(&Router::AdminController::approveEnterprise);
            CROW_ROUTE(app, "/api/v1/admin/enterprises/reject").methods("POST"_method)(&Router::AdminController::rejectEnterprise);

            // 反馈相关路由
            CROW_ROUTE(app, "/api/v1/feedbacks").methods("POST"_method)(&Router::FeedbackController::submitFeedback);
            CROW_ROUTE(app, "/api/v1/feedbacks").methods("GET"_method)(&Router::FeedbackController::getFeedbacks);
        }
    };
}// namespace Routers