/**
 * @file authService.h
 * @brief 认证服务头文件，定义用户认证相关服务接口
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include <string>
#include "../Model/authResult.h"

namespace Service {
    class AuthService {
    public:
        struct LoginResult {
            bool success;
            std::string token;
            std::string message;
        };
        
        struct RegisterResult {
            bool success;
            std::string token;
            std::string message;
        };
        
        static LoginResult login(const std::string& username, const std::string& password);
        static RegisterResult registerUser(const std::string& username, const std::string& password, const std::string& email);
    };
}