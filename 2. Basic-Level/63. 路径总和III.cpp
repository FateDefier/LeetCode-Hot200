#include <unordered_map>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// 思路 1：把所有节点用先序遍历（路径必须向下）一遍，记录到每个结点的前缀和（相等的前缀和记次数 +1），前缀和之差 == targetSum 的两个结点即为所求路径，不妨用哈希表优化，直接查找 need = targetSum - preSum，不用求出完整的前缀和，且不用遍历前缀和数组（O(n)），直接哈希表 O(1)
class Solution {
private:
    std::unordered_map<long long, int> prefix;    // need -- count
    int dfs(TreeNode *node, long long curSum, int targetSum) {
        if (node == nullptr) return 0;
        
        curSum += node->val;            // 更新前缀和
        long long need = curSum - targetSum;    // 当前和减 targetSum 注意路径
        int count = prefix.count(need) ? prefix[need] : 0; // 当前前缀和出现 need 的次数，没有则手动初始化为 0，防止哈希表不必要的 0 计数

        ++prefix[curSum];       // 进入该结点，更新次数
        // dfs 递归更新 count，直至所有结点遍历完
        count += dfs(node->left, curSum, targetSum);
        count += dfs(node->right, curSum, targetSum);
        // 关键：退出该结点，恢复状态
        --prefix[curSum];       // 树分叉，不能在右子树上加上所有左子树的前缀和
        return count;
    }
public:
    int pathSum(TreeNode *root, int targetSum) {
        // 类成员变量，多次调用要清空旧数据，不做函数参数防止多次传参开销，不过好像引用开销不大
        prefix.clear();
        prefix[0] = 1;          // 没有遍历结点（或空树）前缀和为 0 的次数为一
        return dfs(root, 0, targetSum);
    }
};