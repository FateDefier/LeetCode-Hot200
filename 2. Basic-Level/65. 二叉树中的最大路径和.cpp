#include <climits>
#include <algorithm>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// 思路 1：求最大路径和，每次要到下一个结点时，先比较左右节点哪个值大，走值大的那边，但是这可能陷入局部最优（值小的那边可能之后有值很大的结点），难道是用回溯？但是又如何回溯？

// 正确解法：后序遍历 + 动态规划
class Solution {
private:
    int maxSum = INT_MIN;       // 防止结点全为 0 的情况
    int dfs(TreeNode *node) {
        // 递归基 
        if (!node) return 0;

        // 递归计算左右子树对父节点的贡献度，如果为负则舍弃（返回 0）
        int leftMax = std::max(dfs(node->left), 0);
        int rightMax = std::max(dfs(node->right), 0);

        // 跨越当前结点的路径和（可能为最大路径和）
        int throughCurr = leftMax + node->val + rightMax;
        maxSum = std::max(maxSum, throughCurr);

        // 返回给父节点：父节点路径只能走一边
        return node->val + std::max(leftMax, rightMax);
    }
public:
    int maxPathSum(TreeNode *root) {
        dfs(root);
        return maxSum;
    }
};