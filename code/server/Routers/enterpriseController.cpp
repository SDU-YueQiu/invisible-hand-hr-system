/**
 * @file enterpriseController.cpp
 * @brief 企业用户控制器实现，处理企业用户相关操作
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.0
 */

#include "enterpriseController.h"
#include "../Service/applicationService.h"
#include "../Utils/securityUtils.h"
#include <crow/json.h>
#include <string>


namespace Router
{
    void EnterpriseController::getEnterpriseInfo(const crow::request &request, crow::response &response)
    {
        try
        {
            // 从JWT令牌中获取企业ID
            std::string token = request.get_header_value("Authorization");
            std::string enterpriseId = Utils::SecurityUtils::getUserIdFromToken(token);

            if (enterpriseId.empty())
            {
                response.code = 401;
                response.write("无效的授权令牌");
                return;
            }

            // 通过服务层获取企业信息
            auto enterprise = Service::EnterpriseUserService::getInstance().getEnterpriseById(std::stoll(enterpriseId));
            if (enterprise.EnterpriseID == -1)
            {
                response.code = 404;
                response.write("企业不存在");
                return;
            }

            // 构建响应JSON
            crow::json::wvalue result;
            result["enterpriseId"] = enterprise.EnterpriseID;
            result["enterpriseName"] = enterprise.EnterpriseName;
            result["creditCode"] = enterprise.CreditCode;
            result["description"] = enterprise.Description;
            result["industry"] = enterprise.Industry;
            result["scale"] = enterprise.Scale;
            result["address"] = enterprise.Address;
            result["contactPerson"] = enterprise.ContactPerson;
            result["contactPhone"] = enterprise.ContactPhone;
            result["contactEmail"] = enterprise.ContactEmail;
            result["logoURL"] = enterprise.LogoURL;
            result["accountStatus"] = enterprise.AccountStatus;

            response.code = 200;
            response.write(result.dump());
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取企业信息失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void EnterpriseController::updateEnterpriseInfo(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证请求并获取企业ID
            std::string token = request.get_header_value("Authorization");
            std::string enterpriseId = Utils::SecurityUtils::getUserIdFromToken(token);

            if (enterpriseId.empty())
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

            // 构建企业更新对象
            Model::EnterpriseUser enterpriseData;
            enterpriseData.EnterpriseID = std::stoll(enterpriseId);
            if (body.has("enterpriseName")) enterpriseData.EnterpriseName = body["enterpriseName"].s();
            if (body.has("creditCode")) enterpriseData.CreditCode = body["creditCode"].s();
            if (body.has("description")) enterpriseData.Description = body["description"].s();
            if (body.has("industry")) enterpriseData.Industry = body["industry"].s();
            if (body.has("scale")) enterpriseData.Scale = body["scale"].s();
            if (body.has("address")) enterpriseData.Address = body["address"].s();
            if (body.has("contactPerson")) enterpriseData.ContactPerson = body["contactPerson"].s();
            if (body.has("contactPhone")) enterpriseData.ContactPhone = body["contactPhone"].s();
            if (body.has("contactEmail")) enterpriseData.ContactEmail = body["contactEmail"].s();
            if (body.has("logoURL")) enterpriseData.LogoURL = body["logoURL"].s();

            // 调用服务层更新企业信息
            bool success = Service::EnterpriseUserService::getInstance().updateEnterpriseInfo(
                    std::stoll(enterpriseId), enterpriseData);

            if (!success)
            {
                response.code = 400;
                response.write("更新企业信息失败");
                return;
            }

            response.code = 200;
            response.write("企业信息更新成功");
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "更新企业信息失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void EnterpriseController::changePassword(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证请求并获取企业ID
            std::string token = request.get_header_value("Authorization");
            std::string enterpriseId = Utils::SecurityUtils::getUserIdFromToken(token);

            if (enterpriseId.empty())
            {
                response.code = 401;
                response.write("无效的授权令牌");
                return;
            }

            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body || !body.has("oldPassword") || !body.has("newPassword"))
            {
                response.code = 400;
                response.write("无效的请求格式");
                return;
            }

            // 调用服务层修改密码
            bool success = Service::EnterpriseUserService::getInstance().changePassword(
                    std::stoll(enterpriseId),
                    body["oldPassword"].s(),
                    body["newPassword"].s());

            if (!success)
            {
                response.code = 400;
                response.write("修改密码失败，请检查旧密码是否正确");
                return;
            }

            response.code = 200;
            response.write("密码修改成功");
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "修改密码失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void EnterpriseController::getPostedJobs(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证请求并获取企业ID
            std::string token = request.get_header_value("Authorization");
            std::string enterpriseId = Utils::SecurityUtils::getUserIdFromToken(token);

            if (enterpriseId.empty())
            {
                response.code = 401;
                response.write("无效的授权令牌");
                return;
            }

            // 调用服务层获取企业发布的职位
            auto jobs = Service::JobPostingService::getInstance().getJobsByEnterpriseId(std::stoll(enterpriseId));

            // 构建响应JSON数组
            crow::json::wvalue::list jobList;
            for (const auto &job: jobs)
            {
                crow::json::wvalue item;
                item["jobId"] = job.JobID;
                item["jobTitle"] = job.JobTitle;
                item["jobCategory"] = job.JobCategory;
                item["recruitmentCount"] = job.RecruitmentCount;
                item["workLocation"] = job.WorkLocation;
                item["minSalary"] = job.MinSalary;
                item["maxSalary"] = job.MaxSalary;
                item["publishDate"] = job.PublishDate;
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

    void EnterpriseController::postNewJob(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证请求并获取企业ID
            std::string token = request.get_header_value("Authorization");
            std::string enterpriseId = Utils::SecurityUtils::getUserIdFromToken(token);

            if (enterpriseId.empty())
            {
                response.code = 401;
                response.write("无效的授权令牌");
                return;
            }

            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body || !body.has("jobTitle") || !body.has("responsibilities") || !body.has("requirements"))
            {
                response.code = 400;
                response.write("无效的请求格式");
                return;
            }

            // 构建职位对象
            Model::JobPosting jobData;
            jobData.EnterpriseID = std::stoll(enterpriseId);
            jobData.JobTitle = body["jobTitle"].s();
            if (body.has("jobCategory")) jobData.JobCategory = body["jobCategory"].s();
            if (body.has("recruitmentCount")) jobData.RecruitmentCount = body["recruitmentCount"].i();
            if (body.has("workLocation")) jobData.WorkLocation = body["workLocation"].s();
            if (body.has("minSalary")) jobData.MinSalary = body["minSalary"].i();
            if (body.has("maxSalary")) jobData.MaxSalary = body["maxSalary"].i();
            jobData.Responsibilities = body["responsibilities"].s();
            jobData.Requirements = body["requirements"].s();
            if (body.has("experienceRequired")) jobData.ExperienceRequired = body["experienceRequired"].s();
            if (body.has("educationRequired")) jobData.EducationRequired = body["educationRequired"].s();
            if (body.has("benefits")) jobData.Benefits = body["benefits"].s();
            if (body.has("deadlineDate")) jobData.DeadlineDate = body["deadlineDate"].s();
            jobData.JobStatus = "Recruiting";

            // 调用服务层创建职位
            bool success = Service::JobPostingService::getInstance().createJob(
                    std::stoll(enterpriseId), jobData);

            if (!success)
            {
                response.code = 400;
                response.write("创建职位失败");
                return;
            }

            response.code = 200;
            response.write("职位创建成功");
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "创建职位失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void EnterpriseController::updateJob(const crow::request &request, crow::response &response, int jobID)
    {
        try
        {
            // 验证请求并获取企业ID
            std::string token = request.get_header_value("Authorization");
            std::string enterpriseId = Utils::SecurityUtils::getUserIdFromToken(token);

            if (enterpriseId.empty())
            {
                response.code = 401;
                response.write("无效的授权令牌");
                return;
            }

            // 获取路径参数中的职位ID
            std::string jobId = std::to_string(jobID);
            if (jobId.empty())
            {
                response.code = 400;
                response.write("缺少职位ID参数");
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

            // 构建职位更新对象
            Model::JobPosting jobData;
            jobData.EnterpriseID = std::stoll(enterpriseId);
            if (body.has("jobTitle")) jobData.JobTitle = body["jobTitle"].s();
            if (body.has("jobCategory")) jobData.JobCategory = body["jobCategory"].s();
            if (body.has("recruitmentCount")) jobData.RecruitmentCount = body["recruitmentCount"].i();
            if (body.has("workLocation")) jobData.WorkLocation = body["workLocation"].s();
            if (body.has("minSalary")) jobData.MinSalary = body["minSalary"].i();
            if (body.has("maxSalary")) jobData.MaxSalary = body["maxSalary"].i();
            if (body.has("responsibilities")) jobData.Responsibilities = body["responsibilities"].s();
            if (body.has("requirements")) jobData.Requirements = body["requirements"].s();
            if (body.has("experienceRequired")) jobData.ExperienceRequired = body["experienceRequired"].s();
            if (body.has("educationRequired")) jobData.EducationRequired = body["educationRequired"].s();
            if (body.has("benefits")) jobData.Benefits = body["benefits"].s();
            if (body.has("deadlineDate")) jobData.DeadlineDate = body["deadlineDate"].s();
            if (body.has("jobStatus")) jobData.JobStatus = body["jobStatus"].s();

            // 调用服务层更新职位
            bool success = Service::JobPostingService::getInstance().updateJob(
                    std::stoll(jobId), jobData);

            if (!success)
            {
                response.code = 400;
                response.write("更新职位失败");
                return;
            }

            response.code = 200;
            response.write("职位更新成功");
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "更新职位失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void EnterpriseController::deleteJob(const crow::request &request, crow::response &response, int jobID)
    {
        try
        {
            // 验证请求并获取企业ID
            std::string token = request.get_header_value("Authorization");
            std::string enterpriseId = Utils::SecurityUtils::getUserIdFromToken(token);

            if (enterpriseId.empty())
            {
                response.code = 401;
                response.write("无效的授权令牌");
                return;
            }

            // 获取路径参数中的职位ID
            std::string jobId = std::to_string(jobID);
            if (jobId.empty())
            {
                response.code = 400;
                response.write("缺少职位ID参数");
                return;
            }

            // 调用服务层删除职位
            bool success = Service::JobPostingService::getInstance().deleteJob(std::stoll(jobId));

            if (!success)
            {
                response.code = 400;
                response.write("删除职位失败");
                return;
            }

            response.code = 200;
            response.write("职位删除成功");
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "删除职位失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void EnterpriseController::getApplicants(const crow::request &request, crow::response &response, int jobID)
    {
        try
        {
            // 验证请求并获取企业ID
            std::string token = request.get_header_value("Authorization");
            std::string enterpriseId = Utils::SecurityUtils::getUserIdFromToken(token);

            if (enterpriseId.empty())
            {
                response.code = 401;
                response.write("无效的授权令牌");
                return;
            }

            // 获取路径参数中的职位ID
            std::string jobId = std::to_string(jobID);
            if (jobId.empty())
            {
                response.code = 400;
                response.write("缺少职位ID参数");
                return;
            }

            // 调用服务层获取申请者简历列表
            auto resumes = Service::TalentSearchService::getInstance().getApplicantsByJobId(std::stoll(jobId));

            // 构建响应JSON数组
            crow::json::wvalue::list resumeList;
            for (const auto &resume: resumes)
            {
                crow::json::wvalue item;
                item["resumeId"] = resume.ResumeID;
                item["userId"] = resume.UserID;
                item["resumeTitle"] = resume.ResumeTitle;
                item["lastUpdateTime"] = resume.LastUpdateTime;
                item["visibilityStatus"] = resume.VisibilityStatus;
                item["attachmentPath"] = resume.AttachmentPath;
                resumeList.push_back(item);
            }

            response.code = 200;
            response.write(crow::json::wvalue(resumeList).dump());
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取申请者列表失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void EnterpriseController::updateApplicationStatus(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证请求并获取企业ID
            std::string token = request.get_header_value("Authorization");
            std::string enterpriseId = Utils::SecurityUtils::getUserIdFromToken(token);

            if (enterpriseId.empty())
            {
                response.code = 401;
                response.write("无效的授权令牌");
                return;
            }

            // 获取路径参数中的申请ID
            std::string applicationId = request.url_params.get("applicationId");
            if (applicationId.empty())
            {
                response.code = 400;
                response.write("缺少申请ID参数");
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

            // 调用服务层更新申请状态
            bool success = Service::ApplicationService::getInstance().updateApplicationStatus(
                    std::stoll(applicationId),
                    body["status"].s());

            if (!success)
            {
                response.code = 400;
                response.write("更新申请状态失败");
                return;
            }

            response.code = 200;
            response.write("申请状态更新成功");
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "更新申请状态失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

    void EnterpriseController::getPostedJobDetail(const crow::request &request, crow::response &response, int jobID)
    {
        try
        {
            // 验证请求并获取企业ID
            std::string token = request.get_header_value("Authorization");
            std::string enterpriseId = Utils::SecurityUtils::getUserIdFromToken(token);

            if (enterpriseId.empty())
            {
                response.code = 401;
                response.write("无效的授权令牌");
                return;
            }

            // 调用服务层获取职位详情
            auto job = Service::JobPostingService::getInstance().getJobById(jobID);
            if (job.JobID == -1)
            {
                response.code = 404;
                response.write("职位不存在");
                return;
            }

            // 验证职位是否属于当前企业
            if (job.EnterpriseID != std::stoll(enterpriseId))
            {
                response.code = 403;
                response.write("无权访问此职位");
                return;
            }

            // 构建响应JSON
            crow::json::wvalue result;
            result["jobId"] = job.JobID;
            result["jobTitle"] = job.JobTitle;
            result["enterpriseId"] = job.EnterpriseID;
            result["jobCategory"] = job.JobCategory;
            result["recruitmentCount"] = job.RecruitmentCount;
            result["workLocation"] = job.WorkLocation;
            result["minSalary"] = job.MinSalary;
            result["maxSalary"] = job.MaxSalary;
            result["responsibilities"] = job.Responsibilities;
            result["requirements"] = job.Requirements;
            result["experienceRequired"] = job.ExperienceRequired;
            result["educationRequired"] = job.EducationRequired;
            result["benefits"] = job.Benefits;
            result["publishDate"] = job.PublishDate;
            result["deadlineDate"] = job.DeadlineDate;
            result["jobStatus"] = job.JobStatus;

            response.code = 200;
            response.write(result.dump());
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取职位详情失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }


    void EnterpriseController::searchResumes(const crow::request &request, crow::response &response)
    {
        try
        {
            // 验证请求并获取企业ID
            std::string token = request.get_header_value("Authorization");
            std::string enterpriseId = Utils::SecurityUtils::getUserIdFromToken(token);

            if (enterpriseId.empty())
            {
                response.code = 401;
                response.write("无效的授权令牌");
                return;
            }

            // 构建搜索条件
            Service::ResumeSearchCriteria criteria;

            // 从查询参数填充搜索条件
            if (request.url_params.get("keyword"))
                criteria.keyword = request.url_params.get("keyword");
            if (request.url_params.get("education"))
                criteria.education = request.url_params.get("education");
            if (request.url_params.get("experience"))
                criteria.experience = request.url_params.get("experience");
            if (request.url_params.get("location"))
                criteria.location = request.url_params.get("location");
            if (request.url_params.get("sortBy"))
                criteria.sortBy = request.url_params.get("sortBy");
            if (request.url_params.get("sortOrder"))
                criteria.sortOrder = request.url_params.get("sortOrder");
            if (request.url_params.get("returnSize"))
                criteria.returnSize = std::stoi(request.url_params.get("returnSize"));

            // 调用服务层搜索简历
            auto resumes = Service::TalentSearchService::getInstance().searchResumes(criteria);

            // 构建响应JSON数组
            crow::json::wvalue::list resumeList;
            for (const auto &resume: resumes)
            {
                crow::json::wvalue item;
                item["resumeId"] = resume.ResumeID;
                item["userId"] = resume.UserID;
                item["resumeTitle"] = resume.ResumeTitle;
                item["lastUpdateTime"] = resume.LastUpdateTime;
                item["visibilityStatus"] = resume.VisibilityStatus;
                item["attachmentPath"] = resume.AttachmentPath;
                resumeList.push_back(item);
            }

            response.code = 200;
            response.write(crow::json::wvalue(resumeList).dump());
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "搜索简历失败: " << e.what();
            response.code = 500;
            response.write("服务器内部错误");
        }
    }

}// namespace Router