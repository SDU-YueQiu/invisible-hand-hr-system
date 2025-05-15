/**
 * @file announcementDAO.h
 * @brief 公告数据访问对象类声明，封装公告表数据库操作
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once

#include "../Model/announcement.h"
#include "databaseManager.h"
#include <vector>

namespace DAL
{
    class AnnouncementDAO
    {
    public:
        /**
     * @brief 获取 AnnouncementDAO 单例实例
     * @return AnnouncementDAO& AnnouncementDAO 单例实例的引用
     */
        static AnnouncementDAO &getInstance()
        {
            thread_local static AnnouncementDAO instance;
            return instance;
        }
        
        // 私有构造函数，防止外部实例化
        AnnouncementDAO() = default;
        // 禁止拷贝构造函数
        AnnouncementDAO(const AnnouncementDAO &) = delete;
        // 禁止赋值运算符
        AnnouncementDAO &operator=(const AnnouncementDAO &) = delete;
        /**
         * @brief 根据公告ID查询公告信息
         * @param announcementId 公告ID
         * @return Model::Announcement 公告对象
         */
        Model::Announcement findById(int64_t announcementId);

        /**
         * @brief 根据筛选条件查询公告列表
         * @param filter 筛选条件
         * @return std::vector<Model::Announcement> 公告列表
         */
        std::vector<Model::Announcement> findAll(const std::string &filter = "");

        /**
         * @brief 创建新的公告记录
         * @param announcementData 公告数据
         * @return bool 创建成功返回true
         */
        bool create(const Model::Announcement &announcementData);

        /**
         * @brief 更新公告信息
         * @param announcementId 公告ID
         * @param announcementData 更新后的公告数据
         * @return bool 更新成功返回true
         */
        bool update(int64_t announcementId, const Model::Announcement &announcementData);

        /**
         * @brief 删除公告记录
         * @param announcementId 公告ID
         * @return bool 删除成功返回true
         */
        bool deleteById(int64_t announcementId);

    private:
        DatabaseManager &dbManager = DatabaseManager::getInstance();
    };
}// namespace DAL