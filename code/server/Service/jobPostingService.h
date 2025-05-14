/**
 * @file jobPostingService.h
 * @brief 职位发布服务头文件，处理职位发布和管理相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include <string>
#include <crow/json.h>

namespace Service {
    class JobPostingService {
    public:
        struct PostResult {
            bool success;
            std::string message;
            std::string jobId;
        };
        
        struct UpdateResult {
            bool success;
            std::string message;
        };
        
        struct StatusResult {
            bool success;
            std::string message;
        };
        
        static PostResult postJob(const std::string& enterpriseId, const crow::json::rvalue& jobData);
        static UpdateResult updateJob(const std::string& jobId, const crow::json::rvalue& jobData);
        static StatusResult setJobStatus(const std::string& jobId, const std::string& status);
        static StatusResult deleteJob(const std::string& jobId);
    };
}