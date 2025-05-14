/**
 * @file adminUserDAO.h
 * @brief 管理员用户数据访问对象类，封装AdminUsers表的数据库操作
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include "../Model/adminUser.h"
#include "databaseManager.h"
#include <crow/logging.h>

namespace DAL {
    /**
     * @brief 管理员用户数据访问对象类
     */
    class AdminUserDAO {
    public:
        /**
         * @brief 根据管理员ID查询管理员信息
         * @param adminId 管理员ID
         * @return Model::AdminUser 存在则返回管理员对象，否则返回std::nullopt
         */
        Model::AdminUser findById(int64_t adminId);

        /**
         * @brief 根据用户名查询管理员信息
         * @param username 用户名
         * @return Model::AdminUser 存在则返回管理员对象，否则返回std::nullopt
         */
        Model::AdminUser findByUsername(const std::string& username);

        /**
         * @brief 创建新的管理员账户
         * @param adminData 管理员数据
         * @return bool 创建成功返回true，否则返回false
         */
        bool create(const Model::AdminUser& adminData);

        /**
         * @brief 更新管理员信息
         * @param adminId 管理员ID
         * @param adminData 更新后的管理员数据
         * @return bool 更新成功返回true，否则返回false
         */
        bool update(int64_t adminId, const Model::AdminUser& adminData);

        /**
         * @brief 更新管理员密码
         * @param adminId 管理员ID
         * @param passwordHash 新密码哈希
         * @return bool 更新成功返回true，否则返回false
         */
        bool updatePassword(int64_t adminId, const std::string& passwordHash);

        /**
         * @brief 更新管理员账户状态
         * @param adminId 管理员ID
         * @param status 新状态
         * @return bool 更新成功返回true，否则返回false
         */
        bool updateStatus(int64_t adminId, const std::string& status);

    private:
        DatabaseManager& dbManager = DatabaseManager::getInstance(); // 获取数据库管理器单例
    };
}