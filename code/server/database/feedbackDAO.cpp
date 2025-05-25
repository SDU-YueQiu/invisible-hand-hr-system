/**
 * @file feedbackDAO.cpp
 * @brief 反馈数据访问对象类实现，封装Feedbacks表的数据库操作
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#include "feedbackDAO.h"
#include "../Model/feedback.h"
#include "databaseManager.h"
#include <crow/logging.h>
#include <stdexcept>

namespace DAL
{
    using namespace Model;

    Feedback createFromRow(DbRow &row)
    {
        try
        {
            return Feedback{
                    std::get<int64_t>(row.at("FeedbackID")),
                    std::get<int64_t>(row.at("UserID")),
                    std::get<std::string>(row.at("UserType")),
                    std::get<std::string>(row.at("FeedbackType")),
                    std::get<std::string>(row.at("Content")),
                    std::get<std::string>(row.at("ContactInfo")),
                    std::get<std::string>(row.at("ProcessStatus")),
                    std::get<std::string>(row.at("AdminReply")),
                    std::get<std::string>(row.at("SubmitTime"))};
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "创建反馈对象失败: " << e.what();
            return Feedback{};
        }
    }

    Feedback FeedbackDAO::findById(int64_t feedbackId)
    {
        CROW_LOG_INFO << "查询反馈信息，ID: " << feedbackId;
        const std::string sql = "SELECT * FROM Feedbacks WHERE FeedbackID = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(feedbackId)});

        if (!result || result->empty())
        {
            CROW_LOG_WARNING << "未找到反馈信息，ID: " << feedbackId;
            return Feedback{};
        }
        return createFromRow(result->front());
    }

    std::vector<Feedback> FeedbackDAO::findAll(const std::string &filter)
    {
        CROW_LOG_INFO << "查询反馈列表，筛选条件: " << (filter.empty() ? "无" : filter);
        std::string sql = "SELECT * FROM Feedbacks";
        if (!filter.empty())
        {
            sql += " WHERE " + filter;
        }

        auto result = dbManager.executeQuery(sql, {});
        std::vector<Feedback> feedbacks;
        if (!result) return feedbacks;

        for (auto &row: *result)
        {
            feedbacks.push_back(createFromRow(row));
        }
        return feedbacks;
    }

    bool FeedbackDAO::create(const Feedback &feedbackData)
    {
        CROW_LOG_INFO << "创建新反馈记录，用户ID: " << feedbackData.UserID;
        const std::string sql = R"(
            INSERT INTO Feedbacks (
                UserID, UserType, FeedbackType, Content,
                ContactInfo, ProcessStatus, AdminReply, SubmitTime
            ) VALUES (?, ?, ?, ?, ?, ?, ?, ?)
        )";

        std::vector<std::string> params = {
                std::to_string(feedbackData.UserID),
                feedbackData.UserType,
                feedbackData.FeedbackType,
                feedbackData.Content,
                feedbackData.ContactInfo,
                feedbackData.Status,
                feedbackData.AdminReply,
                feedbackData.CreateTime};

        auto result = dbManager.executeQuery(sql, params);
        return !result->empty();
    }

    bool FeedbackDAO::update(int64_t feedbackId, const Feedback &feedbackData)
    {
        CROW_LOG_INFO << "更新反馈信息，ID: " << feedbackId;
        const std::string sql = R"(
            UPDATE Feedbacks SET
                UserID = ?, UserType = ?, FeedbackType = ?, Content = ?,
                ContactInfo = ?, processStatus = ?, AdminReply = ?
            WHERE FeedbackID = ?
        )";

        std::vector<std::string> params = {
                std::to_string(feedbackData.UserID),
                feedbackData.UserType,
                feedbackData.FeedbackType,
                feedbackData.Content,
                feedbackData.ContactInfo,
                feedbackData.Status,
                feedbackData.AdminReply,
                std::to_string(feedbackId)};

        auto result = dbManager.executeQuery(sql, params);
        return result != nullptr;
    }
}// namespace DAL