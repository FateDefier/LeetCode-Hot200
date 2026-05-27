#include <iostream>
#include <stack>
using namespace std;

class MyQueue {
private:
    stack<int> in;
    stack<int> out;             // 要用到队头时才把 in 中的元素倒序放入 out
public:
    MyQueue() {

    }

    void push(int x) {
        in.push(x);
    }

    int pop() {
        if (out.empty()) {              // out 为空时倒转
            while (!in.empty()) {
                out.push(in.top());
                in.pop();               // 注意，进入 out 的元素记得出 in
            }
        }
        int val = out.top();
        out.pop();
        return val;
    }

    int peek() {
        if (out.empty()) {
            while (!in.empty()) {
                out.push(in.top());
                in.pop();
            }
        }
        return out.top();
    }

    bool empty() {                  // 队判空时要检查两个栈：因为 push 时 in 中有元素而 out 为空，pop 时 out 中有元素而 in 为空
        return in.empty() && out.empty();
    }
};