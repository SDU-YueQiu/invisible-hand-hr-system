/**
 * @file announcement.h
 * @brief 系统公告模型类定义
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include <string>

namespace Model {
    struct Announcement {
        int64_t AnnouncementID = -1;
        int64_t AdminID;
        std::string Title;
        std::string Content;
        std::string PublishTime;
        std::string EffectiveTime;
        std::string ExpireTime;
        bool IsTop = false;
    };
}