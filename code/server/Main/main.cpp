/**
 * @file main.cpp
 * @brief 主应用入口文件，负责初始化和启动Crow应用
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#include "../Config/config.h"
#include "../Database/databaseManager.h"
#include <crow.h>


void init()
{
    // 初始化全局配置
    Config::loadConfig();
    DAL::initDatabase();
}

int main()
{
    init();

    // 初始化Crow应用
    crow::SimpleApp app;

    // 启动应用
    app.port(Config::globalConfig["PORT"].i())
            .multithreaded()
            .run();

    return 0;
}