struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// 思路 1：外层迭代 + 内部递归：前 k 个单独翻转，后递归翻转后面的
class Solution {
private:
    // 翻转前 k 个，返回翻转后的新头（即原来的尾节点）
    ListNode* reverseFirstK(ListNode *head, int k) {
        // 三指针翻转
        ListNode *prev = nullptr, *curr = head;
        while (k != 0) {
            ListNode *nxt = curr->next;
            // 翻转，curr反指prev
            curr->next = prev;
            // 更新 prev
            prev = curr;
            // 更新 curr
            curr = nxt;
            // 更新 k
            --k;
        }
        // head 现在是尾，指向之前尾部的下一个结点
        head->next = curr;
        return prev;            // prev 现在是新的头
    }
public:
    ListNode* reverseKGroup(ListNode *head, int k) {
        // 检查是否有 k 个元素
        ListNode *curr = head;
        int cnt = 0;
        while (curr != nullptr && cnt < k) {
            curr = curr->next;
            ++cnt;
        }
        if (cnt < k) return head;       // 不足 k 直接返回

        // 翻转前 k 个，注意，这里就得到了新的头了
        ListNode *newHead = reverseFirstK(head, k);

        // 递归翻转后续链表
        head->next = reverseKGroup(head->next, k);

        return newHead;
    }
};

// 解法 2：迭代法 -- O(1)空间
class Solution {
public:
    ListNode* reverseKGroup(ListNode *head, int k) {
        ListNode dummy(0);      // 虚拟头节点
        dummy.next = head;
        ListNode *prev = &dummy;

        while (true) {
            // 检查剩余结点是否有 k 个
            ListNode *check = prev;     // prev 为待翻转的 KGroup 的前驱
            int cnt = 0;
            while (check->next != nullptr && cnt < k) {
                check = check->next;
                ++cnt;
            }
            if (cnt < k) break;

            // 翻转
            ListNode *cur = prev->next;     // KGroup 的头
            ListNode *groupTail = cur;      // 翻转后变为尾，这里记录，用于后续保持链表连接完整
            ListNode *pre = nullptr;
            // 翻转 k 个
            for (int i = 0; i < k; ++i) {
                ListNode *nxt = cur->next;
                cur->next = pre;
                pre = cur;
                cur = nxt;
            }
            // 更新 KGroup 头尾连接正确
            prev->next = pre;               // 前驱连新头
            groupTail->next = cur;          // 尾部连下一个 KGroup 的头
            prev = groupTail;               // 更新 prev
        }
        return dummy.next;
    }
};