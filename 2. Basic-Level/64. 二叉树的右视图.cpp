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

// 思路 1：先序遍历变种，由 根->左->右 变为 根->右->左，即最右边不为空时，加入最右边的结点，最右边为空时，加入其左边一个，直至递归遍历完所有结点
class Solution {
private:
    void dfs(TreeNode *node, int depth, vector<int> &res) {
        // 递归基
        if (!node) return;

        // 关键：判断当前结点(根据递归基已知该节点存在)是否加入 res
        if (depth == res.size()) {
            res.push_back(node->val);
        }

        // 递归遍历右子树、左子树
        dfs(node->right, depth + 1, res);
        dfs(node->left, depth + 1, res);
    }
public:
    vector<int> rightSideView(TreeNode *root) {
        vector<int> result;
        dfs(root, 0, result);
        return result;
    }
};

// 思路 2：层序遍历变种，普通层序遍历每层遍历时从左到右，而变种可以从右到左，队列存遍历的所有元素，每次取队首元素加入 res，比普通层序遍历更优（循环内少一层比较是否是最右边的结点，而只用加入队首元素）
class Solution {
public:
    vector<int> rightSideView(TreeNode *root) {
        if (root == nullptr) return {};
        vector<int> result;
        
        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            // 队首元素即为该层最右边的元素
            result.push_back(q.front()->val);

            int levelSize = q.size();        // 当前层的结点数
            for (int i = 0; i < levelSize; ++i) {
                TreeNode *node = q.front();
                q.pop();

                // 层序遍历，先右后左
                if (node->right != nullptr) q.push(node->right);
                if (node->left != nullptr) q.push(node->left);
            }
        }
        return result;
    }
};