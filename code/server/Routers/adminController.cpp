/**
 * @file adminController.cpp
 * @brief 管理员控制器实现，处理系统管理员相关API请求
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.1
 */

#include "adminController.h"
#include "../Utils/securityUtils.h"
#include <crow/json.h>
#include <string>

namespace Router
{
    void AdminController::getUsers(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization").substr(7);
            if (Utils::SecurityUtils::getRoleFromToken(token) != "admin")
            {
                response.code = 403;
                crow::json::wvalue error_json;
                error_json["message"] = "无权访问此资源";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 获取筛选条件
            std::string filter = request.url_params.get("filter") ? request.url_params.get("filter") : "";

            // 调用服务层获取用户列表
            auto users = Service::AdminService::getInstance().getUsers(filter);

            // 构建响应JSON对象
            crow::json::wvalue result;
            result["success"] = true;
            result["message"] = "操作成功";

            // 构建用户数据数组
            crow::json::wvalue::list userList;
            for (const auto &user: users)
            {
                crow::json::wvalue item;
                item["userId"] = user.UserID;
                item["username"] = user.Username;
                item["phoneNumber"] = user.PhoneNumber;
                item["email"] = user.Email;
                item["registrationDate"] = user.RegistrationDate;
                item["lastLoginDate"] = user.LastLoginDate;
                item["accountStatus"] = user.AccountStatus;
                item["avatarURL"] = user.AvatarURL;
                userList.push_back(item);
            }

            // 组合最终响应
            result["data"]["users"] = crow::json::wvalue(userList);

            response.code = 200;
            response.write(result.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取用户列表失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }

    void AdminController::updateUserStatus(const crow::request &request, crow::response &response, int id)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization").substr(7);
            if (Utils::SecurityUtils::getRoleFromToken(token) != "admin")
            {
                response.code = 403;
                crow::json::wvalue error_json;
                error_json["message"] = "无权访问此资源";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body || !body.has("newStatus"))
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["message"] = "无效的请求格式";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 调用服务层更新用户状态
            bool success = Service::AdminService::getInstance().updateUserStatus(
                    id,
                    body["newStatus"].s());

            if (!success)
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["message"] = "更新用户状态失败";
                response.write(error_json.dump());
                response.end();
                return;
            }

            response.code = 200;
            crow::json::wvalue success_json;
            success_json["message"] = "用户状态更新成功";
            response.write(success_json.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "更新用户状态失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }

    void AdminController::getEnterprises(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization").substr(7);
            if (Utils::SecurityUtils::getRoleFromToken(token) != "admin")
            {
                response.code = 403;
                crow::json::wvalue error_json;
                error_json["message"] = "无权访问此资源";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 获取筛选条件
            std::string filter = request.url_params.get("filter") ? request.url_params.get("filter") : "";

            // 调用服务层获取企业列表
            auto enterprises = Service::AdminService::getInstance().getEnterprises(filter);

            // 构建响应JSON对象
            crow::json::wvalue result;
            result["success"] = true;
            result["message"] = "操作成功";

            // 构建企业数据数组
            crow::json::wvalue::list enterpriseList;
            for (const auto &enterprise: enterprises)
            {
                crow::json::wvalue item;
                item["enterpriseId"] = enterprise.EnterpriseID;
                item["loginUsername"] = enterprise.LoginUsername;
                item["enterpriseName"] = enterprise.EnterpriseName;
                item["creditCode"] = enterprise.CreditCode;
                item["description"] = enterprise.Description;
                item["industry"] = enterprise.Industry;
                item["scale"] = enterprise.Scale;
                item["address"] = enterprise.Address;
                item["contactPerson"] = enterprise.ContactPerson;
                item["contactPhone"] = enterprise.ContactPhone;
                item["contactEmail"] = enterprise.ContactEmail;
                item["logoURL"] = enterprise.LogoURL;
                item["licenseImageURL"] = enterprise.LicenseImageURL;
                item["registrationDate"] = enterprise.RegistrationDate;
                item["accountStatus"] = enterprise.AccountStatus;
                item["auditOpinion"] = enterprise.AuditOpinion;
                enterpriseList.push_back(item);
            }

            // 组合最终响应
            result["data"]["enterprises"] = crow::json::wvalue(enterpriseList);

            response.code = 200;
            response.write(result.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取企业列表失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }

    void AdminController::approveEnterprise(const crow::request &request, crow::response &response, int eid)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization").substr(7);
            if (Utils::SecurityUtils::getRoleFromToken(token) != "admin")
            {
                response.code = 403;
                crow::json::wvalue error_json;
                error_json["message"] = "无权访问此资源";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body)
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["message"] = "无效的请求格式";
                response.write(error_json.dump());
                response.end();
                return;
            }

            std::string opinion = body.has("auditOpinion") ? body["auditOpinion"].s() : std::string("");

            // 调用服务层批准企业
            bool success = Service::AdminService::getInstance().approveEnterprise(
                    eid,
                    opinion);

            if (!success)
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["message"] = "批准企业失败";
                response.write(error_json.dump());
                response.end();
                return;
            }

            response.code = 200;
            crow::json::wvalue success_json;
            success_json["message"] = "企业已批准";
            response.write(success_json.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "批准企业失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }

    void AdminController::rejectEnterprise(const crow::request &request, crow::response &response, int eid)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization").substr(7);
            if (Utils::SecurityUtils::getRoleFromToken(token) != "admin")
            {
                response.code = 403;
                crow::json::wvalue error_json;
                error_json["message"] = "无权访问此资源";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body)
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["message"] = "无效的请求格式";
                response.write(error_json.dump());
                response.end();
                return;
            }

            std::string opinion = body.has("opinion") ? body["opinion"].s() : std::string("");

            // 调用服务层拒绝企业
            bool success = Service::AdminService::getInstance().rejectEnterprise(
                    eid,
                    opinion);

            if (!success)
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["message"] = "拒绝企业失败";
                response.write(error_json.dump());
                response.end();
                return;
            }

            response.code = 200;
            crow::json::wvalue success_json;
            success_json["message"] = "企业已拒绝";
            response.write(success_json.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "拒绝企业失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }

    void AdminController::getJobs(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization").substr(7);
            if (Utils::SecurityUtils::getRoleFromToken(token) != "admin")
            {
                response.code = 403;
                crow::json::wvalue error_json;
                error_json["message"] = "无权访问此资源";
                response.write(error_json.dump());
                response.end();
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
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取职位列表失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }

    void AdminController::updateJobStatus(const crow::request &request, crow::response &response, int jobID)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization").substr(7);
            if (Utils::SecurityUtils::getRoleFromToken(token) != "admin")
            {
                response.code = 403;
                crow::json::wvalue error_json;
                error_json["message"] = "无权访问此资源";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body || !body.has("newStatus"))// Assuming jobId in body is for potential future use or validation against path param
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["message"] = "无效的请求格式";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 调用服务层更新职位状态
            bool success = Service::AdminService::getInstance().updateJobStatus(
                    jobID,// Use jobID from path parameter
                    body["newStatus"].s());

            if (!success)
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["message"] = "更新职位状态失败";
                response.write(error_json.dump());
                response.end();
                return;
            }

            response.code = 200;
            crow::json::wvalue success_json;
            success_json["message"] = "职位状态更新成功";
            response.write(success_json.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "更新职位状态失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }

    void AdminController::getAnnouncements(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization").substr(7);
            if (Utils::SecurityUtils::getRoleFromToken(token) != "admin")
            {
                response.code = 403;
                crow::json::wvalue error_json;
                error_json["message"] = "无权访问此资源";
                response.write(error_json.dump());
                response.end();
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
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取公告列表失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }

    void AdminController::getAnnouncementDetail(const crow::request &request, crow::response &response, int aid)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization").substr(7);
            if (Utils::SecurityUtils::getRoleFromToken(token) != "admin")
            {
                response.code = 403;
                crow::json::wvalue error_json;
                error_json["message"] = "无权访问此资源";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 获取公告ID
            std::string announcementId = std::to_string(aid);
            // No need to check if announcementId is empty as aid is int, std::to_string(int) is never empty.

            // 调用服务层获取公告详情
            auto announcements = Service::AnnouncementService::getInstance().getAnnouncements("AnnouncementID = " + announcementId);
            if (announcements.empty())
            {
                response.code = 404;
                crow::json::wvalue error_json;
                error_json["message"] = "公告不存在";
                response.write(error_json.dump());
                response.end();
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
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取公告详情失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }

    void AdminController::createAnnouncement(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization").substr(7);
            if (Utils::SecurityUtils::getRoleFromToken(token) != "admin")
            {
                response.code = 403;
                crow::json::wvalue error_json;
                error_json["message"] = "无权访问此资源";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body || !body.has("title") || !body.has("content"))
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["message"] = "无效的请求格式";
                response.write(error_json.dump());
                response.end();
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
                crow::json::wvalue error_json;
                error_json["message"] = "创建公告失败";
                response.write(error_json.dump());
                response.end();
                return;
            }

            response.code = 201;
            crow::json::wvalue success_json;
            success_json["message"] = "公告创建成功";
            response.write(success_json.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "创建公告失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }

    void AdminController::updateAnnouncement(const crow::request &request, crow::response &response, int aid)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization").substr(7);
            if (Utils::SecurityUtils::getRoleFromToken(token) != "admin")
            {
                response.code = 403;
                crow::json::wvalue error_json;
                error_json["message"] = "无权访问此资源";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body)
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["message"] = "无效的请求格式";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 构建公告更新对象
            Model::Announcement announcementData;
            announcementData.AnnouncementID = aid;
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
                crow::json::wvalue error_json;
                error_json["message"] = "更新公告失败";
                response.write(error_json.dump());
                response.end();
                return;
            }

            response.code = 200;
            crow::json::wvalue success_json;
            success_json["message"] = "公告更新成功";
            response.write(success_json.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "更新公告失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }

    void AdminController::deleteAnnouncement(const crow::request &request, crow::response &response, int aid)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization").substr(7);
            if (Utils::SecurityUtils::getRoleFromToken(token) != "admin")
            {
                response.code = 403;
                crow::json::wvalue error_json;
                error_json["message"] = "无权访问此资源";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 获取公告ID
            std::string announcementId = std::to_string(aid);
            // No need to check if announcementId is empty as aid is int, std::to_string(int) is never empty.
            // The original check `if (announcementId.empty())` was redundant here.

            // 调用服务层删除公告
            bool success = Service::AnnouncementService::getInstance().deleteAnnouncement(std::stoll(announcementId));

            if (!success)
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["message"] = "删除公告失败";
                response.write(error_json.dump());
                response.end();
                return;
            }

            response.code = 200;
            crow::json::wvalue success_json;
            success_json["message"] = "公告删除成功";
            response.write(success_json.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "删除公告失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }

    void AdminController::getFeedbacks(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization").substr(7);
            if (Utils::SecurityUtils::getRoleFromToken(token) != "admin")
            {
                response.code = 403;
                crow::json::wvalue error_json;
                error_json["message"] = "无权访问此资源";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 获取筛选条件
            std::string filter = request.url_params.get("filter") ? request.url_params.get("filter") : "";

            // 调用服务层获取反馈列表
            auto feedbacks = Service::FeedbackService::getInstance().getFeedbacks(filter);

            // 构建响应JSON数组
            crow::json::wvalue::list feedbackList;
            for (const auto &feedback: feedbacks)
            {
                crow::json::wvalue item;
                item["feedbackId"] = feedback.FeedbackID;
                item["userId"] = feedback.UserID;
                item["userType"] = feedback.UserType;
                item["feedbackType"] = feedback.FeedbackType;
                item["content"] = feedback.Content;
                item["contactInfo"] = feedback.ContactInfo;
                item["status"] = feedback.Status;
                item["adminReply"] = feedback.AdminReply;
                item["createTime"] = feedback.CreateTime;
                item["updateTime"] = feedback.UpdateTime;
                feedbackList.push_back(item);
            }

            response.code = 200;
            response.write(crow::json::wvalue(feedbackList).dump());
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

    void AdminController::processFeedback(const crow::request &request, crow::response &response, int fid)
    {
        try
        {
            // 验证管理员权限
            std::string token = request.get_header_value("Authorization").substr(7);
            if (Utils::SecurityUtils::getRoleFromToken(token) != "admin")
            {
                response.code = 403;
                crow::json::wvalue error_json;
                error_json["message"] = "无权访问此资源";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body || !body.has("status"))// Assuming 'status' field implies processing, adminReply is optional
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["message"] = "无效的请求格式";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 获取管理员回复
            std::string adminReply = body.has("adminReply") ? body["adminReply"].s() : std::string("");

            // 调用服务层处理反馈
            // Assuming processFeedback updates the status and optionally the adminReply.
            // The original Service::FeedbackService::getInstance().processFeedback only took fid and adminReply.
            // If status update is also part of processing, the service method might need adjustment,
            // or 'status' from body is used to determine the nature of processing.
            // Sticking to the original service call signature for now.
            bool success = Service::FeedbackService::getInstance().processFeedback(
                    fid,
                    adminReply);

            if (!success)
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["message"] = "处理反馈失败";
                response.write(error_json.dump());
                response.end();
                return;
            }

            response.code = 200;
            crow::json::wvalue success_json;
            success_json["message"] = "反馈处理成功";
            response.write(success_json.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "处理反馈失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }

}// namespace Router