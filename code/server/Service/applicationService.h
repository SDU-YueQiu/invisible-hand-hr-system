/**
 * @file applicationService.h
 * @brief 职位申请服务头文件，定义职位申请相关服务接口
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include <string>

namespace Service {
    class ApplicationService {
    public:
        struct ApplyResult {
            bool success;
            std::string message;
        };
        
        static ApplyResult applyForJob(const std::string& userId, const std::string& jobId, const std::string& resumeId);
    };
}