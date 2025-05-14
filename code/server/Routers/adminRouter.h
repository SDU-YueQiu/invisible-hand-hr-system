/**
 * @file adminRouter.h
 * @brief 管理路由头文件，声明管理员相关API路由
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include <crow.h>

namespace Routers {
    class AdminRouter {
    public:
        static void setupRoutes(crow::SimpleApp& app);
    };
}