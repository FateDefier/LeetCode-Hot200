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

// 思路：双指针 + BFS + 单个队列
class Solution {
public:
    bool isSymmetric(TreeNode *root) {
        if (root == nullptr) return true;
        TreeNode *left = root->left, *right = root->right;
        queue<TreeNode*> q;
        q.push(root);
        while (left != nullptr && right != nullptr) {
            int q_size = q.size();
            for (int i = 0; i < q_size; ++i) {
                TreeNode *node_l = q.front();
                q.pop();
                TreeNode *node_r = q.front();
                q.pop();
                if (node_l->val != node_r->val) {
                    return false;
                }
                // 每次压入两个值 left->val, right->val
                if (left->left && right->left) {
                    q.push(left->left);
                    q.push(right->left);
                }
                if (left->right && right->right) {
                    q.push(left->right);
                    q.push(right->right);
                }
            }
        }
        return true;
    }
};

// 解法 1：迭代写法
class Solution {
public:
    bool isSymmetric(TreeNode *root) {
        if (root == nullptr) return true;
        queue<TreeNode*> q;
        q.push(root->left);
        q.push(root->right);
        while (!q.empty()) {
            TreeNode *l = q.front();
            q.pop();
            TreeNode *r = q.front();
            q.pop();
            if (l == nullptr && r == nullptr) continue;
            if (l == nullptr || r == nullptr) return false;
            if (l->val != r->val) return false;
            // 按照镜像顺序入队
            q.push(l->left);
            q.push(r->right);
            q.push(l->right);
            q.push(r->left);
        }
        return true;
    }
};

// 解法 2 -- 递归
class Solution {
private:
    // 判断 两棵树 是否为镜像
    bool isMirror(TreeNode *t1, TreeNode *t2) {
        // 递归基
        if (t1 == nullptr && t2 == nullptr) return true;
        if (t1 == nullptr || t2 == nullptr) return false;
        return (t1->val == t2->val) && isMirror(t1->left, t2->right) && isMirror(t1->right, t2->left);
    }
public:
    bool isSymmetric(TreeNode *root) {
        if (root == nullptr) return true;
        return isMirror(root->left, root->right);
    }
};