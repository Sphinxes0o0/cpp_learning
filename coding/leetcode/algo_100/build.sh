#!/bin/bash

# LeetCode Algorithm 100 - Build Script
# 用于自动化构建所有算法题目

echo "🚀 LeetCode Algorithm 100 - Build Script"
echo "========================================"

# 创建 build 目录
if [ ! -d "build" ]; then
    echo "📁 Creating build directory..."
    mkdir build
fi

# 进入 build 目录
cd build

# 运行 CMake 配置
echo "⚙️  Running CMake configuration..."
cmake ..

# 检查 CMake 是否成功
if [ $? -ne 0 ]; then
    echo "❌ CMake configuration failed!"
    exit 1
fi

# 编译所有目标
echo "🔨 Building all targets..."
make

# 检查编译是否成功
if [ $? -ne 0 ]; then
    echo "❌ Build failed!"
    exit 1
fi

echo "✅ Build completed successfully!"
echo ""
echo "📂 Executables are located in: build/bin/"
echo "🏃 To run all tests: make run_all_tests"
echo "🧹 To clean: make clean_all"
echo ""

# 列出生成的可执行文件
echo "📋 Generated executables:"
ls -la bin/ | grep -v "^d" | awk '{print "   " $9}' | grep -v "^   $" 