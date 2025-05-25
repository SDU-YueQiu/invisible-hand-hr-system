/**
 * @file talentSearchService.cpp
 * @brief 人才搜索服务类实现，处理人才搜索和筛选相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.0
 */

#include "talentSearchService.h"
#include <crow/logging.h>

namespace Service
{
    std::vector<Model::Resume> TalentSearchService::getApplicantsByJobId(int64_t jobId)
    {
        CROW_LOG_INFO << "获取职位申请者列表，职位ID: " << jobId;

        // 1. 获取该职位所有申请记录
        auto applications = applicationDAO.findByJobId(jobId);
        std::vector<Model::Resume> resumes;

        // 2. 获取每份申请对应的简历
        for (const auto &application: applications)
        {
            if (application.Status != "Pending")
                continue;
            auto resume = resumeDAO.findById(application.ResumeID);
            //TODO:技术债
            resume.jobapplicationID = application.ApplicationID;
            if (resume.ResumeID != -1)
            {
                resumes.push_back(resume);
            }
        }

        return resumes;
    }

    std::vector<Model::Resume> TalentSearchService::searchResumes(const ResumeSearchCriteria &criteria)
    {
        CROW_LOG_INFO << "搜索简历，关键词: " << (criteria.keyword.empty() ? "无" : criteria.keyword);

        // 构建筛选条件
        std::vector<std::string> conditions;

        //只搜索公开简历
        conditions.push_back("VisibilityStatus = 'PUBLIC'");

        // 关键词搜索（技能、教育经历等）
        if (!criteria.keyword.empty())
        {
            std::string keywordCondition = "(SkillsCertificates LIKE '%" + criteria.keyword + "%' OR "
                                                                                              "EducationExperience LIKE '%" +
                                           criteria.keyword + "%' OR "
                                                              "WorkExperience LIKE '%" +
                                           criteria.keyword + "%' OR "
                                                              "JobIntent LIKE '%" +
                                           criteria.keyword + "%')";
            conditions.push_back(keywordCondition);
        }

        // 学历筛选
        if (!criteria.education.empty())
        {
            conditions.push_back("EducationExperience LIKE '%" + criteria.education + "%'");
        }

        // 工作经验筛选
        if (!criteria.experience.empty())
        {
            conditions.push_back("WorkExperience LIKE '%" + criteria.experience + "%'");
        }

        // 期望工作地点筛选
        if (!criteria.location.empty())
        {
            conditions.push_back("JobIntent LIKE '%" + criteria.location + "%'");
        }

        // 组合所有条件
        std::string filter;
        if (!conditions.empty())
        {
            filter = conditions[0];
            for (size_t i = 1; i < conditions.size(); ++i)
            {
                filter += " AND " + conditions[i];
            }
        }

        // 添加排序条件
        std::string orderBy;
        if (!criteria.sortBy.empty())
        {
            // 验证排序字段是否合法
            const std::vector<std::string> validSortFields = {
                    "UpdateTime", "CreateTime", "ResumeTitle"};

            if (std::find(validSortFields.begin(), validSortFields.end(), criteria.sortBy) != validSortFields.end())
            {
                orderBy = " ORDER BY " + criteria.sortBy;
                if (!criteria.sortOrder.empty() && (criteria.sortOrder == "asc" || criteria.sortOrder == "desc"))
                {
                    orderBy += " " + criteria.sortOrder;
                } else
                {
                    orderBy += " DESC";// 默认降序
                }
            }
        }

        // 调用DAO获取结果
        auto resumes = resumeDAO.findByFilter(filter + orderBy);

        // 返回前returnSize条记录
        if (resumes.size() > criteria.returnSize)
        {
            resumes.resize(criteria.returnSize);
        }

        return resumes;
    }
}// namespace Service