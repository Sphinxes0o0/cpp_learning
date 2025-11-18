/**
给定一个数字 N，当它满足以下条件的时候返回 true：

原数字旋转 180° 以后可以得到新的数字。

如 0, 1, 6, 8, 9 旋转 180° 以后，得到了新的数字 0, 1, 9, 8, 6 。

2, 3, 4, 5, 7 旋转 180° 后，得到的不是数字。

易混淆数 (confusing number) 在旋转180°以后，可以得到和原来不同的数，且新数字的每一位都是有效的。

*/

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class Solution {
public:
    bool confusingNumber(int N) {
        // 数字旋转180°的映射表
        std::unordered_map<int, int> rotateMap = {
            {0, 0}, {1, 1}, {6, 9}, {8, 8}, {9, 6}
        };
        
        // 将数字转换为字符串，便于逐位处理
        std::string original = std::to_string(N);
        std::string rotated = "";
        
        // 逐位检查并构建旋转后的数字
        for (int i = original.length() - 1; i >= 0; i--) {
            int digit = original[i] - '0';
            
            // 检查当前数字是否可以旋转
            if (rotateMap.find(digit) == rotateMap.end()) {
                return false; // 包含无法旋转的数字
            }
            
            // 添加旋转后的数字到结果字符串
            rotated += std::to_string(rotateMap[digit]);
        }
        
        // 将旋转后的字符串转换为整数
        int rotatedNum = std::stoi(rotated);
        
        // 判断旋转后的数字是否与原数字不同
        return rotatedNum != N;
    }
    
    // 数学方法的优化版本（更高效）
    bool confusingNumberMath(int N) {
        std::unordered_map<int, int> rotateMap = {
            {0, 0}, {1, 1}, {6, 9}, {8, 8}, {9, 6}
        };
        
        int original = N;
        int rotated = 0;
        
        while (N > 0) {
            int digit = N % 10;
            
            // 检查是否可以旋转
            if (rotateMap.find(digit) == rotateMap.end()) {
                return false;
            }
            
            // 构建旋转后的数字
            rotated = rotated * 10 + rotateMap[digit];
            N /= 10;
        }
        
        return rotated != original;
    }
};


int main() {
    Solution solution;
    
    // 演示字符转数字的原理
    std::cout << "字符转数字演示：" << std::endl;
    std::cout << "================" << std::endl;
    
    std::string example = "123";
    for (size_t i = 0; i < example.length(); i++) {
        char ch = example[i];
        int digit = ch - '0';
        
        std::cout << "字符 '" << ch << "'" << std::endl;
        std::cout << "  ASCII码: " << (int)ch << std::endl;
        std::cout << "  '0'的ASCII码: " << (int)'0' << std::endl;
        std::cout << "  " << (int)ch << " - " << (int)'0' << " = " << digit << std::endl;
        std::cout << std::endl;
    }
    
    std::cout << "====================" << std::endl;
    
    // 测试用例
    std::vector<int> testCases = {6, 89, 11, 25, 916, 1};
    
    std::cout << "测试易混淆数问题：" << std::endl;
    std::cout << "====================" << std::endl;
    
    for (int N : testCases) {
        bool result1 = solution.confusingNumber(N);
        bool result2 = solution.confusingNumberMath(N);
        
        std::cout << "N = " << N << std::endl;
        std::cout << "  字符串方法: " << (result1 ? "true" : "false") << std::endl;
        std::cout << "  数学方法:   " << (result2 ? "true" : "false") << std::endl;
        std::cout << "  结果一致:   " << (result1 == result2 ? "✓" : "✗") << std::endl;
        std::cout << std::endl;
    }
    
    return 0;
}