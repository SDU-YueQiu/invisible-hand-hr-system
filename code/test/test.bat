@ECHO OFF
SETLOCAL

REM --- Configuration ---
SET BASE_URL=http://127.0.0.1:8080/api/v1
ECHO Testing API Server at: %BASE_URL%
ECHO.

REM --- JWT Tokens (MANUALLY UPDATE THESE AFTER LOGIN TESTS) ---
SET INDIVIDUAL_TOKEN=
SET ENTERPRISE_TOKEN=
SET ADMIN_TOKEN=

REM --- IDs from created resources (MANUALLY UPDATE THESE AFTER CREATE TESTS) ---
SET USER_ID_FOR_ADMIN_TEST=
SET INDIVIDUAL_USER_ID=
SET CREATED_RESUME_ID=
SET ENTERPRISE_ID_FOR_ADMIN_TEST=
SET ENTERPRISE_ID=
SET CREATED_ENTERPRISE_JOB_ID=
SET PUBLIC_JOB_ID_TO_APPLY=
SET CREATED_APPLICATION_ID=
SET FEEDBACK_ID_FOR_ADMIN_TEST=
SET ANNOUNCEMENT_ID_FOR_ADMIN_TEST=

REM --- Test Data ---
SET INDIVIDUAL_USERNAME_REGISTER=testuser_ind_%RANDOM%
SET INDIVIDUAL_EMAIL_REGISTER=%INDIVIDUAL_USERNAME_REGISTER%@example.com
SET ENTERPRISE_LOGIN_USERNAME_REGISTER=test_ent_login_%RANDOM%
SET ENTERPRISE_NAME_REGISTER="Test Enterprise %RANDOM%"
SET ENTERPRISE_CREDIT_CODE_REGISTER="CREDITCODE%RANDOM%"
SET ADMIN_USERNAME_LOGIN=admin_user_from_bat
SET ADMIN_PASSWORD_LOGIN=AdminPassword123

REM =============================================================================
REM == 2. 认证 (Auth) 模块
REM =============================================================================
ECHO.
ECHO ###################################
ECHO ## 2. 认证 (Auth) 模块          ##
ECHO ###################################

:TEST_AUTH_INDIVIDUAL_REGISTER
ECHO.
ECHO === 2.1 个人用户注册 ===
curl -X POST "%BASE_URL%/auth/individual/register" ^
     -H "Content-Type: application/json" ^
     -d "{\"username\":\"%INDIVIDUAL_USERNAME_REGISTER%\",\"password\":\"Password123\",\"phoneNumber\":\"13800138001\",\"email\":\"%INDIVIDUAL_EMAIL_REGISTER%\"}"
ECHO. & PAUSE

:TEST_AUTH_INDIVIDUAL_LOGIN
ECHO.
ECHO === 2.2 个人用户登录 (获取TOKEN) ===
ECHO Please copy the 'token' from the response below and update INDIVIDUAL_TOKEN variable at the top of this script.
curl -X POST "%BASE_URL%/auth/individual/login" ^
     -H "Content-Type: application/json" ^
     -d "{\"username\":\"%INDIVIDUAL_USERNAME_REGISTER%\",\"password\":\"Password123\"}"
ECHO.
SET /P "INDIVIDUAL_TOKEN_INPUT=Paste Individual User JWT Token here and press Enter: "
IF DEFINED INDIVIDUAL_TOKEN_INPUT SET INDIVIDUAL_TOKEN=%INDIVIDUAL_TOKEN_INPUT%
IF DEFINED INDIVIDUAL_TOKEN (
    ECHO INDIVIDUAL_TOKEN set to: %INDIVIDUAL_TOKEN%
    FOR /F "tokens=2 delims=:, " %%i IN ('ECHO %INDIVIDUAL_TOKEN_INPUT% ^| find "userId"') DO SET INDIVIDUAL_USER_ID=%%i
    ECHO INDIVIDUAL_USER_ID (extracted or guessed): %INDIVIDUAL_USER_ID%
) ELSE (
    ECHO INDIVIDUAL_TOKEN not set. Some tests will be skipped.
)
PAUSE

:TEST_AUTH_ENTERPRISE_REGISTER
ECHO.
ECHO === 2.3 企业用户注册 ===
curl -X POST "%BASE_URL%/auth/enterprise/register" ^
     -H "Content-Type: application/json" ^
     -d "{\"loginUsername\":\"%ENTERPRISE_LOGIN_USERNAME_REGISTER%\",\"password\":\"Password123Ent\",\"enterpriseName\":%ENTERPRISE_NAME_REGISTER%,\"creditCode\":%ENTERPRISE_CREDIT_CODE_REGISTER%,\"contactPerson\":\"Test Contact\",\"contactPhone\":\"13900139001\",\"contactEmail\":\"contact@%ENTERPRISE_LOGIN_USERNAME_REGISTER%.com\",\"licenseImageURL\":\"http://example.com/license.jpg\"}"
ECHO. & PAUSE

:TEST_AUTH_ENTERPRISE_LOGIN
ECHO.
ECHO === 2.4 企业用户登录 (获取TOKEN - Requires account approval by admin first) ===
ECHO Note: This login might fail if the enterprise account from 2.3 is not yet 'Approved' by an admin.
ECHO Please copy the 'token' from the response below and update ENTERPRISE_TOKEN variable.
curl -X POST "%BASE_URL%/auth/enterprise/login" ^
     -H "Content-Type: application/json" ^
     -d "{\"loginUsername\":\"%ENTERPRISE_LOGIN_USERNAME_REGISTER%\",\"password\":\"Password123Ent\"}"
ECHO.
SET /P "ENTERPRISE_TOKEN_INPUT=Paste Enterprise User JWT Token here (if successful) and press Enter: "
IF DEFINED ENTERPRISE_TOKEN_INPUT SET ENTERPRISE_TOKEN=%ENTERPRISE_TOKEN_INPUT%
IF DEFINED ENTERPRISE_TOKEN (
    ECHO ENTERPRISE_TOKEN set to: %ENTERPRISE_TOKEN%
) ELSE (
    ECHO ENTERPRISE_TOKEN not set. Some tests will be skipped.
)
PAUSE

:TEST_AUTH_ADMIN_LOGIN
ECHO.
ECHO === 2.5 管理员登录 (获取TOKEN) ===
ECHO Please copy the 'token' from the response below and update ADMIN_TOKEN variable.
curl -X POST "%BASE_URL%/auth/admin/login" ^
     -H "Content-Type: application/json" ^
     -d "{\"adminUsername\":\"%ADMIN_USERNAME_LOGIN%\",\"password\":\"%ADMIN_PASSWORD_LOGIN%\"}"
ECHO.
SET /P "ADMIN_TOKEN_INPUT=Paste Admin JWT Token here and press Enter: "
IF DEFINED ADMIN_TOKEN_INPUT SET ADMIN_TOKEN=%ADMIN_TOKEN_INPUT%
IF DEFINED ADMIN_TOKEN (
    ECHO ADMIN_TOKEN set to: %ADMIN_TOKEN%
) ELSE (
    ECHO ADMIN_TOKEN not set. Admin tests will be skipped.
)
PAUSE

:TEST_AUTH_LOGOUT_INDIVIDUAL
IF NOT DEFINED INDIVIDUAL_TOKEN GOTO SKIP_AUTH_LOGOUT_INDIVIDUAL
ECHO.
ECHO === 2.6 个人用户退出登录 ===
curl -X POST "%BASE_URL%/auth/logout" -H "Authorization: Bearer %INDIVIDUAL_TOKEN%"
ECHO. & PAUSE
:SKIP_AUTH_LOGOUT_INDIVIDUAL

REM --- To test enterprise/admin logout, you'd re-login them and then call logout with their token.

REM =============================================================================
REM == 3. 个人用户 (Users) 模块
REM =============================================================================
ECHO.
ECHO ###################################
ECHO ## 3. 个人用户 (Users) 模块     ##
ECHO ###################################
IF NOT DEFINED INDIVIDUAL_TOKEN (
    ECHO Skipping Section 3 - INDIVIDUAL_TOKEN not set.
    GOTO SECTION_4_RESUMES
)

:TEST_USERS_ME_GET
ECHO.
ECHO === 3.1 获取当前个人用户信息 ===
curl -X GET "%BASE_URL%/users/me" -H "Authorization: Bearer %INDIVIDUAL_TOKEN%"
ECHO. & PAUSE

:TEST_USERS_ME_PUT
ECHO.
ECHO === 3.2 修改当前个人用户信息 ===
curl -X PUT "%BASE_URL%/users/me" ^
     -H "Authorization: Bearer %INDIVIDUAL_TOKEN%" ^
     -H "Content-Type: application/json" ^
     -d "{\"phoneNumber\":\"13800138002\",\"avatarURL\":\"http://example.com/newavatar.png\"}"
ECHO. & PAUSE

:TEST_USERS_ME_PASSWORD_PUT
ECHO.
ECHO === 3.3 修改当前个人用户密码 ===
curl -X PUT "%BASE_URL%/users/me/password" ^
     -H "Authorization: Bearer %INDIVIDUAL_TOKEN%" ^
     -H "Content-Type: application/json" ^
     -d "{\"oldPassword\":\"Password123\",\"newPassword\":\"NewPassword456\"}"
ECHO.
ECHO NOTE: After password change, the current INDIVIDUAL_TOKEN will be invalid.
ECHO You would need to re-login (Section 2.2) to get a new token.
ECHO For simplicity, this script will continue with the old token, which might cause subsequent tests to fail.
ECHO Or, you can re-run Section 2.2 with new password and update INDIVIDUAL_TOKEN.
PAUSE

:SECTION_4_RESUMES
REM =============================================================================
REM == 4. 简历 (Resumes) 模块
REM =============================================================================
ECHO.
ECHO ###################################
ECHO ## 4. 简历 (Resumes) 模块       ##
ECHO ###################################
IF NOT DEFINED INDIVIDUAL_TOKEN (
    ECHO Skipping Section 4 - INDIVIDUAL_TOKEN not set.
    GOTO SECTION_5_JOBS
)

:TEST_RESUMES_LIST_GET
ECHO.
ECHO === 4.1 获取当前用户的简历列表 ===
curl -X GET "%BASE_URL%/users/me/resumes" -H "Authorization: Bearer %INDIVIDUAL_TOKEN%"
ECHO. & PAUSE

:TEST_RESUMES_CREATE_POST
ECHO.
ECHO === 4.2 创建新简历 ===
ECHO Note the 'resumeId' from the response. Update CREATED_RESUME_ID variable for subsequent tests.
curl -X POST "%BASE_URL%/users/me/resumes" ^
     -H "Authorization: Bearer %INDIVIDUAL_TOKEN%" ^
     -H "Content-Type: application/json" ^
     -d "{\"resumeTitle\":\"My Awesome Resume\",\"basicInfo\":{\"name\":\"Test User\",\"gender\":\"Male\",\"birthDate\":\"1990-01-01\"},\"jobIntent\":{\"industry\":\"IT\",\"position\":\"Developer\",\"salary\":\"15k\"},\"educationExperience\":[{\"school\":\"Test University\",\"degree\":\"Bachelor\"}],\"workExperience\":[{\"company\":\"Test Corp\",\"role\":\"Intern\"}],\"visibilityStatus\":\"Private\"}"
ECHO.
SET /P "CREATED_RESUME_ID_INPUT=Paste created resumeId here and press Enter: "
IF DEFINED CREATED_RESUME_ID_INPUT SET CREATED_RESUME_ID=%CREATED_RESUME_ID_INPUT%
IF DEFINED CREATED_RESUME_ID ( ECHO CREATED_RESUME_ID set to: %CREATED_RESUME_ID% ) ELSE ( ECHO CREATED_RESUME_ID not set. )
PAUSE

:TEST_RESUMES_DETAIL_GET
IF NOT DEFINED CREATED_RESUME_ID GOTO SKIP_RESUMES_DETAIL_GET
ECHO.
ECHO === 4.3 获取简历详情 (ID: %CREATED_RESUME_ID%) ===
curl -X GET "%BASE_URL%/users/me/resumes/%CREATED_RESUME_ID%" -H "Authorization: Bearer %INDIVIDUAL_TOKEN%"
ECHO. & PAUSE
:SKIP_RESUMES_DETAIL_GET

:TEST_RESUMES_UPDATE_PUT
IF NOT DEFINED CREATED_RESUME_ID GOTO SKIP_RESUMES_UPDATE_PUT
ECHO.
ECHO === 4.4 修改简历 (ID: %CREATED_RESUME_ID%) ===
curl -X PUT "%BASE_URL%/users/me/resumes/%CREATED_RESUME_ID%" ^
     -H "Authorization: Bearer %INDIVIDUAL_TOKEN%" ^
     -H "Content-Type: application/json" ^
     -d "{\"resumeTitle\":\"My Updated Awesome Resume\",\"selfDescription\":\"A bit more about me.\",\"visibilityStatus\":\"PublicToAll\"}"
ECHO. & PAUSE
:SKIP_RESUMES_UPDATE_PUT

:TEST_RESUMES_DELETE
IF NOT DEFINED CREATED_RESUME_ID GOTO SKIP_RESUMES_DELETE
ECHO.
ECHO === 4.5 删除简历 (ID: %CREATED_RESUME_ID%) ===
curl -X DELETE "%BASE_URL%/users/me/resumes/%CREATED_RESUME_ID%" -H "Authorization: Bearer %INDIVIDUAL_TOKEN%"
ECHO. & PAUSE
:SKIP_RESUMES_DELETE

:SECTION_5_JOBS
REM =============================================================================
REM == 5. 职位 (Jobs) 模块 (公开查询)
REM =============================================================================
ECHO.
ECHO ###################################
ECHO ## 5. 职位 (Jobs) 模块 (公开)   ##
ECHO ###################################

:TEST_JOBS_LIST_GET
ECHO.
ECHO === 5.1 查询职位列表 (公开) ===
curl -X GET "%BASE_URL%/jobs?keyword=developer&location=Anytown&page=1&limit=5&sortBy=publishDate&sortOrder=desc"
ECHO. & PAUSE

:TEST_JOBS_DETAIL_GET
ECHO.
ECHO === 5.2 获取职位详情 (公开) ===
ECHO You might need a valid jobId from a running system or from enterprise job creation (7.5)
SET /P "PUBLIC_JOB_ID_TO_VIEW_INPUT=Enter a public Job ID to view (e.g., 1, or one created by enterprise): "
IF NOT DEFINED PUBLIC_JOB_ID_TO_VIEW_INPUT (
    ECHO No Job ID entered, skipping.
    GOTO SKIP_JOBS_DETAIL_GET
)
SET PUBLIC_JOB_ID_TO_APPLY=%PUBLIC_JOB_ID_TO_VIEW_INPUT%
curl -X GET "%BASE_URL%/jobs/%PUBLIC_JOB_ID_TO_VIEW_INPUT%"
ECHO. & PAUSE
:SKIP_JOBS_DETAIL_GET

:SECTION_6_APPLICATIONS
REM =============================================================================
REM == 6. 职位申请 (Applications) 模块
REM =============================================================================
ECHO.
ECHO ###################################
ECHO ## 6. 职位申请 (Applications)   ##
ECHO ###################################
IF NOT DEFINED INDIVIDUAL_TOKEN (
    ECHO Skipping Section 6 - INDIVIDUAL_TOKEN not set.
    GOTO SECTION_7_ENTERPRISES
)
IF NOT DEFINED PUBLIC_JOB_ID_TO_APPLY (
    ECHO Skipping 6.1 - PUBLIC_JOB_ID_TO_APPLY not set (get from 5.2 or 7.5).
    GOTO SKIP_JOB_APPLY_POST
)
IF NOT DEFINED CREATED_RESUME_ID (
    ECHO Skipping 6.1 - CREATED_RESUME_ID not set (create one in 4.2 first).
    GOTO SKIP_JOB_APPLY_POST
)

:TEST_JOB_APPLY_POST
ECHO.
ECHO === 6.1 个人用户申请职位 (Job ID: %PUBLIC_JOB_ID_TO_APPLY%, Resume ID: %CREATED_RESUME_ID%) ===
ECHO Note 'applicationId' from response. Update CREATED_APPLICATION_ID for enterprise tests (7.10).
curl -X POST "%BASE_URL%/jobs/%PUBLIC_JOB_ID_TO_APPLY%/apply" ^
     -H "Authorization: Bearer %INDIVIDUAL_TOKEN%" ^
     -H "Content-Type: application/json" ^
     -d "{\"resumeId\":%CREATED_RESUME_ID%}"
ECHO.
SET /P "CREATED_APPLICATION_ID_INPUT=Paste created applicationId here and press Enter: "
IF DEFINED CREATED_APPLICATION_ID_INPUT SET CREATED_APPLICATION_ID=%CREATED_APPLICATION_ID_INPUT%
IF DEFINED CREATED_APPLICATION_ID ( ECHO CREATED_APPLICATION_ID set to: %CREATED_APPLICATION_ID% ) ELSE ( ECHO CREATED_APPLICATION_ID not set. )
PAUSE
:SKIP_JOB_APPLY_POST

:TEST_USER_APPLICATIONS_GET
ECHO.
ECHO === 6.2 获取个人用户的求职申请列表 ===
curl -X GET "%BASE_URL%/users/me/applications?status=Submitted&page=1&limit=5" -H "Authorization: Bearer %INDIVIDUAL_TOKEN%"
ECHO. & PAUSE

:SECTION_7_ENTERPRISES
REM =============================================================================
REM == 7. 企业用户 (Enterprises) 模块
REM =============================================================================
ECHO.
ECHO ###################################
ECHO ## 7. 企业用户 (Enterprises)    ##
ECHO ###################################
IF NOT DEFINED ENTERPRISE_TOKEN (
    ECHO Skipping Section 7 - ENTERPRISE_TOKEN not set (Login at 2.4, requires admin approval).
    GOTO SECTION_8_ADMIN
)

:TEST_ENTERPRISES_ME_GET
ECHO.
ECHO === 7.1 获取当前企业信息 ===
curl -X GET "%BASE_URL%/enterprises/me" -H "Authorization: Bearer %ENTERPRISE_TOKEN%"
ECHO. & PAUSE

:TEST_ENTERPRISES_ME_PUT
ECHO.
ECHO === 7.2 修改当前企业信息 ===
curl -X PUT "%BASE_URL%/enterprises/me" ^
     -H "Authorization: Bearer %ENTERPRISE_TOKEN%" ^
     -H "Content-Type: application/json" ^
     -d "{\"description\":\"Updated enterprise description.\",\"logoURL\":\"http://example.com/new_logo.png\"}"
ECHO. & PAUSE

:TEST_ENTERPRISES_ME_PASSWORD_PUT
ECHO.
ECHO === 7.3 修改当前企业用户密码 ===
curl -X PUT "%BASE_URL%/enterprises/me/password" ^
     -H "Authorization: Bearer %ENTERPRISE_TOKEN%" ^
     -H "Content-Type: application/json" ^
     -d "{\"oldPassword\":\"Password123Ent\",\"newPassword\":\"NewEntPassword456\"}"
ECHO.
ECHO NOTE: After password change, ENTERPRISE_TOKEN will be invalid. Re-login (2.4) if needed.
PAUSE

:TEST_ENTERPRISES_ME_JOBS_GET
ECHO.
ECHO === 7.4 获取企业发布的职位列表 ===
curl -X GET "%BASE_URL%/enterprises/me/jobs?status=Recruiting&page=1&limit=5" -H "Authorization: Bearer %ENTERPRISE_TOKEN%"
ECHO. & PAUSE

:TEST_ENTERPRISES_ME_JOBS_POST
ECHO.
ECHO === 7.5 企业发布新职位 ===
ECHO Note 'jobId' from response. Update CREATED_ENTERPRISE_JOB_ID variable. This can also be used for PUBLIC_JOB_ID_TO_APPLY.
curl -X POST "%BASE_URL%/enterprises/me/jobs" ^
     -H "Authorization: Bearer %ENTERPRISE_TOKEN%" ^
     -H "Content-Type: application/json" ^
     -d "{\"jobTitle\":\"Senior Bat Script Developer\",\"workLocation\":\"Remote HQ\",\"responsibilities\":\"Write amazing bat scripts\",\"requirements\":\"Expert in ECHO and GOTO\",\"minSalary\":5000,\"maxSalary\":10000,\"jobStatus\":\"Recruiting\"}"
ECHO.
SET /P "CREATED_ENTERPRISE_JOB_ID_INPUT=Paste created enterprise jobId here and press Enter: "
IF DEFINED CREATED_ENTERPRISE_JOB_ID_INPUT SET CREATED_ENTERPRISE_JOB_ID=%CREATED_ENTERPRISE_JOB_ID_INPUT%
IF DEFINED CREATED_ENTERPRISE_JOB_ID (
    ECHO CREATED_ENTERPRISE_JOB_ID set to: %CREATED_ENTERPRISE_JOB_ID%
    IF NOT DEFINED PUBLIC_JOB_ID_TO_APPLY SET PUBLIC_JOB_ID_TO_APPLY=%CREATED_ENTERPRISE_JOB_ID%
    ECHO PUBLIC_JOB_ID_TO_APPLY (if not set before) is now: %PUBLIC_JOB_ID_TO_APPLY%
) ELSE (
    ECHO CREATED_ENTERPRISE_JOB_ID not set.
)
PAUSE

:TEST_ENTERPRISES_ME_JOB_DETAIL_GET
IF NOT DEFINED CREATED_ENTERPRISE_JOB_ID GOTO SKIP_ENTERPRISES_ME_JOB_DETAIL_GET
ECHO.
ECHO === 7.6 获取企业发布的某个职位详情 (ID: %CREATED_ENTERPRISE_JOB_ID%) ===
curl -X GET "%BASE_URL%/enterprises/me/jobs/%CREATED_ENTERPRISE_JOB_ID%" -H "Authorization: Bearer %ENTERPRISE_TOKEN%"
ECHO. & PAUSE
:SKIP_ENTERPRISES_ME_JOB_DETAIL_GET

:TEST_ENTERPRISES_ME_JOB_UPDATE_PUT
IF NOT DEFINED CREATED_ENTERPRISE_JOB_ID GOTO SKIP_ENTERPRISES_ME_JOB_UPDATE_PUT
ECHO.
ECHO === 7.7 修改企业发布的职位 (ID: %CREATED_ENTERPRISE_JOB_ID%) ===
curl -X PUT "%BASE_URL%/enterprises/me/jobs/%CREATED_ENTERPRISE_JOB_ID%" ^
     -H "Authorization: Bearer %ENTERPRISE_TOKEN%" ^
     -H "Content-Type: application/json" ^
     -d "{\"jobTitle\":\"Ultra Bat Script Developer\",\"benefits\":\"Unlimited coffee\"}"
ECHO. & PAUSE
:SKIP_ENTERPRISES_ME_JOB_UPDATE_PUT

:TEST_ENTERPRISES_ME_JOB_APPLICANTS_GET
IF NOT DEFINED CREATED_ENTERPRISE_JOB_ID GOTO SKIP_ENTERPRISES_ME_JOB_APPLICANTS_GET
ECHO.
ECHO === 7.9 获取某职位的申请者列表 (Job ID: %CREATED_ENTERPRISE_JOB_ID%) ===
curl -X GET "%BASE_URL%/enterprises/me/jobs/%CREATED_ENTERPRISE_JOB_ID%/applicants?status=Submitted&page=1&limit=5" -H "Authorization: Bearer %ENTERPRISE_TOKEN%"
ECHO. & PAUSE
:SKIP_ENTERPRISES_ME_JOB_APPLICANTS_GET

:TEST_ENTERPRISES_ME_APPLICATION_STATUS_PUT
IF NOT DEFINED CREATED_APPLICATION_ID GOTO SKIP_ENTERPRISES_ME_APPLICATION_STATUS_PUT
ECHO.
ECHO === 7.10 更新职位申请状态 (Enterprise operation, Application ID: %CREATED_APPLICATION_ID%) ===
curl -X PUT "%BASE_URL%/enterprises/me/applications/%CREATED_APPLICATION_ID%/status" ^
     -H "Authorization: Bearer %ENTERPRISE_TOKEN%" ^
     -H "Content-Type: application/json" ^
     -d "{\"newStatus\":\"ViewedByHR\",\"enterpriseNotes\":\"Looks promising\"}"
ECHO. & PAUSE
:SKIP_ENTERPRISES_ME_APPLICATION_STATUS_PUT

:TEST_ENTERPRISES_ME_RESUMES_SEARCH_GET
ECHO.
ECHO === 7.11 企业搜索简历 ===
curl -X GET "%BASE_URL%/enterprises/me/resumes/search?keyword=developer&education=Bachelor&experience=3years" -H "Authorization: Bearer %ENTERPRISE_TOKEN%"
ECHO. & PAUSE

:TEST_ENTERPRISES_ME_JOB_DELETE
IF NOT DEFINED CREATED_ENTERPRISE_JOB_ID GOTO SKIP_ENTERPRISES_ME_JOB_DELETE
ECHO.
ECHO === 7.8 删除企业发布的职位 (ID: %CREATED_ENTERPRISE_JOB_ID%) ===
curl -X DELETE "%BASE_URL%/enterprises/me/jobs/%CREATED_ENTERPRISE_JOB_ID%" -H "Authorization: Bearer %ENTERPRISE_TOKEN%"
ECHO. & PAUSE
:SKIP_ENTERPRISES_ME_JOB_DELETE


:SECTION_8_ADMIN
REM =============================================================================
REM == 8. 管理员 (Admin) 模块
REM =============================================================================
ECHO.
ECHO ###################################
ECHO ## 8. 管理员 (Admin) 模块       ##
ECHO ###################################
IF NOT DEFINED ADMIN_TOKEN (
    ECHO Skipping Section 8 - ADMIN_TOKEN not set (Login at 2.5).
    GOTO SECTION_9_FEEDBACK
)

:TEST_ADMIN_USERS_GET
ECHO.
ECHO === 8.1 获取个人用户列表 (Admin) ===
curl -X GET "%BASE_URL%/admin/users?filter=status%3DActive" -H "Authorization: Bearer %ADMIN_TOKEN%"
ECHO.
IF DEFINED INDIVIDUAL_USER_ID SET USER_ID_FOR_ADMIN_TEST=%INDIVIDUAL_USER_ID%
SET /P "USER_ID_FOR_ADMIN_TEST_INPUT=Enter a userId for Admin tests (e.g., from 2.1 or above list, current: %USER_ID_FOR_ADMIN_TEST%): "
IF DEFINED USER_ID_FOR_ADMIN_TEST_INPUT SET USER_ID_FOR_ADMIN_TEST=%USER_ID_FOR_ADMIN_TEST_INPUT%
PAUSE

:TEST_ADMIN_USERS_STATUS_PUT
IF NOT DEFINED USER_ID_FOR_ADMIN_TEST GOTO SKIP_ADMIN_USERS_STATUS_PUT
ECHO.
ECHO === 8.2 更新个人用户状态 (Admin, User ID: %USER_ID_FOR_ADMIN_TEST%) ===
curl -X PUT "%BASE_URL%/admin/users/%USER_ID_FOR_ADMIN_TEST%/status" ^
     -H "Authorization: Bearer %ADMIN_TOKEN%" ^
     -H "Content-Type: application/json" ^
     -d "{\"newStatus\":\"Disabled\"}"
ECHO. & PAUSE
:SKIP_ADMIN_USERS_STATUS_PUT

:TEST_ADMIN_ENTERPRISES_GET
ECHO.
ECHO === 8.3 获取企业用户列表 (Admin) ===
curl -X GET "%BASE_URL%/admin/enterprises?filter=accountStatus%3DPendingReview" -H "Authorization: Bearer %ADMIN_TOKEN%"
ECHO.
REM Try to get Enterprise ID from enterprise registration if available
FOR /F "tokens=2 delims=:, " %%i IN ('curl -s -X POST "%BASE_URL%/auth/enterprise/register" -H "Content-Type: application/json" -d "{\"loginUsername\":\"temp_ent_for_id_%RANDOM%\",\"password\":\"Password123Ent\",\"enterpriseName\":\"Temp Ent For ID\",\"creditCode\":\"TEMPCODE%RANDOM%\",\"contactPerson\":\"Test\",\"contactPhone\":\"13900139002\",\"contactEmail\":\"contact@temp.com\",\"licenseImageURL\":\"http://example.com/license.jpg\"}" ^| find "enterpriseId"') DO SET ENTERPRISE_ID_FROM_REG=%%i
IF DEFINED ENTERPRISE_ID_FROM_REG SET ENTERPRISE_ID_FOR_ADMIN_TEST=%ENTERPRISE_ID_FROM_REG%

SET /P "ENTERPRISE_ID_FOR_ADMIN_TEST_INPUT=Enter an enterpriseId for Admin tests (e.g., from 2.3 or above list, current: %ENTERPRISE_ID_FOR_ADMIN_TEST%): "
IF DEFINED ENTERPRISE_ID_FOR_ADMIN_TEST_INPUT SET ENTERPRISE_ID_FOR_ADMIN_TEST=%ENTERPRISE_ID_FOR_ADMIN_TEST_INPUT%
PAUSE

:TEST_ADMIN_ENTERPRISES_APPROVE_PUT
IF NOT DEFINED ENTERPRISE_ID_FOR_ADMIN_TEST GOTO SKIP_ADMIN_ENTERPRISES_APPROVE_PUT
ECHO.
ECHO === 8.4 批准企业注册 (Admin, Enterprise ID: %ENTERPRISE_ID_FOR_ADMIN_TEST%) ===
curl -X PUT "%BASE_URL%/admin/enterprises/%ENTERPRISE_ID_FOR_ADMIN_TEST%/approve" ^
     -H "Authorization: Bearer %ADMIN_TOKEN%" ^
     -H "Content-Type: application/json" ^
     -d "{\"auditOpinion\":\"Approved via batch test.\"}"
ECHO. & PAUSE
:SKIP_ADMIN_ENTERPRISES_APPROVE_PUT

:TEST_ADMIN_ENTERPRISES_REJECT_PUT
IF NOT DEFINED ENTERPRISE_ID_FOR_ADMIN_TEST GOTO SKIP_ADMIN_ENTERPRISES_REJECT_PUT
ECHO.
ECHO === 8.5 拒绝企业注册 (Admin, Enterprise ID: %ENTERPRISE_ID_FOR_ADMIN_TEST%) ===
curl -X PUT "%BASE_URL%/admin/enterprises/%ENTERPRISE_ID_FOR_ADMIN_TEST%/reject" ^
     -H "Authorization: Bearer %ADMIN_TOKEN%" ^
     -H "Content-Type: application/json" ^
     -d "{\"auditOpinion\":\"Rejected: Incomplete information (test).\"}"
ECHO. & PAUSE
:SKIP_ADMIN_ENTERPRISES_REJECT_PUT

:TEST_ADMIN_JOBS_GET
ECHO.
ECHO === 8.7 管理所有职位 (Admin) ===
curl -X GET "%BASE_URL%/admin/jobs?filter=jobStatus%3DRecruiting" -H "Authorization: Bearer %ADMIN_TOKEN%"
ECHO.
IF DEFINED CREATED_ENTERPRISE_JOB_ID SET JOB_ID_FOR_ADMIN_TEST=%CREATED_ENTERPRISE_JOB_ID%
SET /P "JOB_ID_FOR_ADMIN_TEST_INPUT=Enter a jobId for Admin tests (e.g., from 7.5 or above list, current: %JOB_ID_FOR_ADMIN_TEST%): "
IF DEFINED JOB_ID_FOR_ADMIN_TEST_INPUT SET JOB_ID_FOR_ADMIN_TEST=%JOB_ID_FOR_ADMIN_TEST_INPUT%
PAUSE

:TEST_ADMIN_JOBS_STATUS_PUT
IF NOT DEFINED JOB_ID_FOR_ADMIN_TEST GOTO SKIP_ADMIN_JOBS_STATUS_PUT
ECHO.
ECHO === 8.8 更新职位状态 (Admin, Job ID: %JOB_ID_FOR_ADMIN_TEST%) ===
curl -X PUT "%BASE_URL%/admin/jobs/%JOB_ID_FOR_ADMIN_TEST%/status" ^
     -H "Authorization: Bearer %ADMIN_TOKEN%" ^
     -H "Content-Type: application/json" ^
     -d "{\"newStatus\":\"Offline\",\"auditOpinion\":\"Temporarily offline by admin.\"}"
ECHO. & PAUSE
:SKIP_ADMIN_JOBS_STATUS_PUT


:SECTION_9_FEEDBACK
REM =============================================================================
REM == 9. 反馈 (Feedback) 模块
REM =============================================================================
ECHO.
ECHO ###################################
ECHO ## 9. 反馈 (Feedback) 模块      ##
ECHO ###################################

:TEST_FEEDBACK_POST_ANONYMOUS
ECHO.
ECHO === 9.1 用户提交反馈 (Anonymous) ===
curl -X POST "%BASE_URL%/feedback" ^
     -H "Content-Type: application/json" ^
     -d "{\"feedbackType\":\"Suggestion\",\"content\":\"Anonymous suggestion for the platform.\",\"contactInfo\":\"anon@example.com\"}"
ECHO. & PAUSE

:TEST_FEEDBACK_POST_AUTHENTICATED
IF NOT DEFINED INDIVIDUAL_TOKEN GOTO SKIP_FEEDBACK_POST_AUTHENTICATED
ECHO.
ECHO === 9.1 用户提交反馈 (Authenticated Individual User) ===
ECHO Note 'feedbackId' from response. Update FEEDBACK_ID_FOR_ADMIN_TEST for admin tests.
curl -X POST "%BASE_URL%/feedback" ^
     -H "Authorization: Bearer %INDIVIDUAL_TOKEN%" ^
     -H "Content-Type: application/json" ^
     -d "{\"feedbackType\":\"BugReport\",\"content\":\"Found a small bug in the user profile page.\",\"contactInfo\":\"user@example.com\"}"
ECHO.
SET /P "FEEDBACK_ID_FOR_ADMIN_TEST_INPUT=Paste created feedbackId here and press Enter: "
IF DEFINED FEEDBACK_ID_FOR_ADMIN_TEST_INPUT SET FEEDBACK_ID_FOR_ADMIN_TEST=%FEEDBACK_ID_FOR_ADMIN_TEST_INPUT%
PAUSE
:SKIP_FEEDBACK_POST_AUTHENTICATED

:TEST_ADMIN_FEEDBACKS_GET
IF NOT DEFINED ADMIN_TOKEN GOTO SKIP_ADMIN_FEEDBACKS_GET
ECHO.
ECHO === 9.2 管理员获取反馈列表 ===
curl -X GET "%BASE_URL%/admin/feedbacks?status=Pending&page=1&limit=5" -H "Authorization: Bearer %ADMIN_TOKEN%"
ECHO. & PAUSE
:SKIP_ADMIN_FEEDBACKS_GET

:TEST_ADMIN_FEEDBACKS_PUT
IF NOT DEFINED ADMIN_TOKEN GOTO SKIP_ADMIN_FEEDBACKS_PUT
IF NOT DEFINED FEEDBACK_ID_FOR_ADMIN_TEST GOTO SKIP_ADMIN_FEEDBACKS_PUT
ECHO.
ECHO === 9.3 管理员处理反馈 (Feedback ID: %FEEDBACK_ID_FOR_ADMIN_TEST%) ===
curl -X PUT "%BASE_URL%/admin/feedbacks/%FEEDBACK_ID_FOR_ADMIN_TEST%" ^
     -H "Authorization: Bearer %ADMIN_TOKEN%" ^
     -H "Content-Type: application/json" ^
     -d "{\"processStatus\":\"Resolved\",\"adminReply\":\"Thank you for your feedback. This has been addressed.\"}"
ECHO. & PAUSE
:SKIP_ADMIN_FEEDBACKS_PUT


:SECTION_10_ANNOUNCEMENTS
REM =============================================================================
REM == 10. 公告 (Announcements) 模块
REM =============================================================================
ECHO.
ECHO ###################################
ECHO ## 10. 公告 (Announcements)     ##
ECHO ###################################

:TEST_ANNOUNCEMENTS_LIST_GET
ECHO.
ECHO === 10.1 获取公告列表 (公开) ===
curl -X GET "%BASE_URL%/announcements?page=1&limit=3"
ECHO. & PAUSE

:TEST_ANNOUNCEMENTS_DETAIL_GET
ECHO.
ECHO === 10.2 获取公告详情 (公开) ===
ECHO You might need a valid announcementId from a running system or from admin creation (10.3)
SET /P "ANNOUNCEMENT_ID_TO_VIEW_INPUT=Enter an Announcement ID to view (e.g., 1): "
IF NOT DEFINED ANNOUNCEMENT_ID_TO_VIEW_INPUT (
    ECHO No Announcement ID entered, skipping.
    GOTO SKIP_ANNOUNCEMENTS_DETAIL_GET
)
curl -X GET "%BASE_URL%/announcements/%ANNOUNCEMENT_ID_TO_VIEW_INPUT%"
ECHO. & PAUSE
:SKIP_ANNOUNCEMENTS_DETAIL_GET

:TEST_ADMIN_ANNOUNCEMENTS_POST
IF NOT DEFINED ADMIN_TOKEN GOTO SKIP_ADMIN_ANNOUNCEMENTS_POST
ECHO.
ECHO === 10.3 管理员创建公告 ===
ECHO Note 'announcementId' from response. Update ANNOUNCEMENT_ID_FOR_ADMIN_TEST for subsequent tests.
curl -X POST "%BASE_URL%/admin/announcements" ^
     -H "Authorization: Bearer %ADMIN_TOKEN%" ^
     -H "Content-Type: application/json" ^
     -d "{\"title\":\"New Platform Update\",\"content\":\"We have released new features! Check them out.\",\"isTop\":true}"
ECHO.
SET /P "ANNOUNCEMENT_ID_FOR_ADMIN_TEST_INPUT=Paste created announcementId here and press Enter: "
IF DEFINED ANNOUNCEMENT_ID_FOR_ADMIN_TEST_INPUT SET ANNOUNCEMENT_ID_FOR_ADMIN_TEST=%ANNOUNCEMENT_ID_FOR_ADMIN_TEST_INPUT%
PAUSE
:SKIP_ADMIN_ANNOUNCEMENTS_POST

:TEST_ADMIN_ANNOUNCEMENTS_PUT
IF NOT DEFINED ADMIN_TOKEN GOTO SKIP_ADMIN_ANNOUNCEMENTS_PUT
IF NOT DEFINED ANNOUNCEMENT_ID_FOR_ADMIN_TEST GOTO SKIP_ADMIN_ANNOUNCEMENTS_PUT
ECHO.
ECHO === 10.4 管理员修改公告 (ID: %ANNOUNCEMENT_ID_FOR_ADMIN_TEST%) ===
curl -X PUT "%BASE_URL%/admin/announcements/%ANNOUNCEMENT_ID_FOR_ADMIN_TEST%" ^
     -H "Authorization: Bearer %ADMIN_TOKEN%" ^
     -H "Content-Type: application/json" ^
     -d "{\"title\":\"Platform Update Revised\",\"content\":\"Revised content for the update.\",\"isTop\":false}"
ECHO. & PAUSE
:SKIP_ADMIN_ANNOUNCEMENTS_PUT

:TEST_ADMIN_ANNOUNCEMENTS_DELETE
IF NOT DEFINED ADMIN_TOKEN GOTO SKIP_ADMIN_ANNOUNCEMENTS_DELETE
IF NOT DEFINED ANNOUNCEMENT_ID_FOR_ADMIN_TEST GOTO SKIP_ADMIN_ANNOUNCEMENTS_DELETE
ECHO.
ECHO === 10.5 管理员删除公告 (ID: %ANNOUNCEMENT_ID_FOR_ADMIN_TEST%) ===
curl -X DELETE "%BASE_URL%/admin/announcements/%ANNOUNCEMENT_ID_FOR_ADMIN_TEST%" -H "Authorization: Bearer %ADMIN_TOKEN%"
ECHO. & PAUSE
:SKIP_ADMIN_ANNOUNCEMENTS_DELETE

ECHO.
ECHO ==========================================
ECHO == All Tests Completed or Skipped      ==
ECHO ==========================================
ECHO.
PAUSE
ENDLOCAL