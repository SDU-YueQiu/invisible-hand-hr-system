/**
 * @file userController.h
 * @brief 处理个人用户账户信息管理相关的API请求
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.0
 */

#pragma once

#include <crow.h>
#include "../Service/individualUserService.h"

namespace Router
{
    /**
     * @brief 个人用户账户信息管理控制器
     */
    class UserController
    {
    public:
        /**
         * @brief 获取当前登录个人用户的详细信息
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void getCurrentUserInfo(const crow::request& request, crow::response& response);

        /**
         * @brief 修改当前登录个人用户的基本信息
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void updateUserInfo(const crow::request& request, crow::response& response);

        /**
         * @brief 当前登录个人用户修改自己的密码
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void changePassword(const crow::request& request, crow::response& response);

        /**
         * @brief 获取当前登录个人用户的所有简历列表
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void getResumes(const crow::request& request, crow::response& response);

        /**
         * @brief 获取当前登录个人用户的所有求职申请记录
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void getApplications(const crow::request& request, crow::response& response);
    };
} // namespace Router