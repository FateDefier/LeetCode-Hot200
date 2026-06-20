#include <vector>
#include <algorithm>
#include <cstdlib>      // abs
using namespace std;

// 思路 1：原地哈希，nums[i] = i + 1; 对于一个长度为 n 的数组，答案范围是 [1, n+1]（最差情况 1~n 填满数组，则没在数组中出现过的最小正整数为 n+1）
class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        int n = nums.size();

        // 构建哈希表 nums[i] = i + 1
        for (int i = 0; i < n; ++i) {
            // 关键点：while 而非 if，因为交换来的正整数大概率与当前下标不满足 nums[i] = i + 1
            while (nums[i] >= 1 && nums[i] <= n && nums[i] != nums[nums[i] - 1]) {
                swap(nums[i], nums[nums[i] - 1]);
            }
        }

        // 再次遍历，找第一个不满足 nums[i] = i + 1 的数
        for (int i = 0; i < n; ++i) {
            if (nums[i] != i + 1) return i + 1;
        }
        // 最差情况 1~n 填满数组，则没在数组中出现过的最小正整数为 n+1
        return n + 1;
    }
};

// 解法 2：标记法
class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        int n = nums.size();

        // 1.负数替换为 n + 1 做哨兵
        for (int i = 0; i < n; ++i) {
            if (nums[i] <= 0) nums[i] = n + 1;
        }

        // 2.对于 1 ~ n 出现过的数，把相应的 nums[i - 1] 标记为负数
        for (int i = 0; i < n; ++i) {
            int x = abs(nums[i]);       // 边标记边写(当处理下标 i 时，nums[i] 可能已经被前面的某个 j < i 标记为负数)，必须用绝对值
            if (x >= 1 && x <= n) {     // 排除哨兵，且只标记范围在 1~n 的正数
                if (nums[x - 1] > 0) nums[x - 1] = - nums[x - 1];                   // 这里可能改变哨兵
            }
        }

        // 3.找第一个值不为负数的下标
        for (int i = 0; i < n; ++i) {
            if (nums[i] > 0) return i + 1;      // 碰到哨兵 n + 1 说明未被标记，正常返回 i + 1
        }
        return n + 1;
    }
};