#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

// 思路：固定左指针到一个非零元素并进栈，移动右指针，直到右指针继续右移是值递减时右指针停止移动并进栈，根据二指针之间各个 height 与 min(height[L], height[R]) 的差值求和，然后更新左指针到右指针的位置（通过栈），右指针从左指针的下一位置开始遍历
// 下面代码没写完，不知道怎么继续写
class Solution {
public:
    int trap(vector<int>& height) {
        int sum = 0;
        stack<int> s;               // index
        int n = height.size();
        int L = 0;
        while (L < n - 1) {
            if (height[L] > 0) continue;
            s.push(L);
            int R = L + 1;
            while (R < n - 1) {
                if (height[R] >= height[L]) {
                    s.push(R);
                }
            }
        }
    }
};

// 双指针 -- 关键点：左右是否有“墙”，一列一列地填，从最左边开始
class Solution {
public:
    int trap(vector<int>& height) {
        int n = height.size();
        if (n < 3) return 0;       // 没有 3 个柱子不能形成水坑
        int sum = 0;

        int LeftMax = 0, RightMax = 0;  // 左、右侧所见最高柱子
        int L = 0, R = n - 1;
        while (L < R) {
            LeftMax = max(LeftMax, height[L]);
            RightMax = max(RightMax, height[R]);

            if (LeftMax < RightMax) {
                sum += LeftMax - height[L];
                ++L;
            } else {
                sum += RightMax - height[R];
                --R;
            }
        }
        return sum;
    }
};

// 单调栈 -- 一行一行地填，从 height = 1 这一行开始填
class Solution {
public:
    int trap(vector<int>& height) {
        int n = height.size();
        if (n < 3) return 0;
        int sum = 0;

        stack<int> s;           // 递减存
        for (int i = 0; i < n; ++i) {
            // 当前高度 > 栈顶元素高度 --> 说明栈顶元素位置处为坑
            while (!s.empty() && height[i] > height[s.top()]) {
                int bottom = s.top();       // 当前坑的高度，用于计算填水的高度     
                s.pop();                    // 取栈顶元素后要出栈
                if (s.empty()) break;       // 下面要取栈顶的值，要判断上面的 bottom 出栈后栈内是否还有元素
                int left = s.top();         // 左墙 left, 右墙 i
                int h = min(height[left], height[i]) - height[bottom];
                int w = i - left - 1;       // 坑内所有元素的填水高度必定相同，因为是一层一层填水的
                sum += w * h;
            }
            s.push(i);
        }
        return sum;
    }
};