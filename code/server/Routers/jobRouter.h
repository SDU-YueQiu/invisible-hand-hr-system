/**
 * @file jobRouter.h
 * @brief 职位路由头文件，声明职位相关API路由
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include <crow.h>

namespace Routers {
    class JobRouter {
    public:
        static void setupRoutes(crow::SimpleApp& app);
    };
}