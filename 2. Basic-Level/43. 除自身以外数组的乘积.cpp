#include <vector>
using namespace std;

// 思路 1：暴力双重循环
class Solution {
public: 
    vector<int> productExceptSelf(vector<int>& nums) {
        int n = nums.size();
        vector<int> ans(n, 1);

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i != j) {
                    ans[j] *= nums[i];
                }
            }
        }

        return ans;
    }
};

// 思路 2：前后缀乘积，用 preProduct 构造 ans 数组存前缀积，postProduct 单个变量存后缀积
class Solution {
public: 
    vector<int> productExceptSelf(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return {};
        vector<int> ans(n, 1);

        int preProduct = 1;
        for (int i = 0; i < n; ++i) {
            ans[i] *= preProduct;
            preProduct *= nums[i];
        }

        int postProduct = 1;
        for (int j = n - 1; j >= 0; --j) {
            ans[j] *= postProduct;
            postProduct *= nums[j];
        }

        return ans;
    }
};