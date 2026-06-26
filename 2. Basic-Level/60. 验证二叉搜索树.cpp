#include <climits>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// 思路 1：中序遍历：左 -> 中 -> 右 判断是否递增，刚好符合二叉搜索树的定义要求

// 写法 1 ：中序遍历 -- 前驱指针（更推荐）
class Solution {
private:
    TreeNode *prev = nullptr;       // 前驱指针，用于保证单调
    bool inOrder(TreeNode *node) {
        // 递归基
        if (node == nullptr) return true;

        if (inOrder(node->left) == false) return false;     // 左子树为不为二叉搜索树，直接返回

        // 第一个节点 prev == nullptr 不比较
        if (prev != nullptr && node->val <= prev->val) return false;
        // 更新前驱
        prev = node;

        return inOrder(node->right);
    }
public:
    bool isValidBST(TreeNode *root) {
        return inOrder(root);
    }
};

// 写法 2：中序遍历 -- long long
class Solution {
private:
    bool inOrder(TreeNode *node, long long &prev) {
        if (node == nullptr) return true;

        if (inOrder(node->left, prev) == false) return false;

        if (node->val <= prev) return false;
        prev = node->val;

        return inOrder(node->right, prev);
    }
public:
    bool isValidBST(TreeNode *root) {
        long long prev= LLONG_MIN;      // 必须用 long long 而不能用 int，防止最左下角的结点 val = signed int 的最小值 -2^31
        return inOrder(root, prev);
    }
};

// 解法 2：范围递归（前序遍历）：每个结点携带一个合法范围开区间（严格大于/小于）(minVal, maxVal)，向左走时左边节点应小于当前根，故更新上界，向右走则更新下界，注意要防溢出
class Solution {
private:
    // 范围递归（前序遍历）-- 用 long long 防止溢出，范围为开区间，这里不能用引用，不然递归不能传 node->val
    bool rangeRecursion(TreeNode *node, long long minVal, long long maxVal) {
        // 递归基
        if (node == nullptr) return true;
        // 根
        if (node->val <= minVal || node->val >= maxVal) return false;
        // 左右
        return rangeRecursion(node->left, minVal, node->val) && // 向左走更新上界
               rangeRecursion(node->right, node->val, maxVal);

    }
public:
    bool isValidBST(TreeNode *root) {
        long long prev= LLONG_MIN;      // 必须用 long long 而不能用 int，防止最左下角的结点 val = signed int 的最小值 -2^31
        return rangeRecursion(root, LLONG_MIN, LLONG_MAX);
    }
};