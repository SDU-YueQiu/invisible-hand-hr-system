/**
 * @file announcementService.cpp
 * @brief 公告服务类实现，处理系统公告相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#include "announcementService.h"
#include <crow/logging.h>

namespace Service
{
    bool AnnouncementService::createAnnouncement(const Model::Announcement &announcementData)
    {
        CROW_LOG_INFO << "Creating new announcement, title: " << announcementData.Title;

        // 验证管理员ID有效性
        if (announcementData.AdminID <= 0)
        {
            CROW_LOG_WARNING << "Invalid admin ID for announcement creation";
            return false;
        }

        // 验证时间有效性
        // if (announcementData.EffectiveTime >= announcementData.ExpireTime)
        // {
        //     CROW_LOG_WARNING << "Invalid time range for announcement";
        //     return false;
        // }

        return announcementDAO.create(announcementData);
    }

    std::vector<Model::Announcement> AnnouncementService::getAnnouncements(const std::string &filter)
    {
        CROW_LOG_INFO << "Getting announcements with filter: " << (filter.empty() ? "none" : filter);

        // 获取当前时间，用于筛选有效公告
        auto now = std::time(nullptr);
        std::string effectiveFilter;//= "EffectiveTime <= datetime('now') AND ExpireTime >= datetime('now')";

        if (!filter.empty())
        {
            effectiveFilter += filter;
        }

        return announcementDAO.findAll(effectiveFilter);
    }

    bool AnnouncementService::updateAnnouncement(int64_t announcementId, const Model::Announcement &announcementData)
    {
        CROW_LOG_INFO << "Updating announcement, ID: " << announcementId;

        // 获取现有公告信息
        auto existingAnnouncement = announcementDAO.findById(announcementId);
        if (existingAnnouncement.AnnouncementID == -1)
        {
            CROW_LOG_WARNING << "Announcement not found for update, ID: " << announcementId;
            return false;
        }
        return announcementDAO.update(announcementId, announcementData);
    }

    bool AnnouncementService::deleteAnnouncement(int64_t announcementId)
    {
        CROW_LOG_INFO << "Deleting announcement, ID: " << announcementId;
        return announcementDAO.deleteById(announcementId);
    }
}// namespace Service