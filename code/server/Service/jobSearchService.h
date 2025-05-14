/**
 * @file jobSearchService.h
 * @brief 职位搜索服务头文件，定义职位搜索相关服务接口
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include <string>
#include <vector>

namespace Service {
    class JobSearchService {
    public:
        struct JobInfo {
            std::string jobId;
            std::string title;
            std::string company;
            std::string location;
            std::string salary;
        };
        
        struct SearchResult {
            bool success;
            std::string message;
            std::vector<JobInfo> jobs;
        };
        
        static SearchResult searchJobs(const std::string& keywords, const std::string& location, int page, int pageSize);
        static SearchResult getJobDetails(const std::string& jobId);
    };
}