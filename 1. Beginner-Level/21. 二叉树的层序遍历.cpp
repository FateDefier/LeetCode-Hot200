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

// 思路：借助队列实现，每次记录上一层遍历的结点的个数以遍历出队当前层
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode *root) {
        vector<vector<int>> res;
        queue<TreeNode*> q;
        if (root == nullptr) {
            return res;
        }
        q.push(root);
        while (!q.empty()) {
            int levelSize = q.size();
            vector<int> level;
            for (int i = 0; i < levelSize; ++i) {
                TreeNode *curr = q.front();
                q.pop();
                level.push_back(curr->val);
                // 将下一层要遍历的结点入队
                if (curr->left != nullptr) q.push(curr->left);
                if (curr->right != nullptr) q.push(curr->right);
            }
            res.push_back(move(level));       // 显式移动，避免拷贝
        }
        return res;
    }
};