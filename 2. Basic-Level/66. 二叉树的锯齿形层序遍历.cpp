#include <vector>
#include <queue>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// 思路 1：层序遍历，必定借助队列实现，而要完成锯齿形遍历，则入队顺序是关键，刚好是两层交互 左->右 和 右->左，可以通过 count=0 计数奇偶来实现，偶数从左到右，计数从右到左，而方向可以由双端队列实现
// 上述思路优化：两种方向，直接用 bool 变量实现即可，不需要借助双端队列，而是在插入队列时根据布尔值确定插入位置，更简洁
class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode *root) {
        if (!root) return {};
        vector<vector<int>> res;
        queue<TreeNode*> q;
        q.push(root);
        int leftToRight = true;

        while (!q.empty()) {
            int levelSize = q.size();
            vector<int> level(levelSize);       // 存储该层的层序序列
            for (int i = 0; i < levelSize; ++i) {
                TreeNode *node = q.front();
                q.pop();                        // 别忘记出队
                // 根据方向确定插入位置 (左->右 i) (右->左 levelSize - 1 - i) 
                int index = leftToRight ? i : (levelSize - 1 - i);
                level[index] = node->val;
                // 正常层序入队：从左到右
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            res.push_back(level);
            // 每次反序，取反
            leftToRight = !leftToRight;
        }
        return res;
    }
};