/**
 * @file resumeController.h
 * @brief 处理个人用户简历相关API请求的控制器
 * @author SDU-YueQiu
 * @date 2025/5/16
 * @version 1.0
 */

#pragma once

#include "../Service/resumeService.h"
#include <crow.h>


namespace Router
{
    /**
     * @brief 简历管理控制器，处理简历相关的CRUD操作
     */
    class ResumeController
    {
    public:
        /**
         * @brief 为当前用户创建新简历
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void createResume(const crow::request &request, crow::response &response);

        /**
         * @brief 获取指定ID的简历详情
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void getResume(const crow::request &request, crow::response &response, int id);

        /**
         * @brief 获取当前用户的所有简历列表
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void getResumes(const crow::request &request, crow::response &response);

        /**
         * @brief 更新指定ID的简历信息
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void updateResume(const crow::request &request, crow::response &response,int id);

        /**
         * @brief 删除指定ID的简历
         * @param request Crow请求对象
         * @param response Crow响应对象
         */
        static void deleteResume(const crow::request &request, crow::response &response);
    };
}// namespace Router