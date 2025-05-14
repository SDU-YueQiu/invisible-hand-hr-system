/**
 * @file authRouter.h
 * @brief 认证路由头文件，声明用户认证相关API路由
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include <crow.h>

namespace Routers {
    class AuthRouter {
    public:
        static void setupRoutes(crow::SimpleApp& app);
    };
}