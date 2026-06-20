#include <vector>
#include <algorithm>
using namespace std;

// 思路 1：数组 tmp[k] 存末尾 k 个元素，然后，这 k 个元素前面所有元素后移，再把这 k 个元素赋值给前 k 个位置
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        int n = nums.size();
        k %= n;         // 关键：处理 k > n 的情况
        if (k == 0) return;
        
        vector<int> tmp(k, 0);

        for (int i = 0; i < k; ++i) {
            tmp[i] = nums[i + n - k];
        }

        // 把前面的元素向后移动 k
        for (int i = n - 1; i >= k; --i) {
            nums[i] = nums[i - k];
        }

        for (int i = 0; i < k; ++i) {
            nums[i] = tmp[i];
        }
    }
};

// 解法 2：三次反转(最优)
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        int n = nums.size();
        k %= n;         // 关键：处理 k > n 的情况
        if (k == 0) return;
        
        // 第一次反转整体
        reverse(nums.begin(), nums.end());
        // 第二、三次反转前 k 个元素和其他元素
        reverse(nums.begin(), nums.begin() + k);
        reverse(nums.begin() + k, nums.end());
    }
};

// 解法 3：环形替换法(了解)
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        int n = nums.size();
        k %= n;
        if (k == 0) return;

        int count = 0;      // 已处理的元素个数 -- 循环结束条件
        for (int start = 0; count < n; ++start) {
            int cur = start;
            int preVal = nums[start];
            do {
                int next = (cur + k) % n;       // 下一个目标位置
                int temp = nums[next];
                nums[next] = preVal;            // 右移 k 个位置
                // 更新 cur 和 preVal
                preVal = temp;
                cur = next;
                ++count;
            } while (cur != start);             // 回到环的起点终止
        }
    }
};
