/**
 * @file talentSearchService.h
 * @brief 人才搜索服务类，处理人才搜索和筛选相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#pragma once

#include "../Database/jobApplicationDAO.h"
#include "../Database/resumeDAO.h"
#include "../Model/jobApplication.h"
#include "../Model/resume.h"
#include <string>
#include <vector>


namespace Service
{
    /**
     * @brief 简历搜索条件结构体，用于封装简历搜索参数
     */
    struct ResumeSearchCriteria
    {
        std::string keyword;    ///< 搜索关键词（技能、教育经历等）
        std::string education;  ///< 学历要求
        std::string experience; ///< 工作经验要求
        std::string location;   ///< 期望工作地点
        int32_t minSalary = 0;  ///< 最低期望薪资
        int32_t maxSalary = 0;  ///< 最高期望薪资
        std::string sortBy;     ///< 排序字段
        std::string sortOrder;  ///< 排序顺序（默认为降序）
        int32_t returnSize = 50;///< 每页显示数量
    };

    /**
     * @brief 人才搜索服务类，封装人才搜索和筛选相关业务逻辑
     */
    class TalentSearchService
    {
    public:
        /**
         * @brief 获取TalentSearchService单例实例（线程安全）
         * @return TalentSearchService& TalentSearchService实例
         */
        static TalentSearchService &getInstance()
        {
            thread_local static TalentSearchService instance;
            return instance;
        }

        /**
         * @brief 获取投递到指定职位的所有申请者（简历）列表
         * @param jobId 职位ID
         * @return std::vector<Model::Resume> 简历列表
         */
        std::vector<Model::Resume> getApplicantsByJobId(int64_t jobId);

        /**
         * @brief 根据条件搜索简历库
         * @param criteria 搜索条件结构体
         * @return std::vector<Model::Resume> 简历列表
         */
        std::vector<Model::Resume> searchResumes(const ResumeSearchCriteria &criteria);

    private:
        DAL::ResumeDAO &resumeDAO = DAL::ResumeDAO::getInstance();
        DAL::JobApplicationDAO &applicationDAO = DAL::JobApplicationDAO::getInstance();
    };
}// namespace Service