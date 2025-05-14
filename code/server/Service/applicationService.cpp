/**
 * @file applicationService.cpp
 * @brief 职位申请服务实现文件，处理职位申请业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#include "applicationService.h"
#include "../Database/databaseManager.h"
#include "../Utils/SecurityUtils.h"
#include <crow/json.h>

namespace Service {
    ApplicationService::ApplyResult ApplicationService::applyForJob(const std::string& userId, const std::string& jobId, const std::string& resumeId) {
        auto& db = DAL::getDatabaseManager();
        
        // 检查职位是否存在
        auto jobQuery = db.prepareStatement("SELECT COUNT(*) FROM Jobs WHERE JobID = ?");
        jobQuery.bind(1, jobId);
        jobQuery.executeStep();
        
        if (jobQuery.getColumn(0).getInt() == 0) {
            return {false, "Job not found"};
        }
        
        // 检查简历是否存在
        auto resumeQuery = db.prepareStatement("SELECT COUNT(*) FROM Resumes WHERE ResumeID = ? AND UserID = ?");
        resumeQuery.bind(1, resumeId);
        resumeQuery.bind(2, userId);
        resumeQuery.executeStep();
        
        if (resumeQuery.getColumn(0).getInt() == 0) {
            return {false, "Resume not found or not owned by user"};
        }
        
        // 创建申请记录
        auto insertQuery = db.prepareStatement(
            "INSERT INTO JobApplications (UserID, JobID, ResumeID, ApplyTime, Status) "
            "VALUES (?, ?, ?, datetime('now'), 'Pending')");
        insertQuery.bind(1, userId);
        insertQuery.bind(2, jobId);
        insertQuery.bind(3, resumeId);
        
        if (!insertQuery.exec()) {
            return {false, "Application failed"};
        }
        
        return {true, "Application submitted successfully"};
    }
}