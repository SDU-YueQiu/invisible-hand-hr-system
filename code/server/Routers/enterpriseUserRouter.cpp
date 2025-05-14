/**
 * @file enterpriseUserRouter.cpp
 * @brief 企业用户路由实现文件，处理企业用户管理相关API路由
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#include "enterpriseUserRouter.h"
#include "../Service/enterpriseUserService.h"
#include "../Middleware/authMiddleware.h"
#include <crow/json.h>

namespace Routers {
    void EnterpriseUserRouter::setupRoutes(crow::SimpleApp& app) {
        CROW_ROUTE(app, "/api/enterprise/register")
        .methods("POST")
        ([](const crow::request& req) {
            auto json = crow::json::load(req.body);
            if (!json) {
                return crow::response(400, "Invalid JSON");
            }
            
            auto result = Service::EnterpriseUserService::registerEnterprise(
                json["companyName"].s(),
                json["username"].s(),
                json["password"].s(),
                json["email"].s(),
                json["contactPerson"].s(),
                json["contactPhone"].s());
                
            if (!result.success) {
                return crow::response(400, result.message);
            }
            
            return crow::response(201, "Enterprise registered successfully");
        });
        
        CROW_ROUTE(app, "/api/enterprise/update")
        .methods("PUT")
        .CROW_MIDDLEWARES(app, AuthMiddleware)
        ([](const crow::request& req) {
            auto json = crow::json::load(req.body);
            if (!json) {
                return crow::response(400, "Invalid JSON");
            }
            
            std::string enterpriseId = req.get_header_value("X-User-ID");
            auto result = Service::EnterpriseUserService::updateEnterpriseInfo(
                enterpriseId,
                json["companyName"].s(),
                json["email"].s(),
                json["contactPerson"].s(),
                json["contactPhone"].s());
                
            if (!result.success) {
                return crow::response(400, result.message);
            }
            
            return crow::response(200, "Enterprise info updated successfully");
        });
    }
}