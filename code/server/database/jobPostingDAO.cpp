/**
 * @file jobPostingDAO.cpp
 * @brief 招聘岗位数据访问对象类实现，封装招聘岗位表数据库操作
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#include "jobPostingDAO.h"
#include "../Model/jobPosting.h"
#include "databaseManager.h"
#include <crow/logging.h>
#include <cstdint>
#include <stdexcept>

namespace DAL
{
    using namespace Model;

    /**
     * @brief 从数据库结果行构造招聘岗位对象
     * @param row 数据库结果行（DbRow类型）
     * @return Model::JobPosting 招聘岗位对象（字段不匹配时返回空对象）
     */
    Model::JobPosting createJobPostingFromRow(DbRow &row)
    {
        try
        {
            return Model::JobPosting{
                    std::get<int64_t>(row.at("JobID")),
                    std::get<int64_t>(row.at("EnterpriseID")),
                    std::get<std::string>(row.at("JobTitle")),
                    std::get<std::string>(row.at("JobCategory")),
                    int(std::get<int64_t>(row.at("RecruitmentCount"))),
                    std::get<std::string>(row.at("WorkLocation")),
                    int(std::get<int64_t>(row.at("MaxSalary"))),
                    int(std::get<int64_t>(row.at("MinSalary"))),
                    std::get<std::string>(row.at("Responsibilities")),
                    std::get<std::string>(row.at("Requirements")),
                    std::get<std::string>(row.at("ExperienceRequired")),
                    std::get<std::string>(row.at("EducationRequired")),
                    std::get<std::string>(row.at("Benefits")),
                    std::get<std::string>(row.at("PublishDate")),
                    std::get<std::string>(row.at("UpdateDate")),
                    std::get<std::string>(row.at("DeadlineDate")),
                    std::get<std::string>(row.at("JobStatus"))};
        } catch (const std::exception &e)
        {
            std::stringstream ss;
            for (const auto &[key, val]: row)
            {
                ss << key << "=" << val.index() << "; ";// 输出字段类型索引
            }
            CROW_LOG_ERROR << "创建招聘岗位对象失败: " << e.what()
                           << " 行数据: " << ss.str();
            return Model::JobPosting{};
        }
    }

    /**
     * @brief 根据岗位ID查询招聘岗位信息
     * @param jobId 岗位唯一标识
     * @return Model::JobPosting 招聘岗位对象（若不存在则返回空对象）
     */
    Model::JobPosting JobPostingDAO::findById(int64_t jobId)
    {
        CROW_LOG_INFO << "查询招聘岗位，ID: " << jobId;
        const std::string sql = "SELECT * FROM JobPostings WHERE JobID = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(jobId)});

        if (!result || result->empty())
        {
            CROW_LOG_WARNING << "未找到招聘岗位，ID: " << jobId;
            return Model::JobPosting{};
        }
        return createJobPostingFromRow(result->front());
    }

    /**
     * @brief 根据企业ID查询所有招聘岗位
     * @param enterpriseId 发布企业ID
     * @return std::vector<Model::JobPosting> 招聘岗位列表
     */
    std::vector<Model::JobPosting> JobPostingDAO::findByEnterpriseId(int64_t enterpriseId)
    {
        CROW_LOG_INFO << "查询企业发布的招聘岗位，企业ID: " << enterpriseId;
        const std::string sql = "SELECT * FROM JobPostings WHERE EnterpriseID = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(enterpriseId)});

        std::vector<Model::JobPosting> jobPostings;
        if (!result) return jobPostings;

        for (auto &row: *result)
        {
            jobPostings.push_back(createJobPostingFromRow(row));
        }
        return jobPostings;
    }

    /**
     * @brief 向数据库中插入一条新的招聘岗位记录
     * @param jobData 待插入的招聘岗位数据
     * @return 插入成功返回true，否则返回false
     */
    bool JobPostingDAO::create(const Model::JobPosting &jobData)
    {
        CROW_LOG_INFO << "创建新招聘岗位，企业ID: " << jobData.EnterpriseID;
        const std::string sql = R"(
            INSERT INTO JobPostings (
                EnterpriseID, JobTitle, JobCategory, RecruitmentCount,
                WorkLocation, MaxSalary, MinSalary, Responsibilities, Requirements,
                ExperienceRequired, EducationRequired, Benefits,
                PublishDate, UpdateDate, DeadlineDate, JobStatus
            ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
        )";

        std::vector<std::string> params = {
                std::to_string(jobData.EnterpriseID),
                jobData.JobTitle,
                jobData.JobCategory,
                std::to_string(jobData.RecruitmentCount),
                jobData.WorkLocation,
                std::to_string(jobData.MaxSalary),
                std::to_string(jobData.MinSalary),
                jobData.Responsibilities,
                jobData.Requirements,
                jobData.ExperienceRequired,
                jobData.EducationRequired,
                jobData.Benefits,
                jobData.PublishDate,
                jobData.UpdateDate,
                jobData.DeadlineDate,
                jobData.JobStatus};

        auto result = dbManager.executeQuery(sql, params);
        return !result->empty();
    }

    /**
     * @brief 更新指定ID的招聘岗位信息
     * @param jobId 待更新的岗位ID
     * @param jobData 更新后的招聘岗位数据
     * @return bool 更新成功返回true，否则返回false
     */
    bool JobPostingDAO::update(int64_t jobId, const Model::JobPosting &jobData)
    {
        CROW_LOG_INFO << "更新招聘岗位，ID: " << jobId;
        const std::string sql = R"(
            UPDATE JobPostings SET
                EnterpriseID = ?, JobTitle = ?, JobCategory = ?, RecruitmentCount = ?,
                WorkLocation = ?, MaxSalary = ?, MinSalary = ?, Responsibilities = ?, Requirements = ?,
                ExperienceRequired = ?, EducationRequired = ?, Benefits = ?,
                PublishDate = ?, UpdateDate = ?, DeadlineDate = ?, JobStatus = ?
            WHERE JobID = ?
        )";

        std::vector<std::string> params = {
                std::to_string(jobData.EnterpriseID),
                jobData.JobTitle,
                jobData.JobCategory,
                std::to_string(jobData.RecruitmentCount),
                jobData.WorkLocation,
                std::to_string(jobData.MaxSalary),
                std::to_string(jobData.MinSalary),
                jobData.Responsibilities,
                jobData.Requirements,
                jobData.ExperienceRequired,
                jobData.EducationRequired,
                jobData.Benefits,
                jobData.PublishDate,
                jobData.UpdateDate,
                jobData.DeadlineDate,
                jobData.JobStatus,
                std::to_string(jobId)};

        auto result = dbManager.executeQuery(sql, params);
        return result != nullptr;
    }

    /**
     * @brief 删除指定ID的招聘岗位记录
     * @param jobId 待删除的岗位ID
     * @return bool 删除成功返回true，否则返回false
     */
    bool JobPostingDAO::deleteById(int64_t jobId)
    {
        CROW_LOG_INFO << "删除招聘岗位，ID: " << jobId;
        const std::string sql = "DELETE FROM JobPostings WHERE JobID = ?";
        auto result = dbManager.executeQuery(sql, {std::to_string(jobId)});
        return result != nullptr;
    }

    /**
    * @brief 根据筛选条件查询招聘岗位信息
    * @param filter SQL筛选条件字符串
    * @return std::vector<Model::JobPosting> 符合条件的招聘岗位列表
    */
    std::vector<Model::JobPosting> JobPostingDAO::findByFilter(const std::string &filter)
    {
        CROW_LOG_INFO << "查询招聘岗位，筛选条件: " << (filter.empty() ? "无" : filter);
        std::string sql = "SELECT * FROM JobPostings";
        if (!filter.empty())
        {
            sql += " WHERE " + filter;
        }

        auto result = dbManager.executeQuery(sql, {});
        std::vector<Model::JobPosting> jobPostings;
        if (!result) return jobPostings;

        for (auto &row: *result)
        {
            jobPostings.push_back(createJobPostingFromRow(row));
        }
        return jobPostings;
    }
}// namespace DAL