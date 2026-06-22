#include <unordered_map>
using namespace std;

class LFUCache {
private:
    struct Node {
        int key, val;
        Node *prev, *next;
        Node(int k, int v) : key(k), val(v), prev{nullptr}, next(nullptr) {}
    };
    int cap;
    unordered_map<int, 
public:
    LFUCache (int capacity) {

    }

    int get(int key) {

    }

    void put(int key, int value) {

    }
};