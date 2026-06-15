#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 双重循环 + 贪心
class Solution {
public:
    int maxArea(vector<int>& height) {
        int max = 0;
        int n = height.size();
        for (int i = 0; i < n - 1; ++i) {
            for (int j = i + 1; j < n; ++j) {
                int currArea = (j - i) * min(height[i], height[j]);
                if (currArea > max) {
                    max = currArea;
                }
            }
        }
        return max;
    }
};

// 双指针 + 贪心，关键：移动 height 较小的指针
class Solution {
public:
    int maxArea(vector<int>& height) {
        int L = 0, R = height.size() - 1;
        int max_Area = 0;
        while (L < R) {
            int currArea = (R - L) * min(height[L], height[R]);
            max_Area = max(max_Area, currArea);
            if (height[L] < height[R]) {
                ++L;
            } else {
                --R;
            }
        }
        return max_Area;
    }
};