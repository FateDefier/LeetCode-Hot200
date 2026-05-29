#include <iostream>
#include <vector>
using namespace std;

// =========== 1.递归解法 ===========：
class Solution {
public:
    int fib(int n) {
        if (n <= 1) {
            return n;
        } else {
            return fib(n - 1) + fib(n - 2);
        }
    }
};

// =========== 2.记忆化搜索 ===========：
class Solution {
private:
    int fib_memo(int n, vector<int> &memo) {
        if (n <= 1) return n;
        // 如果已经计算过 memo[n]，直接返回
        if (memo[n] != -1) return memo[n];      
        // 没有计算 memo[n] 则递归计算
        memo[n] = fib_memo(n - 1, memo) + fib_memo(n - 2, memo);
        return memo[n];
    }
public:
    int fib(int n) {
        vector<int> memo(n + 1, -1);
        return fib_memo(n, memo);
    }
};

// =========== 3.迭代 DP(朴素) ===========
class Solution {
public:
    int fib(int n) {
        if (n <= 1) return n;
        vector<int> dp(n + 1);
        dp[0] = 0;
        dp[1] = 1;
        for (int i = 2; i <= n; ++i) {
            dp[i] = dp[i - 1] + dp[i - 2];
        }
        return dp[n];
    }
};

// 迭代 DP 空间优化版本/滚动数组优化
class Solution {
public:
    int fib(int n) {
        if (n <= 1) return n;
        int pre2 = 0;               // 前两个位置
        int pre1 = 1;               // 前一个位置
        int curr;
        for (int i = 2; i <= n; ++i) {
            curr = pre2 + pre1;
            pre2 = pre1;
            pre1 = curr;
        }
        return curr;
    }
};

// =========== 4.矩阵快速幂 矩阵快速幂
class Solution {
private:
    // 2x2 矩阵乘法
    vector<vector<long long>> multiply(const vector<vector<long long>> &A, const vector<vector<long long>> &B) {
        return {
            {   // 第一行
                A[0][0] * B[0][0] + A[0][1] * B[1][0],      // 第一行第一列
                A[0][0] * B[0][1] + A[0][1] * B[1][1]       // 第一行第二列
            }, 
            {   // 第二行
                A[1][0] * B[0][0] + A[1][1] * B[1][0],      // 第二行第一列
                A[1][0] * B[0][1] + A[1][1] * B[1][1]       // 第二行第二列
            }
        };
    }
    // 矩阵快速幂 -- 位运算实现
    vector<vector<long long>> matrixPower(const vector<vector<long long>> &M, int exp) {
        vector<vector<long long>> res = {{1, 0}, {0, 1}};
        vector<vector<long long>> base = M;
        while (exp > 0) {
            // 指数为奇数，提前面的 M
            if (exp & 1) {          // 等价于 exp % 2 == 0
                res = multiply(res, base);
            }
            // 指数为偶数次，进行 M = M^2
            base = multiply(base, base);
            // 位运算更新 exp
            exp >>= 1;              // 右移一位，等价于 exp %= 2;
        }
        return res;
    }
public:
    int fib(int n) {
        if (n <= 1) return n;
        vector<vector<long long>> M = {{1, 1}, {1, 0}};
        // M^(n-1) 的左上角元素即为 F(n)
        vector<vector<long long>> power = matrixPower(M, n - 1);
        return static_cast<int>(power[0][0]);
    }
};
