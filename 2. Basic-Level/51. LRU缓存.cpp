#include <unordered_map>
using namespace std;

// 思路：哈希表 + 双向链表(Dummy Head/Tail)，要求访问(get)和插入删除(put)时间复杂度均为 O(1)，访问 -- 哈希表查询结点 O(1)，插入删除 -- 双向链表 O(1)，为什么用双向链表，插入删除需要前驱结点，而哈希表只能找当前结点
class LRUCache {
private:
    struct Node {
        int key, val;
        Node *prev;
        Node *next;
        Node(int k, int v) : key(k), val(v), prev(nullptr), next(nullptr) {}
    };

    int cap;        // capacity
    unordered_map<int, Node*> cache;        // 哈希表存的是 key--指针地址
    Node *head;     // 虚拟头尾结点，便于进行对首尾的操作
    Node *tail;

    // 1.头插
    void addToHead(Node *node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

    // 2.删除
    void removeNode(Node *node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    // 3.把当前结点移动到头部(删除+头插)（最近使用）
    void moveToHead(Node *node) {
        removeNode(node);
        addToHead(node);
    }

    // 4.删除尾部结点（最久未使用）
    Node* removeTail() {
        Node *node = tail->prev;
        removeNode(node);
        return node;
    }
public:
    LRUCache(int capacity) {
        head = new Node(0, 0);
        tail = new Node(0, 0);
        cap = capacity;
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if (cache.find(key) == cache.end()) {
            // key 不存在，返回 -1
            return -1;
        }
        // 如果 key 存在，标记为最近使用并返回对应的值
        Node *node = cache[key];
        moveToHead(node);
        return node->val;
    }

    void put(int key, int value) {
        if (cache.find(key) == cache.end()) {
            // 没找到，插入头部，并加入哈希表
            Node *newNode = new Node(key, value);
            cache[key] = newNode;
            addToHead(newNode);

            // 检测是否超出容量，超出则删除最久未使用
            if (cache.size() > cap) {
                // 链表删除
                Node *removedNode = removeTail();
                // 哈希表也要同步更新
                cache.erase(removedNode->key);
                delete removedNode;
            }
        } else {
            // 找到了，更新 key 对应的 value
            Node *node = cache[key];
            node->val = value;
            moveToHead(node);
        }
    }
};

// 优化版
class LRUCache {
private:
    struct Node {
        int key, val;
        Node *prev;
        Node *next;
        Node(int k, int v) : key(k), val(v), prev(nullptr), next(nullptr) {}
    };

    int cap;        // capacity
    unordered_map<int, Node*> cache;        // 哈希表存的是 key--指针地址
    Node *head;     // 虚拟头尾结点，便于进行对首尾的操作
    Node *tail;

    // 1.头插
    void addToHead(Node *node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

    // 2.删除
    void removeNode(Node *node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    // 3.把当前结点移动到头部(删除+头插)（最近使用）
    void moveToHead(Node *node) {
        removeNode(node);
        addToHead(node);
    }

    // 4.删除尾部结点（最久未使用）
    Node* removeTail() {
        Node *node = tail->prev;
        removeNode(node);
        return node;
    }
public:
    LRUCache(int capacity) {
        head = new Node(0, 0);
        tail = new Node(0, 0);
        cap = capacity;
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        auto it = cache.find(key);
        // key 不存在，返回 -1
        if (it == cache.end()) return -1;
        // 如果 key 存在，标记为最近使用并返回对应的值
        moveToHead(it->second);
        return it->second->val;
    }

    void put(int key, int value) {
        auto it = cache.find(key);
        if (it == cache.end()) {
            // 没找到，插入头部，并加入哈希表
            Node *newNode = new Node(key, value);
            cache[key] = newNode;
            addToHead(newNode);

            // 检测是否超出容量，超出则删除最久未使用
            if (cache.size() > cap) {
                // 链表删除
                Node *removedNode = removeTail();
                // 哈希表也要同步更新
                cache.erase(removedNode->key);
                delete removedNode;
            }
        } else {
            // 找到了，更新 key 对应的 value
            it->second->val = value;
            moveToHead(it->second);
        }
    }
};