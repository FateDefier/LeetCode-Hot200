#include <iostream>
#include <queue>
using namespace std;

// 错误解法：
class MyStack {
private:
    queue<int> in;
    queue<int> out;

    void in2out () {
        if (out.empty()) {
            while (!in.empty()) {
                out.push(in.front());
                in.pop();
            }
        }
    }
public:
    MyStack() {

    }

    void push(int x) {
        in.push(x);
    }

    int pop() {
        in2out();
        int val = out.front();
        out.pop();
        return val;
    }

    int top() {
        in2out();
        return out.front();
    }

    bool empty() {
        return in.empty() && out.empty();
    }
};

// 两个队列实现：
class MyStack {
private:
    queue<int> q1, q2;          // q1 作主队列，q2 用于记录最后进的元素在 q1 的基础上重新构造 q1
public:
    MyStack() {

    }

    void push(int x) {
        q2.push(x);
        while (!q1.empty()) {
            q2.push(q1.front());
            q1.pop();
        }
        swap(q1, q2);           // q1 为主队列(即栈)
    }

    int pop() {
        int val = q1.front();
        q1.pop();
        return val;
    }

    int top() {
        return q1.front();
    }

    bool empty() {
        return q1.empty();
    }
};

// 一个队列实现：
class MyStack {
private:
    queue<int> q;
public:
    MyStack() {

    }

    void push(int x) {
        q.push(x);
        // 旋转：将 x 之前的 栈序列 排到 x 后面
        for (int i = 0; i < q.size() - 1; ++i) {        // 注意循环结束条件，保证循环执行 n - 1 次，x 不会移动
            q.push(q.front());
            q.pop();
        }
    }

    int pop() {
        int val = q.front();
        q.pop();
        return val;
    }

    int top() {
        return q.front();
    }

    bool empty() {
        return q.empty();
    }
};