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

            // 优化：增加多行注释状态跟踪和注释内容过滤
            bool inMultiLineComment = false;
            // 逐行读取并执行SQL语句
            std::string line;
            std::string sqlStatement;
            while (std::getline(sqlFile, line)) {
                // 处理多行注释状态
                if (inMultiLineComment) {
                    size_t endComment = line.find("*/");
                    if (endComment != std::string::npos) {
                        inMultiLineComment = false;
                        line = line.substr(endComment + 2); // 保留注释结束后的内容
                    } else {
                        continue; // 仍在多行注释中，跳过当前行
                    }
                }

                // 移除单行注释（-- 之后的内容）
                size_t singleComment = line.find("--");
                if (singleComment != std::string::npos) {
                    line = line.substr(0, singleComment);
                }

                // 检查是否进入新的多行注释
                size_t startComment = line.find("/*");
                if (startComment != std::string::npos) {
                    inMultiLineComment = true;
                    line = line.substr(0, startComment); // 保留注释开始前的内容
                }

                // 去除行首尾空白字符
                line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](int ch) {
                    return !std::isspace(ch);
                }));
                line.erase(std::find_if(line.rbegin(), line.rend(), [](int ch) {
                    return !std::isspace(ch);
                }).base(), line.end());

                if (line.empty()) continue; // 处理后无有效内容则跳过

                sqlStatement += line;

                // 处理可能跨多行的完整SQL语句（支持分号在任意行结尾）
                size_t semicolonPos;
                while ((semicolonPos = sqlStatement.find(';')) != std::string::npos) {
                    std::string currentStmt = sqlStatement.substr(0, semicolonPos + 1);
                    dbManager.executeUpdate(currentStmt);
                    sqlStatement = sqlStatement.substr(semicolonPos + 1);
                }
            }

            // 提交事务
            dbManager.commitTransaction();

        } catch (const std::exception &e) {
            CROW_LOG_ERROR << "Database initialization failed: " << e.what();
        }
    }
}// namespace DAL