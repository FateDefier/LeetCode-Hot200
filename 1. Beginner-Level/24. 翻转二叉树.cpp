#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(): val(0), left(nullptr), right(nullptr) {};
    TreeNode(int x): val(x), left(nullptr), right(nullptr) {};
    TreeNode(int x, TreeNode *l, TreeNode*r): val(x), left(l), right(r) {};
};

// 思路：队列实现层序遍历
class Solution {
public:
    TreeNode* invertTree(TreeNode *root) {
        if (root == nullptr) return root;
        queue<TreeNode*> q;
        q.push(root);
        q.push(root->left);
        q.push(root->right);
        while (!q.empty()) {
            TreeNode *r = q.front(); q.pop();
            TreeNode *lT = q.front(); q.pop();
            TreeNode *rT = q.front(); q.pop();
            r->left = rT;
            r->right = lT;
            q.push(lT);
            q.push(lT->left);
            q.push(lT->right);
            q.push(rT);
            q.push(rT->left);
            q.push(rT->right);
        }
        return root;
    }
};

// 正确迭代解法：
class Solution {
public:
    TreeNode* invertTree(TreeNode *root) {
        if (root == nullptr) return root;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode *curr = q.front();
            q.pop();
            swap(curr->left, curr->right);
            if (curr->left != nullptr) q.push(curr->left);
            if (curr->right != nullptr) q.push(curr->right);
        }
        return root;
    }
};

// 递归解法
class Solution {
public:
    TreeNode* invertTree(TreeNode *root) {
        // 递归基
        if (root == nullptr) return root;
        // 先翻转左、右子树
        invertTree(root->left);
        invertTree(root->right);
        // 再交换左右孩子
        swap(root->left, root->right);
        return root;
    }
};