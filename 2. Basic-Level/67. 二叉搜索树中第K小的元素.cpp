#include <stack>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// 思路 1：任意 DFS/BFS 遍历，用一个数组存所有的值，排序即可找到第 k 小的值，但这明显是最笨的解法 O(n + nlogn)，是否可以进行优化
// 直接构造升序数组：边构建数组边二分插入升序位置 O(n * (logn(二分) + n(插入)))，更加坏，用平衡二叉搜索树（如 std::multiset）来维护，才是 O(n log n)
// 看错题了：看成了二叉线索树，二叉搜索树/二叉排序树/二叉查找树（左节点比根小，头结点比根大），中序序列是天然的升序，遍历到第 k 个停止即可

// 解法 1：递归
class Solution {
private:
    void inOrder(TreeNode *node, int &k, int &res) {
        if (!node) return;

        // 中序遍历：左->根->右
        inOrder(node->left, k, res);
        if (--k == 0) {     // 先更新(访问当前最小的元素)再判断
            res = node->val;
            return;
        }
        inOrder(node->right, k, res);
    }
public:
    int kthSmallest(TreeNode *root, int k) {
        int res = 0;
        inOrder(root, k, res);
        return res;
    }
};

// 解法 2：迭代 + 手动栈 -- 防止栈溢出
class Solution {
public:
    int kthSmallest(TreeNode *root, int k) {
        std::stack<TreeNode*> s;
        TreeNode *cur = root;

        while (cur || !s.empty()) {
            // 一路向左并进栈
            while (cur) {       // 注意：这里不是 cur->left
                s.push(cur);
                cur = cur->left;
            }
            // 访问根
            cur = s.top();
            s.pop();
            --k;
            if (k == 0) return cur->val;

            // 转向右子树
            cur = cur->right;
        }
        return -1;          // 实际不可能到这，除非 k 不合法
    }
};