/**
 * @file JobPostingDAO.h
 * @brief 招聘岗位数据访问对象类，封装招聘岗位表数据库操作
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#ifndef JOB_POSTING_DAO_H
#define JOB_POSTING_DAO_H

#include "databaseManager.h"
#include "../Model/jobPosting.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <cstdint>
#include <string>
#include <vector>

namespace DAL {

/**
 * @brief 招聘岗位数据访问对象，负责招聘岗位表的CRUD操作
 * @note 使用SQLiteCpp库封装，通过DatabaseManager获取数据库连接
 */
class JobPostingDAO {
public:
    /**
     * @brief 根据岗位ID查询招聘岗位信息
     * @param jobId 岗位唯一标识
     * @return Model::JobPosting 招聘岗位对象（若不存在则返回空对象）
     */
    Model::JobPosting findById(int64_t jobId);

    /**
     * @brief 根据企业ID查询所有招聘岗位
     * @param enterpriseId 发布企业ID
     * @return std::vector<Model::JobPosting> 招聘岗位列表
     */
    std::vector<Model::JobPosting> findByEnterpriseId(int64_t enterpriseId);

    /**
     * @brief 向数据库中插入一条新的招聘岗位记录
     * @param jobData 待插入的招聘岗位数据
     * @return 插入成功返回true，否则返回false
     */
    bool create(const Model::JobPosting& jobData);

    /**
     * @brief 更新指定ID的招聘岗位信息
     * @param jobId 待更新的岗位ID
     * @param jobData 更新后的招聘岗位数据
     * @return bool 更新成功返回true，否则返回false
     */
    bool update(int64_t jobId, const Model::JobPosting& jobData);

    /**
     * @brief 删除指定ID的招聘岗位记录
     * @param jobId 待删除的岗位ID
     * @return bool 删除成功返回true，否则返回false
     */
    bool deleteById(int64_t jobId);

private:
    DatabaseManager& dbManager = DatabaseManager::getInstance(); // 获取数据库管理器单例
};

} // namespace DAL

#endif // JOB_POSTING_DAO_H