/**
 * @file DatabaseManager.cpp
 * @brief 类DatabaseManager的实现
 * @author SDU-YueQiu
 * @date 2025/5/12
 * @version 1.0
*/

#include "databaseManager.h"
#include "../Config/config.h"
#include "sqlite3.h"
#include <SQLiteCpp/Column.h>
#include <fstream>
#include <stdexcept>


namespace DAL
{
    DatabaseManager &getDatabaseManager()
    {
        CROW_LOG_INFO << "Getting database manager: " << Config::globalConfig["DB_PATH"].s();

        // 线程独立存储
        thread_local static DatabaseManager dbManager(Config::globalConfig["DB_PATH"].s());
        return dbManager;
    }

    DatabaseManager::DatabaseManager(const std::string dbPath)
    {
        // std::filesystem::path path(dbPath);
        // std::filesystem::create_directories(path.parent_path());
        db = std::make_unique<SQLite::Database>(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    }

    DatabaseManager::~DatabaseManager()
    {
        try
        {
            rollbackTransaction();
        } catch (...)
        {
            // 忽略异常
        }
    }

    DatabaseManager &DatabaseManager::getInstance()
    {
        return getDatabaseManager();
    }

    void DatabaseManager::transactionAddQuery(const std::string &sql)
    {
        try
        {
            db->exec(sql);
        } catch (const std::exception &e)
        {
            throw std::runtime_error("Update execution failed: " + std::string(e.what()));
        }
    }

    std::unique_ptr<ResultSet> DatabaseManager::executeQuery(const std::string &sql, const std::vector<std::string> &params)
    {
        SQLite::Statement query(*db, sql);
        for (size_t i = 0; i < params.size(); ++i)
            query.bind(i + 1, params[i]);
        auto resultSet = std::make_unique<ResultSet>();
        try
        {
            while (query.executeStep())
            {// 逐行遍历结果
                DbRow currentRow;
                for (int i = 0; i < query.getColumnCount(); ++i)
                {// 处理当前行的所有列
                    SQLite::Column col = query.getColumn(i);
                    std::string colName = col.getName();

                    switch (col.getType())
                    {
                        case SQLITE_INTEGER:
                            currentRow[colName] = col.getInt64();
                            break;
                        case SQLITE_FLOAT:
                            currentRow[colName] = col.getDouble();
                            break;
                        case SQLITE_TEXT:
                            currentRow[colName] = col.getString();
                            break;
                        case SQLITE_BLOB: {
                            const void *blobData = col.getBlob();
                            int blobSize = col.getBytes();
                            std::vector<unsigned char> blob(
                                    static_cast<const unsigned char *>(blobData),
                                    static_cast<const unsigned char *>(blobData) + blobSize);
                            currentRow[colName] = blob;
                            break;
                        }
                        case SQLITE_NULL:
                            currentRow[colName] = nullptr;
                            break;
                        default:
                            currentRow[colName] = col.getText();
                            break;
                    }
                }
                resultSet->push_back(currentRow);// 添加当前行到结果集
            }
        } catch (const std::exception &e)
        {
            throw std::runtime_error("Parameterized query failed: " + std::string(e.what()));
        }
        return resultSet;
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


    void initDatabase()
    {
        try
        {
            // 获取数据库管理器实例
            DatabaseManager &dbManager = getDatabaseManager();

            auto query = dbManager.executeQuery("SELECT 1 FROM sqlite_master WHERE type = 'table'");

            // 执行查询并获取结果
            if (!query->empty())
            {
                CROW_LOG_INFO << "Database isn't empty.";
                return;
            }

            CROW_LOG_INFO << "Initializing database...";
            // 打开并读取DatabaseInit.sql文件
            std::ifstream sqlFile(Config::globalConfig["DB_INIT_PATH"].s());
            if (!sqlFile.is_open())
            {
                throw std::runtime_error("Failed to open DatabaseInit.sql");
            }

            // 开始事务
            dbManager.beginTransaction();

            // 优化：增加多行注释状态跟踪和注释内容过滤
            bool inMultiLineComment = false;
            // 逐行读取并执行SQL语句
            std::string line;
            std::string sqlStatement;
            while (std::getline(sqlFile, line))
            {
                // 处理多行注释状态
                if (inMultiLineComment)
                {
                    size_t endComment = line.find("*/");
                    if (endComment != std::string::npos)
                    {
                        inMultiLineComment = false;
                        line = line.substr(endComment + 2);// 保留注释结束后的内容
                    } else
                    {
                        continue;// 仍在多行注释中，跳过当前行
                    }
                }

                // 移除单行注释（-- 之后的内容）
                size_t singleComment = line.find("--");
                if (singleComment != std::string::npos)
                {
                    line = line.substr(0, singleComment);
                }

                // 检查是否进入新的多行注释
                size_t startComment = line.find("/*");
                if (startComment != std::string::npos)
                {
                    inMultiLineComment = true;
                    line = line.substr(0, startComment);// 保留注释开始前的内容
                }

                // 去除行首尾空白字符
                line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](int ch) {
                               return !std::isspace(ch);
                           }));
                line.erase(std::find_if(line.rbegin(), line.rend(), [](int ch) {
                               return !std::isspace(ch);
                           }).base(),
                           line.end());

                if (line.empty()) continue;// 处理后无有效内容则跳过

                sqlStatement += line;

                // 处理可能跨多行的完整SQL语句（支持分号在任意行结尾）
                size_t semicolonPos;
                while ((semicolonPos = sqlStatement.find(';')) != std::string::npos)
                {
                    std::string currentStmt = sqlStatement.substr(0, semicolonPos + 1);
                    dbManager.transactionAddQuery(currentStmt);
                    sqlStatement = sqlStatement.substr(semicolonPos + 1);
                }
            }

            // 提交事务
            dbManager.commitTransaction();

        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "Database initialization failed: " << e.what();
        }
    }
}// namespace DAL