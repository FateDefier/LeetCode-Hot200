#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 思路 1：三重循环 -- O(n^3) 不取
// 思路 2：哈希表存前两个数之和，然后找另一个数是三个数之和为 0，即 b + c = -a -- O(n^2)，哈希表需考虑去重，而双指针不用考虑去重
// 思路 3：排序 + 双指针 -- O(n^2)
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> res;
        // 先排序，方便去重和剪枝
        sort(nums.begin(), nums.end());
        // 排序后剪枝
        if (nums[0] > 0 || nums.back() < 0) return res;

        int n = nums.size();  // 每次循环都调用 nums.size() 虽开销微小，但可避免重复计算
        for (int i = 0; i < n - 2; ++i) {     // 隐式判断了 nums.size() < 3 的情况
            int a = nums[i];            // 避免对此下标访问
            // 循环内因为 i 的移动，也可以优化剪枝
            if (a > 0) break;
            // 第一次去重，防止重复的 nums[i]
            if (i > 0 && a == nums[i - 1]) continue;

            int l = i + 1, r = n - 1;
            while (l < r) {
                int sum = a + nums[l] + nums[r];
                if (sum == 0) {
                    res.push_back({a, nums[l], nums[r]});
                    // 第二次去重，左指针重复去重
                    while (l < r && nums[l] == nums[l + 1]) ++l;
                    // 第三次去重，右指针重复去重
                    while (l < r && nums[r] == nums[r - 1]) --r;
                    ++l;
                    --r;
                } else if (sum < 0) {
                    ++l;
                } else {
                    --r;
                }
            }
        }
        return res;
    }
};