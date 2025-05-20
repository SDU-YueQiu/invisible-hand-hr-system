/**
 * @file validationUtils.h
 * @brief 数据验证工具类，提供各种数据格式验证功能
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#pragma once

#include <string>
#include <crow/json.h>
#include "../Database/adminUserDAO.h"
#include "../Database/enterpriseUserDAO.h"
#include "../Database/individualUserDAO.h"

namespace Utils {
    /**
     * @brief 数据验证工具类
     */
    class ValidationUtils {
    public:
        /**
         * @brief 验证电子邮件格式
         * @param email 待验证的电子邮件字符串
         * @return bool 验证结果
         */
        static bool validateEmail(const std::string& email);

        /**
         * @brief 验证电话号码格式
         * @param phone 待验证的电话号码字符串
         * @return bool 验证结果
         */
        static bool validatePhone(const std::string& phone);

        /**
         * @brief 验证用户名格式
         * @param username 待验证的用户名字符串
         * @return bool 验证结果
         */
        static bool validateUsername(const std::string& username);

        /**
         * @brief 根据JSON Schema验证数据结构
         * @param data 待验证的JSON数据
         * @param schema 验证用的JSON Schema
         * @return bool 验证结果
         */
        static bool validateSchema(const crow::json::rvalue& data, const crow::json::rvalue& schema);

        /**
         * @brief 验证统一社会信用代码格式
         * @param creditCode 待验证的统一社会信用代码字符串
         * @return bool 验证结果
         */
        static bool validateCreditCode(const std::string& creditCode);

        /**
         * @brief 获取ValidationUtils类的单例实例
         * @return ValidationUtils& 单例实例的引用
         */
        static ValidationUtils& getInstance() {
            thread_local static ValidationUtils instance;
            return instance;
        }
    };
}