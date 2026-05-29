#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int search(vector<int> &nums, int target) {
        int left = 0, right = nums.size() - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (nums[mid] == target) {
                return mid;
            }
            if (nums[mid] < target) {       // target 在右半边
                left = mid + 1;
            }
            if (nums[mid] > target) {       // target 在左半边
                right = mid - 1;
            }
        }
        return -1;
    }
};