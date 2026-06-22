#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

// 解法 1：对每个字符串排序后相同的即为异位词，用 pair(key, value)存，其中 key 为排序后的字符串，value 为排序前的串 -- O(m logm)(排序)
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        if (strs.empty()) return {};
        unordered_map<string, vector<string>> mp;
        mp.reserve(strs.size());        // reserve 避免 rehash，养成习惯

        // 排序并根据 key 存入 value
        for (string &s : strs) {        // -- O(n)
            string key = s;
            sort(key.begin(), key.end());   // O(m logm)
            mp[key].push_back(s);       // 根据 key 存 s(value)
        }

        // 遍历 mp，把所有 value(vector<string>)存入 res
        vector<vector<string>> res;
        res.reserve(mp.size());
        for (auto& p : mp) {        // 使用引用，避免不必要的拷贝开销
            res.push_back(p.second);
        }
        return res;
    }
};

// 解法 2：计数编码作为 key
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        if (strs.empty()) return {};
        unordered_map<string, vector<string>> mp;

        // 计数编码 "1#0#2#" 作为 key
        for (string &s : strs) {        // -- O(n)
            vector<int> cnt(26, 0);
            for (char &c : s) {         // -- O(m)
                ++cnt[c - 'a'];
            }
            // 编码
            string key;
            for (int i = 0; i < 26; ++i) {
                key += to_string(cnt[i]) + '#';
            }
            mp[key].push_back(s);
        }

        vector<vector<string>> res;
        for (auto &p : mp) {
            res.push_back(p.second);
        }
        return res;
    }
};

class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        if (strs.empty()) return {};
        unordered_map<string, vector<string>> mp;

        for (string &s : strs) {
            int cnt[26] = {0};
            for (char &c : s) ++cnt[c - 'a'];
            
            string key(26, 0);      // 关键优化：用定长字符数组作为 key，避免 to_string
            for (int i = 0; i < 26; ++i) {
                key[i] = static_cast<char>(cnt[i]);
            }
            mp[key].push_back(s);
        }

        vector<vector<string>> res;
        res.reserve(mp.size());

        for (auto &p : mp) {       
            res.push_back(p.second);
        }
        return res;
    }
};