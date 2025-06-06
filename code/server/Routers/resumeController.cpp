/**
 * @file resumeController.cpp
 * @brief 简历管理控制器实现，处理简历相关的CRUD操作
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.1
 */

#include "resumeController.h"
#include "../Utils/securityUtils.h"
#include <crow/json.h>
#include <string>

namespace Router
{
    void ResumeController::createResume(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证请求并获取用户ID
            std::string token = request.get_header_value("Authorization").substr(7);
            std::string userId = Utils::SecurityUtils::getUserIdFromToken(token);

            if (userId.empty())
            {
                response.code = 401;
                response.write(crow::json::wvalue{{"message", "无效的授权令牌"}}.dump());
                response.end();
                return;
            }

            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body)
            {
                response.code = 400;
                response.write(crow::json::wvalue{{"message", "无效的请求格式"}}.dump());
                response.end();
                return;
            }

            // 构建简历对象
            Model::Resume resumeData;
            resumeData.UserID = std::stoi(userId);
            if (body.has("resumeTitle")) resumeData.ResumeTitle = body["resumeTitle"].s();
            if (body.has("basicInfo"))
            {
                crow::json::wvalue basicInfo = body["basicInfo"];
                resumeData.BasicInfo = basicInfo.dump();
            }
            if (body.has("jobIntent"))
            {
                crow::json::wvalue jobIntent = body["jobIntent"];
                resumeData.JobIntent = jobIntent.dump();
            }
            if (body.has("educationExperience"))
            {
                crow::json::wvalue educationExperience = body["educationExperience"];
                resumeData.EducationExperience = educationExperience.dump();
            }
            if (body.has("workExperience"))
            {
                crow::json::wvalue workExperience = body["workExperience"];
                resumeData.WorkExperience = workExperience.dump();
            }
            if (body.has("projectExperience"))
            {
                crow::json::wvalue projectExperience = body["projectExperience"];
                resumeData.ProjectExperience = projectExperience.dump();
            }
            if (body.has("skillsCertificates"))
            {
                crow::json::wvalue skillsCertificates = body["skillsCertificates"];
                resumeData.SkillsCertificates = skillsCertificates.dump();
            }
            if (body.has("selfDescription")) resumeData.SelfDescription = body["selfDescription"].s();
            if (body.has("visibilityStatus")) resumeData.VisibilityStatus = body["visibilityStatus"].s();
            if (body.has("attachmentPath")) resumeData.AttachmentPath = body["attachmentPath"].s();

            // 调用服务层创建简历
            bool success = Service::ResumeService::getInstance().createResume(
                    std::stoi(userId), resumeData);

            if (!success)
            {
                response.code = 400;
                response.write(crow::json::wvalue{{"message", "创建简历失败"}}.dump());
                response.end();
                return;
            }

            response.code = 201;
            response.write(crow::json::wvalue{{"message", "简历创建成功"}}.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "创建简历失败: " << e.what();
            response.code = 500;
            response.write(crow::json::wvalue{{"message", "服务器内部错误"}}.dump());
            response.end();
        }
    }

    void ResumeController::getResume(const crow::request &request, crow::response &response, int id)
    {
        try
        {
            // 验证请求并获取用户ID
            std::string token = request.get_header_value("Authorization").substr(7);
            std::string userId = Utils::SecurityUtils::getUserIdFromToken(token);

            if (userId.empty())
            {
                response.code = 401;
                response.write(crow::json::wvalue{{"message", "无效的授权令牌"}}.dump());
                response.end();
                return;
            }

            // 获取路径参数中的简历ID
            std::string resumeId = std::to_string(id);
            if (resumeId.empty())// Note: std::to_string(int) will not result in an empty string.
            {
                response.code = 400;
                response.write(crow::json::wvalue{{"message", "缺少简历ID参数"}}.dump());
                response.end();
                return;
            }

            // 调用服务层获取简历
            auto resume = Service::ResumeService::getInstance().getResumeById(std::stoi(resumeId));
            if (resume.ResumeID == -1)
            {
                response.code = 404;
                response.write(crow::json::wvalue{{"message", "简历不存在"}}.dump());
                response.end();
                return;
            }

            // // 验证简历归属
            // if (resume.UserID != std::stoi(userId))
            // {
            //     response.code = 403;
            //     response.write(crow::json::wvalue{{"message", "无权访问该简历"}}.dump());
            //     response.end();
            //     return;
            // }

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
            response.write(result.dump());// This already writes JSON, not a simple string message
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取简历失败: " << e.what();
            response.code = 500;
            response.write(crow::json::wvalue{{"message", "服务器内部错误"}}.dump());
            response.end();
        }
    }

    void ResumeController::getResumes(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证请求并获取用户ID
            std::string token = request.get_header_value("Authorization").substr(7);
            std::string userId = Utils::SecurityUtils::getUserIdFromToken(token);

            if (userId.empty())
            {
                response.code = 401;
                response.write(crow::json::wvalue{{"message", "无效的授权令牌"}}.dump());
                response.end();
                return;
            }

            // 调用服务层获取用户所有简历
            auto resumes = Service::ResumeService::getInstance().getResumesByUserId(std::stoi(userId));

            crow::json::wvalue result;
            result["success"] = true;
            result["message"] = "操作成功";

            crow::json::wvalue::list dataList;
            for (const auto &resume: resumes)
            {
                crow::json::wvalue item;
                item["resumeId"] = resume.ResumeID;
                item["resumeTitle"] = resume.ResumeTitle;
                item["lastUpdateTime"] = resume.LastUpdateTime;
                item["visibilityStatus"] = resume.VisibilityStatus;
                dataList.push_back(item);
            }
            result["data"] = std::move(dataList);

            response.code = 200;
            response.write(result.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取简历列表失败: " << e.what();
            response.code = 500;
            response.write(crow::json::wvalue{{"message", "服务器内部错误"}}.dump());
            response.end();
        }
    }

    void ResumeController::updateResume(const crow::request &request, crow::response &response, int id)
    {
        try
        {
            // 验证请求并获取用户ID
            std::string token = request.get_header_value("Authorization").substr(7);
            std::string userId = Utils::SecurityUtils::getUserIdFromToken(token);

            if (userId.empty())
            {
                response.code = 401;
                response.write(crow::json::wvalue{{"message", "无效的授权令牌"}}.dump());
                response.end();
                return;
            }

            // 获取路径参数中的简历ID
            std::string resumeId = std::to_string(id);
            if (resumeId.empty())
            {
                response.code = 400;
                response.write(crow::json::wvalue{{"message", "缺少简历ID参数"}}.dump());
                response.end();
                return;
            }

            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body)
            {
                response.code = 400;
                response.write(crow::json::wvalue{{"message", "无效的请求格式"}}.dump());
                response.end();
                return;
            }

            // 构建简历更新对象
            Model::Resume resumeData;
            resumeData.UserID = std::stoi(userId);
            if (body.has("resumeTitle")) resumeData.ResumeTitle = body["resumeTitle"].s();
            if (body.has("basicInfo"))
            {
                crow::json::wvalue basicInfo = body["basicInfo"];
                resumeData.BasicInfo = basicInfo.dump();
            }
            if (body.has("jobIntent"))
            {
                crow::json::wvalue jobIntent = body["jobIntent"];
                resumeData.JobIntent = jobIntent.dump();
            }
            if (body.has("educationExperience"))
            {
                crow::json::wvalue educationExperience = body["educationExperience"];
                resumeData.EducationExperience = educationExperience.dump();
            }
            if (body.has("workExperience"))
            {
                crow::json::wvalue workExperience = body["workExperience"];
                resumeData.WorkExperience = workExperience.dump();
            }
            if (body.has("projectExperience"))
            {
                crow::json::wvalue projectExperience = body["projectExperience"];
                resumeData.ProjectExperience = projectExperience.dump();
            }
            if (body.has("skillsCertificates"))
            {
                crow::json::wvalue skillsCertificates = body["skillsCertificates"];
                resumeData.SkillsCertificates = skillsCertificates.dump();
            }
            if (body.has("selfDescription")) resumeData.SelfDescription = body["selfDescription"].s();
            if (body.has("visibilityStatus")) resumeData.VisibilityStatus = body["visibilityStatus"].s();
            if (body.has("attachmentPath")) resumeData.AttachmentPath = body["attachmentPath"].s();

            // 调用服务层更新简历
            bool success = Service::ResumeService::getInstance().updateResume(
                    std::stoi(resumeId), resumeData);

            if (!success)
            {
                response.code = 400;
                response.write(crow::json::wvalue{{"message", "更新简历失败"}}.dump());
                response.end();
                return;
            }

            response.code = 201;
            response.write(crow::json::wvalue{{"message", "简历更新成功"}}.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "更新简历失败: " << e.what();
            response.code = 500;
            response.write(crow::json::wvalue{{"message", "服务器内部错误"}}.dump());
            response.end();
        }
    }

    void ResumeController::deleteResume(const crow::request &request, crow::response &response, int rid)
    {
        try
        {
            // 验证请求并获取用户ID
            std::string token = request.get_header_value("Authorization").substr(7);
            std::string userId = Utils::SecurityUtils::getUserIdFromToken(token);

            if (userId.empty())
            {
                response.code = 401;
                response.write(crow::json::wvalue{{"message", "无效的授权令牌"}}.dump());
                response.end();
                return;
            }

            // 获取路径参数中的简历ID
            std::string resumeId = std::to_string(rid);
            if (resumeId.empty())// Note: std::to_string(int) will not result in an empty string.
            {
                response.code = 400;
                response.write(crow::json::wvalue{{"message", "缺少简历ID参数"}}.dump());
                response.end();
                return;
            }

            // 调用服务层删除简历
            // Assuming service layer handles ownership check (user can only delete their own resume)
            bool success = Service::ResumeService::getInstance().deleteResume(std::stoi(resumeId));

            if (!success)
            {
                response.code = 400;// Or 404 if not found, or 403 if not authorized - depends on service layer.
                response.write(crow::json::wvalue{{"message", "删除简历失败"}}.dump());
                response.end();
                return;
            }

            response.code = 200;
            response.write(crow::json::wvalue{{"message", "简历删除成功"}}.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "删除简历失败: " << e.what();
            response.code = 500;
            response.write(crow::json::wvalue{{"message", "服务器内部错误"}}.dump());
            response.end();
        }
    }
}// namespace Router