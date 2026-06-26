#include <unordered_set>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// 思路 1：快慢指针
class Solution {
public:
    ListNode* detectCycle(ListNode *head) {
        ListNode *slow = head;
        ListNode *fast = head;

        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;

            if (slow == fast) {
                // 有环，找入口
                ListNode *ptr = head;
                while (ptr != slow) {
                    ptr = ptr->next;
                    slow = slow->next;
                }
                return ptr;
            }
        }
        return nullptr;
    }
};

// 思路 2：哈希表
class Solution {
public:
    ListNode* detectCycle(ListNode *head) {
        std::unordered_set<ListNode*> st;
        ListNode *curr = head;
        while (curr != nullptr) {
            if (st.find(curr) != st.end()) {
                // 找到环入口
                return curr;
            }
            st.insert(curr);
            curr = curr->next;
        }
        return nullptr;
    }
};