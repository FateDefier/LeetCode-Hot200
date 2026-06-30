#include <string>
using namespace std;

// 理解一下 Trie：不是二叉树，是多叉树，大多数情况结点不存 value，而是边存字母(核心：下一节点不存在，不能连成路径，没有这个字母，反指可以连成路径，字母存在，完整的路径即为word)，其每个结点的所有孩子不同，如本题为 26 个小写字母。常用于前缀搜索，补全，如输入"app"提示"apple"
class Trie {
private:
    struct TrieNode {
        TrieNode *children[26];     // 只有小写字母，用数组存，空间浪费没有太大
        bool isEnd;

        TrieNode() : isEnd(false) {
            // 左右指针初始化为 nullptr
            for (int i = 0; i < 26; ++i) {
                children[i] = nullptr;
            }
        }
    };

    // 根结点 
    TrieNode *root;

public:
    // 默认构造
    Trie() {
        root = new TrieNode();
    }

    // 插入
    void insert(string word) {
        TrieNode *cur = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (!cur->children[idx]) {
                // 关键：为空则连接路径(下一个 children 不为nullptr)表示该字母存在
                cur->children[idx] = new TrieNode();
            }
            cur = cur->children[idx];
        }
        // 关键：遍历完整个 word，别忘接标记单词结尾
        cur->isEnd = true;
    }

    // 查找
    bool search(string word) {
        TrieNode *cur = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (!cur->children[idx]) return false;  // 路径断了，可以判断 word 不存在
            // 没断则继续走
            cur = cur->children[idx];
        }
        // 关键，必须根据 isEnd 判断是否插入了，比如只插入了 "apple" 找 "app" 如果遍历完 "app" 直接 return true; 就与实际不符合，我只插入了 "apple"，实际没有插入 "app"，所以查找成功的标志不是遍历完 word，而是 isEnd = true;
        return cur->isEnd;
    }

    // 找前缀开头的词(查找阶段，与查找相比遍历完 prefix 返回 true 即可)
    bool startsWith(string prefix) {
        TrieNode *cur = root;
        for (char ch : prefix) {
            int idx = ch - 'a';
            if (!cur->children[idx]) return false;
            cur = cur->children[idx];
        }
        return true;
    }
};