/**
 * @file individualUser.h
 * @brief 个人用户模型类，对应数据库IndividualUsers表
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
     * @brief 个人用户模型类
     */
    class IndividualUser
    {
    public:
        // 构造函数
        IndividualUser() = default;
        IndividualUser(int64_t userId, const std::string &username, const std::string &passwordHash,
                       const std::string &phoneNumber, const std::string &email, time_t registrationDate = 0,
                       time_t lastLoginDate = 0, const std::string &accountStatus = "", const std::string &avatarURL = "")
            : userId(userId), username(username), passwordHash(passwordHash), phoneNumber(phoneNumber),
              email(email), registrationDate(registrationDate), lastLoginDate(lastLoginDate),
              accountStatus(accountStatus), avatarURL(avatarURL) {}

        // Getter和Setter方法
        int getUserId() const { return userId; }
        void setUserId(int userId) { this->userId = userId; }

        std::string getUsername() const { return username; }
        void setUsername(const std::string &username) { this->username = username; }

        std::string getPasswordHash() const { return passwordHash; }
        void setPasswordHash(const std::string &passwordHash) { this->passwordHash = passwordHash; }

        std::string getPhoneNumber() const { return phoneNumber; }
        void setPhoneNumber(const std::string &phoneNumber) { this->phoneNumber = phoneNumber; }

        std::string getEmail() const { return email; }
        void setEmail(const std::string &email) { this->email = email; }

        time_t getRegistrationDate() const { return registrationDate; }
        void setRegistrationDate(time_t registrationDate) { this->registrationDate = registrationDate; }

        time_t getLastLoginDate() const { return lastLoginDate; }
        void setLastLoginDate(time_t lastLoginDate) { this->lastLoginDate = lastLoginDate; }

        std::string getAccountStatus() const { return accountStatus; }
        void setAccountStatus(const std::string &accountStatus) { this->accountStatus = accountStatus; }

        std::string getAvatarURL() const { return avatarURL; }
        void setAvatarURL(const std::string &avatarURL) { this->avatarURL = avatarURL; }

    private:
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