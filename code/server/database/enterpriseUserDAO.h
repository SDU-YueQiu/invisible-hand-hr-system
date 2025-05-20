/**
 * @file EnterpriseUserDAO.h
 * @brief 企业用户数据访问对象类，封装企业用户表数据库操作
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#ifndef ENTERPRISE_USER_DAO_H
#define ENTERPRISE_USER_DAO_H

#include "../Model/enterpriseUser.h"
#include "databaseManager.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <cstdint>
#include <string>
#include <vector>


namespace DAL
{

    /**
 * @brief 企业用户数据访问对象，负责企业用户表的CRUD操作
 * @note 使用SQLiteCpp库封装，通过DatabaseManager获取数据库连接
 */
    class EnterpriseUserDAO
    {
    public:
        /**
         * @brief 获取EnterpriseUserDAO单例实例（线程安全）
         * @return EnterpriseUserDAO& EnterpriseUserDAO实例
         */
        static EnterpriseUserDAO &getInstance()
        {
            thread_local static EnterpriseUserDAO instance;
            return instance;
        }
        /**
     * @brief 根据企业ID查询企业用户信息
     * @param enterpriseId 企业唯一标识
     * @return Model::EnterpriseUser 企业用户对象（若不存在则返回空对象）
     */
        Model::EnterpriseUser findById(int64_t enterpriseId);

        /**
     * @brief 根据登录用户名查询企业用户信息
     * @param loginUsername 企业登录用户名
     * @return Model::EnterpriseUser 企业用户对象（若不存在则返回空对象）
     */
        Model::EnterpriseUser findByLoginUsername(const std::string &loginUsername);

        /**
     * @brief 根据统一社会信用代码查询企业用户信息
     * @param creditCode 企业统一社会信用代码
     * @return Model::EnterpriseUser 企业用户对象（若不存在则返回空对象）
     */
        Model::EnterpriseUser findByCreditCode(const std::string &creditCode);

        /**
     * @brief 根据联系邮箱查询企业用户信息
     * @param email 企业联系邮箱
     * @return Model::EnterpriseUser 企业用户对象（若不存在则返回空对象）
     */
        Model::EnterpriseUser findByContactEmail(const std::string &email);

        /**
     * @brief 根据联系电话查询企业用户信息
     * @param phone 企业联系电话
     * @return Model::EnterpriseUser 企业用户对象（若不存在则返回空对象）
     */
        Model::EnterpriseUser findByContactPhone(const std::string &phone);

        /**
     * @brief 向数据库中插入一条新的企业用户记录
     * @param enterpriseData 待插入的企业用户数据
     * @return 插入成功返回true，否则返回false
     */
        bool create(const Model::EnterpriseUser &enterpriseData);

        /**
     * @brief 更新指定ID的企业用户信息
     * @param enterpriseId 待更新的企业ID
     * @param enterpriseData 更新后的企业用户数据
     * @return bool 更新成功返回true，否则返回false
     */
        bool update(int64_t enterpriseId, const Model::EnterpriseUser &enterpriseData);

        /**
        * @brief 删除指定ID的企业用户记录
        * @param enterpriseId 待删除的企业ID
        * @return bool 删除成功返回true，否则返回false
        */
        bool deleteById(int64_t enterpriseId);

        /**
        * @brief 根据筛选条件查询企业用户信息
        * @param filter SQL筛选条件字符串（如"AccountStatus='正常' AND RegistrationDate>'2025-01-01'"）
        * @return std::vector<Model::EnterpriseUser> 符合条件的企业用户列表
        */
        std::vector<Model::EnterpriseUser> findByFilter(const std::string &filter);

    private:
        EnterpriseUserDAO() = default;
        DatabaseManager &dbManager = DatabaseManager::getInstance();// 获取数据库管理器单例
    };

}// namespace DAL

#endif// ENTERPRISE_USER_DAO_H