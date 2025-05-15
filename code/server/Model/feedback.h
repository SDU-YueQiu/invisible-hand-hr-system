/**
 * @file feedback.h
 * @brief 反馈/留言模型类，定义反馈数据结构
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#pragma once

#include <cstdint>
#include <string>
#include <ctime>

namespace Model
{
    /**
     * @brief 反馈数据模型，对应数据库Feedbacks表
     */
    struct Feedback
    {
        int64_t FeedbackID = -1;       ///< 反馈唯一标识（主键，自增）
        int64_t UserID = -1;           ///< 提交用户ID（外键到IndividualUsers/EnterpriseUsers，匿名用户为-1）
        std::string UserType;          ///< 用户类型（"Individual"/"Enterprise"/"Anonymous"）
        std::string FeedbackType;      ///< 反馈类型（"Suggestion"/"BugReport"/"Complaint"等）
        std::string Content;           ///< 反馈内容
        std::string ContactInfo;       ///< 联系方式（可选）
        std::string Status = "Pending";///< 处理状态（"Pending"/"Processing"/"Resolved"）
        std::string AdminReply;        ///< 管理员回复（可选）
        std::string CreateTime;        ///< 创建时间（默认当前时间）
        std::string UpdateTime;        ///< 最后更新时间
    };
} // namespace Model