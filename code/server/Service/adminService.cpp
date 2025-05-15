/**
 * @file adminService.cpp
 * @brief 管理员服务类实现，处理管理员相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#include "adminService.h"
#include <crow/logging.h>

namespace Service
{
    std::vector<Model::IndividualUser> AdminService::getUsers(const std::string &filter)
    {
        CROW_LOG_INFO << "Getting individual users with filter: " << filter;
        return individualUserDAO.findByFilter(filter);
    }

    bool AdminService::updateUserStatus(int userId, const std::string &status)
    {
        CROW_LOG_INFO << "Updating user status, ID: " << userId << ", new status: " << status;

        // 获取现有用户信息
        auto user = individualUserDAO.findById(userId);
        if (user.UserID == -1)
        {
            CROW_LOG_WARNING << "User not found for status update, ID: " << userId;
            return false;
        }

        return individualUserDAO.updateStatus(userId, status);
    }

    std::vector<Model::EnterpriseUser> AdminService::getEnterprises(const std::string &filter)
    {
        CROW_LOG_INFO << "Getting enterprises with filter: " << filter;
        return enterpriseUserDAO.findByFilter(filter);
    }

    bool AdminService::approveEnterprise(int64_t enterpriseId, const std::string &opinion)
    {
        CROW_LOG_INFO << "Approving enterprise registration, ID: " << enterpriseId;

        // 获取现有企业信息
        auto enterprise = enterpriseUserDAO.findById(enterpriseId);
        if (enterprise.EnterpriseID == -1)
        {
            CROW_LOG_WARNING << "Enterprise not found for approval, ID: " << enterpriseId;
            return false;
        }

        // 更新状态为"Approved"并记录审核意见
        enterprise.AccountStatus = "Approved";
        return enterpriseUserDAO.update(enterpriseId, enterprise);
    }

    bool AdminService::rejectEnterprise(int64_t enterpriseId, const std::string &opinion)
    {
        CROW_LOG_INFO << "Rejecting enterprise registration, ID: " << enterpriseId;

        // 获取现有企业信息
        auto enterprise = enterpriseUserDAO.findById(enterpriseId);
        if (enterprise.EnterpriseID == -1)
        {
            CROW_LOG_WARNING << "Enterprise not found for rejection, ID: " << enterpriseId;
            return false;
        }

        // 更新状态为"Rejected"并记录拒绝理由
        enterprise.AccountStatus = "Rejected";
        return enterpriseUserDAO.update(enterpriseId, enterprise);
    }

    std::vector<Model::JobPosting> AdminService::getAllJobs(const std::string &filter)
    {
        CROW_LOG_INFO << "Getting all jobs with filter: " << filter;
        return jobPostingDAO.findByFilter(filter);
    }

    bool AdminService::updateJobStatus(int64_t jobId, const std::string &status)
    {
        CROW_LOG_INFO << "Updating job status, ID: " << jobId << ", new status: " << status;

        // 获取现有职位信息
        auto job = jobPostingDAO.findById(jobId);
        if (job.JobID == -1)
        {
            CROW_LOG_WARNING << "Job not found for status update, ID: " << jobId;
            return false;
        }

        job.JobStatus = status;

        return jobPostingDAO.update(jobId, job);
    }
}// namespace Service