#include <vector>
#include <algorithm>
using namespace std;

/* 错误版本1 */
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int cur, rear = nums.size();
        for (cur = 0; cur < rear; ++cur) {
            if (nums[cur] == 0) {
                for (int i = cur; i < nums.size() - 1; ++i)
                    nums[i] = nums[i + 1];
                nums[rear] = 0;
                --rear;
            }
        }
    }
};

// 思路：前移 + 最后一位置零 -- 时间复杂度 O(n^2)，空间复杂度O(1)
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int cur = 0;
        int rear = nums.size() - 1;
        while (cur < rear) {
            if (nums[cur] == 0) {       // 碰到 0，则 前移 + 最后一位置零
                for (int i = cur; i < rear; ++i) {
                    nums[i] = nums[i + 1];
                }
                nums[rear] = 0;
                --rear;
            } else {                    // 没碰到 0，则下一位
                ++cur;
            }
        }
    }
};

/* =============================================================== */

// 正确解法：快慢指针 -- 时间复杂度 O(n)，空间复杂度O(1)
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int slow = 0;                                           // 慢指针：指向下一个交换位置到前面的位置
        for (int fast = 0; fast < nums.size(); ++fast) {        // 快指针：指向下一个等待交换的非零元素
            if (nums[fast] != 0) {
                swap(nums[slow], nums[fast]);
                ++slow;                                         // 更新 slow
            }
        }
        // 循环结束后，slow 及其后面的位置自然都是 0
    }
};