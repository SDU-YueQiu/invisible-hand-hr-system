/**
 * @file Config.h
 * @brief 配置文件头文件，声明了一些路径等配置常量。
 * @author SDU-YueQiu
 * @date 2025/5/12
 * @version 1.0
*/

#pragma once
#include "string"
#include "crow.h"

namespace Config
{
    
    void loadConfig();// 加载配置文件;

    extern crow::json::rvalue globalConfig;
}// namespace Config