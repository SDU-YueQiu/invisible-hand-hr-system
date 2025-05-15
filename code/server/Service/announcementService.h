/**
 * @file announcementService.h
 * @brief 公告服务类，处理系统公告相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#pragma once

#include "../Database/announcementDAO.h"
#include "../Model/announcement.h"
#include <string>
#include <vector>


namespace Service
{
    /**
     * @brief 公告服务类，封装公告相关业务逻辑
     */
    class AnnouncementService
    {
    public:
        /**
         * @brief 获取AnnouncementService单例实例（线程安全）
         * @return AnnouncementService& AnnouncementService实例
         */
        static AnnouncementService &getInstance()
        {
            thread_local static AnnouncementService instance;
            return instance;
        }

        /**
         * @brief 创建新公告
         * @param announcementData 公告数据
         * @return bool 创建成功返回true，否则返回false
         */
        bool createAnnouncement(const Model::Announcement &announcementData);

        /**
         * @brief 获取公告列表
         * @param filter 筛选条件
         * @return std::vector<Model::Announcement> 公告列表
         */
        std::vector<Model::Announcement> getAnnouncements(const std::string &filter = "");

        /**
         * @brief 更新公告信息
         * @param announcementId 公告ID
         * @param announcementData 更新后的公告数据
         * @return bool 更新成功返回true，否则返回false
         */
        bool updateAnnouncement(int64_t announcementId, const Model::Announcement &announcementData);

        /**
         * @brief 删除公告
         * @param announcementId 公告ID
         * @return bool 删除成功返回true，否则返回false
         */
        bool deleteAnnouncement(int64_t announcementId);

    private:
        DAL::AnnouncementDAO &announcementDAO = DAL::AnnouncementDAO::getInstance();
    };
}// namespace Service