/**
 * @file EnterpriseUser.h
 * @brief 企业用户模型类，定义企业用户数据结构
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#ifndef ENTERPRISE_USER_H
#define ENTERPRISE_USER_H

#include <cstdint>
#include <string>

namespace Model
{

    /**
 * @brief 企业用户数据模型，对应数据库EnterpriseUsers表
 */
    struct EnterpriseUser
    {
        int64_t EnterpriseID = -1;                  ///< 企业唯一标识（主键，自增）
        std::string LoginUsername;                  ///< 企业登录用户名（唯一）
        std::string PasswordHash;                   ///< 加盐哈希后的密码
        std::string EnterpriseName;                 ///< 企业全称
        std::string CreditCode;                     ///< 统一社会信用代码（唯一）
        std::string Description;                    ///< 企业简介
        std::string Industry;                       ///< 所属行业
        std::string Scale;                          ///< 企业规模
        std::string Address;                        ///< 企业地址
        std::string ContactPerson;                  ///< 联系人姓名
        std::string ContactPhone;                   ///< 联系人手机
        std::string ContactEmail;                   ///< 联系人邮箱
        std::string LogoURL;                        ///< 企业Logo路径/URL
        std::string LicenseImageURL;                ///< 营业执照图片路径/URL（审核用）
        std::string RegistrationDate;               ///< 注册时间（默认当前时间）
        std::string AccountStatus = "PendingReview";///< 账户状态（默认'PendingReview'）
        std::string AuditOpinion;                   ///< 审核意见（管理员填写）
    };

}// namespace Model

#endif// ENTERPRISE_USER_H