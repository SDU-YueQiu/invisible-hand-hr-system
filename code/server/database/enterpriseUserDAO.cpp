/**
 * @file EnterpriseUserDAO.cpp
 * @brief 企业用户数据访问对象类实现，封装企业用户表数据库操作
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#include "enterpriseUserDAO.h"
#include "../Model/enterpriseUser.h"
#include "databaseManager.h"
#include <crow/logging.h>
#include <sstream>
#include <stdexcept>

namespace DAL
{
    using namespace Model;

    /**
     * @brief 从数据库结果行构造企业用户对象
     * @param row 数据库结果行（DbRow类型）
     * @return Model::EnterpriseUser 构造的企业用户对象（字段不匹配时返回空对象）
     */
    Model::EnterpriseUser createEnterpriseUserFromRow(DbRow &row)
    {
        try
        {
            return Model::EnterpriseUser{
                    static_cast<int64_t>(std::get<int64_t>(row.at("EnterpriseID"))),
                    std::get<std::string>(row.at("LoginUsername")),
                    std::get<std::string>(row.at("PasswordHash")),
                    std::get<std::string>(row.at("EnterpriseName")),
                    std::get<std::string>(row.at("CreditCode")),
                    std::get<std::string>(row.at("Description")),
                    std::get<std::string>(row.at("Industry")),
                    std::get<std::string>(row.at("Scale")),
                    std::get<std::string>(row.at("Address")),
                    std::get<std::string>(row.at("ContactPerson")),
                    std::get<std::string>(row.at("ContactPhone")),
                    std::get<std::string>(row.at("ContactEmail")),
                    std::get<std::string>(row.at("LogoURL")),
                    std::get<std::string>(row.at("LicenseImageURL")),
                    std::get<std::string>(row.at("RegistrationDate")),
                    std::get<std::string>(row.at("AccountStatus")),
                    std::get<std::string>(row.at("AuditOpinion"))};
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "创建企业用户对象失败: " << e.what();
            return Model::EnterpriseUser{};
        }
    }

    /**
     * @brief 根据企业ID查询企业用户信息
     * @param enterpriseId 企业唯一标识
     * @return Model::EnterpriseUser 企业用户对象（若不存在则返回空对象）
     */
    Model::EnterpriseUser EnterpriseUserDAO::findById(int64_t enterpriseId)
    {
        CROW_LOG_INFO << "查询企业用户，ID: " << enterpriseId;
        const std::string sql = "SELECT * FROM EnterpriseUsers WHERE EnterpriseID = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(enterpriseId)});

        if (!result || result->empty())
        {
            CROW_LOG_WARNING << "未找到企业用户，ID: " << enterpriseId;
            return Model::EnterpriseUser{};
        }
        return createEnterpriseUserFromRow(result->front());
    }

    /**
     * @brief 根据登录用户名查询企业用户信息
     * @param loginUsername 企业登录用户名
     * @return Model::EnterpriseUser 企业用户对象（若不存在则返回空对象）
     */
    Model::EnterpriseUser EnterpriseUserDAO::findByLoginUsername(const std::string &loginUsername)
    {
        CROW_LOG_INFO << "查询企业用户，登录用户名: " << loginUsername;
        const std::string sql = "SELECT * FROM EnterpriseUsers WHERE LoginUsername = ?";
        auto result = dbManager.executeQuery(sql, {loginUsername});

        if (!result || result->empty())
        {
            CROW_LOG_WARNING << "未找到企业用户，登录用户名: " << loginUsername;
            return Model::EnterpriseUser{};
        }
        return createEnterpriseUserFromRow(result->front());
    }

    /**
     * @brief 向数据库中插入一条新的企业用户记录
     * @param enterpriseData 待插入的企业用户数据
     * @return 插入成功返回true，否则返回false
     */
    bool EnterpriseUserDAO::create(const Model::EnterpriseUser &enterpriseData)
    {
        CROW_LOG_INFO << "创建新企业用户，登录用户名: " << enterpriseData.LoginUsername;
        const std::string sql = R"(
            INSERT INTO EnterpriseUsers (
                LoginUsername, PasswordHash, EnterpriseName, CreditCode,
                Description, Industry, Scale, Address, ContactPerson,
                ContactPhone, ContactEmail, LogoURL, LicenseImageURL,
                RegistrationDate, AccountStatus, AuditOpinion
            ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
        )";

        std::vector<std::string> params = {
                enterpriseData.LoginUsername,
                enterpriseData.PasswordHash,
                enterpriseData.EnterpriseName,
                enterpriseData.CreditCode,
                enterpriseData.Description,
                enterpriseData.Industry,
                enterpriseData.Scale,
                enterpriseData.Address,
                enterpriseData.ContactPerson,
                enterpriseData.ContactPhone,
                enterpriseData.ContactEmail,
                enterpriseData.LogoURL,
                enterpriseData.LicenseImageURL,
                enterpriseData.RegistrationDate,
                enterpriseData.AccountStatus,
                enterpriseData.AuditOpinion};

        auto result = dbManager.executeQuery(sql, params);
        return !result->empty();
    }

    /**
     * @brief 更新指定ID的企业用户信息
     * @param enterpriseId 待更新的企业ID
     * @param enterpriseData 更新后的企业用户数据
     * @return bool 更新成功返回true，否则返回false
     */
    bool EnterpriseUserDAO::update(int64_t enterpriseId, const Model::EnterpriseUser &enterpriseData)
    {
        CROW_LOG_INFO << "更新企业用户，ID: " << enterpriseId;
        const std::string sql = R"(
            UPDATE EnterpriseUsers SET
                LoginUsername = ?, PasswordHash = ?, EnterpriseName = ?, CreditCode = ?,
                Description = ?, Industry = ?, Scale = ?, Address = ?,
                ContactPerson = ?, ContactPhone = ?, ContactEmail = ?,
                LogoURL = ?, LicenseImageURL = ?, RegistrationDate = ?,
                AccountStatus = ?, AuditOpinion = ?
            WHERE EnterpriseID = ?
        )";

        std::vector<std::string> params = {
                enterpriseData.LoginUsername,
                enterpriseData.PasswordHash,
                enterpriseData.EnterpriseName,
                enterpriseData.CreditCode,
                enterpriseData.Description,
                enterpriseData.Industry,
                enterpriseData.Scale,
                enterpriseData.Address,
                enterpriseData.ContactPerson,
                enterpriseData.ContactPhone,
                enterpriseData.ContactEmail,
                enterpriseData.LogoURL,
                enterpriseData.LicenseImageURL,
                enterpriseData.RegistrationDate,
                enterpriseData.AccountStatus,
                enterpriseData.AuditOpinion,
                std::to_string(enterpriseId)};

        auto result = dbManager.executeQuery(sql, params);
        return result != nullptr;
    }

    /**
     * @brief 删除指定ID的企业用户记录
     * @param enterpriseId 待删除的企业ID
     * @return bool 删除成功返回true，否则返回false
     */
    bool EnterpriseUserDAO::deleteById(int64_t enterpriseId)
    {
        CROW_LOG_INFO << "删除企业用户，ID: " << enterpriseId;
        const std::string sql = "DELETE FROM EnterpriseUsers WHERE EnterpriseID = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(enterpriseId)});
        return result != nullptr;
    }

}// namespace DAL