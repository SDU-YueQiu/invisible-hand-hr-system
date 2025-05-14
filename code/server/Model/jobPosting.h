/**
 * @file JobPosting.h
 * @brief 招聘岗位模型类，定义招聘岗位数据结构
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#ifndef JOB_POSTING_H
#define JOB_POSTING_H

#include <cstdint>
#include <string>

namespace Model {

/**
 * @brief 招聘岗位数据模型，对应数据库JobPostings表
 */
struct JobPosting {
    int64_t JobID;                  ///< 岗位唯一标识（主键，自增）
    int64_t EnterpriseID;           ///< 发布企业ID（外键到EnterpriseUsers）
    std::string JobTitle;           ///< 岗位名称（非空）
    std::string JobCategory;        ///< 职位类别
    int32_t RecruitmentCount;       ///< 招聘人数
    std::string WorkLocation;       ///< 工作地点
    std::string SalaryRange;        ///< 薪资范围
    std::string Responsibilities;   ///< 岗位职责（非空）
    std::string Requirements;       ///< 任职要求（非空）
    std::string ExperienceRequired; ///< 工作经验要求
    std::string EducationRequired;  ///< 学历要求
    std::string Benefits;           ///< 福利待遇
    std::string PublishDate;        ///< 发布时间（默认当前时间）
    std::string UpdateDate;         ///< 更新时间
    std::string DeadlineDate;       ///< 截止日期（可选）
    std::string JobStatus;          ///< 岗位状态（非空，默认'Recruiting'）
};

} // namespace Model

#endif // JOB_POSTING_H