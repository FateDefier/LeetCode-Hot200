#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

// 思路：前缀和做差最大即为区间最大 -- O(n)，先找到 maxPreSum 
class Solution {
public:
    int maxSubArray(vector<int> nums) {
        int n = nums.size();
        vector<int> preSum(n + 1, 0);
        int maxPreSum = -10000;
        
        int maxIdx = 0;
        for (int i = 0; i < n; ++i) {
            preSum[i + 1] = preSum[i] + nums[i];
            if (preSum[i + 1] > maxPreSum) {
                maxPreSum = preSum[i + 1];
                maxIdx = i + 1;
            }
        }

        int diff = maxPreSum - preSum[0];
        for (int i = 1; i <= n && i != maxIdx; ++i) {      // 遍历前缀和可以等于 n
            if (diff < maxPreSum - preSum[i]) {
                diff = maxPreSum - preSum[i];
            }
        }
        return diff;
    }
};

// 正确的前缀和：维护最小前缀和和最大前缀和
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int sum = 0;
        int max_sum = INT_MIN;
        int min_sum = 0;
        
        for (int num : nums) {
            sum += num;
            // 关键：用下标 <j 的 min_sum 更新 max_sum，从而保证max_sum 的下标永远在 min_sum 后面
            max_sum = max(max_sum, sum - min_sum);
            min_sum = max(min_sum, sum);
        }
        return max_sum;
    }
};

// DP：状态转移(空间优化后，不用 dp[] 数组，直接用 sum 和 pre)
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int sum = nums[0];  // sum 维护截止当前最大子数组和
        int preSum = 0;     // preSum = dp[i - 1]

        for (int num : nums) {
            preSum = max(num, preSum + num);    // 要么加上当前，要么另起炉灶
            sum = max(sum, preSum);             // 更新 sum
        }
        return sum;
    }
};

// 分治递归
class Solution {
private:
    int crossSum(vector<int>& nums, int L, int mid, int R) {
        // 扩展左边，求左边最大和
        int leftSum = INT_MIN; 
        int sum = 0;
        for (int left = mid; left >= L; --left) {
            sum += nums[left];
            leftSum = max(leftSum, sum);
        }
        // 扩展右边，求右边最大和
        int rightSum = INT_MIN;
        sum = 0;        // sum 重新置 0
        for (int right = mid + 1; right <= R; ++right) {
            sum += nums[right];
            rightSum = max(rightSum, sum);
        }
        return leftSum + rightSum;
    }
    int divide(vector<int>& nums, int L, int R) {
        // 递归基
        if (L == R) return nums[L];
        int mid = L + (R - L) / 2;      // 防溢出写法
        int leftMax = divide(nums, L, mid);
        int rightMax = divide(nums, mid + 1, R);
        int crossMax = crossSum(nums, L, mid, R);
        return max({leftMax, rightMax, crossMax});
    }
public:
    int maxSubArray(vector<int>& nums) {
        return divide(nums, 0, nums.size() - 1);
    }
};