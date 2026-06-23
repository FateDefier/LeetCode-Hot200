struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *n) : val(x), next(n) {}
};

// 思路 1：第一次遍历的长度，后利用 prev 和 curr 删除结点
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode *head, int n) {
        // 求长度
        int len = 0;
        ListNode *curr = head;
        while (curr != nullptr) {
            ++len;
            curr = curr->next;
        }

        int cnt = 0;
        ListNode *dummy = new ListNode(0, head);    // 防止只有一个结点需要删除头结点的情况
        ListNode *prev = dummy;     // 永远指向 curr 的前驱
        curr = head;
        while (curr != nullptr && cnt != len - n) {
            ++cnt;
            curr = curr->next;
            prev = prev->next;
        }
        if (curr != nullptr && cnt == len - n) {
            prev->next = curr->next;
        }
        return dummy->next;
    }
};

// 思路 2：快慢指针：快指针先走 n+1 步，然后快慢指针同时走，当快指针走到 nullptr 时，已经走了 length，而慢指针走了 length - (n + 1)，即慢指针指向待删除结点的前驱，总共只有 fast 走完了数组，进行了一次遍历，时间复杂度 O(n)
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode *head, int n) {
        // 不用 new，因为栈分配比堆分配(new)更优
        ListNode dummy(0);      // 防止要删除的结点是头结点的情况
        dummy.next = head;
        ListNode *slow = &dummy;
        ListNode *fast = &dummy;

        // 快指针走 (n + 1) 步
        for (int i = 0; i <= n; ++i) {
            fast = fast->next;
        }

        // 快慢指针一起走
        while (fast != nullptr) {
            slow = slow->next;
            fast = fast->next;
        }

        slow->next = slow->next->next;
        return dummy.next;
    }
};