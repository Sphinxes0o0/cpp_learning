/**
给你一个下标从 0 开始的整数数组 nums 和两个整数 key 和 k 。K 近邻下标 是 nums 中的一个下标 i ，并满足至少存在一个下标 j 使得 |i - j| <= k 且 nums[j] == key 。

以列表形式返回按 递增顺序 排序的所有 K 近邻下标。
 */

#include <vector>
#include <iostream>
#include <algorithm>

std::vector<int> findKDistantIndices(std::vector<int>& nums, int key, int k) {
    std::vector<int> result;
    int n = nums.size();
    for (int i = 0; i < n; ++i) {
        if (nums[i] == key) {
            for (int j = std::max(0, i - k); j <= std::min(i + k, n - 1); ++j) {    
                result.push_back(j);
            }
        }
    }
    
    // 去重并排序
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    
    return result;
}

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 5};
    int key = 3;
    int k = 2;
    std::vector<int> result = findKDistantIndices(nums, key, k);
    for (int index : result) {
        std::cout << index << " ";
    }
    std::cout << std::endl;
    return 0;
}
