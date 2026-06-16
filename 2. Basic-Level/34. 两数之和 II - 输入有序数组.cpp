#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

// 思路一：哈希表，空间复杂度 O(n) 不符合题目要求 O(1)
class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        unordered_map<int, int> mp;         // value -- index
        int n = numbers.size();
        for (int i = 1; i < n; ++i) {
            int need = target - numbers[i];
            if (mp.find(need) != mp.end()) {
                return {mp[need], i};
            }
            mp[numbers[i]] = i; 
        }
        return {};
    }
};

// 思路二：二分查找 -- 固定 i，在 [i+1, n-1] 中二分查找 target - numbers[i] -- O(n logn)
class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        int n = numbers.size();
        for (int i = 0; i < n - 1; ++i) {
            int num1 = numbers[i];
            int left = i + 1, right = n - 1;
            while (left <= right) {
                int mid = (left + right) / 2;
                int num2 = target - num1;
                if (numbers[mid] == num2) {
                    return {i + 1, mid + 1};
                } else if (numbers[mid] > num2) {
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            }
        }
        return {};
    }
};

// 思路三：左右对撞指针
class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        // if (numbers[0] > target) return {};         // 前面两个也可以用这个剪枝优化 -- 这个剪枝在第一个数为负数时会出现错误
        // 正确的剪枝
        int n = numbers.size();
        if (numbers[0] + numbers[1] > target) return {};
        if (numbers[n - 1] + numbers[n - 2] < target) return {};
        int left = 0, right = n - 1;
        while (left < right) {
            int sum = numbers[left] + numbers[right];
            if (sum == target) {
                return {left + 1, right + 1};
            } else if (sum > target) {  // 类似二分，不多做解释
                --right;
            } else {
                ++left;
            }
        }
        return {};
    }
};