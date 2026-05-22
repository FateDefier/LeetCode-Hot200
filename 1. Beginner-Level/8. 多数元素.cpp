#include <vector>
#include <unordered_map>
using namespace std;

class Solution {
public:
    int majorityElement(vector<int>& nums) {
        unordered_map<int, int> mp;         // value - frequency
        for (auto num : nums) {
            ++mp[num];                      // mp[num] 默认为 0
        }
        // 范围 for 遍历哈希表
        for (const auto& pair : mp) {
            if (pair.second > nums.size() / 2) {
                return pair.first;
            }
        }
        return -1;                          // 若实际情况，数组最多的元素为 -1 则无法保证结果，但是题目明确数组非空且存在多数元素，这样写更简便，工程可以用 throw 抛出错误
    }
};