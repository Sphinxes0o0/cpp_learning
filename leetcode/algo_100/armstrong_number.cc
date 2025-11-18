/**
给你一个整数 n ，让你来判定他是否是 阿姆斯特朗数，是则返回 true，不是则返回 false。

假设存在一个 k 位数 n ，其每一位上的数字的 k 次幂的总和也是 n ，那么这个数是阿姆斯特朗数 。
 */

#include <iostream>
#include <cmath>
#include <vector>
#include <string>

// 原始实现（两次遍历）
bool isArmstrong_original(int n) {
    int k = 0;
    int temp = n;
    while (temp > 0) {
        k++;
        temp /= 10;
    }
    temp = n;
    int sum = 0;
    while (temp > 0) {
        int digit = temp % 10;
        sum += std::pow(digit, k);
        temp /= 10;
    }
    return sum == n;
}

// 优化1：自定义整数幂函数（避免浮点运算）
int intPow(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

bool isArmstrong_intPow(int n) {
    int k = 0;
    int temp = n;
    while (temp > 0) {
        k++;
        temp /= 10;
    }
    temp = n;
    int sum = 0;
    while (temp > 0) {
        int digit = temp % 10;
        sum += intPow(digit, k);
        temp /= 10;
    }
    return sum == n;
}

// 优化2：单次遍历 + 预存储数字
bool isArmstrong_singlePass(int n) {
    std::vector<int> digits;
    int temp = n;
    
    // 一次遍历提取所有数字
    while (temp > 0) {
        digits.push_back(temp % 10);
        temp /= 10;
    }
    
    int k = digits.size();
    int sum = 0;
    
    // 计算幂次和
    for (int digit : digits) {
        sum += intPow(digit, k);
    }
    
    return sum == n;
}

// 优化3：字符串方法（对于大数更安全）
bool isArmstrong_string(int n) {
    std::string num_str = std::to_string(n);
    int k = num_str.length();
    int sum = 0;
    
    for (char c : num_str) {
        int digit = c - '0';
        sum += intPow(digit, k);
    }
    
    return sum == n;
}

// 优化4：预计算幂次表（对于批量检测很有效）
class ArmstrongChecker {
private:
    std::vector<std::vector<int>> powerTable;
    
public:
    ArmstrongChecker() {
        // 预计算0-9的1-10次幂（支持最多10位数）
        powerTable.resize(10);
        for (int digit = 0; digit < 10; digit++) {
            powerTable[digit].resize(11);
            powerTable[digit][0] = 1;
            for (int power = 1; power <= 10; power++) {
                powerTable[digit][power] = powerTable[digit][power-1] * digit;
            }
        }
    }
    
    bool isArmstrong(int n) {
        std::string num_str = std::to_string(n);
        int k = num_str.length();
        int sum = 0;
        
        for (char c : num_str) {
            int digit = c - '0';
            sum += powerTable[digit][k];
        }
        
        return sum == n;
    }
};

// 优化5：早期终止优化
bool isArmstrong_earlyExit(int n) {
    std::vector<int> digits;
    int temp = n;
    
    while (temp > 0) {
        digits.push_back(temp % 10);
        temp /= 10;
    }
    
    int k = digits.size();
    int sum = 0;
    
    for (int digit : digits) {
        sum += intPow(digit, k);
        // 早期终止：如果和已经超过原数，直接返回false
        if (sum > n) {
            return false;
        }
    }
    
    return sum == n;
}

// 性能测试函数
void performanceTest() {
    std::cout << "\n=== 阿姆斯特朗数性能对比测试 ===\n";
    
    // 测试一些已知的阿姆斯特朗数
    std::vector<int> testNumbers = {153, 371, 9474, 54748, 1634, 8208, 92727};
    
    std::cout << "测试数字: ";
    for (int num : testNumbers) {
        std::cout << num << " ";
    }
    std::cout << "\n\n";
    
    // 测试各种实现
    std::cout << "原始实现结果: ";
    for (int num : testNumbers) {
        std::cout << isArmstrong_original(num) << " ";
    }
    std::cout << "\n";
    
    std::cout << "整数幂优化: ";
    for (int num : testNumbers) {
        std::cout << isArmstrong_intPow(num) << " ";
    }
    std::cout << "\n";
    
    std::cout << "单次遍历优化: ";
    for (int num : testNumbers) {
        std::cout << isArmstrong_singlePass(num) << " ";
    }
    std::cout << "\n";
    
    std::cout << "字符串方法: ";
    for (int num : testNumbers) {
        std::cout << isArmstrong_string(num) << " ";
    }
    std::cout << "\n";
    
    std::cout << "预计算表方法: ";
    ArmstrongChecker checker;
    for (int num : testNumbers) {
        std::cout << checker.isArmstrong(num) << " ";
    }
    std::cout << "\n";
    
    std::cout << "早期终止优化: ";
    for (int num : testNumbers) {
        std::cout << isArmstrong_earlyExit(num) << " ";
    }
    std::cout << "\n";
}

int main() {
    int n = 153;
    std::cout << "检测 " << n << " 是否为阿姆斯特朗数: " << isArmstrong_original(n) << std::endl;
    
    performanceTest();
    
    return 0;
}