/*
给你一个整数数组 A，请找出并返回在该数组中仅出现一次的最大整数。

如果不存在这个只出现一次的整数，则返回 -1。
*/

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Solution {
public:
    int largestUniqueNumber(vector<int>& nums) {
        // 统计每个数字的出现次数
        unordered_map<int, int> count;
        for (int num : nums) {
            count[num]++;
        }
        
        // 找出只出现一次的数字中的最大值
        int maxUnique = -1;
        for (auto& pair : count) {
            if (pair.second == 1) {  // 只出现一次
                maxUnique = max(maxUnique, pair.first);
            }
        }
        
        return maxUnique;
    }
};

int main() {
    Solution solution;
    vector<int> nums = {5, 7, 3, 9, 4, 9, 8, 3, 1};
    int result = solution.largestUniqueNumber(nums);
    cout << result << endl;
    return 0;
}