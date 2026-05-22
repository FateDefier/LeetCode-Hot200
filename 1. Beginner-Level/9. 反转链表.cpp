#include <iostream>
using namespace std;

// 链表结构
struct ListNode {
    int val;
    ListNode *next;
    ListNode(): val(0), next(nullptr) {};               // 默认构造
    ListNode(int x): val(x), next(nullptr) {};          // 有参构造
    ListNode(int x, ListNode *n): val(x), next(n) {};   // 有参构造
};

// 思路1：重新构造一个 head2 从 head 后面开始给 head2 赋值(非原地操作)
class Solution {
public:
    ListNode* findFinal(ListNode *head) {
        ListNode *p = head;                 // head 指向第一个结点
        while (p->next) {
            p = p->next;
        }
        return p;
    }
    ListNode* reverseList(ListNode *head) {
        ListNode head2;
        ListNode *p = head->next;
        // 循环需要使用递归(单链表，无前驱指针)：先把最后一个结点(next=nullptr)的结点插入到 head2 的 next
        while (p) {

        }
    }
};

// 解法一：非递归非原地头插
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode *head2 = nullptr;
        ListNode *p = head;
        while (p) {
            ListNode *newNode = new ListNode(p->val);
            // 头插
            newNode->next = head2;
            head2 = newNode;
            p = p->next;
        }
        return head2;
    }
};

// 解法二(推荐)：原地递归
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        if (!head || !head->next) return head;       // 递归基，即最后一层递归终止
        // newHead 始终是原链表的最后一个节点
        ListNode *newHead = reverseList(head->next);
        // 让反转后的子链表的最后一个结点指向 当前递归层 的 head
        head->next->next = head;
        head->next = nullptr;
        return newHead;
    }
};
