/**
 * @file feedbackController.h
 * @brief 反馈控制器，处理用户反馈相关API请求
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.0
 */

#pragma once

#include "../Service/feedbackService.h"
#include <crow.h>

namespace Router
{
    /**
     * @brief 反馈控制器，处理用户反馈提交和查询
     */
    class FeedbackController
    {
    public:
        /**
         * @brief 提交用户反馈
         * @param request Crow请求对象
         * @param response Crow响应对象
         * @note 支持匿名用户和已登录用户提交反馈
         */
        static void submitFeedback(const crow::request &request, crow::response &response);

        /**
         * @brief 获取用户自己的反馈列表
         * @param request Crow请求对象
         * @param response Crow响应对象
         * @note 仅返回当前登录用户提交的反馈
         */
        static void getFeedbacks(const crow::request &request, crow::response &response);
    };
} // namespace Router