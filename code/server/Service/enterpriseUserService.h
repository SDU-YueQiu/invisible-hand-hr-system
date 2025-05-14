/**
 * @file enterpriseUserService.h
 * @brief 企业用户服务头文件，定义企业用户管理相关接口
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include <string>

namespace Service {
    class EnterpriseUserService {
    public:
        struct RegisterResult {
            bool success;
            std::string message;
        };
        
        struct UpdateResult {
            bool success;
            std::string message;
        };
        
        static RegisterResult registerEnterprise(
            const std::string& companyName,
            const std::string& username,
            const std::string& password,
            const std::string& email,
            const std::string& contactPerson,
            const std::string& contactPhone);
            
        static UpdateResult updateEnterpriseInfo(
            const std::string& enterpriseId,
            const std::string& companyName,
            const std::string& email,
            const std::string& contactPerson,
            const std::string& contactPhone);
    };
}