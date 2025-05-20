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

namespace DAL
{
    using namespace Model;

    AdminUser createAdminFromRow(DbRow &row)
    {
        try
        {
            return AdminUser{
                    std::get<int64_t>(row.at("AdminID")),
                    std::get<std::string>(row.at("AdminUsername")),
                    std::get<std::string>(row.at("PasswordHash")),
                    std::get<std::string>(row.at("Role"))};
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "创建管理员对象失败: " << e.what();
            return AdminUser{};
        }
    }

    // AdminUser AdminUserDAO::findById(int64_t adminId)
    // {
    //     CROW_LOG_INFO << "查询管理员信息，ID: " << adminId;
    //     const std::string sql = "SELECT * FROM AdminUsers WHERE AdminID = ?";
    //     auto result = dbManager.executeQuery(sql, {std::to_string(adminId)});

    //     if (!result || result->empty())
    //     {
    //         return AdminUser{};
    //     }
    //     return createAdminFromRow(result->front());
    // }

    AdminUser AdminUserDAO::findByUsername(const std::string &username)
    {
        CROW_LOG_INFO << "查询管理员信息，用户名: " << username;
        const std::string sql = "SELECT * FROM AdminUsers WHERE AdminUsername = ?";
        auto result = dbManager.executeQuery(sql, {username});

        if (!result || result->empty())
        {
            return AdminUser{};
        }
        return createAdminFromRow(result->front());
    }

}// namespace DAL