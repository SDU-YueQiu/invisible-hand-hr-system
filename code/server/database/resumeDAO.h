/**
 * @file ResumeDAO.h
 * @brief 简历数据访问对象类，封装简历表数据库操作
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#ifndef RESUME_DAO_H
#define RESUME_DAO_H

#include "../Database/databaseManager.h"
#include "../Model/resume.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <cstdint>
#include <string>
#include <vector>


namespace DAL
{

    /**
  * @brief 简历数据访问对象，负责简历表的CRUD操作
  * @note 使用SQLiteCpp库封装，通过DatabaseManager获取数据库连接
  */
    class ResumeDAO
    {
    public:
        /**
      * @brief 根据简历ID查询简历信息
      * @param resumeId 简历唯一标识
      * @return Model::Resume 简历对象（若不存在则返回空对象）
      */
        Model::Resume findById(int resumeId);

        /**
      * @brief 根据用户ID查询该用户的所有简历
      * @param userId 用户唯一标识
      * @return std::vector<Model::Resume> 简历对象列表（可能为空）
      */
        std::vector<Model::Resume> findByUserId(int userId);

        /**
      * @brief 向数据库中插入一条新的简历记录
      * @param resumeData 待插入的简历数据
      * @return 插入成功返回true，否则返回false
      */
        bool create(const Model::Resume &resumeData);

        /**
      * @brief 更新指定ID的简历信息
      * @param resumeId 待更新的简历ID
      * @param resumeData 更新后的简历数据
      * @return bool 更新成功返回true，否则返回false
      */
        bool update(int resumeId, const Model::Resume &resumeData);

        /**
      * @brief 删除指定ID的简历记录
      * @param resumeId 待删除的简历ID
      * @return bool 删除成功返回true，否则返回false
      */
        bool deleteById(int resumeId);

    private:
        DatabaseManager &dbManager = DatabaseManager::getInstance();// 获取数据库管理器单例
    };

}// namespace DAL

#endif// RESUME_DAO_H