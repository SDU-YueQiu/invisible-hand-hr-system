/**
 * @file authController.cpp
 * @brief 认证控制器实现，处理用户注册、登录等认证相关API请求
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.4
 */

#include "authController.h"
#include "../Utils/securityUtils.h"
#include <crow/json.h>

namespace Router
{
    void AuthController::registerIndividual(const crow::request &request, crow::response &response)
    {
        try
        {
            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body || !body.has("username") || !body.has("password"))
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["success"] = false;
                error_json["message"] = "无效的请求格式";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 构建用户注册对象
            Model::IndividualUser userData;
            if (body.has("username")) userData.Username = body["username"].s();
            if (body.has("password")) userData.PasswordHash = body["password"].s();
            if (body.has("phoneNumber")) userData.PhoneNumber = body["phoneNumber"].s();
            if (body.has("email")) userData.Email = body["email"].s();

            // 调用服务层注册用户
            auto result = Service::AuthService::getInstance().registerIndividual(userData);

            crow::json::wvalue jsonResponse;
            jsonResponse["success"] = result.success;
            jsonResponse["message"] = result.message;

            if (!result.success)
            {
                response.code = 409;
                response.write(jsonResponse.dump());
                response.end();
                return;
            }

            response.code = 201;
            response.write(jsonResponse.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "用户注册失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["success"] = false;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }

    void AuthController::loginIndividual(const crow::request &request, crow::response &response)
    {
        try
        {
            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body || !body.has("username") || !body.has("password"))
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["success"] = false;
                error_json["message"] = "无效的请求格式: 必须包含username和password";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 调用服务层登录
            auto result = Service::AuthService::getInstance().loginIndividual(
                    body["username"].s(),
                    body["password"].s());

            crow::json::wvalue jsonResponse;
            jsonResponse["success"] = result.success;
            jsonResponse["message"] = result.message;

            if (!result.success)
            {
                response.code = 401;
                response.write(jsonResponse.dump());
                response.end();
                return;
            }

            // 构建成功响应JSON
            jsonResponse["userId"] = result.userId;
            jsonResponse["token"] = result.token;

            response.code = 200;
            response.write(jsonResponse.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "用户登录失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["success"] = false;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }

    void AuthController::registerEnterprise(const crow::request &request, crow::response &response)
    {
        try
        {
            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body || !body.has("loginUsername") || !body.has("password"))
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["success"] = false;
                error_json["message"] = "无效的请求格式";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 构建企业注册对象
            Model::EnterpriseUser enterpriseData;
            if (body.has("loginUsername")) enterpriseData.LoginUsername = body["loginUsername"].s();
            if (body.has("password")) enterpriseData.PasswordHash = body["password"].s();// Hash in service
            if (body.has("contactPhone")) enterpriseData.ContactPhone = body["contactPhone"].s();
            if (body.has("contactEmail")) enterpriseData.ContactEmail = body["contactEmail"].s();
            if (body.has("enterpriseName")) enterpriseData.EnterpriseName = body["enterpriseName"].s();

            // 调用服务层注册企业
            auto result = Service::AuthService::getInstance().registerEnterprise(enterpriseData);

            crow::json::wvalue jsonResponse;
            jsonResponse["success"] = result.success;
            jsonResponse["message"] = result.message;

            if (!result.success)
            {
                response.code = 409;// Conflict or Bad Request
                response.write(jsonResponse.dump());
                response.end();
                return;
            }

            response.code = 201;
            response.write(jsonResponse.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "企业注册失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["success"] = false;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }

    void AuthController::loginEnterprise(const crow::request &request, crow::response &response)
    {
        try
        {
            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body || !body.has("username") || !body.has("password"))
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["success"] = false;
                error_json["message"] = "无效的请求格式: 必须包含username和password";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 调用服务层登录
            auto result = Service::AuthService::getInstance().loginEnterprise(
                    body["username"].s(),
                    body["password"].s());

            crow::json::wvalue jsonResponse;
            jsonResponse["success"] = result.success;
            jsonResponse["message"] = result.message;

            if (!result.success)
            {
                response.code = 401;// Unauthorized
                response.write(jsonResponse.dump());
                response.end();
                return;
            }

            // 构建成功响应JSON
            jsonResponse["enterpriseId"] = result.userId;
            jsonResponse["token"] = result.token;

            response.code = 200;
            response.write(jsonResponse.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "企业登录失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["success"] = false;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }

    void AuthController::loginAdmin(const crow::request &request, crow::response &response)
    {
        try
        {
            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body || !body.has("username") || !body.has("password"))
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["success"] = false;
                error_json["message"] = "无效的请求格式: 必须包含username和password";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 调用服务层登录
            auto result = Service::AuthService::getInstance().loginAdmin(
                    body["username"].s(),
                    body["password"].s());

            crow::json::wvalue jsonResponse;
            jsonResponse["success"] = result.success;
            jsonResponse["message"] = result.message;

            if (!result.success)
            {
                response.code = 401;// Unauthorized
                response.write(jsonResponse.dump());
                response.end();
                return;
            }

            // 构建成功响应JSON
            jsonResponse["adminId"] = result.userId;
            jsonResponse["token"] = result.token;

            response.code = 200;
            response.write(jsonResponse.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "管理员登录失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["success"] = false;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }

    void AuthController::logout(const crow::request &request, crow::response &response)
    {
        try
        {
            // // 从请求头获取token
            // std::string token = request.get_header_value("Authorization");
            // if (token.empty())
            // {
            //     response.code = 401;
            //     crow::json::wvalue error_json;
            //     error_json["success"] = false;
            //     error_json["message"] = "缺少授权令牌";
            //     response.write(error_json.dump());
            //     response.end();
            //     return;
            // }

            response.code = 200;
            crow::json::wvalue success_json;
            success_json["success"] = true;
            success_json["message"] = "退出登录成功";
            response.write(success_json.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "退出登录失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["success"] = false;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }
}// namespace Router