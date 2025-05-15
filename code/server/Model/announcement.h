/**
 * @file announcement.h
 * @brief 系统公告模型类定义
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include <string>

namespace Model
{
    struct Announcement
    {
        int64_t AnnouncementID = -1;
        int64_t AdminID = -1;
        std::string Title;
        std::string Content;
        time_t PublishTime = 0;
        time_t EffectiveTime = 0;
        time_t ExpireTime = 0;
        bool IsTop = false;
    };
}// namespace Model