/**
 * @file authService.cpp
 * @brief 认证服务类实现，处理用户认证和授权相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#include "authService.h"
#include "../Database/adminUserDAO.h"
#include "../Database/enterpriseUserDAO.h"
#include "../Database/individualUserDAO.h"
#include <crow/logging.h>


namespace Service
{
    Model::AuthResult AuthService::registerIndividual(const Model::IndividualUser &userData)
    {
        CROW_LOG_INFO << "Registering individual user: " << userData.Username;
        Model::AuthResult result;

        // 验证用户名格式
        if (!validationUtils.validateUsername(userData.Username))
        {
            result.message = "用户名已存在";
            return result;
        }

        // 验证邮箱格式
        if (!validationUtils.validateEmail(userData.Email))
        {
            result.message = "邮箱已存在";
            return result;
        }

        // 验证手机号格式
        if (!validationUtils.validatePhone(userData.PhoneNumber))
        {
            result.message = "手机号已存在";
            return result;
        }

        // 创建用户
        auto newUser = userData;
        newUser.PasswordHash = Utils::SecurityUtils::hashPassword(userData.PasswordHash);
        bool success = DAL::IndividualUserDAO::getInstance().create(newUser);

        if (success)
        {
            result.success = true;
            result.message = "Registration successful";
        } else
        {
            result.message = "Failed to create user";
        }

        return result;
    }

    Model::AuthResult AuthService::loginIndividual(const std::string &username, const std::string &password)
    {
        CROW_LOG_INFO << "Individual user login attempt: " << username;
        Model::AuthResult result;

        // 查找用户
        auto user = DAL::IndividualUserDAO::getInstance().findByUsername(username);
        if (user.UserID == -1)
        {
            result.message = "User not found";
            return result;
        }

        // 验证密码
        if (!Utils::SecurityUtils::verifyPassword(password, user.PasswordHash))
        {
            result.message = "Invalid password";
            return result;
        }

        // 生成JWT
        result.success = true;
        result.userId = user.UserID;
        result.token = Utils::SecurityUtils::generateJWT(std::to_string(user.UserID), "individual", 3600 * 24);
        result.message = "Login successful";

        return result;
    }

    Model::AuthResult AuthService::registerEnterprise(const Model::EnterpriseUser &enterpriseData)
    {
        CROW_LOG_INFO << "Registering enterprise user: " << enterpriseData.LoginUsername;
        Model::AuthResult result;

        // 验证企业用户名唯一
        if (!validationUtils.validateUsername(enterpriseData.LoginUsername))
        {
            result.message = "用户名已存在";
            return result;
        }

        // 验证企业邮箱唯一
        if (!validationUtils.validateEmail(enterpriseData.ContactEmail))
        {
            result.message = "邮箱已存在";
            return result;
        }

        // 验证CreditCode唯一
        if (!validationUtils.validateCreditCode(enterpriseData.CreditCode))
        {
            result.message = "统一社会信用代码已存在";
            return result;
        }

        // 验证手机号唯一
        if (!validationUtils.validatePhone(enterpriseData.ContactPhone))
        {
            result.message = "手机号已存在";
            return result;
        }

        // 创建企业用户
        auto newEnterprise = enterpriseData;
        newEnterprise.PasswordHash = Utils::SecurityUtils::hashPassword(enterpriseData.PasswordHash);
        bool success = DAL::EnterpriseUserDAO::getInstance().create(newEnterprise);

        if (success)
        {
            result.success = true;
            result.userId = newEnterprise.EnterpriseID;
            result.message = "Enterprise registration successful";
        } else
        {
            result.message = "Failed to create enterprise user";
        }

        return result;
    }

    Model::AuthResult AuthService::loginEnterprise(const std::string &username, const std::string &password)
    {
        CROW_LOG_INFO << "Enterprise user login attempt: " << username;
        Model::AuthResult result;

        // 查找企业用户
        auto enterprise = DAL::EnterpriseUserDAO::getInstance().findByLoginUsername(username);
        if (enterprise.EnterpriseID == -1)
        {
            result.message = "Enterprise not found";
            return result;
        }

        // 验证密码
        if (!Utils::SecurityUtils::verifyPassword(password, enterprise.PasswordHash))
        {
            result.message = "Invalid password";
            return result;
        }

        // 生成JWT
        result.success = true;
        result.userId = enterprise.EnterpriseID;
        result.token = Utils::SecurityUtils::generateJWT(std::to_string(enterprise.EnterpriseID), "enterprise", 3600 * 24);
        result.message = "Enterprise login successful";

        return result;
    }

    Model::AuthResult AuthService::loginAdmin(const std::string &username, const std::string &password)
    {
        CROW_LOG_INFO << "Admin login attempt: " << username;
        Model::AuthResult result;

        // 查找管理员
        auto admin = DAL::AdminUserDAO::getInstance().findByUsername(username);
        if (admin.AdminID == -1)
        {
            result.message = "Admin not found";
            return result;
        }

        // 验证密码
        // if (!Utils::SecurityUtils::verifyPassword(password, admin.PasswordHash))
        // {
        //     result.message = "Invalid password";
        //     return result;
        // }

        // 因为压根没有注册api，管理员全手操sql，管理员使用明文密码登录
        if (password != admin.PasswordHash)
        {
            result.message = "Invalid password";
            return result;
        }

        // 生成JWT
        result.success = true;
        result.userId = admin.AdminID;
        result.token = Utils::SecurityUtils::generateJWT(std::to_string(admin.AdminID), "admin", 3600 * 24);
        result.message = "Admin login successful";

        return result;
    }

    Model::AuthResult AuthService::loginIndividualByPhoneNumber(const std::string &phoneNumber, const std::string &password)
    {
        CROW_LOG_INFO << "Individual user login attempt by phone: " << phoneNumber;
        Model::AuthResult result;

        // 查找用户
        auto user = DAL::IndividualUserDAO::getInstance().findByPhoneNumber(phoneNumber);
        if (user.UserID == -1)
        {
            result.message = "User not found";
            return result;
        }

        // 验证密码
        if (!Utils::SecurityUtils::verifyPassword(password, user.PasswordHash))
        {
            result.message = "Invalid password";
            return result;
        }

        // 生成JWT
        result.success = true;
        result.userId = user.UserID;
        result.token = Utils::SecurityUtils::generateJWT(std::to_string(user.UserID), "individual", 3600 * 24);
        result.message = "Login successful";

        return result;
    }

    Model::AuthResult AuthService::loginIndividualByEmail(const std::string &email, const std::string &password)
    {
        CROW_LOG_INFO << "Individual user login attempt by email: " << email;
        Model::AuthResult result;

        // 查找用户
        auto user = DAL::IndividualUserDAO::getInstance().findByEmail(email);
        if (user.UserID == -1)
        {
            result.message = "User not found";
            return result;
        }

        // 验证密码
        if (!Utils::SecurityUtils::verifyPassword(password, user.PasswordHash))
        {
            result.message = "Invalid password";
            return result;
        }

        // 生成JWT
        result.success = true;
        result.userId = user.UserID;
        result.token = Utils::SecurityUtils::generateJWT(std::to_string(user.UserID), "individual", 3600 * 24);
        result.message = "Login successful";

        return result;
    }

    Model::AuthResult AuthService::loginEnterpriseByPhoneNumber(const std::string &phoneNumber, const std::string &password)
    {
        CROW_LOG_INFO << "Enterprise user login attempt by phone: " << phoneNumber;
        Model::AuthResult result;

        // 查找企业用户
        auto enterprise = DAL::EnterpriseUserDAO::getInstance().findByContactPhone(phoneNumber);
        if (enterprise.EnterpriseID == -1)
        {
            result.message = "Enterprise not found";
            return result;
        }

        // 验证密码
        if (!Utils::SecurityUtils::verifyPassword(password, enterprise.PasswordHash))
        {
            result.message = "Invalid password";
            return result;
        }

        // 生成JWT
        result.success = true;
        result.userId = enterprise.EnterpriseID;
        result.token = Utils::SecurityUtils::generateJWT(std::to_string(enterprise.EnterpriseID), "enterprise", 3600 * 24);
        result.message = "Enterprise login successful";

        return result;
    }

    Model::AuthResult AuthService::loginEnterpriseByEmail(const std::string &email, const std::string &password)
    {
        CROW_LOG_INFO << "Enterprise user login attempt by email: " << email;
        Model::AuthResult result;

        // 查找企业用户
        auto enterprise = DAL::EnterpriseUserDAO::getInstance().findByContactEmail(email);
        if (enterprise.EnterpriseID == -1)
        {
            result.message = "Enterprise not found";
            return result;
        }

        // 验证密码
        if (!Utils::SecurityUtils::verifyPassword(password, enterprise.PasswordHash))
        {
            result.message = "Invalid password";
            return result;
        }

        // 生成JWT
        result.success = true;
        result.userId = enterprise.EnterpriseID;
        result.token = Utils::SecurityUtils::generateJWT(std::to_string(enterprise.EnterpriseID), "enterprise", 3600 * 24);
        result.message = "Enterprise login successful";

        return result;
    }
}// namespace Service