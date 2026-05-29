#include <iostream>
#include <vector>
#include <queue>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(): val(0), left(nullptr), right(nullptr) {};
    TreeNode(int x): val(x), left(nullptr), right(nullptr) {};
    TreeNode(int x, TreeNode *l, TreeNode*r): val(x), left(l), right(r) {};
};

// 思路：二叉树直径 = 左子树最大深度 + 右子树最大深度 + 1
class Solution {
private:
    int maxDepth(TreeNode *root) {
        if (root == nullptr) return 0;
        int leftDepth = maxDepth(root->left);
        int rightDepth = maxDepth(root->right);
        return 1 + (leftDepth > rightDepth ? leftDepth : rightDepth);
    }
public:
    int diameterOfBinatyTree(TreeNode *root) {
        if (root == nullptr) return 0;
        return 1 + maxDepth(root->left) + maxDepth(root->right);
    }
};

// / 思路：每颗子树二叉树直径 = 左子树最大深度 + 右子树最大深度 + 1，每颗子树二叉树直径中的最大值即为二叉树的直径
class Solution {
private:
    int diameter = 0;                       // 全局直径(全局最大边数)
    int depth(TreeNode *root) {
        if (root == nullptr) return 0;
        int L = depth(root->left);
        int R = depth(root->right);
        diameter = diameter > (L + R) ? diameter : L + R;   // L(左子树深度) + R(右子树深度) 即经过该结点的边数
        return 1 + (L > R ? L : R);     // 返回深度
    }
public:
    int diameterOfBinaryTree(TreeNode *root) {
        depth(root);
        return diameter;
    }
};