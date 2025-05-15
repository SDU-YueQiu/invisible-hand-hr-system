/**
 * @file feedbackService.h
 * @brief 反馈服务类，处理用户反馈相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#pragma once

#include "../Model/feedback.h"
#include "../Database/feedbackDAO.h"
#include <vector>
#include <string>

namespace Service
{
    /**
     * @brief 反馈服务类，封装反馈相关业务逻辑
     */
    class FeedbackService
    {
    public:
        /**
         * @brief 获取FeedbackService单例实例（线程安全）
         * @return FeedbackService& FeedbackService实例
         */
        static FeedbackService& getInstance()
        {
            thread_local static FeedbackService instance;
            return instance;
        }

        /**
         * @brief 提交用户反馈
         * @param feedbackData 反馈数据
         * @return bool 提交成功返回true，否则返回false
         */
        bool submitFeedback(const Model::Feedback& feedbackData);

        /**
         * @brief 获取反馈列表
         * @param filter 筛选条件字符串
         * @return std::vector<Model::Feedback> 反馈列表
         */
        std::vector<Model::Feedback> getFeedbacks(const std::string& filter = "");

        /**
         * @brief 处理反馈（管理员操作）
         * @param feedbackId 反馈ID
         * @param reply 处理回复内容
         * @return bool 处理成功返回true，否则返回false
         */
        bool processFeedback(int64_t feedbackId, const std::string& reply);

    private:
        DAL::FeedbackDAO& feedbackDAO = DAL::FeedbackDAO::getInstance();
    };
} // namespace Service