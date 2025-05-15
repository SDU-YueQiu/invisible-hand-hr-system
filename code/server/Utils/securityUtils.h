/**
 * @file securityUtils.h
 * @brief 安全工具类，提供密码哈希和JWT相关功能
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include "../Config/config.h"
#include <jwt-cpp/jwt.h>
#include <openssl/evp.h>
#include <string>
#include <thread>


namespace Utils
{
    /**
     * @brief 安全工具类，提供密码哈希和JWT相关功能
     */
    class SecurityUtils
    {
    public:
        /**
         * @brief 使用bcrypt算法哈希密码
         * @param password 明文密码
         * @return std::string 哈希后的密码
         */
        static std::string hashPassword(const std::string &password);

        /**
         * @brief 验证密码是否匹配哈希值
         * @param password 明文密码
         * @param hash 存储的哈希值
         * @return bool 验证结果
         */
        static bool verifyPassword(const std::string &password, const std::string &hash);

        /**
         * @brief 生成JWT令牌
         * @param userId 用户ID
         * @param role 用户角色
         * @param expiresIn 过期时间(秒)
         * @return std::string JWT令牌字符串
         */
        static std::string generateJWT(const std::string &userId, const std::string &role, int expiresIn = 3600);

        /**
         * @brief 验证JWT令牌有效性
         * @param token JWT令牌字符串
         * @return bool 验证结果
         */
        static bool verifyJWT(const std::string &token);

        /**
         * @brief 从JWT令牌中获取用户ID
         * @param token JWT令牌字符串
         * @return std::string 用户ID
         */
        static std::string getUserIdFromToken(const std::string &token);

        /**
         * @brief 从JWT令牌中获取用户角色
         * @param token JWT令牌字符串
         * @return std::string 用户角色
         */
        static std::string getRoleFromToken(const std::string &token);

        /**
        * @brief 初始化安全工具，从配置文件中读取JWT密钥
        */
        static void init()
        {
            if (JWT_SECRET.empty())
            {
                JWT_SECRET = Config::globalConfig["JWT_SECRET"].s();
            }
        }

    private:
        // 用于JWT签名的密钥
        static std::string JWT_SECRET;
    };
}// namespace Utils