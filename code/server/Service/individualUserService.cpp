/**
 * @file individualUserService.cpp
 * @brief 个人用户服务类实现，处理个人用户相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.1
 */

#include "individualUserService.h"
#include <crow/logging.h>
#include "../Utils/securityUtils.h"

namespace Service
{
    Model::IndividualUser IndividualUserService::getUserById(int userId)
    {
        CROW_LOG_INFO << "Getting user by ID: " << userId;

        // 通过DAO层获取用户信息
        auto user = userDAO.findById(userId);

        if (user.UserID == -1)
        {
            CROW_LOG_WARNING << "User not found with ID: " << userId;
        }

        return user;
    }

    bool IndividualUserService::updateUserInfo(int userId, const Model::IndividualUser &userData)
    {
        CROW_LOG_INFO << "Updating user info for ID: " << userId;

        // 验证用户ID是否匹配
        if (userData.UserID != userId)
        {
            CROW_LOG_WARNING << "User ID mismatch in update request";
            return false;
        }

        // 获取现有用户信息
        auto existingUser = userDAO.findById(userId);
        if (existingUser.UserID == -1)
        {
            CROW_LOG_WARNING << "User not found for update, ID: " << userId;
            return false;
        }

        // 只更新非默认值的字段
        if (!userData.PhoneNumber.empty()) existingUser.PhoneNumber = userData.PhoneNumber;
        if (!userData.Email.empty()) existingUser.Email = userData.Email;
        if (!userData.AccountStatus.empty()) existingUser.AccountStatus = userData.AccountStatus;
        if (!userData.AvatarURL.empty()) existingUser.AvatarURL = userData.AvatarURL;
        if (userData.LastLoginDate != 0) existingUser.LastLoginDate = userData.LastLoginDate;

        // 通过DAO层更新用户信息
        return userDAO.update(userId, existingUser);
    }

    bool IndividualUserService::changePassword(int userId, const std::string &oldPassword, const std::string &newPassword)
    {
        CROW_LOG_INFO << "Changing password for user ID: " << userId;

        // 获取用户信息
        auto user = userDAO.findById(userId);
        if (user.UserID == -1)
        {
            CROW_LOG_WARNING << "User not found with ID: " << userId;
            return false;
        }

        auto oldPasswordHash = Utils::SecurityUtils::hashPassword(oldPassword);

        // 验证旧密码是否正确
        if (user.PasswordHash != oldPasswordHash)
        {
            CROW_LOG_WARNING << "Old password verification failed for user ID: " << userId;
            return false;
        }

        // 通过DAO层更新密码
        return userDAO.updatePassword(userId, Utils::SecurityUtils::hashPassword(newPassword));
    }
}// namespace Service