/**
 * @file resumeService.cpp
 * @brief 简历服务类实现，处理简历相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#include "resumeService.h"
#include <crow/logging.h>

namespace Service
{
    bool ResumeService::createResume(int userId, const Model::Resume &resumeData)
    {
        CROW_LOG_INFO << "Creating resume for user ID: " << userId;

        // 验证用户ID是否匹配
        if (resumeData.UserID != userId)
        {
            CROW_LOG_WARNING << "User ID mismatch in resume creation";
            return false;
        }

        return resumeDAO.create(resumeData);
    }

    Model::Resume ResumeService::getResumeById(int resumeId)
    {
        CROW_LOG_INFO << "Getting resume by ID: " << resumeId;
        return resumeDAO.findById(resumeId);
    }

    std::vector<Model::Resume> ResumeService::getResumesByUserId(int userId)
    {
        CROW_LOG_INFO << "Getting all resumes for user ID: " << userId;
        return resumeDAO.findByUserId(userId);
    }

    bool ResumeService::updateResume(int resumeId, const Model::Resume &resumeData)
    {
        CROW_LOG_INFO << "Updating resume ID: " << resumeId;

        // 先获取现有简历验证用户ID
        auto existingResume = resumeDAO.findById(resumeId);
        if (existingResume.ResumeID == -1)
        {
            CROW_LOG_WARNING << "Resume not found for update, ID: " << resumeId;
            return false;
        }

        // 确保用户ID没有被修改
        if (existingResume.UserID != resumeData.UserID)
        {
            CROW_LOG_WARNING << "User ID cannot be modified in resume update";
            return false;
        }

        // 只更新非空的字段
        if (!resumeData.ResumeTitle.empty()) existingResume.ResumeTitle = resumeData.ResumeTitle;
        if (!resumeData.BasicInfo.empty()) existingResume.BasicInfo = resumeData.BasicInfo;
        if (!resumeData.JobIntent.empty()) existingResume.JobIntent = resumeData.JobIntent;
        if (!resumeData.EducationExperience.empty()) existingResume.EducationExperience = resumeData.EducationExperience;
        if (!resumeData.WorkExperience.empty()) existingResume.WorkExperience = resumeData.WorkExperience;
        if (!resumeData.ProjectExperience.empty()) existingResume.ProjectExperience = resumeData.ProjectExperience;
        if (!resumeData.SkillsCertificates.empty()) existingResume.SkillsCertificates = resumeData.SkillsCertificates;
        if (!resumeData.SelfDescription.empty()) existingResume.SelfDescription = resumeData.SelfDescription;
        if (!resumeData.VisibilityStatus.empty()) existingResume.VisibilityStatus = resumeData.VisibilityStatus;
        if (!resumeData.AttachmentPath.empty()) existingResume.AttachmentPath = resumeData.AttachmentPath;

        return resumeDAO.update(resumeId, existingResume);
    }

    bool ResumeService::deleteResume(int resumeId)
    {
        CROW_LOG_INFO << "Deleting resume ID: " << resumeId;
        return resumeDAO.deleteById(resumeId);
    }
}// namespace Service