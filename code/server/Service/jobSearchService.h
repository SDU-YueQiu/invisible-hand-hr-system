/**
 * @file jobSearchService.h
 * @brief 职位搜索服务类，处理职位搜索和查询相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#pragma once

#include "../Database/jobPostingDAO.h"
#include "../Model/jobPosting.h"
#include <crow/logging.h>
#include <cstdint>
#include <string>
#include <vector>


namespace Service
{
    /**
     * @brief 职位搜索条件结构体，用于封装职位搜索参数
     */
    struct JobSearchCriteria
    {
        std::string keyword;           ///< 搜索关键词（职位名称或公司名称）
        std::string jobCategory;       ///< 职位类别
        std::string workLocation;      ///< 工作地点
        int32_t maxSalary = 0;         ///< 最大工资
        int32_t minSalary = 0;         ///< 最小工资
        std::string experienceRequired;///< 工作经验要求
        std::string educationRequired; ///< 学历要求
        std::string sortBy;            ///< 排序字段
        std::string sortOrder;         ///< 排序顺序（默认为降序）
        std::string jobStatus = "Recruiting";///< 职位状态（默认为Open）
        int32_t returnSize = 50;       ///< 每页显示数量
    };

    /**
     * @brief 职位搜索服务类，封装职位搜索和查询相关业务逻辑
     */
    class JobSearchService
    {
    public:
        /**
         * @brief 获取JobSearchService单例实例（线程安全）
         * @return JobSearchService& JobSearchService实例
         */
        static JobSearchService &getInstance()
        {
            thread_local static JobSearchService instance;
            return instance;
        }

        /**
         * @brief 根据条件搜索职位列表
         * @param criteria 搜索条件结构体
         * @return std::vector<Model::JobPosting> 职位列表
         */
        std::vector<Model::JobPosting> searchJobs(const JobSearchCriteria &criteria);

        /**
         * @brief 根据职位ID获取职位详细信息
         * @param jobId 职位ID
         * @return Model::JobPosting 职位对象
         */
        Model::JobPosting getJobById(int64_t jobId);

        /**
         * @brief 获取与指定职位相关的其他职位
         * @param jobId 职位ID
         * @return std::vector<Model::JobPosting> 相关职位列表
         */
        std::vector<Model::JobPosting> getRelatedJobs(int64_t jobId);

    private:
        DAL::JobPostingDAO &jobPostingDAO = DAL::JobPostingDAO::getInstance();
    };
}// namespace Service