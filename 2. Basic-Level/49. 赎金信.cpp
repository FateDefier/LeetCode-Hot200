#include <string> 
using namespace std;

// 思路：手动哈希表 <char, frequency>，遍历两个字符串，一个加，一个减
class Solution {
public:
    bool canConstruct(string ransomNote, string magazine) {
        if (ransomNote.length() > magazine.length()) return false;

        int mp[26] = {0};       // 手动哈希表，<index, value> = <char, frequency>

        for (char &c : ransomNote) {
            ++mp[c - 'a'];
        }

        for (char &c : magazine) {
            --mp[c - 'a'];
        }

        for (int i = 0; i < 26; ++i) {
            if (mp[i] > 0) return false;
        }
        return true;
    }
};