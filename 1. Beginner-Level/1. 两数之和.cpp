#include <vector>
#include <unordered_map>
using namespace std;

// O(n^2)
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        int n, m;
        for (int i = 0; i < nums.size(); ++i) {
            for (int j = i + 1; j < nums.size(); ++j) {
                if (nums[i] + nums[j] == target) {
                    return {i, j};          // 初始化列表，自动根据返回值推导返回值类型为 vector<int>
                }
            }
        }
        return {};
    }
};

/* =============================================================== */

// 正确解法：哈希表(O(1)) -- 算法时间复杂度 O(n) 空间换时间，边存边查找need，并非一次全部存入哈希表
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> mp;         // value -> index
        for (int i = 0; i < nums.size(); ++i) {
            int need = target - nums[i];
            if (mp.find(need) != mp.end()) {// 如果能找到当前数能凑成target的数(need)
                return {mp[need], i};       // 返回 need 值的下标和当前下标
            }
            mp[nums[i]] = i;                // 否则把当前值计入哈希表供后续元素查找，注意 mp[value] = index;
        }
        return {};
    }
};