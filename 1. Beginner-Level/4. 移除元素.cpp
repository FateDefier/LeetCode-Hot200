#include <vector>
using namespace std;

// 快慢指针（覆盖式），不等于val的值放在前半部分，只返回慢指针的计数即为不等于 val 的元素数量(k)（其余元素和 nums 的大小并不重要）
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int slow = 0;
        for (int fast = 0; fast < nums.size(); ++fast) {
            if (nums[fast] != val) {
                nums[slow] = nums[fast];
                ++slow;
            }
        }
        return slow;
    }
};