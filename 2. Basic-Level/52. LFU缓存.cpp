#include <unordered_map>
using namespace std;

class LFUCache {
private:
    struct Node {
        int key, val, freq;
        Node *prev, *next;
        Node(int k, int v) : key(k), val(v), freq(1), prev{nullptr}, next(nullptr) {}
    };
    // 双向链表 -- LRU（最常使用）链表 
    struct DoublyList {
        // 虚拟头尾
        Node *head, *tail;
        int size;               // 用于判空

        // 默认构造
        DoublyList() {
            head = new Node(0, 0);
            tail = new Node(0, 0);
            size = 0;
            head->next = tail;
            tail->prev = head;
        }

        // 头插
        void addToHead(Node *node) {
            node->prev = head;
            node->next = head->next;
            head->next->prev = node;
            head->next = node;
            ++size;
        }

        // 删除
        void removeNode(Node *node) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            --size;
        }

        // 删除最不常用
        Node* removeLast() {
            Node *node = tail->prev;
            removeNode(node);
            return node;
        }
    };
    int cap;
    int minFreq;            // 当前最小频率
    unordered_map<int, Node*> key2node;     // key --> Node*
    unordered_map<int, DoublyList*> freq2list;// freq --> 该频率的 LRU 链表的地址

    // 更新节点频率
    void updateNodeFreq(Node *node) {
        int oldFreq = node->freq;
        DoublyList *oldList = freq2list[oldFreq];
        // 在旧频率对应的 LRU 链表删除当前结点
        oldList->removeNode(node);
        // 如果删除后，旧频率链表变空且是最小频率，更新 minFreq
        if (oldFreq == minFreq && oldList->size == 0) ++minFreq;

        // 更新当前结点的频率并添加到新频率的 LRU 链表
        ++node->freq;
        int newFreq = node->freq;
        // 如果新频率对应的链表不存在，则新建LRU链表加入
        if (freq2list.find(newFreq) == freq2list.end()) {
            freq2list[newFreq] = new DoublyList();
        }
        // 无论是否新建链表都要加入链表
        freq2list[newFreq]->addToHead(node);
    }
public:
    LFUCache (int capacity) : cap(capacity), minFreq(0) {}

    int get(int key) {
        auto it = key2node.find(key);
        if (it == key2node.end()) return -1;
        // 更新结点访问频率
        updateNodeFreq(it->second);
        return it->second->val;
    }

    void put(int key, int value) {
        if (cap == 0) return ;

        auto it = key2node.find(key);
        if (it != key2node.end()) {
            // key 存在，更新 value 和 结点频率到新链表
            it->second->val = value;
            updateNodeFreq(it->second);
        } else {
            // key 不存在，新建结点
            // 检查容量
            if (key2node.size() >= cap) {
                // 超出容量，淘汰 minFreq 对应的链表的最后一个结点
                Node *removed = freq2list[minFreq]->removeLast();
                key2node.erase(removed->key);
            }

            // 创建新节点，该节点频率为 1
            Node *newNode = new Node(key, value);
            key2node[key] = newNode;        // 更新 key2node
            // 找找是否有频率为 1 的链表，没有则创建
            if (freq2list.find(1) == freq2list.end()) {
                freq2list[1] = new DoublyList();
            }
            // 加入结点
            freq2list[1]->addToHead(newNode);
            minFreq = 1;            // 加入新节点，最小访问频率必定为 1
        }
    }
};