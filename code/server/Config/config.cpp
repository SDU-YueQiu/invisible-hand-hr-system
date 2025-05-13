/**
 * @file Config.cpp
 * @brief 配置文件实现文件，实现加载配置文件函数。
 * @author SDU-YueQiu
 * @date 2025/5/12
 * @version 1.0
*/

#include "config.h"
#include "crow/logging.h"
#include "sstream"
#include <crow/json.h>
#include <fstream>



namespace Config
{
    crow::json::rvalue globalConfig;

    void loadDefaultConfig()
    {
        CROW_LOG_INFO << "Start to load default config file";

        crow::json::wvalue defaultConfig;
        defaultConfig["DB_PATH"] = "./db/hr_system.db";
        defaultConfig["DB_INIT_PATH"] = "./db/databaseInit.sql";
        defaultConfig["IS_DB_INIT"] = false;
        defaultConfig["PORT"] = 8080;

        std::ofstream configFile("config.json");
        configFile << defaultConfig.dump();
        configFile.close();

        globalConfig = crow::json::load(defaultConfig.dump());
    }

    void loadConfig()
    {
        CROW_LOG_INFO << "Loading config file...";

        std::ifstream configFile("config.json");
        if (!configFile.is_open())
        {
            CROW_LOG_WARNING << "Config file not found, load default config";
            loadDefaultConfig();
            return;
        }

        std::stringstream buffer;
        buffer << configFile.rdbuf();
        globalConfig = crow::json::load(buffer.str());
    }
}// namespace Config