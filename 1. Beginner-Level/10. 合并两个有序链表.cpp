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

// 错误写法：
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        if (list1->val > list2->val) {
            ListNode *temp = list2;
            temp->next = list1->next;
            list1->next = temp;
            ++list1;
            ++list2;
        } else {
            ++list2;
        }
        while (list2) {
            list1->next = list2;
        }
        return list1;
    }
};

// 不能只使用一个 tail，如果只使用 tail 最后就不好 return，return 必须要用到头指针 dump
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode dumy(0);                   // dumy 为头指针
        ListNode *tail = &dumy;             // tail 永远指向最后一个元素
        while (list1 && list2) {
            if (list1->val <= list2->val) {
                tail->next = list1;
                list1 = list1->next;
            } else {
                tail->next = list2;
                list2 = list2->next;
            }
            tail = tail->next;
        }
        // 链接剩余部分
        tail->next = list1 ? list1 : list2;
        return dumy.next;
    }
};
