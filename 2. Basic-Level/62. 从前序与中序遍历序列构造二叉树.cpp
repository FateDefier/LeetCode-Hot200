#include <vector>
#include <unordered_map>
#include <stack>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// 思路 1：递归 -- 根据中序序列分左右子树，根据先序序列找根结点，然后在中序序列查找根结点（哈希表优化至 O(1)），利用根结点分左右子树递归
class Solution {
private:
    // 递归构建二叉子树
    TreeNode* buildSubTree(vector<int> &preorder, int preStart, int preEnd, vector<int> &inorder, int inStart, int inEnd, unordered_map<int, int> &inMap) {
        if (preStart > preEnd || inStart > inEnd) return nullptr;

        // 先序序列第一个数即为根结点
        int rootVal = preorder[preStart];
        TreeNode *root = new TreeNode(rootVal);

        // 在中序序列中找 root
        int rootIdx = inMap[rootVal];
        int leftSize = rootIdx - inStart;  // 左子树结点数

        // 递归构建左右子树 -- 注意各个下标索引
        root->left = buildSubTree(preorder, preStart + 1, preStart + leftSize, inorder, inStart, rootIdx - 1, inMap);
        root->right = buildSubTree(preorder, preStart + leftSize + 1, preEnd, inorder, rootIdx + 1, inEnd, inMap);

        return root;
    }
public:
    TreeNode* buildTree(vector<int> &preorder, vector<int> &inorder) {
        int n = preorder.size();    // preorder.size() = inorder.size()
        unordered_map<int, int> inMap;      // value -- index

        // 遍历中序序列构建中序序列哈希表
        for (int i = 0; i < n; ++i) {
            inMap[inorder[i]] = i;
        }

        return buildSubTree(preorder, 0, n - 1, inorder, 0, n - 1, inMap);
    }
};

// 思路相同，解法 2：迭代 -- 手动栈，防止栈溢出
class Solution {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        if (preorder.empty() || inorder.empty()) return nullptr;

        // 前序序列第一个结点必定是根结点
        TreeNode *root = new TreeNode(preorder[0]);
        stack<TreeNode*> stk;       // 存左子树遍历完，而右子树还没有分配的祖先节点
        stk.push(root);             // 根结点入栈
        int inIndex = 0;            // 标记中序序列已经匹配的位置，确定左右子树的边界

        int n = preorder.size();    // 遍历完前序序列也遍历完了中序序列 preorder.size() = inorder.size()
        for (int i = 1; i < n; ++i) {
            // i 用于遍历前序序列，取出下一个要创建的节点值
            TreeNode *cur = stk.top();
            int val = preorder[i];  // 当前待插入的节点值

            if (cur->val != inorder[inIndex]) {
                // 栈顶节点不是中序当前指向的节点，说明还没到达左子树尽头，继续构建左孩子
                cur->left = new TreeNode(val);  // 构建二叉树
                stk.push(cur->left);            // 左子树结点下可能也有左子树，入栈
            } else {
                // 已经遍历完左子树和根，找右子树父节点
                while (!stk.empty() && stk.top()->val == inorder[inIndex]) {
                    // 不断出栈，直至找到右子树的父节点
                    cur = stk.top();        // 记录右子树的父节点
                    stk.pop();
                    ++inIndex;
                }
                cur->right = new TreeNode(val);
                stk.push(cur->right);       // 右子树节点可能也有左子树
            }
        }
        return root;
    }
};