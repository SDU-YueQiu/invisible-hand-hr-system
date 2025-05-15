/**
 * @file resumeController.cpp
 * @brief 简历管理控制器实现，处理简历相关的CRUD操作
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.0
 */

#include "resumeController.h"
#include "../Utils/securityUtils.h"
#include <crow/json.h>

namespace Router
{
    void ResumeController::createResume(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证请求并获取用户ID
            std::string token = request.get_header_value("Authorization");
            std::string userId = Utils::SecurityUtils::getUserIdFromToken(token);
            
            if (userId.empty())
            {
                response.code = 401;
                response.write("无效的授权令牌");
                return;
            }

            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body)
            {
                response.code = 400;
                response.write("无效的请求格式");
                return;
            }

            // 构建简历对象
            Model::Resume resumeData;
            resumeData.UserID = std::stoi(userId);
            if (body.has("resumeTitle")) resumeData.ResumeTitle = body["resumeTitle"].s();
            if (body.has("basicInfo")) resumeData.BasicInfo = body["basicInfo"].s();
            if (body.has("jobIntent")) resumeData.JobIntent = body["jobIntent"].s();
            if (body.has("educationExperience")) resumeData.EducationExperience = body["educationExperience"].s();
            if (body.has("workExperience")) resumeData.WorkExperience = body["workExperience"].s();
            if (body.has("projectExperience")) resumeData.ProjectExperience = body["projectExperience"].s();
            if (body.has("skillsCertificates")) resumeData.SkillsCertificates = body["skillsCertificates"].s();
            if (body.has("selfDescription")) resumeData.SelfDescription = body["selfDescription"].s();
            if (body.has("visibilityStatus")) resumeData.VisibilityStatus = body["visibilityStatus"].s();
            if (body.has("attachmentPath")) resumeData.AttachmentPath = body["attachmentPath"].s();

            // 调用服务层创建简历
            bool success = Service::ResumeService::getInstance().createResume(
                std::stoi(userId), resumeData);

            if (!success)
            {
                response.code = 400;
                response.write("创建简历失败");
                return;
            }

            response.code = 200;
            response.write("简历创建成功");
        }
        catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "创建简历失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void ResumeController::getResume(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证请求并获取用户ID
            std::string token = request.get_header_value("Authorization");
            std::string userId = Utils::SecurityUtils::getUserIdFromToken(token);
            
            if (userId.empty())
            {
                response.code = 401;
                response.write("无效的授权令牌");
                return;
            }

            // 获取路径参数中的简历ID
            std::string resumeId = request.url_params.get("resumeId");
            if (resumeId.empty())
            {
                response.code = 400;
                response.write("缺少简历ID参数");
                return;
            }

            // 调用服务层获取简历
            auto resume = Service::ResumeService::getInstance().getResumeById(std::stoi(resumeId));
            if (resume.ResumeID == -1)
            {
                response.code = 404;
                response.write("简历不存在");
                return;
            }

            // 验证简历归属
            if (resume.UserID != std::stoi(userId))
            {
                response.code = 403;
                response.write("无权访问该简历");
                return;
            }

            // 构建响应JSON
            crow::json::wvalue result;
            result["resumeId"] = resume.ResumeID;
            result["userId"] = resume.UserID;
            result["resumeTitle"] = resume.ResumeTitle;
            result["basicInfo"] = resume.BasicInfo;
            result["jobIntent"] = resume.JobIntent;
            result["educationExperience"] = resume.EducationExperience;
            result["workExperience"] = resume.WorkExperience;
            result["projectExperience"] = resume.ProjectExperience;
            result["skillsCertificates"] = resume.SkillsCertificates;
            result["selfDescription"] = resume.SelfDescription;
            result["lastUpdateTime"] = resume.LastUpdateTime;
            result["visibilityStatus"] = resume.VisibilityStatus;
            result["attachmentPath"] = resume.AttachmentPath;

            response.code = 200;
            response.write(result.dump());
        }
        catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取简历失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void ResumeController::getResumes(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证请求并获取用户ID
            std::string token = request.get_header_value("Authorization");
            std::string userId = Utils::SecurityUtils::getUserIdFromToken(token);
            
            if (userId.empty())
            {
                response.code = 401;
                response.write("无效的授权令牌");
                return;
            }

            // 调用服务层获取用户所有简历
            auto resumes = Service::ResumeService::getInstance().getResumesByUserId(std::stoi(userId));

            // 构建响应JSON数组
            crow::json::wvalue::list resumeList;
            for (const auto &resume : resumes)
            {
                crow::json::wvalue item;
                item["resumeId"] = resume.ResumeID;
                item["resumeTitle"] = resume.ResumeTitle;
                item["lastUpdateTime"] = resume.LastUpdateTime;
                item["visibilityStatus"] = resume.VisibilityStatus;
                resumeList.push_back(item);
            }

            response.code = 200;
            response.write(crow::json::wvalue(resumeList).dump());
        }
        catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取简历列表失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void ResumeController::updateResume(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证请求并获取用户ID
            std::string token = request.get_header_value("Authorization");
            std::string userId = Utils::SecurityUtils::getUserIdFromToken(token);
            
            if (userId.empty())
            {
                response.code = 401;
                response.write("无效的授权令牌");
                return;
            }

            // 获取路径参数中的简历ID
            std::string resumeId = request.url_params.get("resumeId");
            if (resumeId.empty())
            {
                response.code = 400;
                response.write("缺少简历ID参数");
                return;
            }

            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body)
            {
                response.code = 400;
                response.write("无效的请求格式");
                return;
            }

            // 构建简历更新对象
            Model::Resume resumeData;
            resumeData.UserID = std::stoi(userId);
            if (body.has("resumeTitle")) resumeData.ResumeTitle = body["resumeTitle"].s();
            if (body.has("basicInfo")) resumeData.BasicInfo = body["basicInfo"].s();
            if (body.has("jobIntent")) resumeData.JobIntent = body["jobIntent"].s();
            if (body.has("educationExperience")) resumeData.EducationExperience = body["educationExperience"].s();
            if (body.has("workExperience")) resumeData.WorkExperience = body["workExperience"].s();
            if (body.has("projectExperience")) resumeData.ProjectExperience = body["projectExperience"].s();
            if (body.has("skillsCertificates")) resumeData.SkillsCertificates = body["skillsCertificates"].s();
            if (body.has("selfDescription")) resumeData.SelfDescription = body["selfDescription"].s();
            if (body.has("visibilityStatus")) resumeData.VisibilityStatus = body["visibilityStatus"].s();
            if (body.has("attachmentPath")) resumeData.AttachmentPath = body["attachmentPath"].s();

            // 调用服务层更新简历
            bool success = Service::ResumeService::getInstance().updateResume(
                std::stoi(resumeId), resumeData);

            if (!success)
            {
                response.code = 400;
                response.write("更新简历失败");
                return;
            }

            response.code = 200;
            response.write("简历更新成功");
        }
        catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "更新简历失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void ResumeController::deleteResume(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证请求并获取用户ID
            std::string token = request.get_header_value("Authorization");
            std::string userId = Utils::SecurityUtils::getUserIdFromToken(token);
            
            if (userId.empty())
            {
                response.code = 401;
                response.write("无效的授权令牌");
                return;
            }

            // 获取路径参数中的简历ID
            std::string resumeId = request.url_params.get("resumeId");
            if (resumeId.empty())
            {
                response.code = 400;
                response.write("缺少简历ID参数");
                return;
            }

            // 调用服务层删除简历
            bool success = Service::ResumeService::getInstance().deleteResume(std::stoi(resumeId));

            if (!success)
            {
                response.code = 400;
                response.write("删除简历失败");
                return;
            }

            response.code = 200;
            response.write("简历删除成功");
        }
        catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "删除简历失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }
} // namespace Router