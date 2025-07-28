cmake_minimum_required(VERSION 3.10)
project(design_patterns)

set(CMAKE_CXX_STANDARD 17)

# Add subdirectories for each pattern category

add_subdirectory(src/creational)
add_subdirectory(src/structural)
add_subdirectory(src/behavioral)

# 顶层可执行文件
add_executable(design_patterns src/main.cpp)
target_link_libraries(design_patterns behavioral_patterns)
