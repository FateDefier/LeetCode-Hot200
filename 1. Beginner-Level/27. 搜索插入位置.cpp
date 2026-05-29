#include <iostream>
#include <vector>
using namespace std;

// 错误解法：
class Solution {
public:
    int searchInsert(vector<int> &nums, int target) {
        int left = 0, right = nums.size() - 1;
        int mid = 0;
        while (left <= right) {
            // 防止 left + right > INT_MAX 时溢出
            mid = left + (right - left) / 2;
            if (nums[mid] == target) {
                return mid;
            } else if (nums[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }   // mid = left = right;
        return mid;
    }
};

// 正确解法：
class Solution {
public:
    int searchInsert(vector<int> &nums, int target) {
        int left = 0, right = nums.size() - 1;
        int pos = nums.size();      // 默认插入位置在末尾(所有元素 < target)
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] >= target) {
                pos = mid;          // mid 为候选插入位置，先往前找是否有重复的等于 nums[mid] 的值
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        return pos;
    }
};