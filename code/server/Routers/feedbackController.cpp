/**
 * @file feedbackController.cpp
 * @brief 反馈控制器实现，处理用户反馈相关API请求
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.0
 */

#include "feedbackController.h"
#include "../Utils/securityUtils.h"
#include <crow/json.h>

namespace Router
{
    void FeedbackController::submitFeedback(const crow::request &request, crow::response &response)
    {
        try
        {
            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body || !body.has("feedbackType") || !body.has("content"))
            {
                response.code = 400;
                response.write("无效的请求格式");
                return;
            }

            // 构建反馈对象
            Model::Feedback feedbackData;

            // 如果用户已登录，从JWT获取用户信息
            std::string token = request.get_header_value("Authorization");
            if (!token.empty())
            {
                feedbackData.UserID = std::stoi(Utils::SecurityUtils::getUserIdFromToken(token));
                feedbackData.UserType = Utils::SecurityUtils::getRoleFromToken(token);
            }

            // 设置反馈内容
            feedbackData.FeedbackType = body["feedbackType"].s();
            feedbackData.Content = body["content"].s();
            if (body.has("contactInfo"))
                feedbackData.ContactInfo = body["contactInfo"].s();

            // 调用服务层提交反馈
            bool success = Service::FeedbackService::getInstance().submitFeedback(feedbackData);

            if (!success)
            {
                response.code = 400;
                response.write("提交反馈失败");
                return;
            }

            // 构建成功响应
            crow::json::wvalue result;
            result["success"] = true;
            result["message"] = "反馈提交成功";

            response.code = 201;
            response.write(result.dump());
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "提交反馈失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void FeedbackController::getFeedbacks(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证用户权限
            std::string token = request.get_header_value("Authorization");
            if (token.empty())
            {
                response.code = 401;
                response.write("需要登录才能查看反馈");
                return;
            }

            // 获取用户ID
            std::string userId = Utils::SecurityUtils::getUserIdFromToken(token);
            if (userId.empty())
            {
                response.code = 401;
                response.write("无效的授权令牌");
                return;
            }

            // 构建筛选条件 - 只获取当前用户的反馈
            std::string filter = "UserID = " + userId;

            // 调用服务层获取反馈列表
            auto feedbacks = Service::FeedbackService::getInstance().getFeedbacks(filter);

            // 构建响应JSON数组
            crow::json::wvalue::list feedbackList;
            for (const auto &feedback: feedbacks)
            {
                crow::json::wvalue item;
                item["feedbackId"] = feedback.FeedbackID;
                item["feedbackType"] = feedback.FeedbackType;
                item["content"] = feedback.Content;
                item["status"] = feedback.Status;
                item["createTime"] = feedback.CreateTime;
                feedbackList.push_back(item);
            }

            response.code = 200;
            response.write(crow::json::wvalue(feedbackList).dump());
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取反馈列表失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }
}// namespace Router