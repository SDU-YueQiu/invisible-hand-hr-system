# 无形大手 人才招聘系统 - API接口文档 V1.0
## 1. 引言

### 1.1 目的
本文档详细描述了“无形大手 人才招聘系统”前后端之间交互的RESTful API接口。旨在为前端开发人员提供清晰、准确的接口调用指南，并作为后端API实现的规范依据。

### 1.2 基础URL
所有API请求的基础URL为：
`http(s)://your-domain/api/v1`

### 1.3 认证方式
系统采用基于JWT (JSON Web Token) 的认证机制。
1.  用户（个人、企业、管理员）成功登录后，服务器会返回一个JWT。
2.  前端需在后续需要认证的请求的 `Authorization` 头部携带此Token，格式为：
    `Authorization: Bearer <your_jwt_token>`

### 1.4 数据格式
所有请求体 (Request Body) 和响应体 (Response Body) 均使用 `application/json` 格式。

### 1.5 通用响应结构

**成功响应:**
```json
{
  "success": true,
  "data": {
    // 业务数据对象或数组
  },
  "message": "操作成功" // 或具体成功信息
}
```

**失败响应:**
```json
{
  "success": false,
  "data": null,
  "message": "错误描述信息", // 例如: "用户名或密码错误", "请求参数无效"
  "errorCode": "ERROR_CODE_STRING" // (可选) 具体错误码，便于前端处理
}
```

### 1.6 HTTP状态码
API将使用标准的HTTP状态码来指示请求的结果：
*   `200 OK`: 请求成功。对于GET和PUT请求的成功响应。
*   `201 Created`: 资源创建成功。对于POST请求的成功响应。
*   `204 No Content`: 请求成功，但响应体中没有内容（例如，DELETE请求）。
*   `400 Bad Request`: 请求无效。通常由于客户端发送了错误的请求数据（如参数格式错误、缺少必要参数）。
*   `401 Unauthorized`: 未认证。客户端未提供有效的认证凭证。
*   `403 Forbidden`: 已认证，但无权限访问该资源。
*   `404 Not Found`: 请求的资源不存在。
*   `409 Conflict`: 请求冲突，例如尝试创建一个已存在的唯一资源。
*   `500 Internal Server Error`: 服务器内部错误。

### 1.7 约定

以下约定由前端校验，后端则负责进行用户名唯一性校验。

- 用户名规则: 4-20个字符，只能包含字母、数字和下划线
- 密码规则: 至少6个字符，包含字母和数字

---

## 2. 认证 (Auth) 模块

### 2.1 个人用户注册
*   **Endpoint:** `POST /auth/individual/register`
*   **描述:** 个人用户注册新账户。
*   **认证:** 无需
*   **请求体:**
    ```json
    {
      "username": "string (unique, required)",
      "password": "string (required, min_length: 6)",
      "phoneNumber": "string (unique, optional, valid phone format)",
      "email": "string (unique, optional, valid email format)"
    }
    ```
*   **成功响应 (201 Created):**
    ```json
    {
      "success": true,
      "data": {
        "userId": "integer",
        "username": "string",
        "token": "string (JWT)"
      },
      "message": "个人用户注册成功"
    }
    ```
*   **失败响应:** `400 Bad Request`, `409 Conflict` (用户名/手机/邮箱已存在)

### 2.2 个人用户登录
*   **Endpoint:** `POST /auth/individual/login`
*   **描述:** 个人用户登录系统。
*   **认证:** 无需
*   **请求体:**
    ```json
    {
      "username": "string (required)", // 可以是用户名、手机号或邮箱
      "password": "string (required)"
    }
    ```
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": {
        "userId": "integer",
        "username": "string",
        "token": "string (JWT)"
      },
      "message": "登录成功"
    }
    ```
*   **失败响应:** `400 Bad Request`, `401 Unauthorized`

### 2.3 企业用户注册
*   **Endpoint:** `POST /auth/enterprise/register`
*   **描述:** 企业用户提交注册申请。
*   **认证:** 无需
*   **请求体:**
    ```json
    {
      "loginUsername": "string (unique, required)",
      "password": "string (required, min_length: 6)",
      "enterpriseName": "string (required)",
      "creditCode": "string (unique, required)",
      "description": "string (optional)",
      "industry": "string (optional)",
      "scale": "string (optional)",
      "address": "string (optional)",
      "contactPerson": "string (required)",
      "contactPhone": "string (required, valid phone format)",
      "contactEmail": "string (required, valid email format)",
      "licenseImageURL": "string (required, URL of uploaded license image)"
      // logoURL is optional, can be updated later
    }
    ```
*   **成功响应 (201 Created):**
    ```json
    {
      "success": true,
      "data": {
        "enterpriseId": "integer",
        "loginUsername": "string",
        "accountStatus": "PendingReview" // 初始状态
      },
      "message": "企业注册申请已提交，请等待审核"
    }
    ```
*   **失败响应:** `400 Bad Request`, `409 Conflict`

### 2.4 企业用户登录
*   **Endpoint:** `POST /auth/enterprise/login`
*   **描述:** 企业用户登录系统。
*   **认证:** 无需
*   **请求体:**
    ```json
    {
      "loginUsername": "string (required)",
      "password": "string (required)"
    }
    ```
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": {
        "enterpriseId": "integer",
        "loginUsername": "string",
        "token": "string (JWT)",
        "accountStatus": "Approved" // 或其他状态
      },
      "message": "登录成功"
    }
    ```
*   **失败响应:** `400 Bad Request`, `401 Unauthorized` (凭证错误或账户未批准/禁用)

### 2.5 管理员登录
*   **Endpoint:** `POST /auth/admin/login`
*   **描述:** 管理员登录后台管理系统。
*   **认证:** 无需
*   **请求体:**
    ```json
    {
      "adminUsername": "string (required)",
      "password": "string (required)"
    }
    ```
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": {
        "adminId": "integer",
        "adminUsername": "string",
        "token": "string (JWT for admin)"
      },
      "message": "管理员登录成功"
    }
    ```
*   **失败响应:** `400 Bad Request`, `401 Unauthorized`

### 2.6 退出登录
*   **Endpoint:** `POST /auth/logout`
*   **描述:** 用户（个人/企业/管理员）退出登录。
*   **认证:** JWT (个人/企业/管理员均可)
*   **请求体:** 无
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": null,
      "message": "退出成功"
    }
    ```
    *注: 后端通常将JWT加入黑名单或前端直接清除JWT即可。*

### 2.7 忘记密码 (涉及验证码没法搞，让用户提反馈feedback，该接口不实现)

---

## 3. 个人用户 (Users) 模块

### 3.1 获取当前个人用户信息
*   **Endpoint:** `GET /users/me`
*   **描述:** 获取当前已登录个人用户的详细信息。
*   **认证:** JWT (个人用户)
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": {
        "userId": "integer",
        "username": "string",
        "phoneNumber": "string",
        "email": "string",
        "registrationDate": "datetime string (ISO 8601)",
        "lastLoginDate": "datetime string (ISO 8601)",
        "accountStatus": "string ('Active', 'Disabled')",
        "avatarURL": "string (nullable)"
      },
      "message": "获取用户信息成功"
    }
    ```

### 3.2 修改当前个人用户信息
*   **Endpoint:** `PUT /users/me`
*   **描述:** 修改当前已登录个人用户的基本信息 (不包括密码)。
*   **认证:** JWT (个人用户)
*   **请求体:** (只包含需要修改的字段)
    ```json
    {
      "phoneNumber": "string (optional, valid phone format, may require verification)",
      "email": "string (optional, valid email format, may require verification)",
      "avatarURL": "string (optional)"
    }
    ```
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": {
        // 更新后的用户信息对象
      },
      "message": "用户信息更新成功"
    }
    ```
*   **失败响应:** `400 Bad Request`

### 3.3 修改当前个人用户密码
*   **Endpoint:** `PUT /users/me/password`
*   **描述:** 当前已登录个人用户修改自己的密码。
*   **认证:** JWT (个人用户)
*   **请求体:**
    ```json
    {
      "oldPassword": "string (required)",
      "newPassword": "string (required, min_length: 6)"
    }
    ```
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": null,
      "message": "密码修改成功，请重新登录。"
    }
    ```
*   **失败响应:** `400 Bad Request` (旧密码错误, 新密码不符合要求)

---

## 4. 简历 (Resumes) 模块 (隶属于个人用户)

### 4.1 获取当前用户的简历列表
*   **Endpoint:** `GET /users/me/resumes`
*   **描述:** 获取当前登录个人用户的所有简历列表。
*   **认证:** JWT (个人用户)
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": [
        {
          "resumeId": "integer",
          "userId": "integer",
          "resumeTitle": "string",
          "lastUpdateTime": "datetime string (ISO 8601)",
          "visibilityStatus": "string ('PublicToAll', 'PublicToApplied', 'Private')"
        }
        // ... more resumes
      ],
      "message": "获取简历列表成功"
    }
    ```

### 4.2 创建新简历
*   **Endpoint:** `POST /users/me/resumes`
*   **描述:** 为当前登录个人用户创建一份新简历。
*   **认证:** JWT (个人用户)
*   **请求体:**
    ```json
    {
      "resumeTitle": "string (required)",
      "basicInfo": { "name": "string", "gender": "string", "birthDate": "date string" /* ... */ },
      "jobIntent": { "industry": "string", "position": "string", "salary": "string" /* ... */ },
      "educationExperience": [ { "school": "string", "degree": "string" /* ... */ } ],
      "workExperience": [ { "company": "string", "role": "string" /* ... */ } ],
      "projectExperience": [ { "projectName": "string", "description": "string" /* ... */ } ],
      "skillsCertificates": [ { "skillName": "string", "level": "string" /* ... */ } ],
      "selfDescription": "string (optional)",
      "visibilityStatus": "string (default 'Private')",
      "attachmentPath": "string (optional, URL of uploaded attachment)"
    }
    ```
*   **成功响应 (201 Created):**
    ```json
    {
      "success": true,
      "data": {
        // 创建成功的完整简历对象
        "resumeId": "integer",
        // ... all other fields
      },
      "message": "简历创建成功"
    }
    ```
*   **失败响应:** `400 Bad Request`

### 4.3 获取简历详情
*   **Endpoint:** `GET /users/me/resumes/{resumeId}`
*   **描述:** 获取指定ID的简历详细信息。
*   **认证:** JWT (个人用户, 且简历属于该用户)
*   **路径参数:**
    *   `resumeId` (integer): 简历ID
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": {
        // 完整简历对象
      },
      "message": "获取简历详情成功"
    }
    ```
*   **失败响应:** `404 Not Found`

### 4.4 修改简历
*   **Endpoint:** `PUT /users/me/resumes/{resumeId}`
*   **描述:** 修改指定ID的简历信息。
*   **认证:** JWT (个人用户, 且简历属于该用户)
*   **路径参数:**
    *   `resumeId` (integer): 简历ID
*   **请求体:** (同创建简历，只包含需要修改的字段)
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": {
        // 更新后的完整简历对象
      },
      "message": "简历更新成功"
    }
    ```
*   **失败响应:** `400 Bad Request`, `404 Not Found`

### 4.5 删除简历
*   **Endpoint:** `DELETE /users/me/resumes/{resumeId}`
*   **描述:** 删除指定ID的简历。
*   **认证:** JWT (个人用户, 且简历属于该用户)
*   **路径参数:**
    *   `resumeId` (integer): 简历ID
*   **成功响应 (204 No Content):** (或200 OK with message)
    ```json
    {
      "success": true,
      "data": null,
      "message": "简历删除成功"
    }
    ```
*   **失败响应:** `404 Not Found`

---

## 5. 职位 (Jobs) 模块 (公开查询)

### 5.1 查询职位列表
*   **Endpoint:** `GET /jobs`
*   **描述:** 公开查询职位列表，支持筛选和分页。
*   **认证:** 无需 (或可选JWT，用于个性化推荐等未来功能)
*   **查询参数 (Query Parameters):**
    *   `keyword` (string, optional): 关键词 (职位名称, 公司名称, 技能)
    *   `category` (string, optional): 职位类别
    *   `location` (string, optional): 工作地点
    *   `salaryMin` (integer, optional): 最低薪资
    *   `salaryMax` (integer, optional): 最高薪资
    *   `experience` (string, optional): 工作经验要求
    *   `education` (string, optional): 学历要求
    *   `page` (integer, optional, default: 1): 页码
    *   `limit` (integer, optional, default: 10): 每页数量
    *   `sortBy` (string, optional, e.g., 'publishDate'): 排序字段
    *   `sortOrder` (string, optional, 'asc' or 'desc', default: 'desc'): 排序顺序
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": {
        "jobs": [
          {
            "jobId": "integer",
            "enterpriseId": "integer",
            "enterpriseName": "string", // 冗余企业名称方便展示
            "logoURL": "string",        // 冗余企业Logo方便展示
            "jobTitle": "string",
            "workLocation": "string",
            "salaryMin": "string",
            "salaryMax": "string",
            "experienceRequired": "string",
            "educationRequired": "string",
            "publishDate": "datetime string (ISO 8601)"
            // ...其他摘要信息
          }
          // ... more jobs
        ],
        "pagination": {
          "currentPage": "integer",
          "totalPages": "integer",
          "totalItems": "integer",
          "itemsPerPage": "integer"
        }
      },
      "message": "查询职位列表成功"
    }
    ```

### 5.2 获取职位详情
*   **Endpoint:** `GET /jobs/{jobId}`
*   **描述:** 公开获取指定ID的职位详细信息。
*   **认证:** 无需 (或可选JWT)
*   **路径参数:**
    *   `jobId` (integer): 职位ID
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": {
        "jobId": "integer",
        "enterpriseId": "integer",
        "enterpriseInfo": { // 企业简要信息
          "enterpriseName": "string",
          "logoURL": "string",
          "industry": "string",
          "scale": "string"
        },
        "jobTitle": "string",
        "jobCategory": "string",
        "recruitmentCount": "integer",
        "workLocation": "string",
        "maxSalary": "string",
        "minSalary": "string",
        "responsibilities": "text",
        "requirements": "text",
        "experienceRequired": "string",
        "educationRequired": "string",
        "benefits": "text",
        "publishDate": "datetime string (ISO 8601)",
        "updateDate": "datetime string (ISO 8601)",
        "deadlineDate": "datetime string (ISO 8601, nullable)",
        "jobStatus": "string ('Recruiting')" // 只显示招聘中的
      },
      "message": "获取职位详情成功"
    }
    ```
*   **失败响应:** `404 Not Found`

---

## 6. 职位申请 (Applications) 模块

### 6.1 个人用户申请职位
*   **Endpoint:** `POST /jobs/{jobId}/apply`
*   **描述:** 当前登录的个人用户使用其选定的简历申请指定职位。
*   **认证:** JWT (个人用户)
*   **路径参数:**
    *   `jobId` (integer): 要申请的职位ID
*   **请求体:**
    ```json
    {
      "resumeId": "integer (required, ID of the resume to be used)"
    }
    ```
*   **成功响应 (201 Created):**
    ```json
    {
      "success": true,
      "data": {
        "applicationId": "integer",
        "jobId": "integer",
        "resumeId": "integer",
        "applicationTime": "datetime string (ISO 8601)",
        "currentStatus": "Submitted"
      },
      "message": "职位申请成功"
    }
    ```
*   **失败响应:** `400 Bad Request` (简历不存在或不属于该用户), `404 Not Found` (职位不存在), `409 Conflict` (已申请过该职位)

### 6.2 获取个人用户的求职申请列表
*   **Endpoint:** `GET /users/me/applications`
*   **描述:** 获取当前登录个人用户的所有求职申请记录。
*   **认证:** JWT (个人用户)
*   **查询参数 (Query Parameters):**
    *   `status` (string, optional): 按状态筛选 (e.g., 'Submitted', 'ViewedByHR', 'InterviewInvited')
    *   `page` (integer, optional, default: 1)
    *   `limit` (integer, optional, default: 10)
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": {
        "applications": [
          {
            "applicationId": "integer",
            "jobInfo": {
              "jobId": "integer",
              "jobTitle": "string",
              "enterpriseName": "string"
            },
            "resumeInfo": {
              "resumeId": "integer",
              "resumeTitle": "string"
            },
            "applicationTime": "datetime string (ISO 8601)",
            "currentStatus": "string"
          }
          // ... more applications
        ],
        "pagination": { /* ... */ }
      },
      "message": "获取求职申请列表成功"
    }
    ```

---

## 7. 企业用户 (Enterprises) 模块

### 7.1 获取当前企业信息
*   **Endpoint:** `GET /enterprises/me`
*   **描述:** 获取当前登录企业用户的详细信息。
*   **认证:** JWT (企业用户)
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": {
        "enterpriseId": "integer",
        "loginUsername": "string",
        "enterpriseName": "string",
        "creditCode": "string",
        "description": "string",
        "industry": "string",
        "scale": "string",
        "address": "string",
        "contactPerson": "string",
        "contactPhone": "string",
        "contactEmail": "string",
        "logoURL": "string (nullable)",
        "licenseImageURL": "string",
        "registrationDate": "datetime string (ISO 8601)",
        "accountStatus": "string ('Approved', 'PendingReview', 'Rejected', 'Disabled')",
        "auditOpinion": "string (nullable)"
      },
      "message": "获取企业信息成功"
    }
    ```

### 7.2 修改当前企业信息
*   **Endpoint:** `PUT /enterprises/me`
*   **描述:** 修改当前登录企业用户的基本信息 (不包括密码)。关键信息修改可能需要重新审核。
*   **认证:** JWT (企业用户)
*   **请求体:** (只包含需要修改的字段)
    ```json
    {
      "enterpriseName": "string (optional, may trigger re-audit)",
      "description": "string (optional)",
      "industry": "string (optional)",
      // ...其他可修改字段
      "logoURL": "string (optional)"
    }
    ```
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": {
        // 更新后的企业信息对象
      },
      "message": "企业信息更新成功" // 或 "企业信息已提交，部分修改可能需要重新审核"
    }
    ```
*   **失败响应:** `400 Bad Request`

### 7.3 修改当前企业用户密码
*   **Endpoint:** `PUT /enterprises/me/password`
*   **描述:** 当前已登录企业用户修改自己的密码。
*   **认证:** JWT (企业用户)
*   **请求体:**
    ```json
    {
      "oldPassword": "string (required)",
      "newPassword": "string (required, min_length: 6)"
    }
    ```
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": null,
      "message": "密码修改成功，请重新登录。"
    }
    ```
*   **失败响应:** `400 Bad Request`

### 7.4 获取企业发布的职位列表
*   **Endpoint:** `GET /enterprises/me/jobs`
*   **描述:** 获取当前登录企业用户发布的所有职位。
*   **认证:** JWT (企业用户)
*   **查询参数 (Query Parameters):**
    *   `status` (string, optional): 按职位状态筛选 (e.g., 'Recruiting', 'PendingReview', 'Offline')
    *   `page` (integer, optional, default: 1)
    *   `limit` (integer, optional, default: 10)
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": {
        "jobs": [
          {
            "jobId": "integer",
            "jobTitle": "string",
            "publishDate": "datetime string (ISO 8601)",
            "jobStatus": "string",
            "applicantCount": "integer" // 申请人数统计
            // ...其他摘要信息
          }
        ],
        "pagination": { /* ... */ }
      },
      "message": "获取企业职位列表成功"
    }
    ```

### 7.5 企业发布新职位
*   **Endpoint:** `POST /enterprises/me/jobs`
*   **描述:** 当前登录企业用户发布新的招聘职位。
*   **认证:** JWT (企业用户)
*   **请求体:**
    ```json
    {
      "jobTitle": "string (required)",
      "jobCategory": "string (optional)",
      "recruitmentCount": "integer (optional)",
      "workLocation": "string (required)",
      "maxSalary": "integer (optional)",
      "minSalary": "integer (optional)",
      "responsibilities": "text (required)",
      "requirements": "text (required)",
      "experienceRequired": "string (optional)",
      "educationRequired": "string (optional)",
      "benefits": "text (optional)",
      "deadlineDate": "date string (optional, ISO 8601)",
      "jobStatus": "string (optional, default 'Recruiting' or 'PendingReview' based on system config)"
    }
    ```
*   **成功响应 (201 Created):**
    ```json
    {
      "success": true,
      "data": {
        // 创建成功的完整职位对象
      },
      "message": "职位发布成功" // 或 "职位已提交审核"
    }
    ```
*   **失败响应:** `400 Bad Request`

### 7.6 获取企业发布的某个职位详情
*   **Endpoint:** `GET /enterprises/me/jobs/{jobId}`
*   **描述:** 获取当前企业发布的某个职位的详细信息 (通常用于编辑)。
*   **认证:** JWT (企业用户, 且职位属于该企业)
*   **路径参数:** `jobId` (integer)
*   **成功响应 (200 OK):** (结构同 `GET /jobs/{jobId}` 公开接口，但可能包含更多管理字段)

### 7.7 修改企业发布的职位
*   **Endpoint:** `PUT /enterprises/me/jobs/{jobId}`
*   **描述:** 修改当前企业发布的某个职位信息。
*   **认证:** JWT (企业用户, 且职位属于该企业)
*   **路径参数:** `jobId` (integer)
*   **请求体:** (同发布新职位，只包含需修改字段)
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": {
        // 更新后的完整职位对象
      },
      "message": "职位更新成功"
    }
    ```
*   **失败响应:** `400 Bad Request`, `404 Not Found`

### 7.8 删除企业发布的职位
*   **Endpoint:** `DELETE /enterprises/me/jobs/{jobId}`
*   **描述:** 删除当前企业发布的某个职位。
*   **认证:** JWT (企业用户, 且职位属于该企业)
*   **路径参数:** `jobId` (integer)
*   **成功响应 (204 No Content / 200 OK):**
    ```json
    {
      "success": true,
      "data": null,
      "message": "职位删除成功"
    }
    ```
*   **失败响应:** `404 Not Found`

### 7.9 获取某职位的申请者列表
*   **Endpoint:** `GET /enterprises/me/jobs/{jobId}/applicants`
*   **描述:** 获取投递到企业某个职位的所有申请者信息。
*   **认证:** JWT (企业用户, 且职位属于该企业)
*   **路径参数:** `jobId` (integer)
*   **查询参数 (Query Parameters):**
    *   `status` (string, optional): 按申请状态筛选
    *   `page` (integer, optional, default: 1)
    *   `limit` (integer, optional, default: 10)
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": {
        "applicants": [
          {
            "applicationId": "integer",
            "userInfo": { // 申请人简要信息
              "userId": "integer",
              "username": "string",
              "avatarURL": "string"
            },
            "resumeInfo": { // 投递简历简要信息
              "resumeId": "integer",
              "resumeTitle": "string"
              // 前端可根据resumeId进一步请求完整简历
            },
            "applicationTime": "datetime string (ISO 8601)",
            "currentStatus": "string"
          }
        ],
        "pagination": { /* ... */ }
      },
      "message": "获取申请者列表成功"
    }
    ```

### 7.10 更新职位申请状态 (企业操作)
*   **Endpoint:** `PUT /enterprises/me/applications/{applicationId}/status`
*   **描述:** 企业用户更新某个职位申请的处理状态。
*   **认证:** JWT (企业用户, 且申请记录属于该企业的职位)
*   **路径参数:** `applicationId` (integer)
*   **请求体:**
    ```json
    {
      "newStatus": "string (required, e.g., 'ViewedByHR', 'InterviewInvited', 'Rejected', 'Hired')",
      "enterpriseNotes": "string (optional, 企业内部备注)"
    }
    ```
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": {
        "applicationId": "integer",
        "currentStatus": "string (newStatus)"
      },
      "message": "申请状态更新成功"
    }
    ```
*   **失败响应:** `400 Bad Request`, `404 Not Found`


### 7.11 搜索符合条件的简历
*   **Endpoint:** `GET /enterprises/me/resumes/search`
*   **描述:** 企业用户根据条件搜索公开简历库
*   **认证:** JWT (企业用户)
*   **查询参数 (Query Parameters):**
    *   `keyword` (string, optional): 搜索关键词(技能、教育经历等)
    *   `education` (string, optional): 学历要求
    *   `experience` (string, optional): 工作经验要求
    *   `location` (string, optional): 期望工作地点
    *   `minSalary` (integer, optional): 最低期望薪资
    *   `maxSalary` (integer, optional): 最高期望薪资
    *   `sortBy` (string, optional): 排序字段
    *   `sortOrder` (string, optional): 排序顺序(asc/desc)
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": {
        "resumes": [
          {
          }
        ]
        }
      },
      "message": "简历搜索成功"
    }
    ```
*   **失败响应:** `400 Bad Request` (参数错误), `403 Forbidden` (无权限)

## 8. 管理员 (Admin) 模块
**所有管理员接口均需 Admin JWT 认证。**

### 8.1 获取个人用户列表
*   **Endpoint:** `GET /admin/users`
*   **查询参数:** `filter`:完整的SQL查询条件
*   **成功响应 (200 OK):** 返回个人用户列表及分页信息。

### 8.2 更新个人用户状态
*   **Endpoint:** `PUT /admin/users/{userId}/status`
*   **路径参数:** `userId` (integer)
*   **请求体:** `{"newStatus": "string ('Active', 'Disabled')"}`
*   **成功响应 (200 OK):** 返回更新后的用户状态。

### 8.3 获取企业用户列表
*   **Endpoint:** `GET /admin/enterprises`
*   **查询参数:** `filter`:完整的SQL查询条件
*   **成功响应 (200 OK):** 返回企业用户列表及分页信息。

### 8.4 批准企业注册
*   **Endpoint:** `PUT /admin/enterprises/{enterpriseId}/approve`
*   **路径参数:** `enterpriseId` (integer)
*   **请求体 (可选):** `{"auditOpinion": "string"}`
*   **成功响应 (200 OK):** 企业状态更新为 'Approved'。

### 8.5 拒绝企业注册
*   **Endpoint:** `PUT /admin/enterprises/{enterpriseId}/reject`
*   **路径参数:** `enterpriseId` (integer)
*   **请求体 (必须):** `{"auditOpinion": "string (拒绝理由)"}`
*   **成功响应 (200 OK):** 企业状态更新为 'Rejected'。

### 8.7 管理所有职位
*   **Endpoint:** `GET /admin/jobs`
*   **查询参数:** `filter`:完整的SQL查询条件
*   **成功响应 (200 OK):** 返回职位列表及分页。

### 8.8 更新职位状态 (管理员操作)
*   **Endpoint:** `PUT /admin/jobs/{jobId}/status`
*   **路径参数:** `jobId` (integer)
*   **请求体:** `{"newStatus": "string ('Recruiting', 'Offline', 'PendingReview')", "auditOpinion": "string (optional)"}`
*   **成功响应 (200 OK):** 返回更新后的职位状态。

---

## 9. 反馈 (Feedback) 模块

### 9.1 用户提交反馈
*   **Endpoint:** `POST /feedback`
*   **描述:** 任何已登录用户（个人/企业）或匿名用户提交反馈/留言。
*   **认证:** JWT (可选, 若用户已登录则携带)
*   **请求体:**
    ```json
    {
      "feedbackType": "string (required, e.g., 'Suggestion', 'BugReport', 'Complaint')",
      "content": "text (required)",
      "contactInfo": "string (optional, 用户留下的联系方式)"
      // UserID 和 UserType 会在后端根据JWT自动填充（如果已登录）
    }
    ```
*   **成功响应 (201 Created):**
    ```json
    {
      "success": true,
      "data": {
        "feedbackId": "integer"
      },
      "message": "反馈提交成功"
    }
    ```

### 9.2 管理员获取反馈列表
*   **Endpoint:** `GET /admin/feedbacks`
*   **认证:** JWT (Admin)
*   **查询参数:** `type`, `status` ('Pending', 'Processing', 'Resolved'), `page`, `limit`
*   **成功响应 (200 OK):** 返回反馈列表及分页。

### 9.3 管理员处理反馈
*   **Endpoint:** `PUT /admin/feedbacks/{feedbackId}`
*   **认证:** JWT (Admin)
*   **路径参数:** `feedbackId` (integer)
*   **请求体:**
    ```json
    {
      "processStatus": "string (required, e.g., 'Processing', 'Resolved')",
      "adminReply": "text (optional, 回复内容)"
    }
    ```
*   **成功响应 (200 OK):** 返回更新后的反馈对象。

---

## 10. 公告 (Announcements) 模块

### 10.1 获取公告列表 (公开)
*   **Endpoint:** `GET /announcements`
*   **描述:** 公开获取系统公告列表 (通常按发布时间倒序，置顶优先)。
*   **认证:** 无需
*   **查询参数 (Query Parameters):**
    *   `page` (integer, optional, default: 1)
    *   `limit` (integer, optional, default: 5)
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": {
        "announcements": [
          {
            "announcementId": "integer",
            "title": "string",
            "contentSummary": "string (摘要或部分内容)", // 列表页可能只显示摘要
            "publishTime": "datetime string (ISO 8601)",
            "isTop": "boolean"
          }
        ],
        "pagination": { /* ... */ }
      },
      "message": "获取公告列表成功"
    }
    ```

### 10.2 获取公告详情 (公开)
*   **Endpoint:** `GET /announcements/{announcementId}`
*   **描述:** 公开获取指定公告的完整内容。
*   **认证:** 无需
*   **路径参数:** `announcementId` (integer)
*   **成功响应 (200 OK):**
    ```json
    {
      "success": true,
      "data": {
        "announcementId": "integer",
        "title": "string",
        "content": "text (完整内容, 可能含HTML或Markdown)",
        "publishTime": "datetime string (ISO 8601)",
        "effectiveTime": "datetime string (ISO 8601, nullable)",
        "expireTime": "datetime string (ISO 8601, nullable)",
        "isTop": "boolean"
      },
      "message": "获取公告详情成功"
    }
    ```

### 10.3 管理员创建公告
*   **Endpoint:** `POST /admin/announcements`
*   **认证:** JWT (Admin)
*   **请求体:**
    ```json
    {
      "title": "string (required)",
      "content": "text (required)",
      "effectiveTime": "datetime string (optional)",
      "expireTime": "datetime string (optional)",
      "isTop": "boolean (optional, default false)"
    }
    ```
*   **成功响应 (201 Created):** 返回创建的公告对象。

### 10.4 管理员修改公告
*   **Endpoint:** `PUT /admin/announcements/{announcementId}`
*   **认证:** JWT (Admin)
*   **路径参数:** `announcementId` (integer)
*   **请求体:** (同创建，只包含需修改字段)
*   **成功响应 (200 OK):** 返回更新后的公告对象。

### 10.5 管理员删除公告
*   **Endpoint:** `DELETE /admin/announcements/{announcementId}`
*   **认证:** JWT (Admin)
*   **路径参数:** `announcementId` (integer)
*   **成功响应 (204 No Content / 200 OK):**
