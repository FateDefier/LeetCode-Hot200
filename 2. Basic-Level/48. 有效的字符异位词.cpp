#include <string>
using namespace std;

// 思路：分别遍历 s 和 t，遍历 s 时对 cnt ++，遍历 t 时 --，如果对应 cnt 数组所有值为 0，即为异位词
class Solution {
public:
    bool isAnagram(string s, string t) {
        if (s.length() != t.length()) return false;

        int cnt[26] = {0};
        // 遍历 s 对 cnt ++
        for (char &c : s) {
            ++cnt[c - 'a'];
        }
        // 遍历 t 对 cnt --
        for (char &c : t) {
            --cnt[c - 'a'];
            // 剪枝
            // if (cnt[c - 'a'] < 0) return false;
        }
        for (int i = 0; i < 26; ++i) {
            if (cnt[i] != 0) return false;
        }
        return true;
    }
};