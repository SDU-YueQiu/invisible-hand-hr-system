/**
 * @file authMiddleware.cpp
 * @brief 认证中间件AuthMiddleware的实现，用于JWT验证和角色检查
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.0
 */

#include "authMiddleware.h"
#include "../Config/config.h"
#include "../Utils/securityUtils.h"
#include <crow/utility.h>


namespace Middleware
{

    void AuthMiddleware::before_handle(crow::request &req, crow::response &res, context &ctx)
    {
        if (isLoginRoute(req))
            return;

        // 从请求头获取Authorization字段
        auto authHeader = req.get_header_value("Authorization");

        // 检查Authorization头是否存在
        if (authHeader.empty())
        {
            res.code = 401;
            res.write("Missing Authorization header");
            res.end();
            return;
        }

        // 提取Bearer Token
        std::string token;
        if (authHeader.find("Bearer ") == 0)
        {
            token = authHeader.substr(7);
        } else
        {
            res.code = 401;
            res.write("Invalid Authorization header format");
            res.end();
            return;
        }

        if (!Utils::SecurityUtils::verifyJWT(token))
        {
            res.code = 401;
            res.write("Invalid or expired token");
            res.end();
            return;
        }

        // 解析用户信息并存入上下文
        ctx.userId = Utils::SecurityUtils::getUserIdFromToken(token);
        ctx.userRole = Utils::SecurityUtils::getRoleFromToken(token);
    }

    auto requireRole(const std::vector<std::string> &requiredRoles)
    {
        return [requiredRoles](const crow::request &req, crow::response &res,
                               AuthMiddleware::context &ctx) {
            // 检查用户角色是否在允许的角色列表中
            bool hasPermission = false;
            for (const auto &role: requiredRoles)
            {
                if (ctx.userRole == role)
                {
                    hasPermission = true;
                    break;
                }
            }

            if (!hasPermission)
            {
                res.code = 403;
                res.write("Forbidden: insufficient permissions");
                res.end();
            }
        };
    }

}// namespace Middleware