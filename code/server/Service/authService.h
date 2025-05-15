/**
 * @file authService.h
 * @brief 认证服务类，处理用户认证和授权相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#pragma once

#include "../Model/authResult.h"
#include "../Model/enterpriseUser.h"
#include "../Model/individualUser.h"
#include "../Utils/securityUtils.h"
#include "../Utils/validationUtils.h"


namespace Service
{
    /**
     * @brief 认证服务类，封装认证相关业务逻辑
     */
    class AuthService
    {
    public:
        /**
         * @brief 获取AuthService单例实例（线程安全）
         * @return AuthService& AuthService实例
         */
        static AuthService &getInstance()
        {
            thread_local static AuthService instance;
            return instance;
        }

        /**
         * @brief 个人用户注册
         * @param userData 包含注册信息的个人用户对象
         * @return Model::AuthResult 认证结果
         */
        Model::AuthResult registerIndividual(const Model::IndividualUser &userData);

        /**
         * @brief 个人用户登录(用户名)
         * @param username 用户名
         * @param password 密码
         * @return Model::AuthResult 认证结果
         */
        Model::AuthResult loginIndividual(const std::string &username, const std::string &password);

        /**
         * @brief 个人用户登录(手机号)
         * @param phoneNumber 手机号
         * @param password 密码
         * @return Model::AuthResult 认证结果
         */
        Model::AuthResult loginIndividualByPhoneNumber(const std::string &phoneNumber, const std::string &password);

        /**
         * @brief 个人用户登录(邮箱)
         * @param email 邮箱
         * @param password 密码
         * @return Model::AuthResult 认证结果
         */
        Model::AuthResult loginIndividualByEmail(const std::string &email, const std::string &password);

        /**
         * @brief 企业用户注册
         * @param enterpriseData 包含注册信息的企业用户对象
         * @return Model::AuthResult 认证结果
         */
        Model::AuthResult registerEnterprise(const Model::EnterpriseUser &enterpriseData);

        /**
         * @brief 企业用户登录(用户名)
         * @param username 企业用户名
         * @param password 密码
         * @return Model::AuthResult 认证结果
         */
        Model::AuthResult loginEnterprise(const std::string &username, const std::string &password);

        /**
         * @brief 企业用户登录(手机号)
         * @param phoneNumber 联系电话
         * @param password 密码
         * @return Model::AuthResult 认证结果
         */
        Model::AuthResult loginEnterpriseByPhoneNumber(const std::string &phoneNumber, const std::string &password);

        /**
         * @brief 企业用户登录(邮箱)
         * @param email 联系邮箱
         * @param password 密码
         * @return Model::AuthResult 认证结果
         */
        Model::AuthResult loginEnterpriseByEmail(const std::string &email, const std::string &password);

        /**
         * @brief 管理员登录
         * @param username 管理员用户名
         * @param password 密码
         * @return Model::AuthResult 认证结果
         */
        Model::AuthResult loginAdmin(const std::string &username, const std::string &password);

    private:
        Utils::SecurityUtils &securityUtils = Utils::SecurityUtils::getInstance();
        Utils::ValidationUtils &validationUtils = Utils::ValidationUtils::getInstance();
    };
}// namespace Service