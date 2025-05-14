/**
 * @file authMiddleware.cpp
 * @brief 认证中间件实现文件，处理JWT验证逻辑
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#include "authMiddleware.h"
#include "../Utils/SecurityUtils.h"
#include <crow/utility.h>

namespace Middleware {
    struct AuthMiddleware {
        struct context {
            std::string userId;
        };
        
        void before_handle(crow::request& req, crow::response& res, context& ctx) {
            std::string authHeader = req.get_header_value("Authorization");
            if (authHeader.empty()) {
                res.code = 401;
                res.write("Missing authorization header");
                res.end();
                return;
            }
            
            // 提取Bearer token
            std::string token = authHeader.substr(7); // 跳过"Bearer "前缀
            std::string userId;
            
            if (!Utils::SecurityUtils::verifyJWT(token, userId)) {
                res.code = 401;
                res.write("Invalid or expired token");
                res.end();
                return;
            }
            
            ctx.userId = userId;
            req.add_header("X-User-ID", userId);
        }
        
        void after_handle(crow::request& req, crow::response& res, context& ctx) {
            // 可在此处添加后处理逻辑
        }
    };
}