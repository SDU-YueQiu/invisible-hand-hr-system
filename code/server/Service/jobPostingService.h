/**
 * @file jobPostingService.h
 * @brief 职位发布服务类，处理职位发布和管理业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#pragma once

#include "../Model/jobPosting.h"
#include "../Database/jobPostingDAO.h"
#include <vector>
#include <string>
#include <crow/logging.h>

namespace Service
{
    /**
     * @brief 职位发布服务类，封装职位发布和管理相关业务逻辑
     */
    class JobPostingService
    {
    public:
        /**
         * @brief 获取JobPostingService单例实例（线程安全）
         * @return JobPostingService& JobPostingService实例
         */
        static JobPostingService& getInstance()
        {
            thread_local static JobPostingService instance;
            return instance;
        }

        /**
         * @brief 为企业创建新职位
         * @param enterpriseId 企业ID
         * @param jobData 职位数据
         * @return bool 创建成功返回true，否则返回false
         */
        bool createJob(int64_t enterpriseId, const Model::JobPosting& jobData);

        /**
         * @brief 根据职位ID获取职位详情
         * @param jobId 职位ID
         * @return Model::JobPosting 职位对象
         */
        Model::JobPosting getJobById(int64_t jobId);

        /**
         * @brief 获取企业发布的所有职位
         * @param enterpriseId 企业ID
         * @return std::vector<Model::JobPosting> 职位列表
         */
        std::vector<Model::JobPosting> getJobsByEnterpriseId(int64_t enterpriseId);

        /**
         * @brief 更新职位信息
         * @param jobId 职位ID
         * @param jobData 更新后的职位数据
         * @return bool 更新成功返回true，否则返回false
         */
        bool updateJob(int64_t jobId, const Model::JobPosting& jobData);

        /**
         * @brief 删除职位
         * @param jobId 职位ID
         * @return bool 删除成功返回true，否则返回false
         */
        bool deleteJob(int64_t jobId);

        /**
         * @brief 切换职位状态
         * @param jobId 职位ID
         * @param status 新状态
         * @return bool 切换成功返回true，否则返回false
         */
        bool toggleJobStatus(int64_t jobId, const std::string& status);

    private:
        DAL::JobPostingDAO& jobPostingDAO = DAL::JobPostingDAO::getInstance();
    };
} // namespace Service