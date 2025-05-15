/**
 * @file jobController.h
 * @brief 职位信息控制器，处理职位查询和申请相关API请求
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.0
 */

#pragma once

#include "../Service/applicationService.h"
#include "../Service/jobPostingService.h"
#include <crow.h>


namespace Router
{
    /**
     * @brief 职位信息控制器，处理职位查询和申请相关操作
     */
    class JobController
    {
    public:
        /**
         * @brief 查询职位列表(支持筛选和分页)
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void getJobs(const crow::request &request, crow::response &response);

        /**
         * @brief 获取指定ID的职位详情
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void getJobDetails(const crow::request &request, crow::response &response);

        /**
         * @brief 申请职位
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void applyForJob(const crow::request &request, crow::response &response);

        /**
         * @brief 获取当前用户的职位申请记录
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void getApplications(const crow::request &request, crow::response &response);
    };
}// namespace Router