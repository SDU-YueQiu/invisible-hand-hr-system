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

project ("invisible-hand-hr-system" VERSION 0.1)

find_package(Crow CONFIG REQUIRED)

file(GLOB_RECURSE SERVER_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/code/server/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/code/server/*.h"
)

add_executable (invisible-hand-hr-system ${SERVER_SOURCES})

target_link_libraries(invisible-hand-hr-system PRIVATE Crow::Crow asio::asio)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET invisible-hand-hr-system PROPERTY CXX_STANDARD 20)
endif()
