#include <iostream>
#include <vector>
#include <string>
using namespace std;

// 思路 1：题目明确 s 和 p 只含小写字母，且异位词很容易想到哈希表存 p 中出现过的元素用于跟 s 进行比较，找 p 的异位词，只需要固定滑动窗口的大小等于 p.length()，判断该窗口是否出现了 p 中的所有字符
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        if (s.length() < p.length()) return {};

        vector<int> res;
        vector<int> pos(26, -1);        // 哈希表，-1 表示在 p 中不存在，1 表示存在
        for (int i = 0; i < p.length(); ++i) {
            char c = p[i];
            pos[(int)c - (int)'a'] = 1;
        }
        for (int L = 0; L <= s.length() - p.length(); ++L) {
            int R = L + p.length() - 1;
            int i = L;              // 遍历窗口，看滑动窗口元素是否在 p 中
            while (i <= R) {
                if (pos[(int)s[i] - (int)'a'] == -1) {
                    break;
                } else {
                    ++i;
                    continue;
                }
            }
            if (i == R) {
                res.push_back(L);
            }
        }
        return res;
    }
};

// 正确解法：两个哈希表分别存 p 中字符出现的频率和滑动窗口出现的频率，二者相等即为 Anagram(异位词)
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        vector<int> res;
        int m = s.length(), n = p.length();
        if (m < n) return {};

        // need：p 中各字符出现的频率，即需要窗口出现的次数
        // window: 窗口各字符出现的频率
        vector<int> need(26, 0), window(26, 0);
        // 初始化 need
        for (auto c : p) ++need[c - 'a'];
        // 初始化 window(长度 = p.length())
        for (int i = 0; i < n; ++i) ++window[s[i] - 'a'];
        if (need == window) res.push_back(0);

        // 滑动窗口：左右边界动态移动，移出左边界，移入右边界，保证窗口大小不变
        for (int i = 0; i < m - n; ++i) {
            --window[s[i] - 'a'];           // 移出左边界
            ++window[s[i + n] - 'a'];       // 移入右边界
            if (need == window) res.push_back(i + 1);   // 注意这里是 i + 1，因为是先移动窗口，再判断
        }
        return res;
    }
};

// 优化解法：优化空间(need 和 window 哈希表)和时间(need == window)
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        int m = s.length(), n = p.length();
        if (m < n) return {};

        vector<int> diff(26, 0);    // 记录窗口与 p 之间有多少种字母频率不匹配
        int nonZero = 0;            // 记录 diff 中的非零元素，即不匹配的字符个数

        // 初始化 diff，先加 s[n](s 前 n 个元素)，再减 p[n]
        for (int i = 0; i < n; ++i) {
            ++diff[s[i] - 'a'];
            --diff[p[i] - 'a'];
        }
        // 初始化 nonZero
        for (int i = 0; i < 26; ++i) {
            if (diff[i] != 0) ++nonZero;
        }

        vector<int> res;
        if (nonZero == 0) res.push_back(0);

        // 滑动窗口
        for (int i = 0; i < m - n; ++i) {
            // 移出左边界
            int out = s[i] - 'a';
            // 如果当前左边字符为匹配，移出后就变为不匹配了
            if (diff[out] == 0) ++nonZero;
            // 如果当前左边字符为不匹配（前面的if判断了匹配的，到这就是不匹配），移出左边界后可能变成匹配
            --diff[out];
            if (diff[out] == 0) --nonZero;
            
            // 移入右边界
            int in = s[i + n] - 'a';
            if (diff[in] == 0) ++nonZero;
            ++diff[in];
            if (diff[in] == 0) --nonZero;

            if (nonZero == 0) res.push_back(i + 1);
        }
        return res;
    }
};