/**
 * @file adminController.cpp
 * @brief 管理员控制器实现，处理系统管理员相关API请求
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.0
 */

#include "adminController.h"
#include "../Utils/securityUtils.h"
#include <crow/json.h>

namespace Router
{
    void AdminController::getUsers(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization");
            if (Utils::SecurityUtils::getRoleFromToken(token) != "Admin")
            {
                response.code = 403;
                response.write("无权访问此资源");
                return;
            }

            // 获取筛选条件
            std::string filter = request.url_params.get("filter") ? request.url_params.get("filter") : "";

            // 调用服务层获取用户列表
            auto users = Service::AdminService::getInstance().getUsers(filter);

            // 构建响应JSON数组
            crow::json::wvalue::list userList;
            for (const auto &user: users)
            {
                crow::json::wvalue item;
                item["userId"] = user.UserID;
                item["username"] = user.Username;
                item["phoneNumber"] = user.PhoneNumber;
                item["email"] = user.Email;
                item["accountStatus"] = user.AccountStatus;
                userList.push_back(item);
            }

            response.code = 200;
            response.write(crow::json::wvalue(userList).dump());
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取用户列表失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void AdminController::updateUserStatus(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization");
            if (Utils::SecurityUtils::getRoleFromToken(token) != "Admin")
            {
                response.code = 403;
                response.write("无权访问此资源");
                return;
            }

            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body || !body.has("userId") || !body.has("status"))
            {
                response.code = 400;
                response.write("无效的请求格式");
                return;
            }

            // 调用服务层更新用户状态
            bool success = Service::AdminService::getInstance().updateUserStatus(
                    body["userId"].i(),
                    body["status"].s());

            if (!success)
            {
                response.code = 400;
                response.write("更新用户状态失败");
                return;
            }

            response.code = 200;
            response.write("用户状态更新成功");
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "更新用户状态失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void AdminController::getEnterprises(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization");
            if (Utils::SecurityUtils::getRoleFromToken(token) != "Admin")
            {
                response.code = 403;
                response.write("无权访问此资源");
                return;
            }

            // 获取筛选条件
            std::string filter = request.url_params.get("filter") ? request.url_params.get("filter") : "";

            // 调用服务层获取企业列表
            auto enterprises = Service::AdminService::getInstance().getEnterprises(filter);

            // 构建响应JSON数组
            crow::json::wvalue::list enterpriseList;
            for (const auto &enterprise: enterprises)
            {
                crow::json::wvalue item;
                item["enterpriseId"] = enterprise.EnterpriseID;
                item["enterpriseName"] = enterprise.EnterpriseName;
                item["contactPhone"] = enterprise.ContactPhone;
                item["contactEmail"] = enterprise.ContactEmail;
                item["accountStatus"] = enterprise.AccountStatus;
                enterpriseList.push_back(item);
            }

            response.code = 200;
            response.write(crow::json::wvalue(enterpriseList).dump());
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取企业列表失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void AdminController::approveEnterprise(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization");
            if (Utils::SecurityUtils::getRoleFromToken(token) != "Admin")
            {
                response.code = 403;
                response.write("无权访问此资源");
                return;
            }

            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body || !body.has("enterpriseId"))
            {
                response.code = 400;
                response.write("无效的请求格式");
                return;
            }

            std::string opinion = body.has("opinion") ? body["opinion"].s() : std::string("");

            // 调用服务层批准企业
            bool success = Service::AdminService::getInstance().approveEnterprise(
                    body["enterpriseId"].i(),
                    opinion);

            if (!success)
            {
                response.code = 400;
                response.write("批准企业失败");
                return;
            }

            response.code = 200;
            response.write("企业已批准");
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "批准企业失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void AdminController::rejectEnterprise(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization");
            if (Utils::SecurityUtils::getRoleFromToken(token) != "Admin")
            {
                response.code = 403;
                response.write("无权访问此资源");
                return;
            }

            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body || !body.has("enterpriseId"))
            {
                response.code = 400;
                response.write("无效的请求格式");
                return;
            }

            std::string opinion = body.has("opinion") ? body["opinion"].s() : std::string("");

            // 调用服务层拒绝企业
            bool success = Service::AdminService::getInstance().rejectEnterprise(
                    body["enterpriseId"].i(),
                    opinion);

            if (!success)
            {
                response.code = 400;
                response.write("拒绝企业失败");
                return;
            }

            response.code = 200;
            response.write("企业已拒绝");
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "拒绝企业失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void AdminController::getJobs(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization");
            if (Utils::SecurityUtils::getRoleFromToken(token) != "Admin")
            {
                response.code = 403;
                response.write("无权访问此资源");
                return;
            }

            // 获取筛选条件
            std::string filter = request.url_params.get("filter") ? request.url_params.get("filter") : "";

            // 调用服务层获取职位列表
            auto jobs = Service::AdminService::getInstance().getAllJobs(filter);

            // 构建响应JSON数组
            crow::json::wvalue::list jobList;
            for (const auto &job: jobs)
            {
                crow::json::wvalue item;
                item["jobId"] = job.JobID;
                item["jobTitle"] = job.JobTitle;
                item["enterpriseId"] = job.EnterpriseID;
                item["jobCategory"] = job.JobCategory;
                item["jobStatus"] = job.JobStatus;
                jobList.push_back(item);
            }

            response.code = 200;
            response.write(crow::json::wvalue(jobList).dump());
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取职位列表失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void AdminController::updateJobStatus(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization");
            if (Utils::SecurityUtils::getRoleFromToken(token) != "Admin")
            {
                response.code = 403;
                response.write("无权访问此资源");
                return;
            }

            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body || !body.has("jobId") || !body.has("status"))
            {
                response.code = 400;
                response.write("无效的请求格式");
                return;
            }

            // 调用服务层更新职位状态
            bool success = Service::AdminService::getInstance().updateJobStatus(
                    body["jobId"].i(),
                    body["status"].s());

            if (!success)
            {
                response.code = 400;
                response.write("更新职位状态失败");
                return;
            }

            response.code = 200;
            response.write("职位状态更新成功");
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "更新职位状态失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }
}// namespace Router