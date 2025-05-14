/**
 * @file jobPostingService.cpp
 * @brief 职位发布服务类实现，处理职位发布和管理业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#include "jobPostingService.h"
#include <crow/logging.h>

namespace Service
{
    bool JobPostingService::createJob(int64_t enterpriseId, const Model::JobPosting& jobData)
    {
        CROW_LOG_INFO << "Creating job for enterprise ID: " << enterpriseId;
        
        // 验证企业ID是否匹配
        if (jobData.EnterpriseID != enterpriseId)
        {
            CROW_LOG_WARNING << "Enterprise ID mismatch in job creation";
            return false;
        }

        return jobPostingDAO.create(jobData);
    }

    Model::JobPosting JobPostingService::getJobById(int64_t jobId)
    {
        CROW_LOG_INFO << "Getting job by ID: " << jobId;
        return jobPostingDAO.findById(jobId);
    }

    std::vector<Model::JobPosting> JobPostingService::getJobsByEnterpriseId(int64_t enterpriseId)
    {
        CROW_LOG_INFO << "Getting all jobs for enterprise ID: " << enterpriseId;
        return jobPostingDAO.findByEnterpriseId(enterpriseId);
    }

    bool JobPostingService::updateJob(int64_t jobId, const Model::JobPosting& jobData)
    {
        CROW_LOG_INFO << "Updating job ID: " << jobId;
        
        // 先获取现有职位验证企业ID
        auto existingJob = jobPostingDAO.findById(jobId);
        if (existingJob.JobID == -1)
        {
            CROW_LOG_WARNING << "Job not found for update, ID: " << jobId;
            return false;
        }

        // 确保企业ID没有被修改
        if (existingJob.EnterpriseID != jobData.EnterpriseID)
        {
            CROW_LOG_WARNING << "Enterprise ID cannot be modified in job update";
            return false;
        }

        return jobPostingDAO.update(jobId, jobData);
    }

    bool JobPostingService::deleteJob(int64_t jobId)
    {
        CROW_LOG_INFO << "Deleting job ID: " << jobId;
        return jobPostingDAO.deleteById(jobId);
    }

    bool JobPostingService::toggleJobStatus(int64_t jobId, const std::string& status)
    {
        CROW_LOG_INFO << "Toggling job status, ID: " << jobId << ", new status: " << status;
        
        auto job = jobPostingDAO.findById(jobId);
        if (job.JobID == -1)
        {
            CROW_LOG_WARNING << "Job not found for status toggle, ID: " << jobId;
            return false;
        }

        job.JobStatus = status;
        return jobPostingDAO.update(jobId, job);
    }
} // namespace Service