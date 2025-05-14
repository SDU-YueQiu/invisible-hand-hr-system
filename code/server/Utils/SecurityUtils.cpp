/**
 * @file SecurityUtils.cpp
 * @brief 安全工具类实现文件，处理JWT生成与验证
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#include "SecurityUtils.h"
#include <jwt-cpp/jwt.h>
#include <crow/utility.h>

namespace Utils {
    bool SecurityUtils::verifyJWT(const std::string& token, std::string& userId) {
        try {
            auto decoded = jwt::decode(token);
            auto verifier = jwt::verify()
                .allow_algorithm(jwt::algorithm::hs256{Crow::utility::base64decode(Config::globalConfig["JWT_SECRET"].s())});
            
            verifier.verify(decoded);
            userId = decoded.get_payload_claim("userId").as_string();
            return true;
        } catch (...) {
            return false;
        }
    }
    
    std::string SecurityUtils::generateJWT(const std::string& userId) {
        auto token = jwt::create()
            .set_payload_claim("userId", jwt::claim(userId))
            .set_issued_at(std::chrono::system_clock::now())
            .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours{24})
            .sign(jwt::algorithm::hs256{Crow::utility::base64decode(Config::globalConfig["JWT_SECRET"].s())});
            
        return token;
    }
}