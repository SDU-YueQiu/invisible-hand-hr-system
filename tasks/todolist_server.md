# todolist

## 实现

### 中间件层

- [x] AuthMiddleware
~~- [ ] ErrorHandlingMiddleware~~
~~- [ ] LoggingMiddleware~~
- [x] ValidationMiddleware

### 工具层

- [x] SecurityUtils
- [x] ValidationUtils
~~- [ ] Logger (crow logging)~~
~~- [ ] ConfigManager~~

### API层

- [x] `ApiRouter`: 集中管理所有路由配置，将请求映射到对应的控制器方法
- [x] `AuthController`: 处理用户认证相关请求（登录、注册、找回密码等）
- [x] `UserController`: 处理个人用户信息管理
- [x] `ResumeController`: 处理简历的CRUD操作
- [x] `JobController`: 处理职位查询和申请
- [x] `EnterpriseController`: 处理企业用户信息和职位管理
- [x] `AdminController`: 处理管理员的各种管理功能
- [x] `FeedbackController`: 处理用户反馈和留言

### 服务层

- [x] `AuthService`: 处理认证和授权逻辑
- [x] `IndividualUserService`: 个人用户管理逻辑
- [x] `ResumeService`: 简历管理业务逻辑
- [x] `JobSearchService`: 职位搜索和查询逻辑
- [x] `ApplicationService`: 职位申请处理逻辑
- [x] `EnterpriseUserService`: 企业用户管理逻辑
- [x] `JobPostingService`: 职位发布和管理逻辑
- [x] `TalentSearchService`: 人才搜索和筛选逻辑
- [x] `AdminService`: 系统管理功能逻辑
- [x] `FeedbackService`: 用户反馈处理逻辑
- [x] `AnnouncementService`: 系统公告管理逻辑

### 数据访问层DAL

- [x] `DatabaseManager`: 中央数据库连接管理器
- [x] `DAOs`：如`IndividualUserDAO`、`ResumeDAO`等，每个DAO负责特定实体的CRUD操作

## 测试

### 中间件层

- [ ] AuthMiddleware
- [ ] ErrorHandlingMiddleware
- [ ] LoggingMiddleware
- [ ] ValidationMiddleware

### 工具层

- [ ] SecurityUtils
- [ ] ValidationUtils
- [ ] Logger (crow logging)
- [ ] ConfigManager

### API层

- [ ] `ApiRouter`: 集中管理所有路由配置，将请求映射到对应的控制器方法
- [ ] `AuthController`: 处理用户认证相关请求（登录、注册、找回密码等）
- [ ] `UserController`: 处理个人用户信息管理
- [ ] `ResumeController`: 处理简历的CRUD操作
- [ ] `JobController`: 处理职位查询和申请
- [ ] `EnterpriseController`: 处理企业用户信息和职位管理
- [ ] `AdminController`: 处理管理员的各种管理功能
- [ ] `FeedbackController`: 处理用户反馈和留言

### 服务层

- [ ] `AuthService`: 处理认证和授权逻辑
- [ ] `IndividualUserService`: 个人用户管理逻辑
- [ ] `ResumeService`: 简历管理业务逻辑
- [ ] `JobSearchService`: 职位搜索和查询逻辑
- [ ] `ApplicationService`: 职位申请处理逻辑
- [ ] `EnterpriseUserService`: 企业用户管理逻辑
- [ ] `JobPostingService`: 职位发布和管理逻辑
- [ ] `TalentSearchService`: 人才搜索和筛选逻辑
- [ ] `AdminService`: 系统管理功能逻辑
- [ ] `FeedbackService`: 用户反馈处理逻辑
- [ ] `AnnouncementService`: 系统公告管理逻辑

### 数据访问层DAL

- [x] `DatabaseManager`: 中央数据库连接管理器
- [ ] `DAOs`：如`IndividualUserDAO`、`ResumeDAO`等，每个DAO负责特定实体的CRUD操作
