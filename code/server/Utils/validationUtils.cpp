/**
 * @file validationUtils.cpp
 * @brief 数据验证工具类实现，提供各种数据格式验证功能
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#include "validationUtils.h"
#include <crow/logging.h>
#include <regex>
#include <string>

using namespace Utils;

bool ValidationUtils::validateEmail(const std::string &email)
{
    // 电子邮件正则表达式
    const std::regex pattern(
            R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");

    if (!std::regex_match(email, pattern))
    {
        CROW_LOG_WARNING << "邮箱格式验证失败: " << email;
        return false;
    }

    // 检查邮箱是否已被个人用户使用
    auto individualUser = DAL::IndividualUserDAO::getInstance().findByEmail(email);
    if (individualUser.UserID != -1)
    {
        CROW_LOG_WARNING << "邮箱已被个人用户使用: " << email;
        return false;
    }

    // 检查邮箱是否已被企业用户使用
    auto enterpriseUser = DAL::EnterpriseUserDAO::getInstance().findByLoginUsername(email);
    if (enterpriseUser.EnterpriseID != -1)
    {
        CROW_LOG_WARNING << "邮箱已被企业用户使用: " << email;
        return false;
    }

    return true;
}

bool ValidationUtils::validatePhone(const std::string &phone)
{
    // 中国手机号正则表达式 (11位，1开头)
    const std::regex pattern(
            R"(^1[3-9]\d{9}$)");

    if (!std::regex_match(phone, pattern))
    {
        CROW_LOG_WARNING << "电话号码格式验证失败: " << phone;
        return false;
    }

    // 检查电话号码是否已被个人用户使用
    auto individualUser = DAL::IndividualUserDAO::getInstance().findByPhoneNumber(phone);
    if (individualUser.UserID != -1)
    {
        CROW_LOG_WARNING << "手机号已被个人用户使用: " << phone;
        return false;
    }

    return true;
}

bool ValidationUtils::validateUsername(const std::string &username)
{
    // 检查用户名是否存在于三种用户表中
    auto adminUser = DAL::AdminUserDAO::getInstance().findByUsername(username);
    auto enterpriseUser = DAL::EnterpriseUserDAO::getInstance().findByLoginUsername(username);
    auto individualUser = DAL::IndividualUserDAO::getInstance().findByUsername(username);

    if (adminUser.AdminID != -1 ||
        enterpriseUser.EnterpriseID != -1 ||
        individualUser.UserID != -1)
    {
        CROW_LOG_WARNING << "用户名已存在: " << username;
        return false;
    }
    return true;
}

bool ValidationUtils::validateSchema(const crow::json::rvalue &data,
                                     const crow::json::rvalue &schema)
{
    try
    {
        // 简单实现: 检查必需字段是否存在
        if (schema.has("required"))
        {
            auto required = schema["required"];
            for (size_t i = 0; i < required.size(); i++)
            {
                std::string field = required[i].s();
                if (!data.has(field))
                {
                    CROW_LOG_WARNING << "JSON Schema验证失败: 缺少必需字段 " << field;
                    return false;
                }
            }
        }

        // 检查字段类型
        if (schema.has("properties"))
        {
            auto properties = schema["properties"];
            for (const auto &prop: properties)
            {
                std::string field = prop.key();
                if (data.has(field))
                {
                    std::string expectedType = prop["type"].s();
                    std::string actualType;

                    switch (data[field].t())
                    {
                        case crow::json::type::Number:
                            actualType = "number";
                            break;
                        case crow::json::type::String:
                            actualType = "string";
                            break;
                        case crow::json::type::List:
                            actualType = "array";
                            break;
                        case crow::json::type::Object:
                            actualType = "object";
                            break;
                        case crow::json::type::True:
                        case crow::json::type::False:
                            actualType = "boolean";
                            break;
                        default:
                            actualType = "null";
                    }

                    if (actualType != expectedType)
                    {
                        CROW_LOG_WARNING << "JSON Schema验证失败: 字段 " << field
                                         << " 类型不匹配 (期望: " << expectedType
                                         << ", 实际: " << actualType << ")";
                        return false;
                    }
                }
            }
        }

        return true;
    } catch (const std::exception &e)
    {
        CROW_LOG_ERROR << "JSON Schema验证异常: " << e.what();
        return false;
    }
}