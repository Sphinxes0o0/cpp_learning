# LeetCode Algorithm 100 - CMake Project

这是一个基于 CMake 的 LeetCode 算法题目集合，每个 `.cc` 文件都是独立的可执行程序。

## 🏗️ 项目结构

```
leetcode/algo_100/
├── CMakeLists.txt          # CMake 配置文件
├── build.sh                # 自动构建脚本
├── README.md              # 本文件
├── confusing_number.cc    # 易混淆数问题
└── build/                 # 构建目录（自动生成）
    └── bin/               # 可执行文件目录
```

## 🚀 快速开始

### 方法一：使用构建脚本（推荐）

```bash
# 在 leetcode/algo_100 目录下执行
./build.sh
```

### 方法二：手动构建

```bash
# 创建构建目录
mkdir build && cd build

# 配置 CMake
cmake ..

# 编译所有程序
make

# 运行所有测试
make run_all_tests
```

## 🎯 使用方式

### 编译单个程序
```bash
cd build
make confusing_number    # 编译特定程序
```

### 运行单个程序
```bash
cd build
./bin/confusing_number   # 运行可执行文件
```

### 运行所有测试
```bash
cd build
make run_all_tests       # 运行所有算法测试
```

### 清理构建文件
```bash
cd build
make clean_all          # 清理所有可执行文件
```

## 📝 添加新算法

1. 在 `leetcode/algo_100/` 目录下创建新的 `.cc` 文件
2. 文件必须包含 `main()` 函数
3. 重新运行 `./build.sh` 或 `make`

示例新文件 `example.cc`:
```cpp
#include <iostream>

int main() {
    std::cout << "Hello Algorithm!" << std::endl;
    return 0;
}
```

## ⚙️ CMake 特性

- 🔧 **自动检测**: 自动发现所有 `.cc` 文件
- 🎯 **独立编译**: 每个 `.cc` 文件生成独立可执行文件
- 📂 **统一输出**: 所有可执行文件输出到 `build/bin/` 目录
- 🧹 **便捷清理**: 支持一键清理和重构建
- 🏃 **批量测试**: 支持一键运行所有测试

## 🛠️ 编译选项

- **C++ 标准**: C++17
- **编译器选项**: `-Wall -Wextra -O2`
- **CMake 最低版本**: 3.10

## 📋 当前算法列表

- `confusing_number.cc` - 易混淆数问题（字符转换、数字旋转）

## 🤝 贡献

欢迎添加新的算法题目！每个文件应该：
- 包含完整的问题描述（注释）
- 有清晰的解决方案
- 包含测试用例
- 遵循统一的代码风格 