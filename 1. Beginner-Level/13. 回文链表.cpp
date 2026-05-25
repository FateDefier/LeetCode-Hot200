#include <iostream>
using namespace std;

// 链表结构
struct ListNode {
    int val;
    ListNode *next;
    ListNode(): val(0), next(nullptr) {};               
    ListNode(int x): val(x), next(nullptr) {};         
    ListNode(int x, ListNode *n): val(x), next(n) {};  
};

// 错误代码：
class Solution {
public:
    bool isPalindrome(ListNode *head) {
        if (!head || !head->next) {
            return false;
        }
        ListNode *head2 = new ListNode;
        ListNode *p = head->next, *pre = head, *q = head2->next;
        while (p) {
            // 头插法使回文链表前半段倒序
            ListNode *newNode = new ListNode(p->val, q);
            if (pre->val == p->val) {
                break;
            }
            q = newNode;
            p = p->next;
            pre = pre->next;
        }
        while (p) {
            if (q->val == p->val) {
                q = q->next;
                p = p->next;
            } else {
                return false;
            }
        }
        return true;
    }
};