#include <iostream>
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
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        if (!headA || !headB) {
            return nullptr;
        }
        ListNode *p = headA;
        ListNode *q = headB;
        unordered_set<ListNode*> setA;
        while (p) {
            setA.insert(p);
            p = p->next;
        }
        while (q) {
            if (setA.count(q)) {
                return q;
            }
            q = q->next;
        }
        return nullptr;
    }
};

class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        if (headA == nullptr || headB == nullptr) {
            return nullptr;
        }
        ListNode *pA = headA, *pB = headB;
        while (pA != pB) {
            pA = (pA == nullptr) ? headB : pA->next;
            pB = (pB == nullptr) ? headA : pB->next;
        }
        // 要么相交必相遇 pA = pB = 交点地址，要么不相交 pA = pB = nullptr
        return pA;
    }
};