cmake_minimum_required(VERSION 3.20)
project(asio_http)

set(CMAKE_CXX_STANDARD 20)  # ASIO可能不需要C++23标准，使用C++20足够
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置使用brew安装的clang++编译器
set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")
set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")

# 查找ASIO包
find_package(PkgConfig REQUIRED)
pkg_check_modules(ASIO REQUIRED asio)

# 查找线程库
find_package(Threads REQUIRED)

# 查找clang-format工具
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
  # 获取所有源文件
  file(GLOB_RECURSE FORMAT_SOURCE_FILES "*.cc" "*.h" "*.hpp")
  
  # 添加格式化目标
  add_custom_target(
    format
    COMMAND ${CLANG_FORMAT} -i -style=file ${FORMAT_SOURCE_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Formatting source code with clang-format"
  )
  
  # 添加格式检查目标（用于CI等环境）
  add_custom_target(
    check-format
    COMMAND ${CLANG_FORMAT} -output-replacements-xml -style=file ${FORMAT_SOURCE_FILES} | grep -q "replacement" && echo "Code formatting differs from expected" && exit 1 || echo "Code formatting is correct"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Checking code formatting with clang-format"
  )
endif()

# 添加编译选项
add_compile_options(-Wall -Wextra -Wpedantic)
add_compile_definitions(ASIO_STANDALONE)

# 创建可执行文件
add_executable(asio_http asio_http.cc)

# 包含ASIO头文件目录
target_include_directories(asio_http PRIVATE ${ASIO_INCLUDE_DIRS})

# 链接线程库
target_link_libraries(asio_http PRIVATE Threads::Threads)

# 因为asio是header-only库，只需要包含头文件即可，无需链接