struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// 思路 1：从根节点出发，两条路径分别找到 p 和 q，记录路径，两路径最后一个相同的结点即为 LCA

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