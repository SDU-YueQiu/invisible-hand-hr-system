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

    void AdminController::updateUserStatus(const crow::request &request, crow::response &response, int id)
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
            if (!body || !body.has("status"))
            {
                response.code = 400;
                response.write("无效的请求格式");
                return;
            }

            // 调用服务层更新用户状态
            bool success = Service::AdminService::getInstance().updateUserStatus(
                    id,
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

    void AdminController::updateJobStatus(const crow::request &request, crow::response &response, int jobID)
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
                    jobID,
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

    void AdminController::getAnnouncements(const crow::request &request, crow::response &response)
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

            // 调用服务层获取公告列表
            auto announcements = Service::AnnouncementService::getInstance().getAnnouncements(filter);

            // 构建响应JSON数组
            crow::json::wvalue::list announcementList;
            for (const auto &announcement: announcements)
            {
                crow::json::wvalue item;
                item["announcementId"] = announcement.AnnouncementID;
                item["title"] = announcement.Title;
                item["publishTime"] = announcement.PublishTime;
                item["effectiveTime"] = announcement.EffectiveTime;
                item["expireTime"] = announcement.ExpireTime;
                item["isTop"] = announcement.IsTop;
                announcementList.push_back(item);
            }

            response.code = 200;
            response.write(crow::json::wvalue(announcementList).dump());
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取公告列表失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void AdminController::getAnnouncementDetail(const crow::request &request, crow::response &response)
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

            // 获取公告ID
            std::string announcementId = request.url_params.get("announcementId");
            if (announcementId.empty())
            {
                response.code = 400;
                response.write("缺少公告ID参数");
                return;
            }

            // 调用服务层获取公告详情
            auto announcements = Service::AnnouncementService::getInstance().getAnnouncements("AnnouncementID = " + announcementId);
            if (announcements.empty())
            {
                response.code = 404;
                response.write("公告不存在");
                return;
            }

            // 构建响应JSON
            crow::json::wvalue result;
            result["announcementId"] = announcements[0].AnnouncementID;
            result["adminId"] = announcements[0].AdminID;
            result["title"] = announcements[0].Title;
            result["content"] = announcements[0].Content;
            result["publishTime"] = announcements[0].PublishTime;
            result["effectiveTime"] = announcements[0].EffectiveTime;
            result["expireTime"] = announcements[0].ExpireTime;
            result["isTop"] = announcements[0].IsTop;

            response.code = 200;
            response.write(result.dump());
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取公告详情失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void AdminController::createAnnouncement(const crow::request &request, crow::response &response)
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
            if (!body || !body.has("title") || !body.has("content"))
            {
                response.code = 400;
                response.write("无效的请求格式");
                return;
            }

            // 构建公告对象
            Model::Announcement announcementData;
            announcementData.AdminID = std::stoi(Utils::SecurityUtils::getUserIdFromToken(token));
            announcementData.Title = body["title"].s();
            announcementData.Content = body["content"].s();
            announcementData.PublishTime = time(nullptr);

            if (body.has("effectiveTime"))
                announcementData.EffectiveTime = body["effectiveTime"].i();
            if (body.has("expireTime"))
                announcementData.ExpireTime = body["expireTime"].i();
            if (body.has("isTop"))
                announcementData.IsTop = body["isTop"].b();

            // 调用服务层创建公告
            bool success = Service::AnnouncementService::getInstance().createAnnouncement(announcementData);

            if (!success)
            {
                response.code = 400;
                response.write("创建公告失败");
                return;
            }

            response.code = 201;
            response.write("公告创建成功");
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "创建公告失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void AdminController::updateAnnouncement(const crow::request &request, crow::response &response)
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
            if (!body || !body.has("announcementId"))
            {
                response.code = 400;
                response.write("无效的请求格式");
                return;
            }

            // 构建公告更新对象
            Model::Announcement announcementData;
            announcementData.AnnouncementID = body["announcementId"].i();
            if (body.has("title")) announcementData.Title = body["title"].s();
            if (body.has("content")) announcementData.Content = body["content"].s();
            if (body.has("effectiveTime")) announcementData.EffectiveTime = body["effectiveTime"].i();
            if (body.has("expireTime")) announcementData.ExpireTime = body["expireTime"].i();
            if (body.has("isTop")) announcementData.IsTop = body["isTop"].b();

            // 调用服务层更新公告
            bool success = Service::AnnouncementService::getInstance().updateAnnouncement(
                    announcementData.AnnouncementID, announcementData);

            if (!success)
            {
                response.code = 400;
                response.write("更新公告失败");
                return;
            }

            response.code = 200;
            response.write("公告更新成功");
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "更新公告失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void AdminController::deleteAnnouncement(const crow::request &request, crow::response &response)
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

            // 获取公告ID
            std::string announcementId = request.url_params.get("announcementId");
            if (announcementId.empty())
            {
                response.code = 400;
                response.write("缺少公告ID参数");
                return;
            }

            // 调用服务层删除公告
            bool success = Service::AnnouncementService::getInstance().deleteAnnouncement(std::stoll(announcementId));

            if (!success)
            {
                response.code = 400;
                response.write("删除公告失败");
                return;
            }

            response.code = 200;
            response.write("公告删除成功");
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "删除公告失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }
}// namespace Router