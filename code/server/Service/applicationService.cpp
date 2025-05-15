/**
 * @file applicationService.cpp
 * @brief 职位申请服务类实现，处理职位申请相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#include "applicationService.h"
#include "../Service/jobPostingService.h"
#include "../Service/resumeService.h"
#include <crow/logging.h>


namespace Service
{
    bool ApplicationService::applyForJob(int64_t userId, int64_t resumeId, int64_t jobId)
    {
        CROW_LOG_INFO << "User " << userId << " applying for job " << jobId << " with resume " << resumeId;

        // 验证简历是否属于该用户
        auto resume = ResumeService::getInstance().getResumeById(resumeId);
        if (resume.ResumeID == -1 || resume.UserID != userId)
        {
            CROW_LOG_WARNING << "Invalid resume ID or resume does not belong to user";
            return false;
        }

        // 验证职位是否存在且处于招聘状态
        auto job = JobPostingService::getInstance().getJobById(jobId);
        if (job.JobID == -1 || job.JobStatus != "Open")
        {
            CROW_LOG_WARNING << "Invalid job ID or job is not open for application";
            return false;
        }

        // 检查是否已申请过该职位
        auto existingApplications = applicationDAO.findByUserIdAndJobId(userId, jobId);
        if (existingApplications.ApplicationID != -1)
        {
            CROW_LOG_WARNING << "User has already applied for this job";
            return false;
        }

        // 创建申请记录
        Model::JobApplication application;
        application.UserID = userId;
        application.ResumeID = resumeId;
        application.JobID = jobId;
        application.Status = "Pending";
        application.ApplicationTime = std::time(nullptr);

        return applicationDAO.create(application);
    }

    std::vector<Model::JobApplication> ApplicationService::getUserApplications(int64_t userId)
    {
        CROW_LOG_INFO << "Getting all applications for user ID: " << userId;
        return applicationDAO.findByUserId(userId);
    }

    Model::JobApplication ApplicationService::getApplicationById(int64_t applicationId)
    {
        CROW_LOG_INFO << "Getting application by ID: " << applicationId;
        return applicationDAO.findById(applicationId);
    }

    bool ApplicationService::updateApplicationStatus(int64_t applicationId, const std::string &status)
    {
        CROW_LOG_INFO << "Updating application status, ID: " << applicationId << ", new status: " << status;

        // 获取现有申请记录
        auto application = applicationDAO.findById(applicationId);
        if (application.ApplicationID == -1)
        {
            CROW_LOG_WARNING << "Application not found for status update";
            return false;
        }

        // 更新状态
        application.Status = status;
        return applicationDAO.update(applicationId, application);
    }
}// namespace Service