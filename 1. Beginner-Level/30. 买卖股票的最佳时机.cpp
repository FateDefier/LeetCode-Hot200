#include <iostream>
#include <vector>
using namespace std;

// 暴力枚举
class Solution {
public:
    int maxProfit(vector<int> prices) {
        if (prices.empty()) return 0;
        int max = 0;
        for (int i = 0; i < prices.size() - 1; ++i) {
            for (int j = i + 1; j < prices.size(); ++j) {
                if (max < prices[j] - prices[i]) {
                    max = prices[j] - prices[i];
                }
            }
        }
        return max;
    }
};

// 贪心优化：
class Solution {
public:
    int maxProfit(vector<int> prices) {
        if (prices.empty()) return 0;
        int min = prices[0], maxProfit = 0;
        for (int i = 1; i < prices.size(); ++i) {
            maxProfit = maxProfit < (prices[i] - min) ? (prices[i] - min) : maxProfit;
            min = min > prices[i] ? prices[i] : min;
        }
        return maxProfit;
    }
};

// DP / Kadane 变体
class Solution {
public:
    int maxProfit(vector<int> prices) {
        if (prices.empty()) return 0;
        int currProfit = 0;         // 当前累计利润
        int maxProfit = 0;
        for (int i = 1; i < prices.size(); ++i) {
            currProfit = (currProfit + prices[i] - prices[i - 1]) > 0 ? (currProfit + prices[i] - prices[i - 1]) : 0;
            maxProfit = maxProfit > currProfit ? maxProfit : currProfit;
        }
        return maxProfit;
    }
};