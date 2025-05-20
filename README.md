# 无形大手 人才招聘系统 README

## 项目概述
本项目为“无形大手 人才招聘系统”，采用前后端分离架构。

---

# 后端服务
基于C++语言开发，提供RESTful API接口，支持个人用户简历管理、企业职位发布与管理、职位公开查询、管理员审核等核心功能，旨在为招聘场景提供高效、安全的服务端支持。

## 技术栈
- **Web框架**：Crow（轻量级C++ Web框架）
- **数据库**：SQLite3（通过SQLiteCpp库封装，支持事务管理）
- **认证**：JWT（JSON Web Tokens）实现用户身份验证
- **依赖库**：
  - Crow（Web路由与请求处理）
  - SQLiteCpp（SQLite3数据库操作封装）
  - OpenSSL（加密与安全通信）
  - jwt-cpp（JWT生成与校验）
  - asio（异步I/O支持）

## 核心功能模块
### 1. 个人用户模块
- **简历管理**：支持获取当前用户简历列表（`GET /users/me/resumes`）、获取简历详情（`GET /users/me/resumes/{resumeId}`）、创建简历（`POST /users/me/resumes`）及修改简历（`PUT /users/me/resumes/{resumeId}`）。
- 认证方式：JWT（仅个人用户可操作）。

### 2. 职位模块（公开查询）
- **职位列表查询**：公开接口（`GET /jobs`），支持筛选（如职位名称、地点）和分页。
- **职位详情获取**：公开接口（`GET /jobs/{jobId}`），返回职位详细信息及企业简要信息。

### 3. 企业用户模块
- **职位管理**：企业用户可获取已发布职位列表（`GET /enterprises/me/jobs`），支持按状态筛选（如“招聘中”“待审核”）。
- 认证方式：JWT（仅企业用户可操作）。

### 4. 管理员模块
- **企业用户管理**：获取企业列表（`GET /admin/enterprises`），支持按名称、信用代码、状态筛选。
- **职位审核**：管理所有职位（`GET /admin/jobs`），支持按职位名称、企业名称、状态筛选。

## 构建与运行
### 环境要求
- CMake ≥ 3.30
- C++17 (Clang之外编译器未测试)
- 依赖库（通过vcpkg或系统包管理器安装）：
  - Crow
  - SQLite3
  - SQLiteCpp
  - OpenSSL
  - jwt-cpp

### 构建说明

1.项目默认使用vcpkg管理依赖，vcpkg相关json已配置好，保证cmake工具链正确即可构建。

2.项目cmake默认使用clang-debug配置，其他配置未详尽测试。

3.如同样使用vscode系ide和clang，为clangd插件正常工作，你可能需要在根目录下使用（管理员）powershell建立compile_commands.json的符号链接文件：
```bash

New-Item -ItemType SymbolicLink -Path .\compile_commands.json -Value build\clang-debug\compile_commands.json

```

### 运行前准备
构建完成后，自动生成以下目录及文件：
- `build/bin/db`：存放数据库初始化脚本`databaseInit.sql`（首次运行时自动创建数据库）。
- `build/bin/resources/resume`：用于存储用户上传的简历文件（如PDF）。

## 注意

由于时间紧任务重，项目中存在许多未完善的地方，包括但不限于：
- 日志系统与Crow架构集成耦合，没有单独解耦出来的日志工具，不过这部分都是流式输出到宏中，如果你需要更换web架构，请重新定义日志输出宏或依次替换。
- 仅保证api能工作，没有时间进行全面的覆盖测试。
- 后端用户名邮箱等验证部分格式校验和唯一性校验混在一起。