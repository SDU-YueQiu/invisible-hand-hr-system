/**
 * @file adminUser.h
 * @brief 管理员用户模型类定义
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include <cstdint>
#include <string>


namespace Model
{
    /**
     * @brief 管理员用户模型类
     */
    struct AdminUser
    {
        int64_t AdminID = -1;     // 管理员唯一标识
        std::string Username;     // 登录用户名
        std::string PasswordHash; // 密码哈希值
        std::string Role;         // 角色权限
    };
}// namespace Model