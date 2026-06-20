#include <vector>
#include <unordered_set>
#include <algorithm>
using namespace std;

// 思路 1：排序(O(n logn)) + 贪心(记录当前最长连续长度)
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {

    }
};

// 思路 2：哈希集合
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        if (nums.empty()) return 0;
        unordered_set<int> st(nums.begin(), nums.end());
        int res = 0;

        for (int x : st) {
            // 只有当 x 的前一个数 x - 1 不在集合中时，其才能作为起点
            if (st.find(x - 1) == st.end()) {
                int cur = x;
                int len = 1;
                // 看是否有 x(cur) + 1
                while (st.find(cur + 1) != st.end()) {
                    ++cur;
                    ++len;
                }
                res = max(res, len);
            }
        }
        return res;
    }
};