/**
 * @file ResumeModel.h
 * @brief 简历模型类，定义简历数据结构
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

 #ifndef RESUME_MODEL_H
 #define RESUME_MODEL_H
 
 #include <cstdint>
#include <string>
 #include <vector>
 
 namespace Model {
 
 /**
  * @brief 简历数据模型，对应数据库Resumes表
  */
 struct Resume {
     int64_t ResumeID;               ///< 简历唯一标识（主键，自增）
     int64_t UserID;                 ///< 所属用户ID（外键到IndividualUsers）
     std::string ResumeTitle;    ///< 简历标题
     std::string BasicInfo;      ///< 基本信息（JSON格式：姓名、性别、出生日期等）
     std::string JobIntent;      ///< 求职意向（JSON格式：期望行业、职位、薪资等）
     std::string EducationExperience; ///< 教育经历（JSON数组）
     std::string WorkExperience; ///< 工作经历（JSON数组）
     std::string ProjectExperience; ///< 项目经验（JSON数组）
     std::string SkillsCertificates; ///< 技能证书（JSON数组）
     std::string SelfDescription; ///< 自我评价
     std::string LastUpdateTime; ///< 最后更新时间（默认当前时间）
     std::string VisibilityStatus; ///< 公开状态（默认'Private'）
     std::string AttachmentPath; ///< 附件简历路径（可选）
 };
 
 } // namespace Model
 
 #endif // RESUME_MODEL_H