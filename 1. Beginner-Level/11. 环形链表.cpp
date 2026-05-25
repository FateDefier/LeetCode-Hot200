#include <iostream>
#include <unordered_map>
#include <unordered_set>
using namespace std;

// 链表结构
struct ListNode {
    int val;
    ListNode *next;             
    ListNode(int x): val(x), next(nullptr) {};             
};

class Solution {
public:
    bool hasCircle(ListNode *head) {
        unordered_map<int, int> mp;         // value - index
        ListNode *p = head;
        int i = 1;
        while (p) {
            if (mp.find(p->val) != mp.end()) {
                return true;
            }
            mp[p->val] = i;
            ++i;
        }
        return false;
    }
};

class Solution {
public:
    bool hasCircle(ListNode *head) {
        unordered_set<ListNode*> visited;       // 用集合存结点指针(地址)
        ListNode *p = head;
        while (p) {
            if (visited.count(p)) {             // 查找集合中是否有等于 p 的值(这里为地址)，只可能为 0 或 1(集合不允许有重复值)，这里也可以用visited.find(p) != visited.end()
                return true;
            }
            visited.insert(p);
            p = p->next;
        }
        return false;
    }
};

// 最终版本
class Solution {
public:
    bool hasCircle(ListNode *head) {
        if (!head || !head->next) {
            return false;
        }
        ListNode *slow = head, *fast = head->next;
        while (fast && fast->next) {
            if (slow == fast) {
                return true;
            } else {
                slow = slow->next;
                fast = fast->next->next;
            }
        }
        return false;
    }
};
