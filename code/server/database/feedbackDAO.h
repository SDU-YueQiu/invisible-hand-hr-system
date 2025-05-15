/**
 * @file feedbackDAO.h
 * @brief 反馈数据访问对象类，封装Feedbacks表的数据库操作
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#pragma once

#include "../Model/feedback.h"
#include "databaseManager.h"
#include <vector>

namespace DAL
{
    /**
     * @brief 反馈数据访问对象类
     */
    class FeedbackDAO
    {
    public:
        /**
         * @brief 获取FeedbackDAO单例实例（线程安全）
         * @return FeedbackDAO& FeedbackDAO实例
         */
        static FeedbackDAO& getInstance()
        {
            thread_local static FeedbackDAO instance;
            return instance;
        }

        /**
         * @brief 根据反馈ID查询反馈信息
         * @param feedbackId 反馈唯一标识
         * @return Model::Feedback 反馈对象（若不存在则返回空对象）
         */
        Model::Feedback findById(int64_t feedbackId);

        /**
         * @brief 根据筛选条件查询反馈列表
         * @param filter SQL筛选条件字符串（如"Status='Pending' AND UserType='Individual'"）
         * @return std::vector<Model::Feedback> 符合条件的反馈列表
         */
        std::vector<Model::Feedback> findAll(const std::string& filter = "");

        /**
         * @brief 创建新的反馈记录
         * @param feedbackData 反馈数据
         * @return bool 创建成功返回true，否则返回false
         */
        bool create(const Model::Feedback& feedbackData);

        /**
         * @brief 更新反馈信息（主要用于管理员处理反馈）
         * @param feedbackId 反馈ID
         * @param feedbackData 更新后的反馈数据
         * @return bool 更新成功返回true，否则返回false
         */
        bool update(int64_t feedbackId, const Model::Feedback& feedbackData);

    private:
        FeedbackDAO() = default;
        FeedbackDAO(const FeedbackDAO&) = delete;
        FeedbackDAO& operator=(const FeedbackDAO&) = delete;
        
        DatabaseManager& dbManager = DatabaseManager::getInstance(); // 获取数据库管理器单例
    };
} // namespace DAL