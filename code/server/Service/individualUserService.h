/**
 * @file individualUserService.h
 * @brief 个人用户服务类，处理个人用户相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#pragma once

#include "../Model/individualUser.h"
#include "../Database/individualUserDAO.h"

namespace Service
{
    /**
     * @brief 个人用户服务类，封装个人用户相关业务逻辑
     */
    class IndividualUserService
    {
    public:
        /**
         * @brief 获取IndividualUserService单例实例（线程安全）
         * @return IndividualUserService& IndividualUserService实例
         */
        static IndividualUserService& getInstance()
        {
            thread_local static IndividualUserService instance;
            return instance;
        }

        /**
         * @brief 根据用户ID获取个人用户信息
         * @param userId 用户ID
         * @return Model::IndividualUser 用户对象
         */
        Model::IndividualUser getUserById(int userId);

        /**
         * @brief 更新用户信息
         * @param userId 用户ID
         * @param userData 包含更新信息的用户对象
         * @return bool 更新成功返回true，否则返回false
         */
        bool updateUserInfo(int userId, const Model::IndividualUser& userData);

        /**
         * @brief 修改用户密码
         * @param userId 用户ID
         * @param oldPassword 旧密码
         * @param newPassword 新密码
         * @return bool 修改成功返回true，否则返回false
         */
        bool changePassword(int userId, const std::string& oldPassword, const std::string& newPassword);

    private:
        DAL::IndividualUserDAO& userDAO = DAL::IndividualUserDAO::getInstance();
    };
} // namespace Service