#include <string>
#include <sstream>      // istringstream、ostringstream
#include <queue>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 思路 1：类似编码和解码，想到哈夫曼树，前缀编码

// 思路 2：前序遍历DFS
class Codec {
private:
    // 前序遍历递归存入
    void serializeDFS(TreeNode *node, ostringstream &oss) {
        // 递归基 -- 根
        if (!node) {
            // 碰到 null，写入 '#'，别忘记加逗号，最后要用双引号包裹
            oss << "#,";
            return;
        }
        // 不是 null 正常写入值，并用 "," 分割，都放入 oss
        oss << node->val << ",";
        // 左->右
        serializeDFS(node->left, oss);
        serializeDFS(node->right, oss); 
    }

    // 递归解序列化
    TreeNode* deserializeDFS(istringstream &iss) {
        string token;
        getline(iss, token, ',');   // 每次递归读一个结点
        // 这里 # 必须用双引号包裹，意为字符串，才可以和 token(string) 比较
        if (token == "#") return nullptr;

        TreeNode *root = new TreeNode(stoi(token));     // stoi 即 string to integer 如 string "456"-> int 456
        root->left = deserializeDFS(iss);               // iss 的位置已经后移，直接递归
        root->right = deserializeDFS(iss);

        return root;
    }
public:

    // Encodes a tree to a single string 
    string serialize(TreeNode *root) {
        ostringstream oss;
        serializeDFS(root, oss);
        return oss.str();
    }

    // Decode your encoded data to tree
    TreeNode* deserialize(string data) {
        istringstream iss(data);
        return deserializeDFS(iss);
    }
};

// 思路 2：层序遍历BFS -- 借助队列
class Codec {
public:
    string serialize(TreeNode *root) {
        if (!root) return "";       // 注意返回的是空字符串，而非空字符 ''

        ostringstream oss;
        queue<TreeNode*> q;         // 这里实际可以用 vector 优化模拟队列，我这里为语义明了，就使用 vector
        q.push(root);

        while (!q.empty()) {
            TreeNode *node = q.front();
            q.pop();

            if (node) {
                // node != nullptr
                oss << node->val << ",";
                // nullptr 也 push 进队列
                q.push(node->left);
                q.push(node->right);
            } else {
                oss << "#,";
            }
        }
        return oss.str();
    }
    TreeNode* deserialize(string data) {
        if (data.empty()) return nullptr;

        istringstream iss(move(data));      // move 优化，避免将 data 拷贝到缓冲区，使用 move 后原来的 data 不能使用
        // 获取根结点
        string token;
        getline(iss, token, ',');           // 这里用字符 , 分割
        TreeNode *root = new TreeNode(stoi(token));

        // 把根结点加入队列
        queue<TreeNode*> q;     // 同样可以用 vector 优化
        q.push(root);

        while (!q.empty()) {
            TreeNode *node = q.front();
            q.pop();

            // 左孩子
            if (getline(iss, token, ',')) {
                if (token != "#") {
                    node->left = new TreeNode(stoi(token));
                    q.push(node->left);
                }
            }

            // 右孩子
            if (getline(iss, token, ',')) {
                if (token != "#") {
                    node->right = new TreeNode(stoi(token));
                    q.push(node->right);
                }
            }
        }
        return root;
    }
};