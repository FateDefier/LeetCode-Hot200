#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// 思路 1：貌似字符串匹配和这个挺像的，而字符串匹配有两种解法：1.暴力/朴素解法；2.KMP 算法(next[j]数组)，如何应用
// 思路 2：滑动窗口(同向双指针) + 哈希表
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        // 用 vector 维护哈希表检查重复，存取之前的值出现的下标
        vector<int> pos(128, -1);      // 标准 ASCII 码 128 个字符，扩展一些特殊符号、制表符或带音标的字母后为 256 个，-1 表示之前没有出现过
        int left = 0, res = 0;

        for (int right = 0; right < s.length(); ++right) {
            char c = s[right];
            // 如果当前 right 所指字符之前出现过，则寻找无重复字符的下一个子串
            if (pos[c] >= left) {
                left = pos[c] + 1;
            }
            // 更新 c 的最新位置(right 所指)
            pos[c] = right;
            // 更新最长长度
            res = max(res, right - left + 1);
        }
        return res;
    }
};