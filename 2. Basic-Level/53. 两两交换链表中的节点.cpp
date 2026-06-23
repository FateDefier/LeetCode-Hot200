struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *n) : val(x), next(n) {}
};

// 思路一：每两个递归
class Solution {
public:
    ListNode* swapPairs(ListNode *head) {
        // 递归基
        if (head == nullptr || head->next == nullptr) return head;

        // 交换当前head对应的两个
        ListNode *second = head->next;      // 新的头
        ListNode *newHead = second->next;   // 下一组的头
        second->next = head;                // swap
        // 交换之后的所有
        head->next = swapPairs(newHead);
        return second;
    }
};

// 思路二：迭代法：虚拟头结点 + 三指针
class Solution {
public:
    ListNode* swapPairs(ListNode *head) {
        ListNode *dummy = new ListNode(0, head);
        ListNode *prev = dummy;    // prev 为待交换的两个结点的钱去，便于进行交换

        while (prev->next != nullptr && prev->next->next != nullptr) {
            ListNode *first = prev->next;
            ListNode *second = first->next;

            // 交换，三行代码不能交换
            prev->next = second;
            first->next = second->next;
            second->next = first;

            // 更新 prev
            prev = first;
        }
        return dummy->next;
    }
};