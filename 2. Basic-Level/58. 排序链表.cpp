struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// 思路 1：递归 + 快慢指针 + 归并排序（自顶向下）
class Solution {
private:
    // 分左右链表
    ListNode* findMiddleAndCut(ListNode *head) {
        if (head == nullptr || head->next == nullptr) return head;
        ListNode *prev = nullptr, *slow = head, *fast = head;
        // 找中点
        while (fast != nullptr && fast->next != nullptr) {
            prev = slow;
            slow = slow->next;
            fast = fast->next->next;
        }
        // 截断，分左右链表
        prev->next = nullptr;
        return slow;        // 返回右半部分的头
    }
    // 合并有序链表
    ListNode* mergeSortedList(ListNode *l1, ListNode *l2) {
        ListNode dummy(0);
        ListNode *curr = &dummy;

        while (l1 != nullptr && l2 != nullptr) {
            if (l1->val < l2->val) {
                curr->next = l1;
                l1 = l1->next;
            } else {
                curr->next = l2;
                l2 = l2->next;
            }
            curr = curr->next;
        }
        // 连接剩余部分
        curr->next = (l1 != nullptr) ? l1 : l2;
        return dummy.next;

    }
public:
    ListNode* sortList(ListNode *head) {
        // 递归基
        if (!head || !head->next) return head;

        ListNode *mid = findMiddleAndCut(head);
        ListNode *left = sortList(head);
        ListNode *right = sortList(mid);

        return mergeSortedList(left, right);
    }
};

// 思路 2：迭代（自底向上）
class Solution {
private:
    // 合并有序链表
    ListNode* mergeSortedList(ListNode *l1, ListNode *l2) {
        ListNode dummy(0);
        ListNode *curr = &dummy;

        while (l1 != nullptr && l2 != nullptr) {
            if (l1->val < l2->val) {
                curr->next = l1;
                l1 = l1->next;
            } else {
                curr->next = l2;
                l2 = l2->next;
            }
            curr = curr->next;
        }
        // 连接剩余部分
        curr->next = (l1 != nullptr) ? l1 : l2;
        return dummy.next;

    }
public:
    ListNode* sortList(ListNode *head) {
        if (!head || !head->next) return head;

        // 计算链表长度
        int len = 0;
        ListNode *curr = head;
        while (curr) {
            ++len;
            curr = curr->next;
        }

        ListNode dummy(0);
        dummy.next = head;

        // 步长逐步翻倍，直至 >= len
        for (int step = 1; step < len; step *= 2) {
            ListNode *prev = &dummy;
            curr = dummy.next;

            // 按照 step 分出两段并合并，直至整个链表遍历结束
            while (curr) {
                ListNode *h1 = curr;        // 第一段的头
                ListNode *tail1 = curr;     // 第一段的尾
                // 分第一段长度为 step 的链表
                for (int i = 1; i < step && tail1->next; ++i) {
                    tail1 = tail1->next;
                }
                ListNode *h2 = tail1->next;         // 第二段的头
                tail1->next = nullptr;      // 截断第一段

                // 分第二段长度为 step 的链表
                ListNode *tail2 = h2;     // 第二段的尾
                // 如果第二段存在
                if (tail2) {
                    for (int i = 1; i < step && tail2->next; ++i) {
                    tail2 = tail2->next;
                    }
                    curr = tail2->next;
                    tail2->next = nullptr;
                } else {
                    curr = nullptr;
                }

                // 合并两段有序链表(step从 1 开始对应的 step=2的链表必定有序，以此类推)
                prev->next = mergeSortedList(h1, h2);

                // 更新 prev 到末尾
                while (prev->next) prev = prev->next;
            }
        }
        return dummy.next;
    }
};