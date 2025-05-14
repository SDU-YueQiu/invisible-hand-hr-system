/**
 * @file ValidationUtils.h
 * @brief 数据验证工具类，处理各种数据格式验证
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include <string>

namespace Utils {
    class ValidationUtils {
    public:
        /**
         * @brief 验证邮箱格式
         * @param email 待验证的邮箱字符串
         * @return bool 验证是否通过
         */
        static bool isValidEmail(const std::string& email);
        
        /**
         * @brief 验证手机号格式
         * @param phone 待验证的手机号字符串
         * @return bool 验证是否通过
         */
        static bool isValidPhone(const std::string& phone);
        
        /**
         * @brief 验证密码强度
         * @param password 待验证的密码字符串
         * @return bool 验证是否通过
         */
        static bool isStrongPassword(const std::string& password);
    };
}