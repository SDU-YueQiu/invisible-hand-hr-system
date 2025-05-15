/**
 * @file validationMiddleware.h
 * @brief 数据验证中间件，用于校验API请求数据的有效性
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.0
 */

#pragma once

#include <crow.h>
#include <crow/json.h>

namespace Middleware
{
    /**
     * @struct ValidationMiddleware
     * @brief Crow中间件，用于验证请求数据是否符合预定义模式
     */
    struct ValidationMiddleware
    {
        /**
         * @struct Context
         * @brief ValidationMiddleware的自定义上下文
         */
        struct Context
        {
        };

        /**
         * @brief 请求验证工厂方法
         * @param schema 用于验证请求数据的JSON Schema
         * @return 返回一个中间件函数，用于验证请求数据
         */
        static auto validateRequest(const crow::json::rvalue& schema);
    };
}