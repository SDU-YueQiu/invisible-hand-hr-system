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

namespace DAL {
    using namespace Model;

    JobApplication JobApplicationDAO::createFromRow(DbRow &row) {
        try {
            return JobApplication{
                std::get<int64_t>(row.at("ApplicationID")),
                std::get<int64_t>(row.at("UserID")),
                std::get<int64_t>(row.at("ResumeID")),
                std::get<int64_t>(row.at("JobID")),
                std::get<int64_t>(row.at("EnterpriseID")),
                std::get<std::string>(row.at("ApplicationDate")),
                std::get<std::string>(row.at("Status")),
                std::get<std::string>(row.at("Feedback")),
                std::get<std::string>(row.at("UpdateTime"))
            };
        } catch (const std::bad_variant_access &e) {
            CROW_LOG_ERROR << "职位申请字段类型不匹配: " << e.what();
            return JobApplication{};
        } catch (const std::out_of_range &e) {
            CROW_LOG_ERROR << "职位申请字段缺失: " << e.what();
            return JobApplication{};
        }
    }

    JobApplication JobApplicationDAO::findById(int64_t applicationId) {
        CROW_LOG_INFO << "查询职位申请记录，ID: " << applicationId;
        const std::string sql = "SELECT * FROM JobApplications WHERE ApplicationID = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(applicationId)});

        if (!result || result->empty()) {
            CROW_LOG_WARNING << "未找到职位申请记录，ID: " << applicationId;
            return JobApplication{};
        }
        return createFromRow(result->front());
    }

    JobApplication JobApplicationDAO::findByUserIdAndJobId(int64_t userId, int64_t jobId) {
        CROW_LOG_INFO << "查询用户职位申请记录，用户ID: " << userId << " 职位ID: " << jobId;
        const std::string sql = "SELECT * FROM JobApplications WHERE UserID = ? AND JobID = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(userId), std::to_string(jobId)});

        if (!result || result->empty()) {
            CROW_LOG_WARNING << "未找到用户职位申请记录，用户ID: " << userId << " 职位ID: " << jobId;
            return JobApplication{};
        }
        return createFromRow(result->front());
    }

    std::vector<JobApplication> JobApplicationDAO::findByUserId(int64_t userId) {
        CROW_LOG_INFO << "查询用户所有职位申请记录，用户ID: " << userId;
        const std::string sql = "SELECT * FROM JobApplications WHERE UserID = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(userId)});

        std::vector<JobApplication> applications;
        if (!result) return applications;

        for (auto &row : *result) {
            applications.push_back(createFromRow(row));
        }
        return applications;
    }

    std::vector<JobApplication> JobApplicationDAO::findByJobId(int64_t jobId) {
        CROW_LOG_INFO << "查询职位所有申请记录，职位ID: " << jobId;
        const std::string sql = "SELECT * FROM JobApplications WHERE JobID = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(jobId)});

        std::vector<JobApplication> applications;
        if (!result) return applications;

        for (auto &row : *result) {
            applications.push_back(createFromRow(row));
        }
        return applications;
    }

    bool JobApplicationDAO::create(const JobApplication &applicationData) {
        CROW_LOG_INFO << "创建新职位申请记录，用户ID: " << applicationData.UserID << " 职位ID: " << applicationData.JobID;
        const std::string sql = R"(
            INSERT INTO JobApplications (
                UserID, ResumeID, JobID, EnterpriseID,
                ApplicationDate, Status, Feedback, UpdateTime
            ) VALUES (?, ?, ?, ?, ?, ?, ?, ?)
        )";

        std::vector<std::string> params = {
            std::to_string(applicationData.UserID),
            std::to_string(applicationData.ResumeID),
            std::to_string(applicationData.JobID),
            std::to_string(applicationData.EnterpriseID),
            applicationData.ApplicationDate,
            applicationData.Status,
            applicationData.Feedback,
            applicationData.UpdateTime
        };

        auto result = dbManager.executeQuery(sql, params);
        return !result->empty();
    }

    bool JobApplicationDAO::updateStatus(int64_t applicationId, const std::string &status) {
        CROW_LOG_INFO << "更新职位申请状态，ID: " << applicationId << " 新状态: " << status;
        const std::string sql = "UPDATE JobApplications SET Status = ?, UpdateTime = datetime('now') WHERE ApplicationID = ?";
        auto result = dbManager.executeQuery(sql, {status, std::to_string(applicationId)});
        return result != nullptr;
    }
}