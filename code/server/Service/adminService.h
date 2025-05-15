/**
 * @file adminService.h
 * @brief 管理员服务类，处理管理员相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/15
 * @version 1.0
 */

#pragma once

#include "../Database/adminUserDAO.h"
#include "../Database/enterpriseUserDAO.h"
#include "../Database/individualUserDAO.h"
#include "../Database/jobPostingDAO.h"
#include "../Model/adminUser.h"
#include "../Model/enterpriseUser.h"
#include "../Model/individualUser.h"
#include "../Model/jobPosting.h"
#include <string>
#include <vector>


namespace Service
{
    /**
     * @brief 管理员服务类，封装管理员相关业务逻辑
     */
    class AdminService
    {
    public:
        /**
         * @brief 获取AdminService单例实例（线程安全）
         * @return AdminService& AdminService实例
         */
        static AdminService &getInstance()
        {
            thread_local static AdminService instance;
            return instance;
        }

        /**
         * @brief 获取个人用户列表
         * @param filter 筛选条件
         * @return std::vector<Model::IndividualUser> 用户列表
         */
        std::vector<Model::IndividualUser> getUsers(const std::string &filter);

        /**
         * @brief 更新个人用户状态
         * @param userId 用户ID
         * @param status 新状态
         * @return bool 更新成功返回true，否则返回false
         */
        bool updateUserStatus(int userId, const std::string &status);

        /**
         * @brief 获取企业用户列表
         * @param filter 筛选条件
         * @return std::vector<Model::EnterpriseUser> 企业列表
         */
        std::vector<Model::EnterpriseUser> getEnterprises(const std::string &filter);

        /**
         * @brief 批准企业注册
         * @param enterpriseId 企业ID
         * @param opinion 审核意见
         * @return bool 操作成功返回true，否则返回false
         */
        bool approveEnterprise(int64_t enterpriseId, const std::string &opinion);

        /**
         * @brief 拒绝企业注册
         * @param enterpriseId 企业ID
         * @param opinion 拒绝理由
         * @return bool 操作成功返回true，否则返回false
         */
        bool rejectEnterprise(int64_t enterpriseId, const std::string &opinion);

        /**
         * @brief 获取所有职位列表
         * @param filter 筛选条件
         * @return std::vector<Model::JobPosting> 职位列表
         */
        std::vector<Model::JobPosting> getAllJobs(const std::string &filter);

        /**
         * @brief 更新职位状态
         * @param jobId 职位ID
         * @param status 新状态
         * @return bool 更新成功返回true，否则返回false
         */
        bool updateJobStatus(int64_t jobId, const std::string &status);

    private:
        DAL::IndividualUserDAO &individualUserDAO = DAL::IndividualUserDAO::getInstance();
        DAL::EnterpriseUserDAO &enterpriseUserDAO = DAL::EnterpriseUserDAO::getInstance();
        DAL::JobPostingDAO &jobPostingDAO = DAL::JobPostingDAO::getInstance();
    };
}// namespace Service