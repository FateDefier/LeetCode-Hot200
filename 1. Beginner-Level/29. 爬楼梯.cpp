#include <iostream>
using namespace std;

// 思路：讨论 2 的不同个数 + 相应个数的 2 怎么在 n 中排列组合
class Solution {
private:
    // 辗转相除法/欧几里得算法 求 最大公约数用于之后组合数的约分(Greatest Common Divisor)
    long long gcd(long long a, long long b) {       // a >= b
        if (b != 0) {
            long long tmp = b;
            b = a % b;
            a = tmp;
        }
        return a;
    }
    // long long + 求组合数 C(n, k) 时边乘边除最大公约数约分的技巧防止溢出
    long long combination(int n, int k) {
        if (k < 0 || k > n) return 0;
        // 利用对称性 C(n, k) = C(n, n - k) 挑选 k 和 n - k 的较小值减少循环次数
        if (k > n - k) k = n - k;
        long long numerator = 1;        // 分子
        long long denominator = 1;      // 分母
        for (int i = k; i >= 1; ++i) {
            numerator *= i;
            denominator *= (n - k + i);
            // 核心优化：边乘边约分
            long long g = gcd(denominator, numerator);      // 注意 a >= b，这里的传值顺序要对应
            numerator /= g;
            denominator /= g;
        }
        return numerator / denominator;
    }
public:
    int climbStairs(int n) {
        int totalWay = 0;
        for (int i = 0; i <= n / 2; ++i) {
            totalWay += combination(n, i);
        }
        return totalWay;
    }
};

// 滚动数组
class Solution {
public:
    int climbStairs(int n) {
        if (n <= 2) return n;
        int prev = 1;           // 与 fibonacci 不同的是前两个值
        int curr = 2;    
        for (int i = 3; i <= n; ++i) {
            int next = prev + curr;
            prev = curr;
            curr = next;
        }
        return curr;
    }
};