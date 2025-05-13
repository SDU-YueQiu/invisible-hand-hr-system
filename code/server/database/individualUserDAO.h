/**
 * @file individualUserDAO.h
 * @brief 个人用户数据访问对象类，封装IndividualUsers表的数据库操作
 * @author SDU-YueQiu
 * @date 2025/5/12
 * @version 1.0
 */

#pragma once

#include "../Model/individualUser.h"
#include "databaseManager.h"
#include <optional>
#include <vector>

namespace DAL {
    /**
     * @brief 个人用户数据访问对象类
     */
    class IndividualUserDAO {
    public:
        /**
         * @brief 根据用户ID查询个人用户信息
         * @param id 用户ID
         * @return std::optional<Model::IndividualUser> 存在则返回用户对象，否则返回std::nullopt
         */
        std::optional<Model::IndividualUser> findById(int id);

        /**
         * @brief 根据用户名查询个人用户信息
         * @param username 用户名
         * @return std::optional<Model::IndividualUser> 存在则返回用户对象，否则返回std::nullopt
         */
        std::optional<Model::IndividualUser> findByUsername(const std::string& username);

        /**
         * @brief 根据邮箱查询个人用户信息
         * @param email 邮箱地址
         * @return std::optional<Model::IndividualUser> 存在则返回用户对象，否则返回std::nullopt
         */
        std::optional<Model::IndividualUser> findByEmail(const std::string& email);

        /**
         * @brief 插入新的个人用户记录
         * @param userData 待插入的用户对象
         * @return bool 插入成功返回true，否则返回false
         */
        int create(const Model::IndividualUser& userData);

        /**
         * @brief 更新指定ID的个人用户信息
         * @param id 用户ID
         * @param userData 包含新信息的用户对象（需包含所有要更新的字段）
         * @return bool 更新成功返回true，否则返回false
         */
        bool update(int id, const Model::IndividualUser& userData);

        /**
         * @brief 更新指定ID的用户密码哈希
         * @param id 用户ID
         * @param passwordHash 新的密码哈希值
         * @return bool 更新成功返回true，否则返回false
         */
        bool updatePassword(int id, const std::string& passwordHash);

        /**
         * @brief 更新指定ID的用户账户状态
         * @param id 用户ID
         * @param status 新的账户状态（如"正常"/"冻结"）
         * @return bool 更新成功返回true，否则返回false
         */
        bool updateStatus(int id, const std::string& status);

    private:
        DatabaseManager& dbManager = DatabaseManager::getInstance(); // 获取数据库管理器单例
    };
} // namespace DAL
