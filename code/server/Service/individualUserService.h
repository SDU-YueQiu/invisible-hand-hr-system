/**
 * @file individualUserService.h
 * @brief 个人用户服务类，处理个人用户相关的业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#pragma once

#include "../Model/individualUser.h"
#include "../Database/individualUserDAO.h"
#include <crow/json.h>

namespace Service
{
    /**
     * @brief 个人用户服务类
     */
    class IndividualUserService
    {
    public:
        /**
         * @brief 获取IndividualUserService单例实例（线程安全）
         * @return IndividualUserService& IndividualUserService实例
         */
        static IndividualUserService& getInstance() {
            thread_local static IndividualUserService instance;
            return instance;
        }

        /**
         * @brief 根据用户ID获取个人用户信息
         * @param userId 用户ID
         * @return crow::json::wvalue 包含用户信息的JSON对象
         */
        crow::json::wvalue getUserById(int userId);

        /**
         * @brief 更新用户信息
         * @param userId 用户ID
         * @param userData 包含更新字段的JSON数据
         * @return crow::json::wvalue 包含操作结果的JSON对象
         */
        crow::json::wvalue updateUserInfo(int userId, const crow::json::rvalue& userData);

        /**
         * @brief 修改用户密码
         * @param userId 用户ID
         * @param oldPassword 旧密码
         * @param newPassword 新密码
         * @return crow::json::wvalue 包含操作结果的JSON对象
         */
        crow::json::wvalue changePassword(int userId, const std::string& oldPassword, 
                                         const std::string& newPassword);

    private:
        IndividualUserService() = default;
        DAL::IndividualUserDAO& userDao = DAL::IndividualUserDAO::getInstance();
    };
}