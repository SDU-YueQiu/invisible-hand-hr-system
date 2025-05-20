/**
 * @file userController.cpp
 * @brief 个人用户账户信息管理控制器实现
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.0
 */

#include "userController.h"
#include "../Model/individualUser.h"
#include "../Service/applicationService.h"
#include "../Service/individualUserService.h"
#include "../Utils/securityUtils.h"
#include <crow/json.h>
#include <string>// 为 std::stoi, std::stoll


namespace Router
{
    void UserController::getCurrentUserInfo(const crow::request &request, crow::response &response)
    {
        try
        {
            // 从JWT令牌中获取用户ID
            std::string token = request.get_header_value("Authorization").substr(7);
            std::string userId = Utils::SecurityUtils::getUserIdFromToken(token);

            if (userId.empty())
            {
                response.code = 401;
                response.write(crow::json::wvalue({{"message", "无效的授权令牌"}}).dump());
                response.end();
                return;
            }

            // 通过服务层获取用户信息
            auto user = Service::IndividualUserService::getInstance().getUserById(std::stoi(userId));
            if (user.UserID == -1)// 假设UserID为-1表示用户不存在
            {
                response.code = 404;
                response.write(crow::json::wvalue({{"message", "用户不存在"}}).dump());
                response.end();
                return;
            }

            // 构建响应JSON
            crow::json::wvalue result;
            result["userId"] = user.UserID;
            result["username"] = user.Username;
            result["phoneNumber"] = user.PhoneNumber;
            result["email"] = user.Email;
            result["accountStatus"] = user.AccountStatus;
            result["avatarURL"] = user.AvatarURL;

            response.code = 200;
            response.write(result.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取用户信息失败: " << e.what();
            response.code = 500;
            response.write(crow::json::wvalue({{"message", "服务器内部错误"}}).dump());
            response.end();
        }
    }

    void UserController::updateUserInfo(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证请求并获取用户ID
            std::string token = request.get_header_value("Authorization").substr(7);
            std::string userId = Utils::SecurityUtils::getUserIdFromToken(token);

            if (userId.empty())
            {
                response.code = 401;
                response.write(crow::json::wvalue({{"message", "无效的授权令牌"}}).dump());
                response.end();
                return;
            }

            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body)
            {
                response.code = 400;
                response.write(crow::json::wvalue({{"message", "无效的请求格式"}}).dump());
                response.end();
                return;
            }

            // 构建用户更新对象
            Model::IndividualUser userData;// 假设 Model::IndividualUser 定义了这些成员
            userData.UserID = std::stoi(userId);
            if (body.has("phoneNumber")) userData.PhoneNumber = body["phoneNumber"].s();
            if (body.has("email")) userData.Email = body["email"].s();
            if (body.has("avatarURL")) userData.AvatarURL = body["avatarURL"].s();

            // 调用服务层更新用户信息
            bool success = Service::IndividualUserService::getInstance().updateUserInfo(
                    std::stoi(userId), userData);

            if (!success)
            {
                response.code = 400;// 或者其他合适的错误码，如422 Unprocessable Entity
                response.write(crow::json::wvalue({{"message", "更新用户信息失败"}}).dump());
                response.end();
                return;
            }

            response.code = 200;
            response.write(crow::json::wvalue({{"message", "用户信息更新成功"}}).dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "更新用户信息失败: " << e.what();
            response.code = 500;
            response.write(crow::json::wvalue({{"message", "服务器内部错误"}}).dump());
            response.end();
        }
    }

    void UserController::changePassword(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证请求并获取用户ID
            std::string token = request.get_header_value("Authorization").substr(7);
            std::string userId = Utils::SecurityUtils::getUserIdFromToken(token);

            if (userId.empty())
            {
                response.code = 401;
                response.write(crow::json::wvalue({{"message", "无效的授权令牌"}}).dump());
                response.end();
                return;
            }

            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body || !body.has("oldPassword") || !body.has("newPassword"))
            {
                response.code = 400;
                response.write(crow::json::wvalue({{"message", "无效的请求格式"}}).dump());
                response.end();
                return;
            }

            // 调用服务层修改密码
            bool success = Service::IndividualUserService::getInstance().changePassword(
                    std::stoi(userId),
                    body["oldPassword"].s(),
                    body["newPassword"].s());

            if (!success)
            {
                response.code = 400;
                response.write(crow::json::wvalue({{"message", "修改密码失败，请检查旧密码是否正确"}}).dump());
                response.end();
                return;
            }

            response.code = 200;
            response.write(crow::json::wvalue({{"message", "密码修改成功"}}).dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "修改密码失败: " << e.what();
            response.code = 500;
            response.write(crow::json::wvalue({{"message", "服务器内部错误"}}).dump());
            response.end();
        }
    }

    void UserController::getApplications(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证请求并获取用户ID
            std::string token = request.get_header_value("Authorization").substr(7);
            std::string userId = Utils::SecurityUtils::getUserIdFromToken(token);

            if (userId.empty())
            {
                response.code = 401;
                response.write(crow::json::wvalue({{"message", "无效的授权令牌"}}).dump());
                response.end();
                return;
            }

            // 调用服务层获取申请记录
            // 假设 ApplicationService::getUserApplications 和 Model::Application 定义如下
            auto applications = Service::ApplicationService::getInstance().getUserApplications(std::stoll(userId));

            // 构建响应JSON数组
            crow::json::wvalue::list applicationList;
            for (const auto &app: applications)
            {
                crow::json::wvalue item;
                item["applicationId"] = app.ApplicationID;
                item["jobId"] = app.JobID;
                item["enterpriseId"] = app.EnterpriseID;
                item["resumeId"] = app.ResumeID;
                item["applicationTime"] = app.ApplicationTime;
                item["status"] = app.Status;
                item["feedback"] = app.Feedback;
                item["updateTime"] = app.UpdateTime;
                applicationList.push_back(item);
            }

            response.code = 200;
            response.write(crow::json::wvalue(applicationList).dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取申请记录失败: " << e.what();
            response.code = 500;
            response.write(crow::json::wvalue({{"message", "服务器内部错误"}}).dump());
            response.end();
        }
    }
}// namespace Router