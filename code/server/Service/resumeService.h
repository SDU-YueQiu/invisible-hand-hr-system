/**
 * @file resumeService.h
 * @brief 简历服务类，处理简历相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#pragma once

#include "../Model/resume.h"
#include "../Database/resumeDAO.h"
#include <vector>
#include <string>

namespace Service
{
    /**
     * @brief 简历服务类，封装简历相关业务逻辑
     */
    class ResumeService
    {
    public:
        /**
         * @brief 获取ResumeService单例实例（线程安全）
         * @return ResumeService& ResumeService实例
         */
        static ResumeService& getInstance()
        {
            thread_local static ResumeService instance;
            return instance;
        }

        /**
         * @brief 为指定用户创建新简历
         * @param userId 用户ID
         * @param resumeData 简历数据
         * @return bool 创建成功返回true，否则返回false
         */
        bool createResume(int userId, const Model::Resume& resumeData);

        /**
         * @brief 根据简历ID获取简历详情
         * @param resumeId 简历ID
         * @return Model::Resume 简历对象
         */
        Model::Resume getResumeById(int resumeId);

        /**
         * @brief 获取用户的所有简历列表
         * @param userId 用户ID
         * @return std::vector<Model::Resume> 简历列表
         */
        std::vector<Model::Resume> getResumesByUserId(int userId);

        /**
         * @brief 更新简历信息
         * @param resumeId 简历ID
         * @param resumeData 更新后的简历数据
         * @return bool 更新成功返回true，否则返回false
         */
        bool updateResume(int resumeId, const Model::Resume& resumeData);

        /**
         * @brief 删除简历
         * @param resumeId 简历ID
         * @return bool 删除成功返回true，否则返回false
         */
        bool deleteResume(int resumeId);

    private:
        DAL::ResumeDAO& resumeDAO = DAL::ResumeDAO::getInstance();
    };

} // namespace Service