/**
 * @file validationMiddleware.h
 * @brief 定义验证中间件ValidationMiddleware，用于请求数据的JSON Schema验证
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.0
 */

#pragma once

#include <crow.h>
#include "../Utils/validationUtils.h"

namespace Middleware {

    /**
     * @struct ValidationMiddleware
     * @brief Crow中间件，用于请求数据的JSON Schema验证
     */
    struct ValidationMiddleware {
        /**
         * @struct context
         * @brief ValidationMiddleware的自定义上下文
         */
        struct context {};

        /**
         * @brief 构造函数
         */
        ValidationMiddleware() = default;

        /**
         * @brief 请求处理前的钩子函数
         */
        void before_handle(crow::request& req, crow::response& res, context& ctx);

        /**
         * @brief 请求处理后的钩子函数
         */
        void after_handle(crow::request& req, crow::response& res, context& ctx);
    };

    /**
     * @brief 创建请求验证中间件
     * @param schema 用于验证的JSON Schema
     * @return 返回一个中间件函数，用于验证请求数据
     */
    auto validateRequest(const crow::json::rvalue& schema);

} // namespace Middleware