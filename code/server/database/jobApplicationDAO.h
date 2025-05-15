/**
 * @file jobApplicationDAO.h
 * @brief 职位申请数据访问对象类定义
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once
#include "../Model/jobApplication.h"
#include "databaseManager.h"
#include <crow/logging.h>

namespace DAL
{
    class JobApplicationDAO
    {
    public:
        /**
         * @brief 获取JobApplicationDAO单例实例（线程安全）
         * @return JobApplicationDAO& JobApplicationDAO实例
         */
        static JobApplicationDAO &getInstance()
        {
            thread_local static JobApplicationDAO instance;
            return instance;
        }

        /**
         * @brief 根据申请ID查询职位申请记录
         * @param applicationId 申请记录ID
         * @return Model::JobApplication 申请记录对象
         */
        Model::JobApplication findById(int64_t applicationId);

        /**
         * @brief 根据用户ID和职位ID查询申请记录
         * @param userId 用户ID
         * @param jobId 职位ID
         * @return Model::JobApplication 申请记录对象
         */
        Model::JobApplication findByUserIdAndJobId(int64_t userId, int64_t jobId);

        /**
         * @brief 根据用户ID查询所有申请记录
         * @param userId 用户ID
         * @return std::vector<Model::JobApplication> 申请记录列表
         */
        std::vector<Model::JobApplication> findByUserId(int64_t userId);

        /**
         * @brief 根据职位ID查询所有申请记录
         * @param jobId 职位ID
         * @return std::vector<Model::JobApplication> 申请记录列表
         */
        std::vector<Model::JobApplication> findByJobId(int64_t jobId);

        /**
         * @brief 创建新的职位申请记录
         * @param applicationData 申请数据
         * @return bool 创建成功返回true
         */
        bool create(const Model::JobApplication &applicationData);

        /**
         * @brief 更新职位申请记录
         * @param applicationId 申请记录ID
         * @param applicationData 更新后的申请数据
         * @return bool 更新成功返回true，否则返回false
         */
        bool update(int64_t applicationId, const Model::JobApplication &applicationData);

    private:
        DatabaseManager &dbManager = DatabaseManager::getInstance();

        /**
         * @brief 从数据库行创建JobApplication对象
         * @param row 数据库行
         * @return Model::JobApplication 申请记录对象
         */
        Model::JobApplication createFromRow(DbRow &row);
    };
}// namespace DAL