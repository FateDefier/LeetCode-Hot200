#include <iostream>
#include <string>
#include <stack>
using namespace std;

class Solution {
public:
    bool isValid(string s) {
        stack<char> left;
        for (char ch : s) {
            if (ch == '(' || ch == '[' || ch == '{') {
                left.push(ch);
                continue;                   // 退出本次循环，避免下面不必要的判断
            } 
            if (ch == ')' || ch == ']' || ch == '}') {
                // 注意，这里栈若为空，直接返回
                if (left.empty()) return false;
                char top = left.top();
                left.pop();
                if ((ch == ')' && top == '(') || (ch == ']' && top == '[') || (ch == '}' && top == '{')) {
                    continue;
                } else {
                    return false;
                }
            }
        }
        return left.empty();            // 注意:栈空才合法
    }
};