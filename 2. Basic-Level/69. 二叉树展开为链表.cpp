#include <stack>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// 思路 1：二叉树展开成链表，类似完成二叉树的线索化，只是其要求左指针为空，右指针指向下一个元素(相当于next)，最笨的方法：用队列存第一次先序遍历的所有元素，再重新遍历构建链表
// 优化：显然，我们不会使用上述“笨方法”，我们想的应该是一次遍历，通过指针操作，一遍先序遍历，一遍构建链表，那么是否需要额外空间？好像不用，我要改变 node 的右指针，只用一个 temp 指针把右指针存着之后遍历右子树从这里走就行，但是左子树每个结点貌似都需要一个 temp 指针（空间(O(h)，这里分析错误，实际存了 n/2 ，空间 O(n)）
// 空间 O(h)：要想空间为当前深度 O(h)，则使用递归（隐式系统栈），但是会有溢出风险

// 正确解法 1：先序遍历 + 递归(空间O(h))
class Solution {
private:
    void dfs(TreeNode *node, TreeNode*& prev) {
        if (!node) return;

        // 暂存 node 的左右子树，因为之后要把 node 插入到 prev 右指针后面
        TreeNode *left = node->left;
        TreeNode *right = node->right;

        // 先序遍历：根
        if (prev) {
            prev->right = node;
            prev->left = nullptr;
        }
        // 更新 prev
        prev = node;
        // 左->右
        dfs(left, prev);
        dfs(right, prev);
    }
public:
    void flatten(TreeNode *root) {
        TreeNode *prev = nullptr;
        dfs(root, prev);
    }
};

// 解法 2：先序遍历变种：利用栈“后进先出”的特性，栈存储先序遍历的逆序序列，出栈时则为先序序列，连接即可，O(n) -- 防止爆栈
class Solution {
public:
    void flatten(TreeNode *root) {
        if (!root) return;
        std::stack<TreeNode*> st;
        st.push(root);

        while (!st.empty()) {
            // 获取栈顶结点为当前结点并出栈，用于连接下一个栈顶元素
            TreeNode *node = st.top();
            st.pop();

            // 注意这里的入栈顺序，先右后左，逆序入栈
            if (node->right) st.push(node->right);
            if (node->left) st.push(node->left);

            // 连接
            if (!st.empty()) {              // 注意条件
                node->right = st.top();     // 这里不出栈
            }
            node->left = nullptr;           // 左指针始终置空
        }
    }
};

// 解法 3：Morris 前驱挂接 -- 最优，即二叉树的线索化
class Solution {
public:
    void flatten(TreeNode *root) {
        TreeNode *cur = root;

        while (cur) {
            if (cur->left) {
                // 1.找左子树先序遍历最后一个结点，即左子树最右下角的结点
                TreeNode *pre = cur->left;
                while (pre->right) {
                    pre = pre->right;
                }

                // 2. 线索化：把右子树挂接到左子树先序遍历最后一个结点
                pre->right = cur->right;

                // 3.将左子树移动到右子树，左指针置空
                cur->right = cur->left;
                cur->left = nullptr;
            }
            cur = cur->right;       // 注意：这里左指针已经为空，找下一个应该往右走
        }
    }
};
