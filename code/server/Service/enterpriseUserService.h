/**
 * @file enterpriseUserService.h
 * @brief 企业用户服务类，处理企业用户相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#pragma once

#include "../Model/enterpriseUser.h"
#include "../Database/enterpriseUserDAO.h"

namespace Service
{
    /**
     * @brief 企业用户服务类，封装企业用户相关业务逻辑
     */
    class EnterpriseUserService
    {
    public:
        /**
         * @brief 获取EnterpriseUserService单例实例（线程安全）
         * @return EnterpriseUserService& EnterpriseUserService实例
         */
        static EnterpriseUserService& getInstance()
        {
            thread_local static EnterpriseUserService instance;
            return instance;
        }

        /**
         * @brief 根据企业ID获取企业用户信息
         * @param enterpriseId 企业ID
         * @return Model::EnterpriseUser 企业用户对象
         */
        Model::EnterpriseUser getEnterpriseById(int64_t enterpriseId);

        /**
         * @brief 更新企业信息
         * @param enterpriseId 企业ID
         * @param enterpriseData 包含更新信息的企业对象
         * @return bool 更新成功返回true，否则返回false
         */
        bool updateEnterpriseInfo(int64_t enterpriseId, const Model::EnterpriseUser& enterpriseData);

        /**
         * @brief 修改企业用户密码
         * @param enterpriseId 企业ID
         * @param oldPassword 旧密码
         * @param newPassword 新密码
         * @return bool 修改成功返回true，否则返回false
         */
        bool changePassword(int64_t enterpriseId, const std::string& oldPassword, const std::string& newPassword);

    private:
        DAL::EnterpriseUserDAO& enterpriseDAO = DAL::EnterpriseUserDAO::getInstance();
    };
} // namespace Service