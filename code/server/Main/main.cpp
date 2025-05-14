/**
 * @file main.cpp
 * @brief 主应用入口文件，负责初始化和启动Crow应用
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#include <crow.h>
#include "../Config/config.h"
#include "../Middleware/authMiddleware.h"
#include "../Routers/authRouter.h"
#include "../Routers/jobRouter.h"
#include "../Routers/adminRouter.h"

int main()
{
    // 加载配置
    Config::loadConfig();
    
    // 初始化Crow应用
    crow::SimpleApp app;
    
    // 注册中间件
    app.use(AuthMiddleware::handleAuth);
    
    // 注册路由
    AuthRouter::setupRoutes(app);
    JobRouter::setupRoutes(app);
    AdminRouter::setupRoutes(app);
    
    // 启动应用
    app.port(Config::globalConfig["PORT"].i())
       .multithreaded()
       .run();
    
    return 0;
}