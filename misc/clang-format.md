# Clang-Format 代码风格对比：Google vs LLVM

## 简介

Clang-Format 是一个强大的代码格式化工具，可以自动格式化 C/C++/Objective-C 代码。它支持多种预定义的代码风格，其中 Google 和 LLVM 是最常用的两种风格。

本文档将详细对比 Google 和 LLVM 两种代码风格的主要差异，帮助开发者选择合适的代码风格并在团队中保持一致性。

## Google 风格

Google 风格是 Google 公司为其 C++ 项目制定的代码规范，强调代码的可读性和一致性。该风格在业界广泛使用，特别是在开源项目中。

### 特点

- 缩进使用 2 个空格
- 行宽限制为 80 个字符
- 大括号放置采用 Attach 模式（左大括号放在行尾）
- 命名规范偏向于使用小写字母和下划线

## LLVM 风格

LLVM 风格是 LLVM 项目使用的代码风格，LLVM 是一个模块化和可重用的编译器和工具链技术集合。

### 特点

- 缩进使用 2 个空格
- 行宽限制为 80 个字符
- 大括号放置采用 Attach 模式
- 命名规范偏向于使用驼峰命名法

## 详细对比

### 1. 访问修饰符缩进

| 配置项 | Google | LLVM |
|--------|--------|------|
| `AccessModifierOffset` | -1 | -2 |
| `IndentAccessModifiers` | false | false |

**说明：**
- Google 风格中，访问修饰符（public、private、protected）相对类内容缩进减少 1 个空格
- LLVM 风格中，访问修饰符相对类内容缩进减少 2 个空格，通常与类定义对齐

**示例：**
```cpp
// Google 风格
class MyClass {
 public:        // 相对于成员缩进减少1个空格
  void method(); // 成员函数缩进2个空格

 private:       // 相对于成员缩进减少1个空格
  int value;    // 成员变量缩进2个空格
};

// LLVM 风格
class MyClass {
public:         // 与class关键字对齐
  void method(); // 成员函数缩进2个空格

private:        // 与class关键字对齐
  int value;    // 成员变量缩进2个空格
};
```

### 2. 包含块处理

| 配置项 | Google | LLVM |
|--------|--------|------|
| `IncludeBlocks` | Regroup | Preserve |

**说明：**
- Google 风格会重新分组和排序包含块
- LLVM 风格保持现有的包含块分组

### 3. 转义换行对齐

| 配置项 | Google | LLVM |
|--------|--------|------|
| `AlignEscapedNewlines` | Left | Right |

**说明：**
- Google 风格将转义换行符左对齐
- LLVM 风格将转义换行符右对齐

### 4. 短 if 语句处理

| 配置项 | Google | LLVM |
|--------|--------|------|
| `AllowShortIfStatementsOnASingleLine` | WithoutElse | Never |

**说明：**
- Google 风格允许没有 else 分支的短 if 语句在同一行
- LLVM 风格从不允许短 if 语句在同一行

**示例：**
```cpp
// Google 风格允许
if (condition) return;

// LLVM 风格要求
if (condition) {
  return;
}
```

### 5. 短循环处理

| 配置项 | Google | LLVM |
|--------|--------|------|
| `AllowShortLoopsOnASingleLine` | true | false |

**说明：**
- Google 风格允许短循环在同一行
- LLVM 风格不允许短循环在同一行

### 6. 多行字符串处理

| 配置项 | Google | LLVM |
|--------|--------|------|
| `AlwaysBreakBeforeMultilineStrings` | true | false |

**说明：**
- Google 风格总是在多行字符串前断行
- LLVM 风格不强制在多行字符串前断行

### 7. 模板声明断行

| 配置项 | Google | LLVM |
|--------|--------|------|
| `BreakTemplateDeclarations` | Yes | MultiLine |

**说明：**
- Google 风格总是断行模板声明
- LLVM 风格只在多行时断行模板声明

### 8. 注释前空格

| 配置项 | Google | LLVM |
|--------|--------|------|
| `SpacesBeforeTrailingComments` | 2 | 1 |

**说明：**
- Google 风格在行尾注释前添加 2 个空格
- LLVM 风格在行尾注释前添加 1 个空格

### 9. 指针和引用对齐

| 配置项 | Google | LLVM |
|--------|--------|------|
| `PointerAlignment` | Left | Right |
| `ReferenceAlignment` | Pointer | Pointer |

**说明：**
- Google 风格将指针和引用靠近类型（左对齐）
- LLVM 风格将指针和引用靠近变量（右对齐）

**示例：**
```cpp
// Google 风格
int* ptr;
int& ref;

// LLVM 风格
int *ptr;
int &ref;
```

### 10. 其他重要差异

| 配置项 | Google | LLVM | 说明 |
|--------|--------|------|------|
| `AllowShortFunctionsOnASingleLine` | All | InlineOnly | Google允许所有短函数单行，LLVM仅允许内联函数 |
| `AllowShortBlocksOnASingleLine` | false | Never | Google不允许短块单行，LLVM明确禁止 |
| `AllowShortCaseLabelsOnASingleLine` | false | false | 两者都不允许短case标签单行 |
| `AllowShortLambdasOnASingleLine` | All | Empty | Google允许所有短lambda单行，LLVM仅允许空lambda |
| `Cpp11BracedListStyle` | true | true | 两者都使用C++11大括号列表风格 |
| `SpaceBeforeParens` | ControlStatements | ControlStatements | 控制语句括号前添加空格 |
| `Standard` | Auto | Latest | Google使用Auto标准，LLVM使用Latest标准 |

## 实际应用建议

### 选择合适的风格

1. **Google 风格适用场景：**
   - 开源项目，特别是与 Google 生态相关的项目
   - 团队成员熟悉 Google C++ 风格指南
   - 希望代码更加紧凑，减少垂直空间占用

2. **LLVM 风格适用场景：**
   - 编译器相关项目
   - 团队偏好使用驼峰命名法
   - 希望更严格的代码格式控制

### 自定义配置

在实际项目中，通常需要基于预定义风格进行自定义配置。可以通过以下方式实现：

1. 生成基础配置：
   ```bash
   clang-format -style=google -dump-config > .clang-format
   ```

2. 修改配置文件以满足项目需求

3. 在团队中统一使用相同的配置文件

### 集成到开发流程

1. **编辑器集成：**
   - 大多数现代编辑器（VSCode、CLion、Vim等）都支持 clang-format 插件
   - 可以配置保存时自动格式化

2. **Git 钩子：**
   - 使用 pre-commit 钩子在提交前自动格式化代码
   - 确保所有提交的代码都符合规范

3. **CI/CD 集成：**
   - 在持续集成流程中检查代码格式
   - 使用 `check-format` 目标防止格式不正确的代码合并

## 结论

Google 和 LLVM 风格各有特点，选择哪种风格主要取决于项目需求和团队偏好。Google 风格更注重代码的紧凑性和可读性，而 LLVM 风格更注重严格的格式控制。在实际项目中，可以根据这两种风格进行自定义配置，以满足特定的编码规范要求。