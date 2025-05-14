/**
 * @file authRouter.cpp
 * @brief 认证路由实现文件，处理用户认证相关API路由
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#include "authRouter.h"
#include "../Service/authService.h"
#include <crow/json.h>

namespace Routers {
    void AuthRouter::setupRoutes(crow::SimpleApp& app) {
        CROW_ROUTE(app, "/api/auth/login")
        .methods("POST")
        ([](const crow::request& req) {
            auto json = crow::json::load(req.body);
            if (!json) {
                return crow::response(400, "Invalid JSON");
            }
            
            std::string username = json["username"].s();
            std::string password = json["password"].s();
            
            auto result = Service::AuthService::login(username, password);
            if (!result.success) {
                return crow::response(401, result.message);
            }
            
            crow::json::wvalue response;
            response["token"] = result.token;
            return crow::response(200, response);
        });
        
        CROW_ROUTE(app, "/api/auth/register")
        .methods("POST")
        ([](const crow::request& req) {
            auto json = crow::json::load(req.body);
            if (!json) {
                return crow::response(400, "Invalid JSON");
            }
            
            std::string username = json["username"].s();
            std::string password = json["password"].s();
            std::string email = json["email"].s();
            
            auto result = Service::AuthService::registerUser(username, password, email);
            if (!result.success) {
                return crow::response(400, result.message);
            }
            
            crow::json::wvalue response;
            response["token"] = result.token;
            return crow::response(201, response);
        });
    }
}