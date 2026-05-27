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

// 1.拷贝递归实现：根->左->右
class Solution {
public:
    vector<int> preorderTraversal(TreeNode *root) {
        vector<int> res;
        if (root == nullptr) {
            return res;
        }
        res.push_back(root->val);
        vector<int> left = preorderTraversal(root->left);
        res.insert(res.end(), left.begin(), left.end());
        vector<int> right = preorderTraversal(root->right);
        res.insert(res.end(), right.begin(), right.end());
        return res;
    }
};

// 2.借助辅助函数原地递归实现
class Solution {
private:
    void preorder(TreeNode *root, vector<int> &res) {
        if (root == nullptr) {
            return ;
        }
        res.push_back(root->val);
        preorder(root->left, res);
        preorder(root->right, res);
    }
public:
    vector<int> preorderTraversal(TreeNode *root) {
        vector<int> res;
        preorder(root, res);
        return res;
    }
};

// 3.借助栈非递归实现
class Solution {
public:
    vector<int> preorderTraversal(TreeNode *root) {
        vector<int> res;
        stack<TreeNode*> s;
        if (root == nullptr) {
            return res;
        }
        s.push(root);                       // 这里要提前让 root 进栈，否则循环中 curr 为野指针
        while (!s.empty()) {
            TreeNode *curr = s.top();
            s.pop();
            // 1.先访问根结点
            res.push_back(curr->val);
            // 依次把右孩子、左孩子放入栈中(对应顺序左孩子、右孩子出栈)
            if (curr->right != nullptr) {
                s.push(curr->right);
            }
            if (curr->left != nullptr) {
                s.push(curr->left);
            }
        }
        return res;
    }
};