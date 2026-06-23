struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *n) : val(x), next(n) {}
};

// 思路 1：逆序存储，当前位满 10 进 1，两个链表右边，根据 curr1 和 curr2 哪个为 nullptr 返回另一个
class Solution {
public:
    ListNode* addTwoNumbers(ListNode *l1, ListNode *l2) {
        if (l1 == nullptr && l2 == nullptr) return nullptr;
        int carry = 0;      // 进位
        ListNode *curr1 = l1;
        ListNode *curr2 = l2;
        while (curr1 != nullptr && curr2 != nullptr) {
            int sum = curr1->val + curr2->val;
            curr1->val = sum % 10 + carry;
            curr2->val = sum % 10 + carry;
            carry = sum / 10;
            curr1 = curr1->next;
            curr2 = curr2->next;
        }

        if (curr1 == nullptr) {
            curr2->val += carry;
            return l2;
        }
        if (curr2 == nullptr) {
            curr1->val += carry;
            return l1;
        }
        return nullptr;
    }
};

// 修正：
class Solution {
public:
    ListNode* addTwoNumbers(ListNode *l1, ListNode *l2) {
        // 栈上更优
        ListNode dummy(0);
        ListNode *curr = &dummy;
        int carry = 0;          // 进位值

        while (l1 != nullptr || l2 != nullptr || carry != 0) {  // 关键点：carry 不等于 0 进位后最终的数比 l1,l2 长，需要继续进入循环
            int sum = carry;            // sum = l1 + l2 + carry
            // 相面两个分支不进入时表示已经遍历完链表，已经补 0 相加
            if (l1 != nullptr) {
                sum += l1->val;
                l1 = l1->next;
            }
            if (l2 != nullptr) {
                sum += l2->val;
                l2 = l2->next;
            }

            curr->next = new ListNode(sum % 10);
            curr = curr->next;
            carry = sum % 10;
        }
        return dummy.next;
    }
};