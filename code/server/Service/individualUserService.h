/**
 * @file individualUserService.h
 * @brief 个人用户服务头文件，处理个人用户相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include <string>
#include <crow/json.h>

namespace Service {
    class IndividualUserService {
    public:
        struct RegisterResult {
            bool success;
            std::string message;
            std::string userId;
        };
        
        struct LoginResult {
            bool success;
            std::string message;
            std::string token;
        };
        
        struct UpdateResult {
            bool success;
            std::string message;
        };
        
        static RegisterResult registerUser(const crow::json::rvalue& userData);
        static LoginResult loginUser(const std::string& username, const std::string& password);
        static UpdateResult updateUserInfo(const std::string& userId, const crow::json::rvalue& userData);
        static UpdateResult resetPassword(const std::string& userId, const std::string& oldPassword, const std::string& newPassword);
    };
}