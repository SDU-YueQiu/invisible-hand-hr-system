/**
 * @file applicationService.h
 * @brief 职位申请服务类，处理职位申请相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#pragma once

#include "../Model/jobApplication.h"
#include "../Database/jobApplicationDAO.h"
#include <vector>
#include <string>

namespace Service
{
    /**
     * @brief 职位申请服务类，封装职位申请相关业务逻辑
     */
    class ApplicationService
    {
    public:
        /**
         * @brief 获取ApplicationService单例实例（线程安全）
         * @return ApplicationService& ApplicationService实例
         */
        static ApplicationService& getInstance()
        {
            thread_local static ApplicationService instance;
            return instance;
        }

        /**
         * @brief 申请职位
         * @param userId 用户ID
         * @param resumeId 简历ID
         * @param jobId 职位ID
         * @return bool 申请成功返回true，否则返回false
         */
        bool applyForJob(int64_t userId, int64_t resumeId, int64_t jobId);

        /**
         * @brief 获取用户的所有职位申请记录
         * @param userId 用户ID
         * @return std::vector<Model::JobApplication> 申请记录列表
         */
        std::vector<Model::JobApplication> getUserApplications(int64_t userId);

        /**
         * @brief 根据申请ID获取申请详情
         * @param applicationId 申请ID
         * @return Model::JobApplication 申请记录对象
         */
        Model::JobApplication getApplicationById(int64_t applicationId);

        /**
         * @brief 更新申请状态
         * @param applicationId 申请ID
         * @param status 新状态
         * @return bool 更新成功返回true，否则返回false
         */
        bool updateApplicationStatus(int64_t applicationId, const std::string& status);

    private:
        DAL::JobApplicationDAO& applicationDAO = DAL::JobApplicationDAO::getInstance();
    };
} // namespace Service