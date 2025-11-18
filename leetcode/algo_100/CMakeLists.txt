cmake_minimum_required(VERSION 3.10)

# 项目名称
project(LeetCode_Algo_100)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 查找所有的.cc文件
file(GLOB CC_FILES "*.cc")

# 为每个.cc文件创建独立的可执行文件
foreach(CC_FILE ${CC_FILES})
    # 获取文件名（不包含扩展名）
    get_filename_component(EXEC_NAME ${CC_FILE} NAME_WE)
    
    # 创建可执行文件
    add_executable(${EXEC_NAME} ${CC_FILE})
    
    # 打印信息
    message(STATUS "Creating executable: ${EXEC_NAME} from ${CC_FILE}")
endforeach()

# 添加一个自定义目标来运行所有测试
add_custom_target(run_all_tests
    COMMENT "Running all algorithm tests"
)

# 为每个可执行文件添加运行命令
foreach(CC_FILE ${CC_FILES})
    get_filename_component(EXEC_NAME ${CC_FILE} NAME_WE)
    add_custom_command(TARGET run_all_tests POST_BUILD
        COMMAND echo "Running ${EXEC_NAME}..."
        COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXEC_NAME}
        COMMAND echo "Finished ${EXEC_NAME}"
        COMMAND echo "------------------------"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endforeach()

# 添加清理目标
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin
    COMMENT "Cleaning all executables"
) 