/**
 * @file DatabaseManager.h
 * @brief 类DatabaseManager，用于链接和操作数据库。
 * @author SDU-YueQiu
 * @date 2025/5/12
 * @version 1.0
*/

#pragma once
#include "SQLiteCpp/SQLiteCpp.h"
#include <memory>
#include <string>
#include <vector>


namespace DAL
{

    /**
     * @brief 类DatabaseManager，用于链接和操作数据库。
     * @details 该类提供了执行查询和更新SQL语句的方法，以及管理事务的功能，是系统与sqlite数据库的唯一接口。
     */
    class DatabaseManager
    {
    public:
        DatabaseManager(const std::string dbPath);
        ~DatabaseManager();

        // 执行查询SQL，返回结果集
        std::unique_ptr<SQLite::Statement> executeQuery(const std::string &sql);

        // 执行更新SQL，返回影响行数
        int executeUpdate(const std::string &sql);

        // 执行带参数的SQL
        int executeWithParams(const std::string &sql, const std::vector<std::string> &params);

        // 开始事务
        bool beginTransaction();

        // 提交事务
        bool commitTransaction();

        // 回滚事务
        bool rollbackTransaction();

    private:
        std::unique_ptr<SQLite::Database> db;
        bool inTransaction = false;
        std::unique_ptr<SQLite::Transaction> transaction;
    };

    /**
     * @brief 获取数据库管理器实例，线程独立。
     * @return DatabaseManager& 数据库管理器实例
     */
    DatabaseManager &getDatabaseManager();

    /**
     * @brief 初始化数据库配置，创建表。
     */
    void initDatabase();
}// namespace DAL