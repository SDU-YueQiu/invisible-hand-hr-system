/**
 * @file validationMiddleware.cpp
 * @brief 验证中间件ValidationMiddleware的实现，用于请求数据的JSON Schema验证，目前没用
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.1
 */

#include "validationMiddleware.h"
#include <crow/json.h>
#include <crow/logging.h>

namespace Middleware
{

    void ValidationMiddleware::before_handle(crow::request &req, crow::response &res, context &ctx)
    {
        // 空实现，由validateRequest工厂方法提供具体验证逻辑
    }

    void ValidationMiddleware::after_handle(crow::request &req, crow::response &res, context &ctx)
    {
        // 空实现
    }

    auto validateRequest(const crow::json::rvalue &schema)
    {
        return [schema](const crow::request &req, crow::response &res,
                        ValidationMiddleware::context &ctx) {
            try
            {
                // 解析请求体为JSON
                auto body = crow::json::load(req.body);
                if (!body)
                {
                    res.code = 400;
                    res.write("Invalid JSON format in request body");
                    res.end();
                    return;
                }

                // 使用ValidationUtils验证请求体
                if (!Utils::ValidationUtils::validateSchema(body, schema))
                {
                    res.code = 400;
                    res.write("Request validation failed");
                    res.end();
                    return;
                }
            } catch (const std::exception &e)
            {
                CROW_LOG_ERROR << "Request validation error: " << e.what();
                res.code = 400;
                res.write("Request validation error");
                res.end();
            }
        };
    }

}// namespace Middleware