/**
 * @file enterpriseController.h
 * @brief 企业用户控制器，处理企业用户账户信息管理和职位管理相关API请求
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.0
 */

#pragma once

#include "../Service/enterpriseUserService.h"
#include "../Service/jobPostingService.h"
#include "../Service/talentSearchService.h"
#include <crow.h>


namespace Router
{
    /**
     * @brief 企业用户控制器，处理企业用户相关操作
     */
    class EnterpriseController
    {
    public:
        /**
         * @brief 获取当前登录企业用户的详细信息
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void getEnterpriseInfo(const crow::request &request, crow::response &response);

        /**
         * @brief 修改当前登录企业用户的基本信息
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void updateEnterpriseInfo(const crow::request &request, crow::response &response);

        /**
         * @brief 修改企业用户密码
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void changePassword(const crow::request &request, crow::response &response);

        /**
         * @brief 获取当前企业发布的所有职位
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void getPostedJobs(const crow::request &request, crow::response &response);

        /**
         * @brief 发布新职位
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void postNewJob(const crow::request &request, crow::response &response);

        /**
         * @brief 更新职位信息
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void updateJob(const crow::request &request, crow::response &response);

        /**
         * @brief 删除职位
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void deleteJob(const crow::request &request, crow::response &response);

        /**
         * @brief 获取职位申请者（简历）列表
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void getApplicants(const crow::request &request, crow::response &response);

        /**
         * @brief 更新申请状态
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void updateApplicationStatus(const crow::request &request, crow::response &response);
    };
}// namespace Router