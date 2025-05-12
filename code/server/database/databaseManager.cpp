/**
 * @file DatabaseManager.cpp
 * @brief 类DatabaseManager的实现
 * @author SDU-YueQiu
 * @date 2025/5/12
 * @version 1.0
*/

#include "databaseManager.h"
#include "../Config/config.h"
#include <fstream>
#include <stdexcept>

namespace DAL
{
    DatabaseManager::DatabaseManager(const std::string dbPath)
    {
        // std::filesystem::path path(dbPath);
        // std::filesystem::create_directories(path.parent_path());
        db = std::make_unique<SQLite::Database>(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    }

    DatabaseManager::~DatabaseManager()
    {
        try {
            rollbackTransaction();
        } catch (...) {
            // 忽略异常
        }
    }

    std::unique_ptr<SQLite::Statement> DatabaseManager::executeQuery(const std::string &sql)
    {
        try {
            return std::make_unique<SQLite::Statement>(*db, sql);
        } catch (const std::exception &e) {
            throw std::runtime_error("Query execution failed: " + std::string(e.what()));
        }
    }

    int DatabaseManager::executeUpdate(const std::string &sql)
    {
        try {
            return db->exec(sql);
        } catch (const std::exception &e) {
            throw std::runtime_error("Update execution failed: " + std::string(e.what()));
        }
    }

    int DatabaseManager::executeWithParams(const std::string &sql, const std::vector<std::string> &params)
    {
        try {
            SQLite::Statement query(*db, sql);
            for (size_t i = 0; i < params.size(); ++i) {
                query.bind(i + 1, params[i]);
            }
            return query.exec();
        } catch (const std::exception &e) {
            throw std::runtime_error("Parameterized query failed: " + std::string(e.what()));
        }
    }

    bool DatabaseManager::beginTransaction()
    {
        if (inTransaction)
            return false;// 已经在事务中

        transaction = std::make_unique<SQLite::Transaction>(*db);
        inTransaction = true;
        return true;
    }

    bool DatabaseManager::commitTransaction()
    {
        if (!inTransaction)
            return false;// 没有事务

        transaction->commit();
        transaction.reset();
        inTransaction = false;
        return true;
    }

    bool DatabaseManager::rollbackTransaction()
    {
        if (!inTransaction)
            return false;// 没有事务

        transaction->rollback();
        transaction.reset();
        inTransaction = false;
        return true;
    }

    DatabaseManager &getDatabaseManager()
    {
        CROW_LOG_INFO << "Getting database manager: " << Config::globalConfig["DB_PATH"].s();

        // 线程独立存储
        thread_local static DatabaseManager dbManager(Config::globalConfig["DB_PATH"].s());
        return dbManager;
    }

    void initDatabase()
    {
        try {
            // 获取数据库管理器实例
            DatabaseManager &dbManager = getDatabaseManager();

            auto query = dbManager.executeQuery("SELECT COUNT(*) FROM sqlite_master WHERE type = 'table'");

            // 执行查询并获取结果
            if (query->executeStep()) {
                int tableCount = query->getColumn(0).getInt();
                if (tableCount > 0) 
                {
                    CROW_LOG_INFO << "Database isn't empty.";
                    return;
                }
            }

            CROW_LOG_INFO << "Initializing database...";
            // 打开并读取DatabaseInit.sql文件
            std::ifstream sqlFile(Config::globalConfig["DB_INIT_PATH"].s());
            if (!sqlFile.is_open()) {
                throw std::runtime_error("Failed to open DatabaseInit.sql");
            }

            // 开始事务
            dbManager.beginTransaction();

            // 逐行读取并执行SQL语句
            std::string line;
            std::string sqlStatement;
            while (std::getline(sqlFile, line)) {
                // 跳过注释和空行
                if (line.empty() || line.find("--") == 0 || line.find("/*") == 0)
                    continue;

                sqlStatement += line;

                // 如果行以分号结尾，表示一个完整的SQL语句
                if (line.find(';') != std::string::npos) {
                    dbManager.executeUpdate(sqlStatement);
                    sqlStatement.clear();
                }
            }

            // 提交事务
            dbManager.commitTransaction();

        } catch (const std::exception &e) {
            CROW_LOG_ERROR << "Database initialization failed: " << e.what();
        }
    }
}// namespace DAL