#include <iostream>
#include <vector>
#include <stack>
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

// 1.拷贝递归实现：左->右->根
class Solution {
public:
    vector<int> postorderTraversal(TreeNode *root) {
        vector<int> res;
        if (root == nullptr) {
            return res;
        }
        vector<int> left = postorderTraversal(root->left);
        res.insert(res.end(), left.begin(), left.end());
        vector<int> right = postorderTraversal(root->right);
        res.insert(res.end(), right.begin(), right.end());
        res.push_back(root->val);
        return res;
    }
};

// 2.借助辅助函数原地递归实现
class Solution {
private:
    void postorder(TreeNode *root, vector<int> &res) {
        if (root == nullptr) {
            return ;
        }
        postorder(root->left, res);
        postorder(root->right, res);
        res.push_back(root->val);
    }
public:
    vector<int> postorderTraversal(TreeNode *root) {
        vector<int> res;
        postorder(root, res);
        return res;
    }
};

// 3.借助栈非递归实现 -- 前序遍历变种(根右左) + 反转
class Solution {
public:
    vector<int> postorderTraversal(TreeNode *root) {
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
            // 进栈：左->右   出栈：右->左(即 根->右->左)
            if (curr->left != nullptr) {
                s.push(curr->left);
            }
            if (curr->right != nullptr) {
                s.push(curr->right);
            }
        }
        // 反转：根->右->左 ----> 左->右->根
        reverse(res.begin(), res.end());
        return res;
    }
};

// 4.借助栈非递归正式迭代法实现
class Solution {
public:
    vector<int> postorderTraversal(TreeNode *root) {
        vector<int> res;
        stack<TreeNode*> s;
        TreeNode *curr = root;
        TreeNode *lastVisited = nullptr;
        while (curr != nullptr || !s.empty()) {
            // 一路向左，压栈
            while (curr != nullptr) {
                s.push(curr);
                curr = curr->left;
            }
            TreeNode *top = s.top();
            // 如果右子树为空或右子树已访问，则访问当前节点
            if (top->right == nullptr || top->right ==lastVisited) {
                res.push_back(top->val);
                s.pop();
                lastVisited = top;              // 标记为已访问
            } else {
                // 右子树不为空且未访问，转右子树
                curr = top->right;
            }
        }
        return res;
    }
};