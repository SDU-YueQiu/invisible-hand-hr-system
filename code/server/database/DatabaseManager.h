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
#include <variant>
#include <vector>


namespace DAL
{
    using DbValue = std::variant<std::nullptr_t, int, long long, double, std::string, std::vector<unsigned char>>;
    using DbRow = std::map<std::string, DbValue>;
    using ResultSet = std::vector<DbRow>;

    /**
     * @brief 类DatabaseManager，用于链接和操作数据库。
     * @details 该类提供了执行查询和更新SQL语句的方法，以及管理事务的功能，是系统与sqlite数据库或者说其封装SQLiteCpp的唯一接口。
     */
    class DatabaseManager
    {
    public:
        DatabaseManager(const std::string dbPath);
        ~DatabaseManager();

        static DatabaseManager &getInstance();

        /**
         * @brief 执行带参数的SQL，返回结果集
         */
        std::unique_ptr<ResultSet> executeQuery(const std::string &sql, const std::vector<std::string> &params = {});

        /**
         * @brief 开始事务
         */
        bool beginTransaction();

        /**
         * @brief 执行添加事务SQL
         */
        void transactionAddQuery(const std::string &sql);

        /**
         * @brief 提交事务
         */
        bool commitTransaction();

        /**
         * @brief 回滚事务
         */
        bool rollbackTransaction();

    private:
        std::unique_ptr<SQLite::Database> db;
        bool inTransaction = false;
        std::unique_ptr<SQLite::Transaction> transaction;
    };

    /**
     * @brief 初始化数据库配置，创建表。
     */
    void initDatabase();
}// namespace DAL