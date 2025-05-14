/**
 * @file individualUserDAO.cpp
 * @brief 个人用户数据访问对象类实现，封装IndividualUsers表的数据库操作
 * @author SDU-YueQiu
 * @date 2025/5/13
 * @version 1.0
 */

#include "individualUserDAO.h"
#include "../Model/individualUser.h"
#include "databaseManager.h"
#include <crow/logging.h>
#include <cstdint>
#include <ctime>
#include <stdexcept>


namespace DAL
{
    using namespace Model;

    IndividualUser createUserFromRow(DAL::DbRow &row)
    {
        try
        {
            // 提取时间字段并增加空值检查
            auto getTimeFromRow = [&](const std::string &key) -> time_t {
                auto strVal = std::get<std::string>(row.at(key));
                if (strVal.empty()) return 0;// 处理空值情况
                return static_cast<time_t>(std::stoll(strVal));
            };

            return IndividualUser{
                    std::get<int64_t>(row.at("UserID")),
                    std::get<std::string>(row.at("Username")),
                    std::get<std::string>(row.at("PasswordHash")),
                    std::get<std::string>(row.at("PhoneNumber")),
                    std::get<std::string>(row.at("Email")),
                    getTimeFromRow("RegistrationDate"),// 使用稳健转换函数
                    getTimeFromRow("LastLoginDate"),   // 使用稳健转换函数
                    std::get<std::string>(row.at("AccountStatus")),
                    std::get<std::string>(row.at("AvatarURL"))};
        } catch (const std::exception& e) {
            CROW_LOG_ERROR << "创建用户对象失败: " << e.what();
            return IndividualUser{};
        }
    }

    /**
     * @brief 根据用户ID查询个人用户信息
     * @param id 用户ID
     * @return IndividualUser 存在则返回用户对象，否则返回std::nullopt
     */
    IndividualUser IndividualUserDAO::findById(int id)
    {
        CROW_LOG_INFO << "Attempting to find user by ID: " << id;
        const std::string sql = "SELECT * FROM IndividualUsers WHERE userId = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(id)});

        if (!result || result->empty())
        {
            return IndividualUser{};
        }
        return createUserFromRow(result->front());
    }

    /**
     * @brief 根据用户名查询个人用户信息
     * @param username 用户名
     * @return IndividualUser 存在则返回用户对象，否则返回std::nullopt
     */
    IndividualUser IndividualUserDAO::findByUsername(const std::string &username)
    {
        CROW_LOG_INFO << "Attempting to find user by username: " << username;

        const std::string sql = "SELECT * FROM IndividualUsers WHERE username = ?";
        auto result = dbManager.executeQuery(sql, {username});

        if (!result || result->empty())
        {
            return IndividualUser{};
        }
        return createUserFromRow(result->front());
    }

    /**
     * @brief 根据邮箱查询个人用户信息
     * @param email 邮箱地址
     * @return IndividualUser 存在则返回用户对象，否则返回std::nullopt
     */
    IndividualUser IndividualUserDAO::findByEmail(const std::string &email)
    {
        CROW_LOG_INFO << "Attempting to find user by email: " << email;

        const std::string sql = "SELECT * FROM IndividualUsers WHERE email = ?";
        auto result = dbManager.executeQuery(sql, {email});

        if (!result || result->empty())
        {
            return IndividualUser{};
        }
        return createUserFromRow(result->front());
    }

    /**
     * @brief 插入新的个人用户记录
     * @param userData 待插入的用户对象（userId会被数据库自增生成，无需传入）
     * @return 插入成功返回true，否则返回false
     */
    bool IndividualUserDAO::create(const IndividualUser &userData)
    {
        CROW_LOG_INFO << "Attempting to create user with username: " << userData.username;

        const std::string sql = R"(
            INSERT INTO IndividualUsers (
                username, passwordHash, phoneNumber, email, 
                registrationDate, lastLoginDate, accountStatus, avatarURL
            ) VALUES (?, ?, ?, ?, ?, ?, ?, ?)
        )";

        std::vector<std::string> params = {
                userData.username,
                userData.passwordHash,
                userData.phoneNumber,
                userData.email,
                std::to_string(userData.registrationDate),
                std::to_string(userData.lastLoginDate),
                userData.accountStatus,
                userData.avatarURL};

        auto result = dbManager.executeQuery(sql, params);
        return !result->empty();
    }

    /**
     * @brief 更新指定ID的个人用户信息（全字段更新）
     * @param id 用户ID
     * @param userData 包含新信息的用户对象（需包含所有要更新的字段）
     * @return bool 更新成功返回true，否则返回false
     */
    bool IndividualUserDAO::update(int id, const IndividualUser &userData)
    {
        CROW_LOG_INFO << "Attempting to update user with ID: " << id;

        const std::string sql = R"(
            UPDATE IndividualUsers SET
                username = ?, passwordHash = ?, phoneNumber = ?, email = ?,
                registrationDate = ?, lastLoginDate = ?, accountStatus = ?, avatarURL = ?
            WHERE userId = ?
        )";

        std::vector<std::string> params = {
                userData.username,
                userData.passwordHash,
                userData.phoneNumber,
                userData.email,
                std::to_string(userData.registrationDate),
                std::to_string(userData.lastLoginDate),
                userData.accountStatus,
                userData.avatarURL,
                std::to_string(id)};

        auto result = dbManager.executeQuery(sql, params);
        return result != nullptr;
    }

    /**
     * @brief 更新指定ID的用户密码哈希
     * @param id 用户ID
     * @param passwordHash 新的密码哈希值
     * @return bool 更新成功返回true，否则返回false
     */
    bool IndividualUserDAO::updatePassword(int id, const std::string &passwordHash)
    {
        CROW_LOG_INFO << "Attempting to update password for user with ID: " << id;

        const std::string sql = "UPDATE IndividualUsers SET passwordHash = ? WHERE userId = ?";
        auto result = dbManager.executeQuery(sql, {passwordHash, std::to_string(id)});
        return result != nullptr;
    }

    /**
     * @brief 更新指定ID的用户账户状态
     * @param id 用户ID
     * @param status 新的账户状态（如"正常"/"冻结"）
     * @return bool 更新成功返回true，否则返回false
     */
    bool IndividualUserDAO::updateStatus(int id, const std::string &status)
    {
        CROW_LOG_INFO << "Attempting to update status for user with ID: " << id;

        const std::string sql = "UPDATE IndividualUsers SET accountStatus = ? WHERE userId = ?";
        auto result = dbManager.executeQuery(sql, {status, std::to_string(id)});
        return result != nullptr;
    }
}// namespace DAL