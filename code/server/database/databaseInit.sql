/**
 * @file databaseInit.sql
 * @brief 数据库初始化脚本
 * @author SDU-YueQiu
 * @date 2025/5/12
 * @version 1.1
*/

-- 创建个人用户表
CREATE TABLE IF NOT EXISTS IndividualUsers (
    UserID INTEGER PRIMARY KEY AUTOINCREMENT,
    Username TEXT UNIQUE NOT NULL,
    PasswordHash TEXT NOT NULL,
    PhoneNumber TEXT UNIQUE,
    Email TEXT UNIQUE,
    RegistrationDate TEXT DEFAULT CURRENT_TIMESTAMP,
    LastLoginDate TEXT,
    AccountStatus TEXT NOT NULL DEFAULT 'Active',
    AvatarURL TEXT
);

-- 创建企业用户表
CREATE TABLE IF NOT EXISTS EnterpriseUsers (
    EnterpriseID INTEGER PRIMARY KEY AUTOINCREMENT,
    LoginUsername TEXT UNIQUE NOT NULL,
    PasswordHash TEXT NOT NULL,
    EnterpriseName TEXT NOT NULL,
    CreditCode TEXT UNIQUE NOT NULL,
    Description TEXT,
    Industry TEXT,
    Scale TEXT,
    Address TEXT,
    ContactPerson TEXT,
    ContactPhone TEXT UNIQUE,
    ContactEmail TEXT UNIQUE,
    LogoURL TEXT,
    LicenseImageURL TEXT NOT NULL,
    RegistrationDate TEXT DEFAULT CURRENT_TIMESTAMP,
    AccountStatus TEXT NOT NULL DEFAULT 'PendingReview',
    AuditOpinion TEXT
);

-- 创建简历表
CREATE TABLE IF NOT EXISTS Resumes (
    ResumeID INTEGER PRIMARY KEY AUTOINCREMENT,
    UserID INTEGER NOT NULL,
    ResumeTitle TEXT,
    BasicInfo TEXT,  -- JSON格式
    JobIntent TEXT,  -- JSON格式
    EducationExperience TEXT,  -- JSON数组
    WorkExperience TEXT,  -- JSON数组
    ProjectExperience TEXT,  -- JSON数组
    SkillsCertificates TEXT,  -- JSON数组
    SelfDescription TEXT,
    LastUpdateTime TEXT DEFAULT CURRENT_TIMESTAMP,
    VisibilityStatus TEXT DEFAULT 'Private',
    AttachmentPath TEXT,
    FOREIGN KEY (UserID) REFERENCES IndividualUsers(UserID)
);

-- 创建招聘岗位表
CREATE TABLE IF NOT EXISTS JobPostings (
    JobID INTEGER PRIMARY KEY AUTOINCREMENT,
    EnterpriseID INTEGER NOT NULL,
    JobTitle TEXT NOT NULL,
    JobCategory TEXT,
    RecruitmentCount INTEGER,
    WorkLocation TEXT,
    MaxSalary INTEGER,
    MinSalary INTEGER,
    Responsibilities TEXT NOT NULL,
    Requirements TEXT NOT NULL,
    ExperienceRequired TEXT,
    EducationRequired TEXT,
    Benefits TEXT,
    PublishDate TEXT DEFAULT CURRENT_TIMESTAMP,
    UpdateDate TEXT,
    DeadlineDate TEXT,
    JobStatus TEXT NOT NULL DEFAULT 'Recruiting',
    FOREIGN KEY (EnterpriseID) REFERENCES EnterpriseUsers(EnterpriseID)
);

-- 创建职位申请记录表
CREATE TABLE IF NOT EXISTS JobApplications (
    ApplicationID INTEGER PRIMARY KEY AUTOINCREMENT,
    UserID INTEGER NOT NULL,
    ResumeID INTEGER NOT NULL,
    JobID INTEGER NOT NULL,
    EnterpriseID INTEGER NOT NULL,
    ApplicationTime TEXT DEFAULT CURRENT_TIMESTAMP,
    CurrentStatus TEXT NOT NULL DEFAULT 'Submitted',
    EnterpriseNotes TEXT,
    FOREIGN KEY (UserID) REFERENCES IndividualUsers(UserID),
    FOREIGN KEY (ResumeID) REFERENCES Resumes(ResumeID),
    FOREIGN KEY (JobID) REFERENCES JobPostings(JobID),
    FOREIGN KEY (EnterpriseID) REFERENCES EnterpriseUsers(EnterpriseID)
);

-- 创建管理员用户表
CREATE TABLE IF NOT EXISTS AdminUsers (
    AdminID INTEGER PRIMARY KEY AUTOINCREMENT,
    AdminUsername TEXT UNIQUE NOT NULL,
    PasswordHash TEXT NOT NULL,
    Role TEXT NOT NULL DEFAULT 'Admin'
);

-- 创建系统留言/反馈表
CREATE TABLE IF NOT EXISTS Feedbacks (
    FeedbackID INTEGER PRIMARY KEY AUTOINCREMENT,
    UserID INTEGER,
    UserType TEXT,
    FeedbackType TEXT,
    Content TEXT NOT NULL,
    ContactInfo TEXT,
    SubmitTime TEXT DEFAULT CURRENT_TIMESTAMP,
    ProcessStatus TEXT NOT NULL DEFAULT 'Pending',
    AdminReply TEXT,
    ReplyTime TEXT
);

-- 创建系统公告表
CREATE TABLE IF NOT EXISTS Announcements (
    AnnouncementID INTEGER PRIMARY KEY AUTOINCREMENT,
    AdminID INTEGER NOT NULL,
    Title TEXT NOT NULL,
    Content TEXT NOT NULL,
    PublishTime TEXT DEFAULT CURRENT_TIMESTAMP,
    EffectiveTime TEXT,
    ExpireTime TEXT,
    IsTop INTEGER DEFAULT 0,
    FOREIGN KEY (AdminID) REFERENCES AdminUsers(AdminID)
);

-- 创建索引
CREATE INDEX IF NOT EXISTS idx_individual_users_username ON IndividualUsers(Username);
CREATE INDEX IF NOT EXISTS idx_individual_users_email ON IndividualUsers(Email);
CREATE INDEX IF NOT EXISTS idx_individual_users_phone ON IndividualUsers(PhoneNumber);
CREATE INDEX IF NOT EXISTS idx_enterprise_users_name ON EnterpriseUsers(EnterpriseName);
CREATE INDEX IF NOT EXISTS idx_enterprise_users_credit ON EnterpriseUsers(CreditCode);
CREATE INDEX IF NOT EXISTS idx_job_postings_title ON JobPostings(JobTitle);
CREATE INDEX IF NOT EXISTS idx_job_postings_location ON JobPostings(WorkLocation);
CREATE INDEX IF NOT EXISTS idx_job_postings_category ON JobPostings(JobCategory);
CREATE INDEX IF NOT EXISTS idx_job_applications_status ON JobApplications(CurrentStatus);
CREATE INDEX IF NOT EXISTS idx_announcements_publish ON Announcements(PublishTime);

-- 启用WAL模式以提高并发性能
PRAGMA journal_mode=WAL;