/**
 * @file individualUser.h
 * @brief 个人用户模型，对应数据库IndividualUsers表
 * @author SDU-YueQiu
 * @date 2025/5/13
 * @version 1.0
 */

#pragma once

#include <ctime>
#include <string>

namespace Model
{
    /**
     * @brief 个人用户模型
     */
    struct IndividualUser
    {
        int64_t userId;           // 用户ID（主键）
        std::string username;     // 用户名
        std::string passwordHash; // 密码哈希
        std::string phoneNumber;  // 联系电话
        std::string email;        // 邮箱
        time_t registrationDate;  // 注册时间
        time_t lastLoginDate;     // 最后登录时间
        std::string accountStatus;// 账户状态（如"正常"/"冻结"）
        std::string avatarURL;    // 头像URL
    };
}// namespace Model