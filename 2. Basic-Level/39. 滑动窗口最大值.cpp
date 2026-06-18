#include <vector>
#include <deque>
using namespace std;

// 思路：手动控制左指针，right = left + k，定义一个函数找两个指针之间数组的最大值
class Solution {
private:
    int maxValue(vector<int>& nums, int L, int R) {
        int max = nums[L];
        for (int i = L + 1; i <= R; ++i) {
            if (nums[i] > max) {
                max = nums[i];
            }
        }
        return max;
    }
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        int n = nums.size();
        if (n < k) return {};

        vector<int> res;
        for (int left = 0; left <= n - k; ++left) {
            int right = left + k - 1;
            res.push_back(maxValue(nums, left, right));
        }
        return res;
    }
};

// 正确解法：单调双端队列
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        int n = nums.size();
        if (n < k) return {};

        deque<int> dq;              // 存窗口内元素的递减序列(队头最大)，存 index
        vector<int> res;

        for (int i = 0; i < n; ++i) {
            // 1. 清理队头的过期元素：队头出队(有多个过期清理多个)
            while (!dq.empty() && dq.front() <= i - k) {
                dq.pop_front();
            }
            // 2. 清除队尾 <= nums[i] 的元素，它们 <= nums[i]，不可能为窗口的最大值(因为 nums[i] 已经 >= 它们)，这里比较时是值比较
            while (!dq.empty() && nums[dq.back()] <= nums[i]) {
                dq.pop_back();
            }
            // 3. i 入队 -- 存下标
            dq.push_back(i);
            // 4. 判断是否达到窗口大小，达到则加入 res
            if (i + 1 >= k) res.push_back(nums[dq.front()]);    // 注意，dq 存的是下标，加入 res 的是 value
        }
        return res;
    }
};

// 优化解法
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        int n = nums.size();
        if (n == 0 || k == 0) return {};
        if (n < k) return {};

        int dq[100000];          // 1 <= nums.length <= 10^5，存下标，存值在有重复值时无法判断删除哪一个保留在窗口的元素
        // int* dq = new int[n];       // 极致的空间优化：动态分配存储空间，但是 new 比直接定义数组时间效率更低，但是可以避免栈溢出的问题
        int head = 0, tail = 0;     // dq 的首尾指针，head 指向队头元素，tail 指向队尾元素的下一位置

        vector<int> res;
        res.reserve(n - k + 1);     // 极致的空间优化：预留空间，避免 push_back 扩容

        for (int i = 0; i < n; ++i) {
            // 1. 删除队头过期元素(逻辑删除)
            while (tail > head && dq[head] <= i - k) {
                ++head;
            }
            // 2. 剔除队尾 <= nums[i] 的元素(逻辑删除)
            while (tail > head && nums[dq[tail - 1]] <= nums[i]) {
                --tail;
            }
            // 3. 加入 i -- 存下标
            dq[tail++] = i;
            // 4. 判断窗口是否形成
            if (i + 1 >= k) {
                res.push_back(nums[dq[head]]);
            }
        }
        return res;
    }
};