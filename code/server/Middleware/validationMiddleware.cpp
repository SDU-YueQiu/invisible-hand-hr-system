/**
 * @file validationMiddleware.cpp
 * @brief 数据验证中间件实现，用于校验API请求数据的有效性
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.0
 */

#include "validationMiddleware.h"
#include "../Utils/validationUtils.h"
#include <crow/logging.h>

namespace Middleware
{
    auto ValidationMiddleware::validateRequest(const crow::json::rvalue& schema)
    {
        return [schema](const crow::request& req, crow::response& res, Context& ctx) {
            try {
                // 获取请求体数据
                crow::json::rvalue requestData;
                if (!req.body.empty()) {
                    requestData = crow::json::load(req.body);
                    if (requestData.error()) {
                        res.code = 400;
                        res.write("Invalid JSON format in request body");
                        res.end();
                        return;
                    }
                }

                // 使用ValidationUtils验证数据
                if (!Utils::ValidationUtils::validateSchema(requestData, schema)) {
                    res.code = 400;
                    res.write("Request data validation failed");
                    res.end();
                    return;
                }

                // 验证通过，继续处理
            } catch (const std::exception& e) {
                CROW_LOG_ERROR << "Validation middleware error: " << e.what();
                res.code = 500;
                res.write("Internal server error during validation");
                res.end();
            }
        };
    }
}