#include <vector>
#include <algorithm>
using namespace std;

// 思路 1（大错特错）：固定 i 个，递归剩余 (n-i) 个元素的全排列个数，二者相乘，每次二分，时间复杂度为 O(logn n!) , 空间 O(n/2)。求全排列的个数上述思路可行，但是时间空间复杂度可能不对

// 正确解法：回溯法
class Solution {
private:
    void backTrack(const vector<int> &nums, vector<vector<int>> &res, vector<int> &cur, vector<bool> &used) {
        // 递归基
        int n = nums.size();
        if (cur.size() == n) {
            res.push_back(cur);
            return;
        }

        // 每次已经选中一个数（n 个数每个选一遍，for循环），其他数递归加入（相当于一个新的全序列）
        for (int i = 0; i < n; ++i) {
            if (used[i]) continue;      // 已经使用了，退出本轮循环

            // 没有使用 -- 则加入 cur
            used[i] = true;     // 标记已使用，下一次递归不会选
            cur.push_back(nums[i]);
            // 迭代
            backTrack(nums, res, cur, used);
            // 回溯
            cur.pop_back();
            used[i] = false;
        }
    }
public:
    vector<vector<int>> permute(vector<int> &nums) {
        vector<vector<int>> result;
        vector<int> current;
        vector<bool> used(nums.size(), false);

        backTrack(nums, result, current, used);

        return result;
    }
};

// 空间优化版本：
class Solution {
private:
    void dfs(vector<int>& nums, int flag, vector<vector<int>>& res) {
        int n = nums.size();
        // 递归基
        if (flag == n) {
            res.push_back(nums);
            return;
        }

        for (int i = flag; i < n; ++i) {
            swap(nums[flag], nums[i]);
            dfs(nums, flag + 1, res);       // 关键：flag 在这里更新
            // 回溯还原
            swap(nums[flag], nums[i]);
        }
    }
public:
    vector<vector<int>> permute(vector<int> &nums) {
        vector<vector<int>> result;
        dfs(nums, 0, result);
        return result;
    }
};