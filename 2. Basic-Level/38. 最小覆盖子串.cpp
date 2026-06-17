#include <string>
#include <climits>
using namespace std;

// window 要包含个数相同的 t 中的所有字符，且可以多出其他字符
class Solution {
private:
    // 把字母映射到 0 ~ 51 下标
    int index(char c) {
        return (c >= 'a') ? (c - 'a') : (c - 'A' + 26);
    }
public:
    string minWindow(string s, string t) {
        if (s.empty() || t.empty()) return "";
        int m = s.length(), n = t.length();
        if (m < n) return "";

        int diff[52] = {0};             // 0~25大写字母，26~51小写字母 = window - need
        int satisfied = 0;              // 当前窗口已满足的字符个数
        int totalNeed = 0;              // t 需要的字符个数

        // 初始化 diff 和 totalNeed -- 只记录 need
        for (char c : t) {
            if (diff[index(c)] == 0) ++totalNeed;       // 只记录字符种类数，故而判断 diff[index(c)] 是否为 0
            --diff[index(c)];
        }

        int minLen = INT_MAX, minLeft = 0;           // 初始化最短子串窗口长度及最短窗口子串的起始位置(左指针)
        int left = 0;                   // 左指针

        for (int right = 0; right < m; ++right) {
            // 右边界字符不断进入窗口，直至 satisfied == totalNeed
            if (diff[index(s[right])] == -1) ++satisfied;
            ++diff[index(s[right])];

            // 窗口包含 t 后，尝试缩小左边界以满足最短窗口
            while (satisfied == totalNeed) {
                // 更新长度
                if (right - left + 1 < minLen) {
                    minLen = right - left + 1;
                    minLeft = left;         // 关键点
                }
                // 缩小边界并更新 satisfied
                --diff[index(s[left])];
                if (diff[index(s[left])] == -1) --satisfied;
                ++left;
            }
        }
        return minLen == INT_MAX ? "" : s.substr(minLeft, minLen);
    }
};