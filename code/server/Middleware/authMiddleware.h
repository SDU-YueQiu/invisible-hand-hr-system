/**
 * @file authMiddleware.h
 * @brief 定义认证中间件AuthMiddleware及其上下文，用于JWT验证和角色检查
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.0
 */

#pragma once

#include "../Config/config.h"
#include "../Utils/securityUtils.h"
#include <crow.h>
#include <string>
#include <vector>

namespace Middleware
{

    /**
     * @struct AuthMiddleware
     * @brief Crow中间件，用于JWT认证并将解析出的用户信息存入中间件上下文中
     */
    struct AuthMiddleware
    {
        /**
         * @struct Context
         * @brief AuthMiddleware的自定义上下文，用于在请求处理链中传递认证信息
         */
        struct Context
        {
            std::string userId;  ///< 从JWT中解析出的用户ID
            std::string userRole;///< 从JWT中解析出的用户角色
        };

        /**
         * @brief 构造函数，初始化安全工具
         */
        AuthMiddleware();

        /**
         * @brief 请求处理前的钩子函数，执行JWT验证
         * @param req Crow请求对象
         * @param res Crow响应对象
         * @param ctx 中间件的自定义上下文
         */
        void before_handle(crow::request &req, crow::response &res, Context &ctx);

        /**
         * @brief 请求处理后的钩子函数（占位）
         * @param req Crow请求对象
         * @param res Crow响应对象
         * @param ctx 中间件的自定义上下文
         */
        void after_handle(crow::request &req, crow::response &res, Context &ctx);
    };

    /**
     * @brief 角色检查中间件工厂函数
     * @param requiredRoles 允许访问的角色列表
     * @return 返回一个中间件函数，用于检查用户角色
     */
    auto requireRole(const std::vector<std::string> &requiredRoles);
}// namespace Middleware