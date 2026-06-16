#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 思路 1：暴力解法，四重循环 --  O(n^4)
// 思路 2：固定 nums[i] + 三数之和 -- O(n^3)
class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        int n = nums.size();
        if (n < 4) return {};
        sort(nums.begin(), nums.end());
        // 第一次剪枝：先排序，再剪枝
        if ((long long)nums[n - 1] + nums[n - 2] + nums[n - 3] + nums[n - 4] < target) return {};

        vector<vector<int>> res;
        for (int i = 0; i < n - 3; ++i) {
            int num1 = nums[i];
            // 第二次剪枝，注意四数之和防溢出
            // 剪枝 1：当前最小的四个数之和 > target，则任取其他三个数，四数之和均大于 target，直接退出循环
            if ((long long)num1 + nums[i + 1] + nums[i + 2] + nums[i + 3] > target) break;
            // 剪枝 2：当前 i 加上最大的三个数之和 < target，则当前 i 偏小，下面的循环肯定找不到三个数使四数之和等于 target，则退出本次循环
            if ((long long)num1 + nums[n - 1] + nums[n - 2] + nums[n - 3] < target) continue;
            // 第一次去重
            if (i > 0 && num1 == nums[i - 1]) continue;

            for (int j = i + 1; j < n - 2; ++j) {
                int num2 = nums[j];
                // 第三次剪枝
                // 剪枝 1
                if ((long long)num1 + num2 + nums[j + 1] + nums[j + 2] > target) break;
                // 剪枝 2
                if ((long long)num1 + num2 + nums[n - 1] + nums[n - 2] < target) continue;
                // 第二次去重
                if (j > i + 1 && num2 == nums[j - 1]) continue;

                int left = j + 1, right = n - 1;
                while (left < right) {
                    long long sum = (long long)num1 + num2 + nums[left] + nums[right];
                    if (sum == target) {
                        res.push_back({num1, num2, nums[left], nums[right]});
                        // 第三层去重 -- left
                        while (left < right && nums[left] == nums[left + 1]) ++left;
                        while (left < right && nums[right] == nums[right - 1]) --right;
                        // 更新 left 和 right
                        ++left;
                        --right;
                    } else if (sum > target) {  // 因为 sum != target，所以不用去重，只用更新
                        --right;
                    } else {
                        ++left;
                    }
                }
            }
        }
        return res;
    }
};

// 思路三：拆成两个两数之和，实际去重更麻烦，O(n^2) 的哈希表插入和查询，伴随着巨大的内存分配和哈希冲突开销，不取