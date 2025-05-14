/**
 * @file individualUserService.cpp
 * @brief 个人用户服务实现文件，处理个人用户管理相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#include "individualUserService.h"
#include "../Database/databaseManager.h"
#include "../Utils/SecurityUtils.h"
#include <crow/json.h>

namespace Service {
    IndividualUserService::UserInfoResult IndividualUserService::getUserById(const std::string& userId) {
        auto& db = DAL::getDatabaseManager();
        auto query = db.prepareStatement(
            "SELECT UserID, Username, Email, Phone, Address FROM Users WHERE UserID = ?");
        query.bind(1, userId);
        
        if (!query.executeStep()) {
            return {false, "User not found", {}};
        }
        
        crow::json::wvalue userInfo;
        userInfo["userId"] = query.getColumn(0).getString();
        userInfo["username"] = query.getColumn(1).getString();
        userInfo["email"] = query.getColumn(2).getString();
        userInfo["phone"] = query.getColumn(3).getString();
        userInfo["address"] = query.getColumn(4).getString();
        
        return {true, "", userInfo};
    }
    
    IndividualUserService::UpdateResult IndividualUserService::updateUserInfo(
        const std::string& userId, 
        const crow::json::rvalue& userData) {
        
        auto& db = DAL::getDatabaseManager();
        auto query = db.prepareStatement(
            "UPDATE Users SET Email = ?, Phone = ?, Address = ? WHERE UserID = ?");
        query.bind(1, userData["email"].s());
        query.bind(2, userData["phone"].s());
        query.bind(3, userData["address"].s());
        query.bind(4, userId);
        
        if (!query.exec()) {
            return {false, "Update failed"};
        }
        
        return {true, "Update successful"};
    }
    
    IndividualUserService::UpdateResult IndividualUserService::changePassword(
        const std::string& userId, 
        const std::string& oldPassword, 
        const std::string& newPassword) {
        
        auto& db = DAL::getDatabaseManager();
        
        // 验证旧密码
        auto checkQuery = db.prepareStatement(
            "SELECT PasswordHash FROM Users WHERE UserID = ?");
        checkQuery.bind(1, userId);
        checkQuery.executeStep();
        
        std::string storedHash = checkQuery.getColumn(0).getString();
        if (!Utils::SecurityUtils::verifyPassword(oldPassword, storedHash)) {
            return {false, "Old password incorrect"};
        }
        
        // 更新密码
        std::string newHash = Utils::SecurityUtils::hashPassword(newPassword);
        auto updateQuery = db.prepareStatement(
            "UPDATE Users SET PasswordHash = ? WHERE UserID = ?");
        updateQuery.bind(1, newHash);
        updateQuery.bind(2, userId);
        
        if (!updateQuery.exec()) {
            return {false, "Password update failed"};
        }
        
        return {true, "Password updated successfully"};
    }
}