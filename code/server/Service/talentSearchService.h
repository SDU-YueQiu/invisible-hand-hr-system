/**
 * @file talentSearchService.h
 * @brief 人才搜索服务头文件，处理人才搜索和筛选相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include <string>
#include <crow/json.h>

namespace Service {
    class TalentSearchService {
    public:
        struct SearchResult {
            bool success;
            std::string message;
            crow::json::wvalue talents;
        };
        
        struct FilterResult {
            bool success;
            std::string message;
            crow::json::wvalue talents;
        };
        
        static SearchResult searchTalents(const std::string& enterpriseId, const crow::json::rvalue& searchCriteria);
        static FilterResult filterTalents(const std::string& enterpriseId, const crow::json::rvalue& filterCriteria);
    };
}