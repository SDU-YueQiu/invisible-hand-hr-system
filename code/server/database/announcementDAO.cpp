/**
 * @file announcementDAO.cpp
 * @brief 公告数据访问对象类实现，封装公告表数据库操作
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#include "announcementDAO.h"
#include "../Model/announcement.h"
#include "databaseManager.h"
#include <crow/logging.h>
#include <stdexcept>
#include <string>

namespace DAL
{
    using namespace Model;

    Announcement createFromRow(DbRow &row)
    {
        try
        {
            return Announcement{
                    std::get<int64_t>(row.at("AnnouncementID")),
                    std::get<int64_t>(row.at("AdminID")),
                    std::get<std::string>(row.at("Title")),
                    std::get<std::string>(row.at("Content")),
                    std::get<std::string>(row.at("PublishTime")),
                    std::get<std::string>(row.at("EffectiveTime")),
                    std::get<std::string>(row.at("ExpireTime")),
                    static_cast<bool>(std::get<int64_t>(row.at("IsTop")))};
        } catch (const std::exception &e)
        {
            CROW_LOG_ERROR << "创建公告对象失败: " << e.what();
            for (const auto &[key, value]: row)
            {
                CROW_LOG_ERROR << "列名: " << key << ", 类型: " << value.index();
            }
            return Announcement{};
        }
    }
    Announcement AnnouncementDAO::findById(int64_t announcementId)
    {
        CROW_LOG_INFO << "查询公告信息，ID: " << announcementId;
        const std::string sql = "SELECT * FROM Announcements WHERE AnnouncementID = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(announcementId)});

        if (!result || result->empty())
        {
            CROW_LOG_WARNING << "未找到公告信息，ID: " << announcementId;
            return Announcement{};
        }
        return createFromRow(result->front());
    }

    std::vector<Announcement> AnnouncementDAO::findAll(const std::string &filter)
    {
        CROW_LOG_INFO << "查询公告列表，筛选条件: " << (filter.empty() ? "无" : filter);
        std::string sql = "SELECT * FROM Announcements";
        if (!filter.empty())
        {
            sql += " WHERE " + filter;
        }

        auto result = dbManager.executeQuery(sql, {});
        std::vector<Announcement> announcements;
        if (!result) return announcements;

        for (auto &row: *result)
        {
            announcements.push_back(createFromRow(row));
        }
        return announcements;
    }

    bool AnnouncementDAO::create(const Announcement &announcementData)
    {
        CROW_LOG_INFO << "创建新公告，标题: " << announcementData.Title;
        const std::string sql = R"(
            INSERT INTO Announcements (
                AdminID, Title, Content, PublishTime,
                EffectiveTime, ExpireTime, IsTop
            ) VALUES (?, ?, ?, ?, ?, ?, ?)
        )";

        std::vector<std::string> params = {
                std::to_string(announcementData.AdminID),
                announcementData.Title,
                announcementData.Content,
                (announcementData.PublishTime),
                (announcementData.EffectiveTime),
                (announcementData.ExpireTime),
                announcementData.IsTop ? "1" : "0"};

        auto result = dbManager.executeQuery(sql, params);
        return !result->empty();
    }

    bool AnnouncementDAO::update(int64_t announcementId, const Announcement &announcementData)
    {
        CROW_LOG_INFO << "更新公告信息，ID: " << announcementId;
        const std::string sql = R"(
            UPDATE Announcements SET
                AdminID = ?, Title = ?, Content = ?,
                PublishTime = ?, EffectiveTime = ?,
                ExpireTime = ?, IsTop = ?
            WHERE AnnouncementID = ?
        )";

        std::vector<std::string> params = {
                std::to_string(announcementData.AdminID),
                announcementData.Title,
                announcementData.Content,
                (announcementData.PublishTime),
                (announcementData.EffectiveTime),
                (announcementData.ExpireTime),
                announcementData.IsTop ? "1" : "0",
                std::to_string(announcementId)};

        auto result = dbManager.executeQuery(sql, params);
        return result != nullptr;
    }

    bool AnnouncementDAO::deleteById(int64_t announcementId)
    {
        CROW_LOG_INFO << "删除公告，ID: " << announcementId;
        const std::string sql = "DELETE FROM Announcements WHERE AnnouncementID = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(announcementId)});
        return result != nullptr;
    }
}// namespace DAL