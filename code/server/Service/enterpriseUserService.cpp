/**
 * @file enterpriseUserService.cpp
 * @brief 企业用户服务实现文件，处理企业用户管理业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#include "enterpriseUserService.h"
#include "../Database/databaseManager.h"
#include "../Database/enterpriseUserDAO.h"
#include <crow/json.h>

namespace Service {
    EnterpriseUserService::RegisterResult EnterpriseUserService::registerEnterprise(
        const std::string& companyName,
        const std::string& username,
        const std::string& password,
        const std::string& email,
        const std::string& contactPerson,
        const std::string& contactPhone) {
            
        auto& db = DAL::getDatabaseManager();
        
        // 检查用户名是否已存在
        if (EnterpriseUserDAO::isUsernameExists(username)) {
            return {false, "Username already exists"};
        }
        
        // 创建企业用户
        std::string passwordHash = Utils::SecurityUtils::hashPassword(password);
        std::string enterpriseId = EnterpriseUserDAO::createEnterprise(
            companyName, username, passwordHash, email, contactPerson, contactPhone);
            
        if (enterpriseId.empty()) {
            return {false, "Failed to register enterprise"};
        }
        
        return {true, "Enterprise registered successfully"};
    }
    
    EnterpriseUserService::UpdateResult EnterpriseUserService::updateEnterpriseInfo(
        const std::string& enterpriseId,
        const std::string& companyName,
        const std::string& email,
        const std::string& contactPerson,
        const std::string& contactPhone) {
            
        if (!EnterpriseUserDAO::updateEnterprise(enterpriseId, companyName, email, contactPerson, contactPhone)) {
            return {false, "Failed to update enterprise info"};
        }
        
        return {true, "Enterprise info updated successfully"};
    }
}