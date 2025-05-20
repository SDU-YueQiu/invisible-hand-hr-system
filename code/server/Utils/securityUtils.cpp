/**
 * @file SecurityUtils.cpp
 * @brief 安全工具类实现，提供密码哈希和JWT相关功能
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#include "securityUtils.h"
#include "../Config/config.h"
#include <crow/logging.h>
#include <iomanip>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <sstream>


using namespace Utils;

std::string SecurityUtils::JWT_SECRET = "";

std::string SecurityUtils::hashPassword(const std::string &password)
{
    // 生成随机盐值
    unsigned char salt[16] = "onlyfortestonly";

    // 使用PBKDF2算法进行密码哈希
    unsigned char hash[32];// 输出32字节的哈希值
    if (PKCS5_PBKDF2_HMAC(password.c_str(), password.length(),
                          salt, sizeof(salt),
                          10000,// 迭代次数
                          EVP_sha256(),
                          sizeof(hash), hash) != 1)
    {
        CROW_LOG_ERROR << "密码哈希失败";
        return "";
    }

    // 将盐值和哈希值组合存储
    std::stringstream ss;
    ss << "pbkdf2_sha256$10000$";
    for (unsigned char i: salt)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int) i;
    }
    ss << "$";
    for (unsigned char i: hash)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int) i;
    }

    return ss.str();
}

bool SecurityUtils::verifyPassword(const std::string &password, const std::string &hash)
{
    // 解析存储的哈希字符串
    size_t salt_pos = hash.find('$', 17);// 跳过算法前缀
    if (salt_pos == std::string::npos)
    {
        return false;
    }

    size_t hash_pos = hash.find('$', salt_pos + 1);
    if (hash_pos == std::string::npos)
    {
        return false;
    }

    std::string salt_hex = hash.substr(salt_pos + 1, hash_pos - salt_pos - 1);
    std::string stored_hash_hex = hash.substr(hash_pos + 1);

    // 将十六进制盐值转换为字节
    unsigned char salt[16];
    for (size_t i = 0; i < salt_hex.length(); i += 2)
    {
        salt[i / 2] = std::stoi(salt_hex.substr(i, 2), nullptr, 16);
    }

    // 使用相同的参数计算输入密码的哈希值
    unsigned char computed_hash[32];
    if (PKCS5_PBKDF2_HMAC(password.c_str(), password.length(),
                          salt, sizeof(salt),
                          10000,
                          EVP_sha256(),
                          sizeof(computed_hash), computed_hash) != 1)
    {
        CROW_LOG_ERROR << "密码验证失败";
        return false;
    }

    // 将计算出的哈希值转换为十六进制字符串进行比较
    std::stringstream ss;
    for (unsigned char i: computed_hash)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int) i;
    }

    return ss.str() == stored_hash_hex;
}

std::string SecurityUtils::generateJWT(const std::string &userId, const std::string &role, int expiresIn)
{
    auto token = jwt::create()
                         .set_issuer("invisible-hand-hr-system")
                         .set_type("JWS")
                         .set_payload_claim("userId", jwt::claim(userId))
                         .set_payload_claim("role", jwt::claim(role))
                         .set_issued_at(std::chrono::system_clock::now())
                         .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds(expiresIn))
                         .sign(jwt::algorithm::hs256{JWT_SECRET});

    return token;
}

bool SecurityUtils::verifyJWT(const std::string &token)
{
    try
    {
        auto decoded = jwt::decode(token);
        auto verifier = jwt::verify()
                                .allow_algorithm(jwt::algorithm::hs256{JWT_SECRET})
                                .with_issuer("invisible-hand-hr-system");

        verifier.verify(decoded);
        return true;
    } catch (const std::exception &e)
    {
        CROW_LOG_WARNING << "JWT验证失败: " << e.what();
        return false;
    }
}

std::string SecurityUtils::getUserIdFromToken(const std::string &token)
{
    try
    {
        auto decoded = jwt::decode(token);
        return decoded.get_payload_claim("userId").as_string();
    } catch (const std::exception &e)
    {
        CROW_LOG_WARNING << "从JWT获取用户ID失败: " << e.what();
        return "";
    }
}

std::string SecurityUtils::getRoleFromToken(const std::string &token)
{
    try
    {
        auto decoded = jwt::decode(token);
        return decoded.get_payload_claim("role").as_string();
    } catch (const std::exception &e)
    {
        CROW_LOG_WARNING << "从JWT获取用户角色失败: " << e.what();
        return "";
    }
}