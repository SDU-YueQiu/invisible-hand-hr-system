/**
 * @file jobApplication.h
 * @brief 职位申请模型类定义
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#pragma once
#include <chrono>
#include <string>


namespace Model
{
    struct JobApplication
    {
        int64_t ApplicationID = -1; // 申请记录ID
        int64_t UserID;             // 申请人ID
        int64_t ResumeID;           // 简历ID
        int64_t JobID;              // 职位ID
        int64_t EnterpriseID;       // 企业ID
        std::string ApplicationDate;// 申请日期
        std::string Status;         // 申请状态(如:待处理/已查看/已拒绝/已录用)
        std::string Feedback;       // 企业反馈
        std::string UpdateTime;     // 最后更新时间
    };
}// namespace Model