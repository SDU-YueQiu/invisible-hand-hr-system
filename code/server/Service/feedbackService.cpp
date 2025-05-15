/**
 * @file feedbackService.cpp
 * @brief 反馈服务类实现，处理用户反馈相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#include "feedbackService.h"
#include <crow/logging.h>
#include <ctime>

namespace Service
{
    bool FeedbackService::submitFeedback(const Model::Feedback &feedbackData)
    {
        CROW_LOG_INFO << "Submitting feedback from user ID: " << feedbackData.UserID;

        // 验证必要字段
        if (feedbackData.FeedbackType.empty() || feedbackData.Content.empty())
        {
            CROW_LOG_WARNING << "Invalid feedback data - missing required fields";
            return false;
        }

        // 设置默认状态和时间戳
        Model::Feedback feedbackToSave = feedbackData;
        if (feedbackToSave.Status.empty())
        {
            feedbackToSave.Status = "Pending";
        }

        std::time_t now = std::time(nullptr);
        feedbackToSave.CreateTime = std::to_string(now);
        feedbackToSave.UpdateTime = std::to_string(now);

        return feedbackDAO.create(feedbackToSave);
    }

    std::vector<Model::Feedback> FeedbackService::getFeedbacks(const std::string &filter)
    {
        CROW_LOG_INFO << "Getting feedbacks with filter: " << (filter.empty() ? "none" : filter);
        return feedbackDAO.findAll(filter);
    }

    bool FeedbackService::processFeedback(int64_t feedbackId, const std::string &reply)
    {
        CROW_LOG_INFO << "Processing feedback ID: " << feedbackId;

        // 获取现有反馈记录
        auto feedback = feedbackDAO.findById(feedbackId);
        if (feedback.FeedbackID == -1)
        {
            CROW_LOG_WARNING << "Feedback not found for processing, ID: " << feedbackId;
            return false;
        }

        // 更新状态和回复
        feedback.Status = "Resolved";
        feedback.AdminReply = reply;
        feedback.UpdateTime = std::to_string(std::time(nullptr));

        return feedbackDAO.update(feedbackId, feedback);
    }
}// namespace Service