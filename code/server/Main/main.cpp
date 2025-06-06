/**
 * @file main.cpp
 * @brief Crow应用主入口，负责初始化并启动整个HR系统后端服务
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.0
 */

#include "../Config/config.h"
#include "../Database/databaseManager.h"
#include "../Middleware/authMiddleware.h"
#include "../Middleware/validationMiddleware.h"
#include "../Routers/apiRouter.h"
#include <crow.h>
#include <crow/middlewares/cors.h>

int main()
{
    // 初始化配置
    Config::loadConfig();
    DAL::initDatabase();
    Utils::SecurityUtils::init();

    // 创建Crow应用并配置中间件
    crow::App<crow::CORSHandler, Middleware::AuthMiddleware, Middleware::ValidationMiddleware> app;


    // 配置CORS
    auto &cors = app.get_middleware<crow::CORSHandler>();
    cors
            .global()
            .headers("Content-Type", "Authorization")
            .methods("GET"_method, "POST"_method, "PUT"_method, "DELETE"_method);

    // 设置API路由
    Router::ApiRouter::setupRoutes(app);

    // 配置日志级别
    crow::logger::setLogLevel(crow::LogLevel::Error);

    // 启动服务器
    try
    {
        app.port(Config::globalConfig["PORT"].i())
                .bindaddr("127.0.0.1")
                .multithreaded()
                .run();
    } catch (std::exception &e)
    {
        CROW_LOG_ERROR << "Failed to start server: " << e.what();
    }

    return 0;
}