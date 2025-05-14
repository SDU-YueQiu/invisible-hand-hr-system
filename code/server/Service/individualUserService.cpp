/**
 * @file individualUserService.cpp
 * @brief 个人用户服务类实现，处理个人用户相关的业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#include "individualUserService.h"
#include <crow/logging.h>
#include "../Utils/securityUtils.h"

namespace Service
{
    crow::json::wvalue IndividualUserService::getUserById(int userId)
    {
        CROW_LOG_INFO << "Getting user info for ID: " << userId;
        
        auto user = userDao.findById(userId);
        if (user.UserID == -1)
        {
            return crow::json::wvalue{
                {"success", false},
                {"message", "用户不存在"}
            };
        }

        return crow::json::wvalue{
            {"success", true},
            {"data", {
                {"userId", user.UserID},
                {"username", user.Username},
                {"phoneNumber", user.PhoneNumber},
                {"email", user.Email},
                {"registrationDate", user.RegistrationDate},
                {"lastLoginDate", user.LastLoginDate},
                {"accountStatus", user.AccountStatus},
                {"avatarURL", user.AvatarURL}
            }},
            {"message", "获取用户信息成功"}
        };
    }

    crow::json::wvalue IndividualUserService::updateUserInfo(int userId, const crow::json::rvalue& userData)
    {
        CROW_LOG_INFO << "Updating user info for ID: " << userId;
        
        auto user = userDao.findById(userId);
        if (user.UserID == -1)
        {
            return crow::json::wvalue{
                {"success", false},
                {"message", "用户不存在"}
            };
        }

        // 更新可修改字段
        if (userData.has("phoneNumber")) 
            user.PhoneNumber = userData["phoneNumber"].s();
        if (userData.has("email"))
            user.Email = userData["email"].s();
        if (userData.has("avatarURL"))
            user.AvatarURL = userData["avatarURL"].s();

        bool success = userDao.update(userId, user);
        return crow::json::wvalue{
            {"success", success},
            {"message", success ? "更新成功" : "更新失败"}
        };
    }

    crow::json::wvalue IndividualUserService::changePassword(int userId, 
        const std::string& oldPassword, const std::string& newPassword)
    {
        CROW_LOG_INFO << "Changing password for user ID: " << userId;
        
        auto user = userDao.findById(userId);
        if (user.UserID == -1)
        {
            return crow::json::wvalue{
                {"success", false},
                {"message", "用户不存在"}
            };
        }

        // 验证旧密码
        if (!Utils::SecurityUtils::verifyPassword(oldPassword, user.PasswordHash))
        {
            return crow::json::wvalue{
                {"success", false},
                {"message", "旧密码不正确"}
            };
        }

        // 更新为新密码
        std::string newHash = Utils::SecurityUtils::hashPassword(newPassword);
        bool success = userDao.updatePassword(userId, newHash);
        
        return crow::json::wvalue{
            {"success", success},
            {"message", success ? "密码修改成功" : "密码修改失败"}
        };
    }
}