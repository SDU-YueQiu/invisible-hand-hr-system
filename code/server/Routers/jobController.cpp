/**
 * @file jobController.cpp
 * @brief 职位信息控制器实现，处理职位查询和申请相关API请求
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.1
 */

#include "jobController.h"
#include "../Service/applicationService.h"
#include "../Service/jobSearchService.h"
#include "../Utils/securityUtils.h"
#include <crow/json.h>
#include <string>


namespace Router
{
    void JobController::getJobs(const crow::request &request, crow::response &response)
    {
        try
        {
            // 从查询参数构建搜索条件
            Service::JobSearchCriteria criteria;

            if (request.url_params.get("keyword"))
                criteria.keyword = request.url_params.get("keyword");
            if (request.url_params.get("category"))
                criteria.jobCategory = request.url_params.get("category");
            if (request.url_params.get("location"))
                criteria.workLocation = request.url_params.get("location");
            if (request.url_params.get("salaryMin") && !std::string_view(request.url_params.get("salaryMin")).empty())
                criteria.minSalary = std::stoi(request.url_params.get("salaryMin"));
            // if (request.url_params.get("experienceRequired"))
            //     criteria.experienceRequired = request.url_params.get("experienceRequired");
            if (request.url_params.get("education"))
                criteria.educationRequired = request.url_params.get("education");
            if (request.url_params.get("sortBy"))
                criteria.sortBy = request.url_params.get("sortBy");
            if (request.url_params.get("sortOrder"))
                criteria.sortOrder = request.url_params.get("sortOrder");
            // if (request.url_params.get("jobStatus"))
            //     criteria.jobStatus = request.url_params.get("jobStatus");
            // if (request.url_params.get("returnSize"))
            // criteria.returnSize = std::stoi(request.url_params.get("returnSize"));

            // 调用服务层搜索职位
            auto jobs = Service::JobSearchService::getInstance().searchJobs(criteria);

            // 构建响应JSON数组
            crow::json::wvalue::list jobList;
            for (const auto &job: jobs)
            {
                crow::json::wvalue item;
                item["jobId"] = job.JobID;
                item["jobTitle"] = job.JobTitle;
                item["enterpriseId"] = job.EnterpriseID;
                item["jobCategory"] = job.JobCategory;
                item["workLocation"] = job.WorkLocation;
                item["minSalary"] = job.MinSalary;
                item["maxSalary"] = job.MaxSalary;
                item["publishDate"] = job.PublishDate;
                item["jobStatus"] = job.JobStatus;
                jobList.push_back(item);
            }

            crow::json::wvalue response_json;
            response_json["message"] = std::move(jobList);
            response.code = 200;
            response.write(response_json.dump());
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

    void JobController::getJobDetails(const crow::request &request, crow::response &response, int id)
    {
        try
        {
            // 获取路径参数中的职位ID
            std::string jobId = std::to_string(id);
            if (jobId.empty())// Note: std::to_string(int) will not produce an empty string. This condition might always be false.
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["message"] = "缺少职位ID参数";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 调用服务层获取职位详情
            auto job = Service::JobSearchService::getInstance().getJobById(std::stoll(jobId));
            if (job.JobID == -1)
            {
                response.code = 404;
                crow::json::wvalue error_json;
                error_json["message"] = "职位不存在";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 获取相关职位
            auto relatedJobs = Service::JobSearchService::getInstance().getRelatedJobs(std::stoll(jobId));

            // 构建响应JSON
            crow::json::wvalue result_data;// Renamed from 'result' to avoid confusion
            result_data["jobId"] = job.JobID;
            result_data["jobTitle"] = job.JobTitle;
            result_data["enterpriseId"] = job.EnterpriseID;
            result_data["jobCategory"] = job.JobCategory;
            result_data["recruitmentCount"] = job.RecruitmentCount;
            result_data["workLocation"] = job.WorkLocation;
            result_data["minSalary"] = job.MinSalary;
            result_data["maxSalary"] = job.MaxSalary;
            result_data["responsibilities"] = job.Responsibilities;
            result_data["requirements"] = job.Requirements;
            result_data["experienceRequired"] = job.ExperienceRequired;
            result_data["educationRequired"] = job.EducationRequired;
            result_data["benefits"] = job.Benefits;
            result_data["publishDate"] = job.PublishDate;
            result_data["deadlineDate"] = job.DeadlineDate;
            result_data["jobStatus"] = job.JobStatus;

            // 添加相关职位
            crow::json::wvalue::list relatedList;
            for (const auto &related: relatedJobs)
            {
                crow::json::wvalue item;
                item["jobId"] = related.JobID;
                item["jobTitle"] = related.JobTitle;
                item["workLocation"] = related.WorkLocation;
                item["minSalary"] = related.MinSalary;
                item["maxSalary"] = related.MaxSalary;
                relatedList.push_back(item);
            }
            result_data["relatedJobs"] = std::move(relatedList);

            crow::json::wvalue response_json;
            response_json["message"] = std::move(result_data);
            response.code = 200;
            response.write(response_json.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "获取职位详情失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }

    void JobController::applyForJob(const crow::request &request, crow::response &response, int jobID)
    {
        try
        {
            // 验证请求并获取用户ID
            std::string token = request.get_header_value("Authorization").substr(7);
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

            // 获取路径参数中的职位ID
            std::string jobId = std::to_string(jobID);

            // 解析请求体
            auto body = crow::json::load(request.body);
            if (!body || !body.has("resumeId"))
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["message"] = "无效的请求格式";
                response.write(error_json.dump());
                response.end();
                return;
            }

            // 调用服务层申请职位
            bool success = Service::ApplicationService::getInstance().applyForJob(
                    std::stoll(userId),
                    body["resumeId"].i(),
                    std::stoll(jobId));

            if (!success)
            {
                response.code = 400;
                crow::json::wvalue error_json;
                error_json["message"] = "申请职位失败";
                response.write(error_json.dump());
                response.end();
                return;
            }

            response.code = 200;
            crow::json::wvalue success_json;
            success_json["message"] = "职位申请成功";
            response.write(success_json.dump());
            response.end();
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "申请职位失败: " << e.what();
            response.code = 500;
            crow::json::wvalue error_json;
            error_json["message"] = "服务器内部错误";
            response.write(error_json.dump());
            response.end();
        }
    }
}// namespace Router