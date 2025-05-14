# CMakeList.txt: invisible-hand-hr-system 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.30)

# # 如果支持，请为 MSVC 编译器启用热重载。
# if (POLICY CMP0141)
#   cmake_policy(SET CMP0141 NEW)
#   set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
# endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(VCPKG_TARGET_TRIPLET "x64-mingw-static" CACHE STRING "" FORCE)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstandalone-debug")

project ("invisible-hand-hr-system" VERSION 0.1)

find_package(Crow CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(SQLiteCpp CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(jwt-cpp CONFIG REQUIRED)

file(GLOB_RECURSE SERVER_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/code/server/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/code/server/*.cpp"
)

add_executable (invisible-hand-hr-system ${SERVER_SOURCES})

target_compile_definitions(invisible-hand-hr-system PRIVATE CROW_USE_LOCALTIMEZONE)

target_link_libraries(invisible-hand-hr-system PRIVATE Crow::Crow asio::asio unofficial::sqlite3::sqlite3 SQLiteCpp OpenSSL::SSL OpenSSL::Crypto jwt-cpp::jwt-cpp)

# 添加构建后自定义命令：创建目录并复制SQL文件
add_custom_command(
    TARGET invisible-hand-hr-system
    POST_BUILD
    # 创建 build/bin/db 目录
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/db"
    # 复制 databaseInit.sql 到目标目录
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/code/server/Database/databaseInit.sql" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/db/"
    COMMENT "Creating db directory and copying database initialization SQL..."
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET invisible-hand-hr-system PROPERTY CXX_STANDARD 17)
endif()
