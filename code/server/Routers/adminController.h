/**
 * @file adminController.h
 * @brief 管理员控制器，处理系统管理员相关API请求
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.0
 */

#pragma once

#include "../Service/adminService.h"
#include "../Service/announcementService.h"
#include "../Service/feedbackService.h"
#include <crow.h>


namespace Router
{
    /**
     * @brief 管理员控制器，处理系统管理相关操作
     */
    class AdminController
    {
    public:
        /**
         * @brief 获取用户列表(支持分页和筛选)
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void getUsers(const crow::request &request, crow::response &response);

        /**
         * @brief 更新用户状态(启用/禁用)
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void updateUserStatus(const crow::request &request, crow::response &response, int id);

        /**
         * @brief 获取企业列表(支持分页和筛选)
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void getEnterprises(const crow::request &request, crow::response &response);

        /**
         * @brief 批准企业注册申请
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void approveEnterprise(const crow::request &request, crow::response &response);

        /**
         * @brief 拒绝企业注册申请
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void rejectEnterprise(const crow::request &request, crow::response &response);

        /**
         * @brief 获取职位列表(管理员视图)
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void getJobs(const crow::request &request, crow::response &response);

        /**
         * @brief 更新职位状态(审核通过/不通过)
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void updateJobStatus(const crow::request &request, crow::response &response);

        /**
         * @brief 获取反馈/留言列表
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void getFeedbacks(const crow::request &request, crow::response &response);

        /**
         * @brief 处理反馈
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void processFeedback(const crow::request &request, crow::response &response);

        /**
         * @brief 获取系统公告列表
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void getAnnouncements(const crow::request &request, crow::response &response);

        /**
         * @brief 创建系统公告
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void createAnnouncement(const crow::request &request, crow::response &response);

        /**
         * @brief 更新系统公告
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void updateAnnouncement(const crow::request &request, crow::response &response);

        /**
         * @brief 删除系统公告
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void deleteAnnouncement(const crow::request &request, crow::response &response);
    };
}// namespace Router