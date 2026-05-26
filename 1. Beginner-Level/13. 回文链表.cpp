#include <iostream>
#include <vector>
#include <stack>
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

// 解法 1：数组拷贝
class Solution {
public:
    bool isPalindrome(ListNode *head) {
        if (head == nullptr || head->next == nullptr) {
            return true;                // 空链表和单元素默认回文
        }
        vector<int> vals;                // 自动管理长度
        ListNode *curr = head;
        while (curr != nullptr) {
            vals.push_back(curr->val);
            curr = curr->next;
        }
        // 数组双指针判断是否回文
        int left = 0, right = (int)vals.size() - 1;
        while (left < right) {          // 循环终止条件是 left < right，不能写成 left != right(奇数满足，偶数不满足)
            if (vals[left] != vals[right]) {
                return false;
            }
            ++left;
            --right;
        }
        return true;
    }
};

// 解法 2：栈
class Solution {
public:
    bool isPalindrome(ListNode *head) {
        if (head == nullptr || head->next == nullptr) {
            return true;                
        }
        stack<int> vals;
        ListNode *curr = head;
        while (curr != nullptr) {
            vals.push(curr->val);
            curr = curr->next;
        }
        curr = head;
        while (curr != nullptr) {
            int val = vals.top();
            vals.pop();                 // 出栈不返回值
            if (curr->val != val) {
                return false;
            }
            curr = curr->next;
        }
        return true;
    }
};
// 解法 3：反转后半段链表(不建议前半段，没有后半段的头指针)
class Solution {
public:
    // 反转链表 -- 参考题 9 反转链表
    ListNode* reverseList(ListNode *head) {
        ListNode *prev = nullptr, *curr = head;
        while (curr != nullptr) {
            ListNode *tmp = curr->next;
            curr->next = prev;
            prev = curr;
            curr = tmp;
        }
        return prev;
    }

    bool isPalindrome(ListNode *head) {
        if (head == nullptr || head->next == nullptr) {
            return true;                
        }
        // 1.快慢指针找中点，参考题 11 环形链表 
        ListNode *slow = head, *fast = head;
        while (fast->next != nullptr && fast->next->next != nullptr) {          // 注意循环终止条件
            slow = slow->next;
            fast = fast->next->next;
        }
        // 2. 反转链表，slow->next 为后半段头结点
        ListNode *secondHalf = reverseList(slow->next);
        // 3.判断回文
        ListNode *p1 = head;
        ListNode *p2 = secondHalf;       //  防止头结点丢失并便于反转还原
        while (p2 != nullptr) {         //  后半段 可能少一个元素，用 p2 判断
            if (p1->val != p2->val) {
                return false;
            }
            p1 = p1->next;
            p2 = p2->next;
        }        
        // 4.还原(再次反转)后半段
        // slow->next = reverseList(secondHalf);
        return true;
    }
};

