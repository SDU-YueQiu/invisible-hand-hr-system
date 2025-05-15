/**
 * @file resumeDAO.cpp
 * @brief 简历数据访问对象类实现，封装简历表数据库操作
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#include "resumeDAO.h"
#include "../Model/resume.h"
#include "databaseManager.h"
#include <crow/logging.h>
#include <sstream>
#include <stdexcept>


namespace DAL
{
    using namespace Model;

    /**
     * @brief 从数据库结果行构造简历对象
     * @param row 数据库结果行（DbRow类型）
     * @return Model::Resume 构造的简历对象（字段不匹配时返回空对象）
     */
    Model::Resume createResumeFromRow(DbRow &row)
    {
        try
        {
            return Model::Resume{
                    static_cast<int>(std::get<int64_t>(row.at("ResumeID"))),
                    static_cast<int>(std::get<int64_t>(row.at("UserID"))),
                    std::get<std::string>(row.at("ResumeTitle")),
                    std::get<std::string>(row.at("BasicInfo")),
                    std::get<std::string>(row.at("JobIntent")),
                    std::get<std::string>(row.at("EducationExperience")),
                    std::get<std::string>(row.at("WorkExperience")),
                    std::get<std::string>(row.at("ProjectExperience")),
                    std::get<std::string>(row.at("SkillsCertificates")),
                    std::get<std::string>(row.at("SelfDescription")),
                    std::get<std::string>(row.at("LastUpdateTime")),
                    std::get<std::string>(row.at("VisibilityStatus")),
                    std::get<std::string>(row.at("AttachmentPath"))};
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "创建简历对象失败: " << e.what();
            return Model::Resume{};
        }
    }

    /**
     * @brief 根据简历ID查询简历信息
     * @param resumeId 简历唯一标识
     * @return Model::Resume 简历对象（若不存在则返回空对象）
     */
    Model::Resume ResumeDAO::findById(int resumeId)
    {
        CROW_LOG_INFO << "查询简历，ID: " << resumeId;
        const std::string sql = "SELECT * FROM Resumes WHERE ResumeID = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(resumeId)});

        if (!result || result->empty())
        {
            CROW_LOG_WARNING << "未找到简历，ID: " << resumeId;
            return Model::Resume{};
        }
        return createResumeFromRow(result->front());
    }

    /**
     * @brief 根据用户ID查询该用户的所有简历
     * @param userId 用户唯一标识
     * @return std::vector<Model::Resume> 简历对象列表（可能为空）
     */
    std::vector<Model::Resume> ResumeDAO::findByUserId(int userId)
    {
        CROW_LOG_INFO << "查询用户简历，用户ID: " << userId;
        const std::string sql = "SELECT * FROM Resumes WHERE UserID = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(userId)});

        std::vector<Model::Resume> resumes;
        if (!result) return resumes;

        for (auto &row: *result)
        {
            resumes.push_back(createResumeFromRow(row));
        }
        return resumes;
    }

    /**
     * @brief 向数据库中插入一条新的简历记录
     * @param resumeData 待插入的简历数据
     * @return 插入成功返回true，否则返回false
     */
    bool ResumeDAO::create(const Model::Resume &resumeData)
    {
        CROW_LOG_INFO << "创建新简历，用户ID: " << resumeData.UserID;
        const std::string sql = R"(
            INSERT INTO Resumes (
                UserID, ResumeTitle, BasicInfo, JobIntent, 
                EducationExperience, WorkExperience, ProjectExperience, 
                SkillsCertificates, SelfDescription, LastUpdateTime, 
                VisibilityStatus, AttachmentPath
            ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
        )";

        std::vector<std::string> params = {
                std::to_string(resumeData.UserID),
                resumeData.ResumeTitle,
                resumeData.BasicInfo,
                resumeData.JobIntent,
                resumeData.EducationExperience,
                resumeData.WorkExperience,
                resumeData.ProjectExperience,
                resumeData.SkillsCertificates,
                resumeData.SelfDescription,
                resumeData.LastUpdateTime,
                resumeData.VisibilityStatus,
                resumeData.AttachmentPath};

        auto result = dbManager.executeQuery(sql, params);
        return !result->empty();
    }

    /**
     * @brief 更新指定ID的简历信息
     * @param resumeId 待更新的简历ID
     * @param resumeData 更新后的简历数据
     * @return bool 更新成功返回true，否则返回false
     */
    bool ResumeDAO::update(int resumeId, const Model::Resume &resumeData)
    {
        CROW_LOG_INFO << "更新简历，ID: " << resumeId;
        const std::string sql = R"(
            UPDATE Resumes SET
                UserID = ?, ResumeTitle = ?, BasicInfo = ?, JobIntent = ?,
                EducationExperience = ?, WorkExperience = ?, ProjectExperience = ?,
                SkillsCertificates = ?, SelfDescription = ?, LastUpdateTime = ?,
                VisibilityStatus = ?, AttachmentPath = ?
            WHERE ResumeID = ?
        )";

        std::vector<std::string> params = {
                std::to_string(resumeData.UserID),
                resumeData.ResumeTitle,
                resumeData.BasicInfo,
                resumeData.JobIntent,
                resumeData.EducationExperience,
                resumeData.WorkExperience,
                resumeData.ProjectExperience,
                resumeData.SkillsCertificates,
                resumeData.SelfDescription,
                resumeData.LastUpdateTime,
                resumeData.VisibilityStatus,
                resumeData.AttachmentPath,
                std::to_string(resumeId)};

        auto result = dbManager.executeQuery(sql, params);
        return result != nullptr;
    }

    /**
     * @brief 删除指定ID的简历记录
     * @param resumeId 待删除的简历ID
     * @return bool 删除成功返回true，否则返回false
     */
    bool ResumeDAO::deleteById(int resumeId)
    {
        CROW_LOG_INFO << "删除简历，ID: " << resumeId;
        const std::string sql = "DELETE FROM Resumes WHERE ResumeID = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(resumeId)});
        return result != nullptr;
    }

    std::vector<Model::Resume> ResumeDAO::findByFilter(const std::string &filter)
    {
        CROW_LOG_INFO << "查询简历列表，筛选条件: " << (filter.empty() ? "无" : filter);
        std::string sql = "SELECT * FROM Resumes";
        if (!filter.empty())
        {
            sql += " WHERE " + filter;
        }

        auto result = dbManager.executeQuery(sql, {});
        std::vector<Model::Resume> resumes;
        if (!result) return resumes;

        for (auto &row: *result)
        {
            resumes.push_back(createResumeFromRow(row));
        }
        return resumes;
    }

}// namespace DAL