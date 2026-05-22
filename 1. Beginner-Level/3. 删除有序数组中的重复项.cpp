#include <vector>
using namespace std;

// 手动实现erase功能，时间复杂度 -- O(n^3)，空间复杂度 -- O(n)(手动维护 len)
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if (nums.empty()) return 0;
        int len = nums.size();                          // 手动维护长度
        for (int i = 0; i < len - 1; ++i) {
            for (int j = i + 1; j < len;) {
                if (nums[i] == nums[j]) {
                    // 前移
                    for (int k = j; k < len - 1; ++k) {
                        nums[k] = nums[k + 1];
                    }
                    // --nums.size();                  // 错误，nums.size() 由 C++ 的vector内部维护，不能直接修改，需手动维护长度
                    --len;
                    // j 不递增，继续检查当前 j 位置的新元素
                } else {
                    ++j;
                }
            }
        }
        return len;
    }
};

// erase：内层的批量前移被压缩成单次 memmove 调用 + 一次 size 自减。C++ 层面的循环开销显著更低
// 时间复杂度 -- O(n^2), 空间复杂度 O(1)(vector内部维护长度)
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if (nums.empty()) return 0;
        for (auto it = nums.begin(); it != nums.end(); ++it) {
            for (auto jt = it + 1; jt != nums.end();) {// 错误 jt = ++it，会改变外层 it 的值，先前置自增it，再赋值给 jt，应改为 jt = it + 1
                if (*it == *jt) {
                    jt = nums.erase(jt);
                } else {
                    ++jt;
                }
            }
        }
        return nums.size();
    }
};

// 快慢指针（覆盖式）
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if (nums.empty()) return 0;
        int slow = 0;                                       // 慢指针：指向已去重部分的最后一个元素
        for (int fast = 1; fast < nums.size(); ++fast) {    // 快指针：不断寻找新的元素加到slow的下一位置
            if (nums[slow] != nums[fast]) {                 // 找到新元素，则加到slow的下一位置  
                ++slow;
                nums[slow] = nums[fast];
            }
        }
        return slow + 1;                                    // slow 为下标，最终去重部分元素总数应该是 index + 1
    }
};