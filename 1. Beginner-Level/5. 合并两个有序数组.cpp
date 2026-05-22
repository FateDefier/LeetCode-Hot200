#include <vector>
using namespace std;

// 错误的迭代器使用版本
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        auto it1 = nums1.begin(), it2 = nums2.begin();
        for(; it1 != nums1.end() && it2 != nums2.end(); ) {
            if (*it1 <= *it2) {
                ++it1;
            } else {
                nums1.insert(it1 - 1, *it2);        // 插入操作后，迭代器失效，下面的 ++it1 是未定义行为
                ++it1;
                ++it2;
            }
        }
        // 插入剩余部分, 只用检查 it1 是否到达末尾
        if (it1 == nums1.end()) {
                nums1.insert(it1, it2, nums2.end());
        }
    }
};

// 思路：从nums1和nums2后面开始比，谁大就放最后面（因为nums1固定长 m+n，一定能填满）
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int i = m - 1, j = n - 1, k = m + n - 1;
        while (i >= 0 && j >= 0) {
            if (nums1[i] > nums2[j]) {
                nums1[k--] = nums1[i--];
            } else {
                nums1[k--] = nums2[j--];
            }
        }
        // 如果 nums2 还有剩余(m < n), 则把 nums2 剩余部分加入 nums1
        while (j >= 0) {
            nums1[k--] = nums2[j--];
        }
    }
};