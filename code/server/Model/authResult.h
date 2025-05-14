/**
 * @file authResult.h
 * @brief 认证结果数据结构定义文件
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include <string>

namespace Model {
    struct AuthResult {
        bool success;
        std::string token;
        std::string message;
    };
}