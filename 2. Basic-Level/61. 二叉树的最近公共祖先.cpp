#include <vector>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// 思路 1：从根节点出发，两条路径分别找到 p 和 q，记录路径，两路径最后一个相同的结点即为 LCA
class Solution {
private:
    // 从 root 查找 target，并把从 root 到 target 的路径存入 path
    bool findPath(TreeNode *root, TreeNode *target, vector<TreeNode*> &path) {
        if (root == nullptr) return false;

        // 加入当前结点
        path.push_back(root);
        // 当前结点为 target 直接返回
        if (root == target) return true;
        // 当前结点不为 target，在左右子树中递归找 target
        if (findPath(root->left, target, path) == true) return true;
        if (findPath(root->right, target, path) == true) return true;

        // 没找到，回溯：移除当前节点
        path.pop_back();
        return false;
    }
public:
    TreeNode* lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q) {
        // 找到从 root 到 p 和 q 的路径
        vector<TreeNode*> pathP, pathQ;
        findPath(root, p, pathP);
        findPath(root, q, pathQ);

        // 比较两条路径，最后一个相同的即为 LCA
        TreeNode *LCA = nullptr;
        int i = 0;
        while (i < pathP.size() && i < pathQ.size() && pathP[i] == pathQ[i]) {
            LCA = pathP[i];
            ++i;
        }

        return LCA;
    }
};

// 思路 2：核心思想：分治，思路见注释
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q) {
        // 递归基：找到空节点或者 p/q 其中一个
        if (root == nullptr || root == p || root == q) return root;

        // 遍历在左右子树找 p/q
        TreeNode *left = lowestCommonAncestor(root->left, p, q);
        TreeNode *right = lowestCommonAncestor(root->right, p, q);

        // 两个在两侧子树找到，则当前结点即为LCA
        if (left != nullptr && right != nullptr) return root;

        // 如果只有一边找到，则二者在同一边，向上传递找到的结果（p/q或已确定的LCA）p、q都在同一边可能 p/q 本身就是p和q的LCA，所以直接上抛
        return left != nullptr ? left : right;
    }
};