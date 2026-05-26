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

// stack
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

// 最优版本：但较难理解 -- 差值栈+int min
class MinStack {
private:
    stack<long long> diffstack;         // 用 long long 防止 int 减法溢出(如 INT_MAX - (-1)) -- 存 val - 当前最小值的 差值
    long long min;                      // 当前最小值
public:
    MinStack() {            
        
    }

    void push(int val) {
        long long v = val;              // 类型转换
        if (diffstack.empty()) {
            min = v;
            diffstack.push(0LL);        // 第一个元素差值为 0(long long类型)
        } else {
            diffstack.push(v - min);
            if (v < min) {
                min = v;
            }
        }
    }

    void pop() {
        long long diff = diffstack.top();           // diff = val(当前最小) - min(上一个最小)
        diffstack.pop();
        if (diff < 0) {
            // 如果栈顶元素即是“创造了新低”的元素(存差值)，则出栈需更新min到上一个最小值
            min = min - diff;
        }
        // diff > 0 则说明栈顶元素不是“创造了新低”的元素
    }

    int top() {
        long long diff = diffstack.top();
        if (diff < 0) {
            return (int)min;            // 注意类型转换
        } else {
            return (int)(min + diff);
        }
    }

    int getMin() {
        return min;
    }

    // 自动析构
};