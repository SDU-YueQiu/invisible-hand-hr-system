/**
 * @file adminUserDAO.cpp
 * @brief 管理员用户数据访问对象类实现
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#include "adminUserDAO.h"
#include "../Model/adminUser.h"
#include "databaseManager.h"
#include <crow/logging.h>
#include <stdexcept>

namespace DAL {
    using namespace Model;

    AdminUser createAdminFromRow(DbRow &row) {
        try {
            return AdminUser{
                std::get<int64_t>(row.at("AdminID")),
                std::get<std::string>(row.at("Username")),
                std::get<std::string>(row.at("PasswordHash")),
                std::get<std::string>(row.at("Email")),
                std::get<std::string>(row.at("PhoneNumber")),
                std::get<std::string>(row.at("Role")),
                std::get<std::string>(row.at("AccountStatus")),
                std::get<std::string>(row.at("LastLoginTime")),
                std::get<std::string>(row.at("CreateTime"))
            };
        } catch (const std::exception& e) {
            CROW_LOG_ERROR << "创建管理员对象失败: " << e.what();
            return AdminUser{};
        }
    }

    AdminUser AdminUserDAO::findById(int64_t adminId) {
        CROW_LOG_INFO << "查询管理员信息，ID: " << adminId;
        const std::string sql = "SELECT * FROM AdminUsers WHERE AdminID = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(adminId)});

        if (!result || result->empty()) {
            return AdminUser{};
        }
        return createAdminFromRow(result->front());
    }

    AdminUser AdminUserDAO::findByUsername(const std::string& username) {
        CROW_LOG_INFO << "查询管理员信息，用户名: " << username;
        const std::string sql = "SELECT * FROM AdminUsers WHERE AdminUsername = ?";
        auto result = dbManager.executeQuery(sql, {username});

        if (!result || result->empty()) {
            return AdminUser{};
        }
        return createAdminFromRow(result->front());
    }

    bool AdminUserDAO::create(const AdminUser& adminData) {
        CROW_LOG_INFO << "创建新管理员账户，用户名: " << adminData.Username;
        const std::string sql = R"(
            INSERT INTO AdminUsers (
                Username, PasswordHash, Email, PhoneNumber,
                Role, AccountStatus, LastLoginTime, CreateTime
            ) VALUES (?, ?, ?, ?, ?, ?, ?, ?)
        )";

        std::vector<std::string> params = {
            adminData.Username,
            adminData.PasswordHash,
            adminData.Email,
            adminData.PhoneNumber,
            adminData.Role,
            adminData.AccountStatus,
            adminData.LastLoginTime,
            adminData.CreateTime
        };

        auto result = dbManager.executeQuery(sql, params);
        return !result->empty();
    }

    bool AdminUserDAO::update(int64_t adminId, const AdminUser& adminData) {
        CROW_LOG_INFO << "更新管理员信息，ID: " << adminId;
        const std::string sql = R"(
            UPDATE AdminUsers SET
                Username = ?, Email = ?, PhoneNumber = ?,
                Role = ?, AccountStatus = ?, LastLoginTime = ?
            WHERE AdminID = ?
        )";

        std::vector<std::string> params = {
            adminData.Username,
            adminData.Email,
            adminData.PhoneNumber,
            adminData.Role,
            adminData.AccountStatus,
            adminData.LastLoginTime,
            std::to_string(adminId)
        };

        auto result = dbManager.executeQuery(sql, params);
        return result != nullptr;
    }

    bool AdminUserDAO::updatePassword(int64_t adminId, const std::string& passwordHash) {
        CROW_LOG_INFO << "更新管理员密码，ID: " << adminId;
        const std::string sql = "UPDATE AdminUsers SET PasswordHash = ? WHERE AdminID = ?";
        auto result = dbManager.executeQuery(sql, {passwordHash, std::to_string(adminId)});
        return result != nullptr;
    }

    bool AdminUserDAO::updateStatus(int64_t adminId, const std::string& status) {
        CROW_LOG_INFO << "更新管理员状态，ID: " << adminId;
        const std::string sql = "UPDATE AdminUsers SET AccountStatus = ? WHERE AdminID = ?";
        auto result = dbManager.executeQuery(sql, {status, std::to_string(adminId)});
        return result != nullptr;
    }
}