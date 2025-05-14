/**
 * @file adminUser.h
 * @brief 管理员用户模型类定义
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include <string>
#include <cstdint>

namespace Model {
    /**
     * @brief 管理员用户模型类
     */
    struct AdminUser {
        int64_t AdminID;            // 管理员唯一标识
        std::string Username;       // 登录用户名
        std::string PasswordHash;   // 密码哈希值
        std::string Email;          // 联系邮箱
        std::string PhoneNumber;    // 联系电话
        std::string Role;           // 角色权限
        std::string AccountStatus;  // 账户状态(正常/禁用)
        std::string LastLoginTime;  // 最后登录时间
        std::string CreateTime;     // 创建时间
    };
}