/**
 * @file SecurityUtils.h
 * @brief 安全工具类，处理JWT生成与验证
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include <string>

namespace Utils {
    class SecurityUtils {
    public:
        /**
         * @brief 验证JWT令牌
         * @param token JWT令牌字符串
         * @param userId 输出参数，解析出的用户ID
         * @return bool 验证是否成功
         */
        static bool verifyJWT(const std::string& token, std::string& userId);
        
        /**
         * @brief 生成JWT令牌
         * @param userId 用户ID
         * @return std::string 生成的JWT令牌
         */
        static std::string generateJWT(const std::string& userId);
    };
}