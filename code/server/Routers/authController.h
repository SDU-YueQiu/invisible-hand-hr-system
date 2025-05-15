/**
 * @file authController.h
 * @brief 认证控制器，处理用户注册、登录等认证相关API请求
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.0
 */

#pragma once

#include "../Model/authResult.h"
#include "../Service/authService.h"
#include <crow.h>

namespace Router
{
    /**
     * @brief 认证控制器，处理所有认证相关的API请求
     */
    class AuthController
    {
    public:
        /**
         * @brief 个人用户注册
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void registerIndividual(const crow::request& request, crow::response& response);

        /**
         * @brief 个人用户登录(用户名)
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void loginIndividual(const crow::request& request, crow::response& response);

        /**
         * @brief 个人用户登录(手机号)
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void loginIndividualByPhone(const crow::request& request, crow::response& response);

        /**
         * @brief 企业用户注册
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void registerEnterprise(const crow::request& request, crow::response& response);

        /**
         * @brief 企业用户登录(用户名)
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void loginEnterprise(const crow::request& request, crow::response& response);

        /**
         * @brief 管理员登录
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void loginAdmin(const crow::request& request, crow::response& response);

        /**
         * @brief 用户退出登录
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void logout(const crow::request& request, crow::response& response);
    };
} // namespace Router