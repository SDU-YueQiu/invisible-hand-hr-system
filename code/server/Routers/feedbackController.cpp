/**
 * @file feedbackController.cpp
 * @brief 反馈控制器实现，处理用户反馈相关API请求
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.1
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
                crow::json::wvalue error_json;
                error_json["message"] = "无效的请求格式";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 构建反馈对象
            Model::Feedback feedbackData;

            // 如果用户已登录，从JWT获取用户信息
            std::string auth_header = request.get_header_value("Authorization");
            if (!auth_header.empty() && auth_header.rfind("Bearer ", 0) == 0)
            {
                std::string token = auth_header.substr(7);
                if (!token.empty())
                {
                    std::string userIdStr = Utils::SecurityUtils::getUserIdFromToken(token);
                    if (!userIdStr.empty())
                    {// Ensure userIdStr is not empty before stoi
                        try
                        {
                            feedbackData.UserID = std::stoi(userIdStr);
                        } catch (const std::invalid_argument &ia)
                        {
                            CROW_LOG_WARNING << "Invalid user ID format from token: " << userIdStr;
                            // Decide how to handle: treat as anonymous or error out
                            // For now, let's assume it might proceed as anonymous if UserID remains 0
                        } catch (const std::out_of_range &oor)
                        {
                            CROW_LOG_WARNING << "User ID from token out of range: " << userIdStr;
                        }
                    }
                    feedbackData.UserType = Utils::SecurityUtils::getRoleFromToken(token);
                }
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
                crow::json::wvalue error_json;
                error_json["message"] = "提交反馈失败";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 构建成功响应
            crow::json::wvalue result;
            result["success"] = true;
            result["message"] = "反馈提交成功";

            response.code = 201;
            response.write(result.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "提交反馈失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }

    void FeedbackController::getFeedbacks(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证用户权限
            std::string token;
            std::string auth_header = request.get_header_value("Authorization");
            if (!auth_header.empty() && auth_header.rfind("Bearer ", 0) == 0)
            {
                token = auth_header.substr(7);
            }

            if (token.empty())
            {
                response.code = 401;
                crow::json::wvalue error_json;
                error_json["message"] = "需要登录才能查看反馈";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 获取用户ID
            std::string userId = Utils::SecurityUtils::getUserIdFromToken(token);
            if (userId.empty())
            {
                response.code = 401;
                crow::json::wvalue error_json;
                error_json["message"] = "无效的授权令牌";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 构建筛选条件 - 只获取当前用户的反馈
            std::string filter = "UserID = " + userId;

            // 调用服务层获取反馈列表
            auto feedbacks_data = Service::FeedbackService::getInstance().getFeedbacks(filter);

            // 构建响应JSON数组
            crow::json::wvalue::list feedbackList_json_array;
            for (const auto &feedback: feedbacks_data)
            {
                crow::json::wvalue item;
                item["feedbackId"] = feedback.FeedbackID;
                item["feedbackType"] = feedback.FeedbackType;
                item["content"] = feedback.Content;
                item["status"] = feedback.Status;
                item["createTime"] = feedback.CreateTime;
                feedbackList_json_array.push_back(std::move(item));
            }

            crow::json::wvalue result_json;
            result_json["message"] = "反馈列表获取成功";
            result_json["feedbacks"] = std::move(feedbackList_json_array);

            response.code = 200;
            response.write(result_json.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取反馈列表失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }
}// namespace Router