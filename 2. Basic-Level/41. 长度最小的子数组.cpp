#include <vector>
#include <unordered_map>
#include <climits>
#include <algorithm>
#include <deque>
using namespace std;

// 思路：找总和等于 target 的最短的子数组，实际上就是找 preSum[i] - preSum[j] = target 且 len = i - j + 1 最小，不妨用哈希表存 pair<preSum[j], j>(j 为离 i 最近的值为 target - preSum[i] 的下标)，这时候 preSum 也不用数组
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        unordered_map<int, int> mp;     // preSum -- index

        int preSum = 0, len = 0;

        int n = nums.size();
        for (int i = 0; i < n; ++i) {
            preSum += nums[i];
            if (mp.find(target - preSum) != mp.end()) {
                len = i - mp[target - preSum] + 1;
            }
            mp[preSum] = i;
        }
        return len;
    }
};

// 滑动窗口：要求 nums 元素均为正数（前缀和单调）
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int n = nums.size();
        int len = n + 1;    // 注意这里的空间优化，不用 INT_MAX
        int left = 0, sum = 0;

        for (int right = 0; right < n; ++right) {
            sum += nums[right];
            // 满足要求时，收缩左边界求最短子数组
            while (sum >= target) {
                // 更新长度
                len = min(len, right - left + 1);
                // 收缩左边界
                sum -= nums[left];
                ++left;
            }
        }
        return len == (n + 1) ? 0 : len;
    }
};

// 前缀和数组 + 二分搜索 -- 正数
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int n = nums.size();
        int len = n + 1;
        vector<int> preSum(n + 1, 0);        // 这里已经初始化了 preSum[0]，注意这里 preSum 长度为 n+1，因为 preSum[i] = nums[0..i-1]

        // 初始化 preSum
        for (int i = 0; i < n; ++i) {
            preSum[i + 1] = preSum[i] + nums[i];
        }

        // 二分搜索
        for (int i = 0; i < n; ++i) {
            int need = target + preSum[i];  // 注意这里是加号
            // 下面代码等价于：
            // auto it = lower_bound(preSum.begin() + i, preSum.end(), need);  // lower_bound >=，upper_bound <=
            // if (it != preSum.end()) {
            //     int curLen = it - preSum.begin() + 1;
            //     len = min(len, curLen);
            // }
            int left = i + 1, right = n;
            while (left <= right) {         // 等于不能省略，可能 left = right 且 preSum[left] = need
                int mid = (left + right) / 2;
                if (preSum[mid] >= need) {
                    len = min(len, mid - i);// 不用 + 1，因为 preSum[i] = nums[0..i-1] index 已经比 nums 大 1了
                    // 关键：继续向左收缩
                    right = mid - 1;
                } else {
                    left = mid + 1;
                } 
            }
        }
        return len == (n + 1) ? 0 : len;
    }
};

// 前缀和 + 单调队列：适用于有负数（前缀和非单调，用单调队列存 单调 的前缀和下标）
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int n = nums.size();
        vector<long long> preSum(n + 1, 0);     // 有负数时必须用 long long 防溢出
        for (int i = 0; i < n; ++i) {
            preSum[i + 1] = preSum[i] + nums[i];
        }

        deque<int> dq;          // 存单调递增(head-->tail)前缀和的 index，只维护候选左端点
        int len = n + 1;

        for (int j = 0; j <= n; ++j) {      // preSum 这里可以等
            // 1. 队首检查，当前前缀和 减(注意是减法) 最小前缀和
            while (!dq.empty() && preSum[j] - preSum[dq.front()] >= target) {
                len = min(len, j - dq.front()); // 不用 +1，preSum 下标比 nums 大 1
                dq.pop_front();
            }
            // 2. 更新左边界，即维护队尾
            // 如果队尾的元素即比 preSum[j] 大，index 又比 j 小，则出队，因为其候选左端点优先度永远小于 j 及其后续index
            while (!dq.empty() && preSum[dq.back()] >= preSum[j]) {
                dq.pop_back();
            }
            dq.push_back(j);
        }
        return len == (n + 1) ? 0 : len;
    }
};