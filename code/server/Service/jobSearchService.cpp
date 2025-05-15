/**
 * @file jobSearchService.cpp
 * @brief 职位搜索服务类实现，处理职位搜索和查询相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#include "jobSearchService.h"
#include <crow/logging.h>
#include <sstream>
#include <algorithm>

namespace Service
{
    using namespace Model;

    /**
     * @brief 根据条件搜索职位列表
     * @param criteria 搜索条件结构体
     * @return std::vector<Model::JobPosting> 职位列表
     */
    std::vector<JobPosting> JobSearchService::searchJobs(const JobSearchCriteria &criteria)
    {
        CROW_LOG_INFO << "搜索职位，关键词: " << (criteria.keyword.empty() ? "无" : criteria.keyword);
        
        // 构建筛选条件SQL
        std::vector<std::string> conditions;
        
        // 关键词搜索（职位名称或公司名称）
        if (!criteria.keyword.empty()) {
            std::string keywordCondition = "(JobTitle LIKE '%" + criteria.keyword + "%' OR "
                                          "JobCategory LIKE '%" + criteria.keyword + "%' OR "
                                          "Responsibilities LIKE '%" + criteria.keyword + "%')";
            conditions.push_back(keywordCondition);
        }
        
        // 其他筛选条件
        if (!criteria.jobCategory.empty()) {
            conditions.push_back("JobCategory = '" + criteria.jobCategory + "'");
        }
        
        if (!criteria.workLocation.empty()) {
            conditions.push_back("WorkLocation = '" + criteria.workLocation + "'");
        }
        
        if (!criteria.salaryRange.empty()) {
            conditions.push_back("SalaryRange = '" + criteria.salaryRange + "'");
        }
        
        if (!criteria.experienceRequired.empty()) {
            conditions.push_back("ExperienceRequired = '" + criteria.experienceRequired + "'");
        }
        
        if (!criteria.educationRequired.empty()) {
            conditions.push_back("EducationRequired = '" + criteria.educationRequired + "'");
        }
        
        // 默认只搜索开放职位
        conditions.push_back("JobStatus = '" + criteria.jobStatus + "'");
        
        // 组合所有条件
        std::string filter;
        if (!conditions.empty()) {
            filter = conditions[0];
            for (size_t i = 1; i < conditions.size(); ++i) {
                filter += " AND " + conditions[i];
            }
        }
        
        // 添加排序条件
        std::string orderBy;
        if (!criteria.sortBy.empty()) {
            // 验证排序字段是否合法
            const std::vector<std::string> validSortFields = {
                "PublishDate", "UpdateDate", "DeadlineDate", "JobTitle", "SalaryRange"
            };
            
            if (std::find(validSortFields.begin(), validSortFields.end(), criteria.sortBy) != validSortFields.end()) {
                orderBy = " ORDER BY " + criteria.sortBy;
                if (!criteria.sortOrder.empty() && (criteria.sortOrder == "asc" || criteria.sortOrder == "desc")) {
                    orderBy += " " + criteria.sortOrder;
                } else {
                    orderBy += " DESC"; // 默认降序
                }
            }
        }
        
        // 调用DAO获取结果
        auto jobs = jobPostingDAO.findByFilter(filter + orderBy);
        
        //返回前returnSize条记录
        if (jobs.size() > criteria.returnSize) {
            jobs.resize(criteria.returnSize);
        }
        return jobs;
    }

    /**
     * @brief 根据职位ID获取职位详细信息
     * @param jobId 职位ID
     * @return Model::JobPosting 职位对象
     */
    JobPosting JobSearchService::getJobById(int64_t jobId)
    {
        CROW_LOG_INFO << "获取职位详情，ID: " << jobId;
        return jobPostingDAO.findById(jobId);
    }

    /**
     * @brief 获取与指定职位相关的其他职位
     * @param jobId 职位ID
     * @return std::vector<Model::JobPosting> 相关职位列表
     */
    std::vector<JobPosting> JobSearchService::getRelatedJobs(int64_t jobId)
    {
        CROW_LOG_INFO << "获取相关职位，ID: " << jobId;
        
        // 先获取当前职位信息
        auto currentJob = jobPostingDAO.findById(jobId);
        if (currentJob.JobID == -1) {
            return {};
        }
        
        // 构建相关职位查询条件（同一公司、同一类别）
        std::string filter = "JobID != " + std::to_string(jobId) + 
                            " AND EnterpriseID = " + std::to_string(currentJob.EnterpriseID) + 
                            " AND JobCategory = '" + currentJob.JobCategory + "'" +
                            " AND JobStatus = 'Open'";
        
        // 限制返回数量
        auto relatedJobs = jobPostingDAO.findByFilter(filter);
        if (relatedJobs.size() > 5) {
            relatedJobs.resize(5);
        }
        
        return relatedJobs;
    }
}// namespace Service