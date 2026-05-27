#include <iostream>
#include <vector>
#include <stack>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(): val(0), left(nullptr), right(nullptr) {};
    TreeNode(int x): val(x), left(nullptr), right(nullptr) {};
    TreeNode(int x, TreeNode *l, TreeNode*r): val(x), left(l), right(r) {};
};

// 1.拷贝递归实现：左->中->右
class Solution {
public:
    vector<int> inorderTraversal(TreeNode *root) {
        vector<int> res;
        if (root == nullptr) {              // 递归基
            return res;
        }
        // 左子树结果
        vector<int> left = inorderTraversal(root->left);
        res.insert(res.end(), left.begin(), left.end());        // 用 insert 而不用 push_back 时因为 left 是 vector 而不是 元素
        res.push_back(root->val);
        // 右子树结果
        vector<int> right = inorderTraversal(root->right);
        res.insert(res.end(), right.begin(), right.end());
        return res;
    }
};

// 2.借用辅助函数原地递归实现
class Solution {
private:
    void inorder(TreeNode *root, vector<int> &res) {
        if (root == nullptr) {              // 递归基
            return ;
        }
        inorder(root->left, res);
        res.push_back(root->val);
        inorder(root->right, res);
    }
public:
    vector<int> inorderTraversal(TreeNode *root) {
        vector<int> res;
        inorder(root, res);
        return res;
    }
};

// 3.借助栈非递归迭代实现
class Solution {
public:
    vector<int> inorderTraversal(TreeNode *root) {
        vector<int> res;
        stack<TreeNode*> s;
        TreeNode *curr = root;
        while (curr != nullptr || !s.empty()) {         // 注意：只有当 curr 为空且栈空时才退出循环
            // 1.沿着左子树走到底，并每走一步压栈
            while (curr != nullptr) {
                s.push(curr);
                curr = curr->left;
            }
            // 2.弹栈并加入 res
            curr = s.top();
            s.pop();
            res.push_back(curr->val);
            // 3.对右子树重复上述操作
            curr = curr->right;
        }
        return res;
    }
};