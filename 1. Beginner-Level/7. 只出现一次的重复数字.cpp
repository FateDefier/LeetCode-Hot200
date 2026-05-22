#include <vector>
#include <unordered_map>
using namespace std;

// 要求：时间复杂度 O(n), 空间复杂度 O(1)
// 使用哈希表不能实现空间复杂度 O(1)，而是 O(n)
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        unordered_map<int, int> mp;     // value - index
        int i;
        for (i = 0; i < nums.size(); ++i) {
            if (mp.find(nums[i]) == mp.end()) {
                break;
            }
            mp[nums[i]] = i;
        }
        return nums[i];
    }
};

// 正确：
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int result = 0;
        // 遍历数组，不断异或
        // 成对出现的元素互相抵消（x ^ x = 0）
        // 最终 result 就是那只出现一次的元素
        for (int x : nums) {
            result ^= x;
        }
        return result;
    }
};