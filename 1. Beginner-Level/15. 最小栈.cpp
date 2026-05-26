#include <iostream>
#include <vector>
#include <stack>
#include <climits>
using namespace std;

class MinStack {
private:
    vector<int> stack;      // 用数组表示栈 -- 非尾部插入删除开销大
    vector<int> mins;       // 所有的最小值，有重复
public:
    MinStack() {            // 默认构造

    }

    void push(int val) {
        stack.push_back(val);
        if (mins.empty()) {
            mins.push_back(val);
        } else {
            mins.push_back(mins.back() > val ? val : mins.back());
        }
    }

    void pop() {
        if (!stack.empty()) {
            stack.pop_back();
            mins.pop_back();
        }
    }

    int top() {
        return stack.back();
    }

    int getMin() {
        return mins.back();
    }

    // 自动析构
};

class MinStack {
private:
    stack<int> s;
    stack<int> mins;        // 最小值栈
public:
    MinStack() {            // 默认构造
        mins.push(INT_MAX); // mins push 时不用判断栈空 -- 哨兵
    }

    void push(int val) {
        s.push(val);
        mins.push(val < mins.top() ? val : mins.top());
    }

    void pop() {
        s.pop();
        mins.pop();
    }

    int top() {
        return s.top();
    }

    int getMin() {
        return mins.top();
    }

    // 自动析构
};