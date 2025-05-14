/**
 * @file jobRouter.cpp
 * @brief 职位路由实现文件，处理职位相关API路由
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#include "jobRouter.h"
#include "../Service/applicationService.h"
#include "../Middleware/authMiddleware.h"
#include <crow/json.h>

namespace Routers {
    void JobRouter::setupRoutes(crow::SimpleApp& app) {
        CROW_ROUTE(app, "/api/jobs/apply")
        .methods("POST")
        .CROW_MIDDLEWARES(app, AuthMiddleware)
        ([](const crow::request& req) {
            auto json = crow::json::load(req.body);
            if (!json) {
                return crow::response(400, "Invalid JSON");
            }
            
            std::string userId = req.get_header_value("X-User-ID");
            std::string jobId = json["jobId"].s();
            std::string resumeId = json["resumeId"].s();
            
            auto result = Service::ApplicationService::applyForJob(userId, jobId, resumeId);
            if (!result.success) {
                return crow::response(400, result.message);
            }
            
            return crow::response(201, "Application submitted successfully");
        });
    }
}