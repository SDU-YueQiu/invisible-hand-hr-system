/**
 * @file enterpriseUserService.cpp
 * @brief 企业用户服务类实现，处理企业用户相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#include "enterpriseUserService.h"
#include <crow/logging.h>

namespace Service
{
    Model::EnterpriseUser EnterpriseUserService::getEnterpriseById(int64_t enterpriseId)
    {
        CROW_LOG_INFO << "Getting enterprise by ID: " << enterpriseId;

        // 通过DAO层获取企业信息
        auto enterprise = enterpriseDAO.findById(enterpriseId);

        if (enterprise.EnterpriseID == -1)
        {
            CROW_LOG_WARNING << "Enterprise not found with ID: " << enterpriseId;
        }

        return enterprise;
    }

    bool EnterpriseUserService::updateEnterpriseInfo(int64_t enterpriseId, const Model::EnterpriseUser &enterpriseData)
    {
        CROW_LOG_INFO << "Updating enterprise info for ID: " << enterpriseId;

        // 验证企业ID是否匹配
        if (enterpriseData.EnterpriseID != enterpriseId)
        {
            CROW_LOG_WARNING << "Enterprise ID mismatch in update request";
            return false;
        }

        // 获取现有企业信息
        auto existingEnterprise = enterpriseDAO.findById(enterpriseId);
        if (existingEnterprise.EnterpriseID == -1)
        {
            CROW_LOG_WARNING << "Enterprise not found for update, ID: " << enterpriseId;
            return false;
        }

        // 只更新非默认值的字段
        if (!enterpriseData.EnterpriseName.empty()) existingEnterprise.EnterpriseName = enterpriseData.EnterpriseName;
        if (!enterpriseData.Description.empty()) existingEnterprise.Description = enterpriseData.Description;
        if (!enterpriseData.Industry.empty()) existingEnterprise.Industry = enterpriseData.Industry;
        if (!enterpriseData.Scale.empty()) existingEnterprise.Scale = enterpriseData.Scale;
        if (!enterpriseData.Address.empty()) existingEnterprise.Address = enterpriseData.Address;
        if (!enterpriseData.ContactPerson.empty()) existingEnterprise.ContactPerson = enterpriseData.ContactPerson;
        if (!enterpriseData.ContactPhone.empty()) existingEnterprise.ContactPhone = enterpriseData.ContactPhone;
        if (!enterpriseData.ContactEmail.empty()) existingEnterprise.ContactEmail = enterpriseData.ContactEmail;
        if (!enterpriseData.LogoURL.empty()) existingEnterprise.LogoURL = enterpriseData.LogoURL;
        if (!enterpriseData.LicenseImageURL.empty()) existingEnterprise.LicenseImageURL = enterpriseData.LicenseImageURL;

        // 通过DAO层更新企业信息
        return enterpriseDAO.update(enterpriseId, existingEnterprise);
    }

    bool EnterpriseUserService::changePassword(int64_t enterpriseId, const std::string &oldPassword, const std::string &newPassword)
    {
        CROW_LOG_INFO << "Changing password for enterprise ID: " << enterpriseId;

        // 获取企业信息
        auto enterprise = enterpriseDAO.findById(enterpriseId);
        if (enterprise.EnterpriseID == -1)
        {
            CROW_LOG_WARNING << "Enterprise not found with ID: " << enterpriseId;
            return false;
        }

        // 验证旧密码是否正确
        if (enterprise.PasswordHash != oldPassword)
        {
            CROW_LOG_WARNING << "Old password verification failed for enterprise ID: " << enterpriseId;
            return false;
        }

        // 创建更新后的企业对象
        Model::EnterpriseUser updatedEnterprise = enterprise;
        updatedEnterprise.PasswordHash = newPassword;

        // 通过DAO层更新密码
        return enterpriseDAO.update(enterpriseId, updatedEnterprise);
    }
}// namespace Service