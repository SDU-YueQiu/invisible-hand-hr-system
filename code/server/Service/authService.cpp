/**
 * @file authService.cpp
 * @brief 认证服务实现文件，处理用户认证和授权逻辑
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#include "authService.h"
#include "../Utils/SecurityUtils.h"
#include "../Database/databaseManager.h"
#include <crow/json.h>

namespace Service {
    AuthService::AuthResult AuthService::login(const std::string& username, const std::string& password) {
        auto& db = DAL::getDatabaseManager();
        auto query = db.prepareStatement("SELECT PasswordHash FROM Users WHERE Username = ?");
        query.bind(1, username);
        
        if (!query.executeStep()) {
            return {false, "Invalid username or password", ""};
        }
        
        std::string storedHash = query.getColumn(0).getString();
        if (!Utils::SecurityUtils::verifyPassword(password, storedHash)) {
            return {false, "Invalid username or password", ""};
        }
        
        std::string token = Utils::SecurityUtils::generateJWT(username);
        return {true, "Login successful", token};
    }
    
    AuthService::AuthResult AuthService::registerUser(const std::string& username, const std::string& password, const std::string& email) {
        auto& db = DAL::getDatabaseManager();
        
        // 检查用户名是否已存在
        auto checkQuery = db.prepareStatement("SELECT COUNT(*) FROM Users WHERE Username = ?");
        checkQuery.bind(1, username);
        checkQuery.executeStep();
        
        if (checkQuery.getColumn(0).getInt() > 0) {
            return {false, "Username already exists", ""};
        }
        
        // 创建新用户
        std::string passwordHash = Utils::SecurityUtils::hashPassword(password);
        auto insertQuery = db.prepareStatement(
            "INSERT INTO Users (Username, PasswordHash, Email) VALUES (?, ?, ?)");
        insertQuery.bind(1, username);
        insertQuery.bind(2, passwordHash);
        insertQuery.bind(3, email);
        
        if (!insertQuery.exec()) {
            return {false, "Registration failed", ""};
        }
        
        std::string token = Utils::SecurityUtils::generateJWT(username);
        return {true, "Registration successful", token};
    }
}