#include <algorithm>
#include <cmath>            // abs

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// 思路：遍历每个结点，每个结点递归得到左右子树深度，判断相差是否 <=1，只要有一个结点相差 >1 直接 return false，若遍历完所有结点都没有 >1 的结点，最终 return true -- 暴力解法，自顶而下，时间 O(n^2)
class Solution {
private:
    int depth(TreeNode *root) {
        // 递归基
        if (root == nullptr) return 0;
        int leftDepth = depth(root->left);
        int rightDepth = depth(root->right);

        return std::max(leftDepth, rightDepth) + 1;  // 加上根结点
    }
public:
    bool isBalanced(TreeNode *root) {
        // 遍历所有结点并判断是否平衡，这里为先序：根 -> 左 -> 右
        if (root == nullptr) return true;

        int leftH = depth(root->left);
        int rightH = depth(root->right);
        // 判断是否平衡
        if (abs(leftH - rightH) > 1) return false;

        // 递归检查左右子树
        return isBalanced(root->left) && isBalanced(root->right);
    }
};

// 优化解法 -- 自下而上遍历
class Solution {
private:
    int checkBalance(TreeNode *root) {
        // 后序遍历边遍历边判断是否平衡，用 -1 做哨兵表示不平衡，平衡返回深度
        if (root == nullptr) return 0;

        int leftH = checkBalance(root->left);
        if (leftH == -1) return -1;

        int rightH = checkBalance(root->right);
        if (rightH == -1) return -1;

        if (abs(leftH - rightH) > 1) return -1;

        return std::max(leftH, rightH) + 1;
    }
public:
    bool isBalanced(TreeNode *root) {
        return checkBalance(root) != -1;    // 返回值为深度不等于 -1 则平衡
    }
};