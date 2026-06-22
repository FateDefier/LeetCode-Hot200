#include <unordered_set>
using namespace std;

// 错误解法：思路很容易想到递归
class Solution {
private:
    int isHappyInternal(int n) {
        // 递归基
        if (n == 1) return true;
        
        int m = 0;
        while (n / 10 > 0) {
            m += (n % 10) * (n % 10);
            n /= 10;
        }

        isHappyInternal(m);
        return false;
    }
public:
    bool isHappy(int n) {
        return isHappyInternal(n);
    }
};

// 解法 1 -- 哈希表检测循环终止，碰到已经出现过的元素退出循环，时间 O(n)
class Solution {
private:
    int next(int n) {
        int sum = 0;
        while (n > 0) {
            int digit = n % 10;
            sum += digit * digit;
            n /= 10;
        }
        return sum;
    }
public:
    bool isHappy(int n) {
        unordered_set<int> seen;        // 已经出现过的元素
        while (n != 1 && seen.find(n) == seen.end()) {
            seen.insert(n);
            n = next(n);
        }
        return n == 1;      // 不用单独判断 1，在这里统一判断
    }
};

// 解法 2：快慢指针（Floyd 判圈法）-- 空间 O(1) -- 最优解法
class Solution {
private:
    int next(int n) {
        int sum = 0;
        while (n > 0) {
            int digit = n % 10;
            sum += digit * digit;
            n /= 10;
        }
        return sum;
    }
public:
    bool isHappy(int n) {
        int slow = n;
        int fast = next(n);
        // fast = 1，循环结束，是快乐数，slow == fast 有环
        while (fast != 1 && slow != fast) {
            slow = next(slow);          // 慢指针每次走一步
            fast = next(next(fast));    // 快指针每次走两步
        }
        return fast == 1;
    }
};