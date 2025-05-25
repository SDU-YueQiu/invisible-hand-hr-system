/**
 * @file jobApplicationDAO.cpp
 * @brief 职位申请数据访问对象类实现，封装职位申请表数据库操作
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#include "jobApplicationDAO.h"
#include "../Model/jobApplication.h"
#include "databaseManager.h"
#include <crow/logging.h>
#include <stdexcept>

namespace DAL
{
    using namespace Model;

    JobApplication JobApplicationDAO::createFromRow(DbRow &row)
    {
        try
        {
            return JobApplication{
                    std::get<int64_t>(row.at("ApplicationID")),
                    std::get<int64_t>(row.at("UserID")),
                    std::get<int64_t>(row.at("ResumeID")),
                    std::get<int64_t>(row.at("JobID")),
                    std::get<int64_t>(row.at("EnterpriseID")),
                    std::get<std::string>(row.at("ApplicationTime")),
                    std::get<std::string>(row.at("CurrentStatus")),
                    std::get<std::string>(row.at("EnterpriseNotes")), ""};
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "创建职位申请对象失败: " << e.what();
            return JobApplication{};
        }
    }

    JobApplication JobApplicationDAO::findById(int64_t applicationId)
    {
        CROW_LOG_INFO << "查询职位申请记录，ID: " << applicationId;
        const std::string sql = "SELECT * FROM JobApplications WHERE ApplicationID = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(applicationId)});

        if (!result || result->empty())
        {
            CROW_LOG_WARNING << "未找到职位申请记录，ID: " << applicationId;
            return JobApplication{};
        }
        return createFromRow(result->front());
    }

    JobApplication JobApplicationDAO::findByUserIdAndJobId(int64_t userId, int64_t jobId)
    {
        CROW_LOG_INFO << "查询用户职位申请记录，用户ID: " << userId << " 职位ID: " << jobId;
        const std::string sql = "SELECT * FROM JobApplications WHERE UserID = ? AND JobID = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(userId), std::to_string(jobId)});

        if (!result || result->empty())
        {
            CROW_LOG_WARNING << "未找到用户职位申请记录，用户ID: " << userId << " 职位ID: " << jobId;
            return JobApplication{};
        }
        return createFromRow(result->front());
    }

    std::vector<JobApplication> JobApplicationDAO::findByUserId(int64_t userId)
    {
        CROW_LOG_INFO << "查询用户所有职位申请记录，用户ID: " << userId;
        const std::string sql = "SELECT * FROM JobApplications WHERE UserID = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(userId)});

        std::vector<JobApplication> applications;
        if (!result) return applications;

        for (auto &row: *result)
        {
            applications.push_back(createFromRow(row));
        }
        return applications;
    }

    std::vector<JobApplication> JobApplicationDAO::findByJobId(int64_t jobId)
    {
        CROW_LOG_INFO << "查询职位所有申请记录，职位ID: " << jobId;
        const std::string sql = "SELECT * FROM JobApplications WHERE JobID = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(jobId)});

        std::vector<JobApplication> applications;
        if (!result) return applications;

        for (auto &row: *result)
        {
            applications.push_back(createFromRow(row));
        }
        return applications;
    }

    bool JobApplicationDAO::create(const JobApplication &applicationData)
    {
        CROW_LOG_INFO << "创建新职位申请记录，用户ID: " << applicationData.UserID << " 职位ID: " << applicationData.JobID;
        const std::string sql = R"(
            INSERT INTO JobApplications (
                UserID, ResumeID, JobID, EnterpriseID,
                ApplicationTime, CurrentStatus, EnterpriseNotes
            ) VALUES (?, ?, ?, ?, ?, ?, ?)
        )";

        std::vector<std::string> params = {
                std::to_string(applicationData.UserID),
                std::to_string(applicationData.ResumeID),
                std::to_string(applicationData.JobID),
                std::to_string(applicationData.EnterpriseID),
                applicationData.ApplicationTime,
                applicationData.Status,
                applicationData.Feedback};

        auto result = dbManager.executeQuery(sql, params);
        return !result->empty();
    }

    bool JobApplicationDAO::update(int64_t applicationId, const Model::JobApplication &applicationData)
    {
        CROW_LOG_INFO << "更新职位申请记录，ID: " << applicationId;
        const std::string sql = R"(
            UPDATE JobApplications SET
                UserID = ?, ResumeID = ?, JobID = ?, EnterpriseID = ?,
                ApplicationTime = ?, Status = ?, Feedback = ?, UpdateTime = ?
            WHERE ApplicationID = ?
        )";

        std::vector<std::string> params = {
                std::to_string(applicationData.UserID),
                std::to_string(applicationData.ResumeID),
                std::to_string(applicationData.JobID),
                std::to_string(applicationData.EnterpriseID),
                applicationData.ApplicationTime,
                applicationData.Status,
                applicationData.Feedback,
                applicationData.UpdateTime,
                std::to_string(applicationId)};

        auto result = dbManager.executeQuery(sql, params);
        return result != nullptr;
    }
}// namespace DAL