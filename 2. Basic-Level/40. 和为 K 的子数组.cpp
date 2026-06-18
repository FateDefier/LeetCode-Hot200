#include <vector>
#include <unordered_map>
using namespace std;

// 思路 1：排序后用窗口，但是窗口大小不好控制，且左边界遍历麻烦
// 思路 2：前缀和，先求出数组截止到每个元素的前缀和存入数组中，然后看前缀和哪两个之间差值为 k，单纯这样做类似暴力解法，时间复杂度为 O(n^2)，故而借用哈希表可将时间复杂度优化到 O(n)
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int n = nums.size();
        long long preSum[n] = {0};
        unordered_map<long long, int> mp;   // value -- index

        preSum[0] = nums[0];
        for (int i = 1; i < n; ++i) {
            preSum[i] = preSum[i - 1] + nums[i];
        }

        mp[preSum[0]] = 0;
        int count = 0;
        for (int i = 1; i < n; ++i) {
            long long target = preSum[i] - k;
            if (mp.find(target) != mp.end()) {
                ++count;
            }
            mp[target] = i;
        }
        return count;
    }
};

// 哈希表 preSum -- count
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        unordered_map<long long, int> mp;   // preSum -- count
        mp.reserve(nums.size() + 1);        // 插入前预分配，避免 rehash
        mp[0] = 1;                          // 第 0 各元素前缀和为 0，未遍历数组时出现一次

        int count = 0;
        long long preSum = 0;       // 不用数组 O(n)，只用 O(1)
        for (int num : nums) {
            preSum += num;
            if (mp.find(preSum - k) != mp.end()) {
                count += mp[preSum - k];    // 哈希表存的是前缀和出现的次数，对应不同子数组的个数，这里 count 应该加它，而不是自增
            }
            ++mp[preSum];
        }
        return count;
    }
};