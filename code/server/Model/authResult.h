/**
 * @file authResult.h
 * @brief 认证结果模型类，定义认证操作返回的数据结构
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#pragma once

#include <string>
#include <cstdint>

namespace Model {

    /**
     * @brief 认证结果数据结构，用于统一返回认证操作的结果
     */
    struct AuthResult {
        bool success = false;          ///< 操作是否成功
        int64_t userId = -1;          ///< 用户ID（个人/企业/管理员）
        std::string token;            ///< JWT令牌
        std::string message;          ///< 操作结果消息
    };

} // namespace Model