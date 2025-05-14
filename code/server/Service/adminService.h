/**
 * @file adminService.h
 * @brief 管理员服务头文件，定义系统管理相关服务接口
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include <string>

namespace Service {
    class AdminService {
    public:
        struct AdminResult {
            bool success;
            std::string message;
        };
        
        static AdminResult approveEnterprise(const std::string& enterpriseId);
        static AdminResult manageUser(const std::string& userId, const std::string& action);
        static AdminResult postAnnouncement(const std::string& title, const std::string& content);
    };
}