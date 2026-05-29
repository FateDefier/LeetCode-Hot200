# Hello

## 1. 题号：1 -- 两数之和（简单）
### 1.1 待优化解法(自己写的)
```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        int n, m;
        for (int i = 0; i < nums.size(); ++i) {
            for (int j = i + 1; j < nums.size(); ++j) {
                if (nums[i] + nums[j] == target) {
                    return {i, j};          // 初始化列表，自动根据返回值推导返回值类型为 vector<int>
                }
            }
        }
        return {};
    }
};
```

- **思路**：分别用 `i` 和 `j` 遍历数组(**注意：`j` 从 `i + 1` 开始**)，二者相加为目标值，对应下标即为返回值;

- **错误点**：
1. **逻辑错误**：无；
2. **可优化**：**当前代码时间复杂度为 $O(n^2)$**，可以**通过哈希表优化为 $$O(n)$$**，用**空间换时间**;

### 1.2 正确解法及分析
代码如下：
```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> mp;             // value - index
        for (int i = 0; i < nums.size(); ++i) {
            int need = target - nums[i];
            if (mp.find(need) != mp.end()) {
                return {mp[need], i};
            }
            mp[nums[i]] = i;
        }
        return {};
    }
};
```

- **知识点(关键词)**：**哈希表**，**边找边存**

- **思路**：遍历链表时，把每次遍历过的 `index-value`(即 `i - nums[i]`) 对用哈希表按照 `value-index` 存储，这样，每次就只需用哈希表回溯查找前面是否有 `target - nums[i]`，找到后，哈希表存储的 `index` 可以通过哈希表的映射与当前 `i` 一起作为返回值，**关键点：边查边存**；

---

## 2. 题号：283 -- 移动零（简单）
### 2.1 待优化解法(自己写的)
```cpp
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int cur = 0;
        int rear = nums.size() - 1;
        while (cur < rear) {
            if (nums[cur] == 0) {       // 碰到 0，则 前移 + 最后一位置零
                for (int i = cur; i < rear; ++i) {
                    nums[i] = nums[i + 1];
                }
                nums[rear] = 0;
                --rear;
            } else {                    // 没碰到 0，则下一位
                ++cur;
            }
        }
    }
};
```

- **思路**：遇到 0，**前移** + **最后一位置零** -- 每次完成 $O(n)$ 级别的移动，共 n 次，时间复杂度 $O(n^2)$，空间复杂度O(1);

- **错误点**：
1. **逻辑错误**：无；
2. **可优化**：当前算法时间复杂度为 $O(n^2)$;

### 2.2 正确解法及分析
代码如下：
```cpp
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int slow = 0;                                           // 慢指针：指向下一个交换位置到前面的位置
        for (int fast = 0; fast < nums.size(); ++fast) {        // 快指针：指向下一个等待交换的非零元素
            if (slow != fast) {
                if (nums[fast] != 0) {
                    swap(nums[slow], nums[fast]);
                    ++slow;                                         // 更新 slow
                }
            }
        }
        // 循环结束后，slow 及其后面的位置自然都是 0
    }
};
```

- **知识点(关键词)**：**快慢指针**，**原地操作**

- **思路**：**慢指针指向下一个前面可交换的位置，快指针寻找后面可交换的非零元素**，时间复杂度降至 $O(n)$，循环结束后，slow 极其后面的位置自然都是 0；

---

## 3. 题号：26 -- 删除有序数组中的重复项（简单）
### 3.1 可优化解法 1(C 风格)
```cpp
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
```

- **思路**：**两次遍历检查是否有重复，最后一次遍历用于前移覆盖重复元素** -- 时间复杂度 $O(n^3)$;

- **注意**：
1. 最后要返回删除重复元素后数组的长度，所以要更新 `nums.size()` 的长度，但是对于 C 风格代码，因为 **`vector.size`() 由 C++ 内部维护，不对外暴露,不能直接 `--nums.size()`**；
2. **如果进行了元素前移，`j` 不用自增，因为之前位置的 j 对应的元素已经被覆盖了**；

### 3.2 可优化解法 2(迭代器，C++ 风格)
```cpp
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
```

- **思路**：`erase`：内层的批量前移被压缩成单次 memmove 调用 + 一次 size 自减。**C++ 层面的循环开销显著更低** -- 时间复杂度 $O(n)$，空间复杂度 $O(1)$(vector内部维护长度)；

- **注意**：`jt = it + 1` 不能写成 `jt = ++it`，因为 **`jt = ++it` 会改变 `it` 的指向**；

### 3.3 正确解法及分析
代码如下：
```cpp
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
```

- **知识点(关键词)**：**有序数组**(题目明确说明)，**快慢指针(覆盖式)**

- **思路**：**因为数组有序，所以重复元素必近邻**，只用让**慢指针指向最后一个非重复元素的位置，快指针寻找与前面不重复的第一个元素**即可 -- 时间复杂度 $O(n)$；

- **注意**：最后**返回移除后数组的新长度。不要求改变数组长度，只需保证前 k 个元素是去重后的结果。**，而 `slow` 指针指向最后一个非重复元素的位置，所以**返回的长度为 `slow + 1`**；

---

## 4. 题号：27 -- 移除元素（简单）
### 4.1 正确解法(自己写的，这次一次过，和上一次思路基本一致)
```cpp
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int slow = 0;
        for (int fast = 0; fast < nums.size(); ++fast) {
            if (nums[fast] != val) {
                nums[slow] = nums[fast];
                ++slow;
            }
        }
        return slow;
    }
};
```

- **关键词**：**快慢指针(覆盖式)**；

- **思路**：不等于val的值放在前半部分，只返回慢指针的计数即为不等于 val 的元素数量(k)（其余元素和 nums 的大小并不重要）;

- **注意**：这里的 **慢指针指向最后一位待改变的位置，所以其值即为不等于 val 的元素数量(k)**，所以最后返回 `slow` 即可；

---

## 5. 题号：88 -- 合并两个有序数组（简单）
### 5.1 错误解法(自己写的，错误的迭代其实用)
```cpp
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        auto it1 = nums1.begin(), it2 = nums2.begin();
        for(; it1 != nums1.end() && it2 != nums2.end(); ) {
            if (*it1 <= *it2) {
                ++it1;
            } else {
                nums1.insert(it1 - 1, *it2);        // 插入操作后，迭代器失效，下面的 ++it1 是未定义行为
                ++it1;
                ++it2;
            }
        }
        // 插入剩余部分, 只用检查 it1 是否到达末尾
        if (it1 == nums1.end()) {
                nums1.insert(it1, it2, nums2.end());
        }
    }
};
```

- **思路**：用迭代器**从第一个位置依次比较(数组有序)** `nums1` 和 `nums2` 各个元素的大小，从而把 `nums2` 每一个元素插入 `nums` ;

- **错误点**：当 `insert` 未导致重新分配时，**插入点之后的所有迭代器失效，插入点之前的迭代器保持有效**。由于 it1 指向的元素位于插入点（it1-1）之后，所以 it1 失效；之后 `++it1` 是未定义行为；


### 5.2 正确解法及分析
代码如下：
```cpp
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int i = m - 1, j = n - 1, k = m + n - 1;
        while (i >= 0 && j >= 0) {
            if (nums1[i] > nums2[j]) {
                nums1[k--] = nums1[i--];
            } else {
                nums1[k--] = nums2[j--];
            }
        }
        // 如果 nums2 还有剩余(m < n), 则把 nums2 剩余部分加入 nums1
        while (j >= 0) {
            nums1[k--] = nums2[j--];
        }
    }
};
```

- **知识点(关键词)**：**三指针**，**原地操作**

- **思路**：**从nums1和nums2后面开始比，谁大就放最后面（因为nums1固定长 m+n，一定能填满）**，其中：

    * `i` 从后往前依次指向 `nums1` 的各个元素
    * `j` 从后往前依次指向 `nums2` 的各个元素
    * `k` 从后往前指向下一个可赋值的位置；

- **注意**：只用**检查 `nums2` 是否有元素剩余(i <= j)**，不用检查 `nums1`，因为就是**在 `nums1` 上进行的原地操作**；
---

## 6. 题号：217 -- 存在重复元素（简单）
### 6.1 正确解法(一次通过，嚯嚯哈嘿)
```cpp
class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        unordered_map<int, int> mp;
        for (int i = 0; i < nums.size(); ++i) {
            if (mp.find(nums[i]) != mp.end()) {
                return true;
            }
            mp[nums[i]] = i;
        }
        return false;
    }
};
```

- **关键词**：**哈希表**

- **思路**：用哈希表存 `pair` (对) `value - index`，如果在哈希表中找到了与当前 `i` 对应元素相同的元素，则直接返回 `true`(退出循环)，如果遍历完所有元素，在哈希表中没有找到与之前相同的元素，则返回 `false`;

---

## 7. 题号：136 -- 只出现一次的数字（简单）
### 7.1 错误解法(自己写的)
代码如下:
```cpp
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        unordered_map<int, int> mp;     // value - index
        int i;
        for (i = 0; i < nums.size(); ++i) {
            if (mp.find(nums[i]) == mp.end()) {
                break;
            }
            mp[nums[i]] = i;
        }
        return nums[i];
    }
};
```

- **思路**：使用**哈希表**存取读到过的元素，碰见相同的继续存储；如果在哈希表中找不到与当前 `i` 对应的元素相同的值（元素），则说明找到只出现一次的数字，退出循环，返回当前 `i` 对应的值 `nums[i]`;

- **错误点**：
1. **逻辑严重错误**：当 `i = 0` 时，进入分支结构，一定会**直接退出循环**，因为当前哈希表为空，一定没有相同的值；
2. **潜在错误**：`i` 没有初始化，若函数输入为空数组，则循环体不会执行，最后直接 `return nums[i]` 属于 **未定义行为**

> **最终该题没有想出答案！！！**

### 7.2 正确解法及分析
代码如下：
```cpp
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int result = 0;
        // 遍历数组，不断异或
        // 成对出现的元素互相抵消（x ^ x = 0）
        // 最终 result 就是那只出现一次的元素
        for (int x : nums) {
            result ^= x;
        }
        return result;
    }
};
```

- **知识点(关键词)**：**位运算(异或运算)**

- **思路**：让**重复元素之间相互抵消**，而 **$0 \times a = a$**(给 `result` 赋值用于后续相同元素的计算)，最后的 result 必定为唯一的元素；

* 如果用哈希表统计频率，时间 O(n)、空间 O(n) 可以轻松解决。但题目要求常量空间，哈希表不符合。

```
题目中用到的异或运算(不同才为真)：
a ^ 0 = a;（满足交换律，即 0 ^ a = a）
a ^ a = 0;
a ^ b = a;
```

### 7.3 位运算补充
在 C 和 C++ 中，位运算直接操作整数的二进制位，效率高，常用于底层编程、标志位管理、加密等场景。下面列出所有位运算符的符号及其计算方法。

---

#### 1. 按位与 `&`
- **符号**：`&`
- **规则**：对应位都是 1 时结果为 1，否则为 0。
- **真值表**：  
  `1 & 1 = 1`  
  `1 & 0 = 0`  
  `0 & 1 = 0`  
  `0 & 0 = 0`
- **示例**（以 8 位为例）：  
  `0b1100 & 0b1010 = 0b1000` (12 & 10 = 8)

---

#### 2. 按位或 `|`
- **符号**：`|`
- **规则**：对应位至少有一个为 1 时结果为 1，否则为 0。
- **真值表**：  
  `1 | 1 = 1`  
  `1 | 0 = 1`  
  `0 | 1 = 1`  
  `0 | 0 = 0`
- **示例**：  
  `0b1100 | 0b1010 = 0b1110` (12 | 10 = 14)

---

#### 3. 按位异或 `^`
- **符号**：`^`
- **规则**：对应位不同时结果为 1，相同时为 0。
- **真值表**：  
  `1 ^ 1 = 0`  
  `1 ^ 0 = 1`  
  `0 ^ 1 = 1`  
  `0 ^ 0 = 0`
- **示例**：  
  `0b1100 ^ 0b1010 = 0b0110` (12 ^ 10 = 6)

---

#### 4. 按位取反 `~`
- **符号**：`~`（一元运算符）
- **规则**：将每一位取反，0 变 1，1 变 0。
- **注意**：结果受整型提升影响，通常按补码表示，负数会参与运算。
- **示例**（8 位有符号整数，实际 C 中会提升到 int）：  
  `~0b00001100 = 0b11110011` (在有符号 8 位下，~12 = -13)

---

#### 5. 左移 `<<`
- **符号**：`<<`
- **规则**：`a << b` 将 `a` 的二进制位向左移动 `b` 位，右端补 0。  
  等价于 `a * 2^b`（在不溢出且无符号数的前提下）。
- **注意**：有符号数的左移若导致符号位改变，属于**未定义行为**（C/C++ 标准）。通常建议仅对无符号数使用左移。
- **示例**：  
  `0b0011 << 2 = 0b1100` (3 << 2 = 12)

---

#### 6. 右移 `>>`
- **符号**：`>>`
- **规则**：`a >> b` 将 `a` 的二进制位向右移动 `b` 位。
  - **无符号数**：左端补 0（逻辑右移）。
  - **有符号数**：行为由实现定义（通常是算术右移，即左端补符号位，负数会保持负号）。
- **示例**（假设采用算术右移）：  
  `0b1100 >> 2 = 0b0011` (12 >> 2 = 3)  
  `-8 >> 1` 在补码系统中可能等于 -4。

---

#### 7. 复合赋值位运算符
- **符号**：`&=`, `|=`, `^=`, `<<=`, `>>=`
- **规则**：`a &= b` 等价于 `a = a & b`，其他类似。
- **示例**：  
  `int a = 5; a |= 2;` → `a` 变为 7。

---

#### 8. 注意事项
- **整型提升**：位运算的操作数会先被提升为 `int` 或 `unsigned int`。例如 `char` 运算时实际按 `int` 进行。
- **未定义/实现定义行为**：
  - 左移溢出符号位：未定义（C++20 对无符号明确定义，但有符号仍为未定义）。
  - 有符号负数的右移：结果依赖实现，可移植代码应避免。
  - 移位位数大于等于类型宽度（如 `int` 左移 32 位）：未定义行为。
- **优先级**：`~` 优先级较高，`<<` `>>` 次之，`&` 高于 `^` 高于 `|`。建议用括号明确顺序。

---

掌握这些位运算符，可以高效地完成置位、清零、翻转、提取特定位等操作。

## 8. 题号：169 -- 多数元素（简单）
### 8.1 错误解法（自己写的）
代码如下：
```cpp
class Solution {
public:
    int majorityElement(vector<int> nums) {
        unordered_map<int, int> mp;         // value - frequency
        for (int i = 0; i < nums.size(); ++i) {
            if (mp.find(nums[i]) != mp.end()) {
                ++mp[nums[i]];
            } else {
                mp[nums[i]] = 1;
            }
        }
        // 范围 for 遍历哈希表
        for (const auto pair : mp) {
            if (pair.second > nums.size() / 2) {
                return pair.first;
            }
        }
    }
};
```

- **思路**（✅）：用**哈希表**存储每个值出现的次数，之后遍历哈希表，找出出现次数大于 `n / 2` 的值(`nums[i]`);

- **错误点**（无逻辑错误）：
1. 在构建哈希表的 `for` 循环中，代码太过复杂，哈希表的 `int` **默认初始化就是 0**，直接每个元素值(`pair.first`)对应的 `pair.second` 自增就行，且**范围 `for`** 代码量更少；
2. **如果数组为空或无解，函数结束无 `return`**。
* 虽然我想到了大的作用域没有返回，但是：一、题目明确说明`假设数组是非空的，并且给定的数组总是存在多数元素。`；二、返回 `0` 或 `-1` 都可能是数组中出现最多次的元素，如果返回这两个值，则当数组中出现最多次的元素是 `0` 或 `-1` 时，**无法判断究竟是找到了多数元素还是数组为空或无解的情况**，所以最后我没有加返回；
* 实际上，就像我说的，题目明确：`假设数组是非空的，并且给定的数组总是存在多数元素。`，所以**可以返回任意值，因为题目总是有解的**，而工程实践中，碰到数组为空或者无解的情况，可以**使用 `throw` 抛出错误**，但是这里做算法题就以简便为主，直接 `return -1`；

改正后的**正确代码**：
```cpp
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        unordered_map<int, int> mp;         // value - frequency
        for (auto num : nums) {
            ++mp[num];                      // mp[num] 默认为 0
        }
        // 范围 for 遍历哈希表
        for (const auto& pair : mp) {
            if (pair.second > nums.size() / 2) {
                return pair.first;
            }
        }
        return -1;                          // 若实际情况，数组最多的元素为 -1 则无法保证结果，但是题目明确数组非空且存在多数元素，这样写更简便，工程可以用 throw 抛出错误
    }
};
```

- 上述算法：**时间复杂度$O(n)$，空间复杂度$O(n)$**。

## 9. 题号：206 -- 反转链表（简单）
### 9.1 错误解法(自己写的)
```cpp
// 思路1：重新构造一个 head2 从 head 后面开始给 head2 赋值(非原地操作)
class Solution {
public:
    ListNode* findFinal(ListNode *head) {
        ListNode *p = head->next;
        while (p->next) {
            p = p->next;
        }
        return p;
    }
    ListNode* reverseList(ListNode *head) {
        ListNode head2;
        ListNode *p = head->next;
        // 循环需要使用递归(单链表，无前驱指针)：先把最后一个结点(next=nullptr)的结点插入到 head2 的 next
        while (p) {
            
        }
    }
};
```
- **关键词**：**递归**，**非原地操作**，**尾插法**
- **思路**(❌)：重新构造一个 head2 从 head 后面**尾插法**给 head2 赋值;（过于麻烦，且效率低O(n^2)）

### 9.2 正确解法及分析
#### 9.2.1 解法一： **非递归非原地头插**
代码如下：
```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode *head2 = nullptr;
        ListNode *p = head;
        while (p) {
            ListNode *newNode = new ListNode(p->val);
            // 头插
            newNode->next = head2;
            head2 = newNode;
            p = p->next;
        }
        return head2;
    }
};
```

- **知识点(关键词)**：**头插法**

- **思路**：从 `head` 的第一个元素开始，头插进 `head2` 中，就实现了倒序

#### 9.2.2 解法二：**原地递归**
```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        if (!head || !head->next) return head;       // 递归基，即最后一层递归终止
        // newHead 始终是原链表的最后一个节点
        ListNode *newHead = reverseList(head->next);
        // 让反转后的子链表的最后一个结点指向 当前递归层 的 head
        head->next->next = head;
        head->next = nullptr;
        return newHead;
    }
};
```

- **知识点(关键词)**：**递归**，**原地操作**

- **思路**：要求 `head` 的倒序链表 `->` 先求 `head->next` 的倒序链表 `->` 先求 `head->next->next` 的倒序链表 `->` ... `->` 递归基，然后一层层**倒退**，直至求出 `head` 的倒序链表（这里的 **`head` 是同一个**）

- **`head->next->next = head`** 的理解：**将 `head` 尾插到反转后的子链表**(除去 `head`)。`head` 是**每一层递归调用传入的 `head`**，**`head->next` 指向反转后的子链表(除去 `head`)的最后一个结点**

## 10. 题号：21 -- 合并两个有序链表（简单）
### 10.1 错误解法(自己写的)
```cpp
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        if (list1->val > list2->val) {
            ListNode *temp = list2;
            temp->next = list1->next;
            list1->next = temp;
            ++list1;
            ++list2;
        } else {
            ++list1;
        }
        while (list2) {
            list1->next = list2;
        }
        return list1;
    }
};
```

- **思路**(**`严重错误`**)：在 `list1` 链表中**原地操作**，使用临时变量 `temp` 先复制为当前 `list2`，便于之后用于完成插入，最后更新 `list1` 和 `list2`，如果 `list2` 的值更大，则更新 `list1`;

> 使用 `temp = list2`，**本质 temp 和 list2 指向同一个位置**，对 `temp` 的操作已经造成**链表断裂，结点丢失**了，实际上，`temp` 没有发挥任何作用。

- **错误点**：
1. **逻辑错误(`特错大错`)**：

(1) **`++list1` 和 `++list2` 只有对于连续空间的指针才适用**，对于**链表**，存储空间不连续，不能使用自增运算符，**应该使用 `next` 域迭代**；

(2) 链接剩余部分的代码，**`list2` 永运不为 `nullptr`，陷入死循环**；

(3) **分支结构外没有循环**；

(4) **头结点丢失**，在其中更新了 `list1`，更新 `list1` 后 `list1` 就不能作为返回值；

(5) **未处理空链表**；

(6) **尾连接不安全**：list 可能也为 `nullptr` 了；

(7) **指针移动错误，造成覆盖**：应该先更新 `list2`，否则 `list1` 的更新会覆盖 `list2`

...

### 10.2 正确解法及分析
代码如下：
```cpp
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        if (!list1) return list2;
        if (!list2) return list1;
        ListNode dumy(0);                   // dumy 为头指针(虚拟头节点)
        ListNode *tail = &dumy;             // tail 永远指向最后一个元素
        while (list1 && list2) {
            if (list1->val <= list2->val) {
                tail->next = list1;
                list1 = list1->next;
            } else {
                tail->next = list2;
                list2 = list2->next;
            }
            tail = tail->next;
        }
        // 链接剩余部分
        tail->next = list1 ? list1 : list2;
        return dumy.next;
    }
};
```

- **知识点(关键词)**：**虚拟头节点(头指针)**

- **思路**：虚拟头节点即头指针用于返回链表，尾指针 `tail` 永远指向构造的链表的最后一个元素，用于把 `list1` 和 `list2` 的元素放入新构造的链表，**本质是 `改变` 链表各个结点的连接顺序，算作原地操作**；

## 11. 题号：141 -- 环形链表（简单）
### 11.1 错误解法(自己写的)
```cpp
class Solution {
public:
    bool hasCircle(ListNode *head) {
        unordered_map<int, int> mp;         // value - index
        ListNode *p = head;
        int i = 1;
        while (p) {
            if (mp.find(p->val) != mp.end()) {
                return true;
            }
            mp[p->val] = i;
            ++i;
        }
        return false;
    }
};
```

- **思路**(**严重逻辑错误**)：通过哈希表建立 “value - 出现顺序” 映射，在哈希表中找到当前值，说明找到重复值，存在环，`return false`;经历完循环没有 `return false`，说明没有环，返回 `true`；

- **错误点**：
1. **逻辑错误**：

- 如果链表中**不同节点具有相同的值**，即使链表完全没有环，程序也会错误地返回 `true`。

- 反之，如果链表**确实有环，但环上的节点值都不重复**，程序反而检查不出来，会陷入死循环或遍历到环上一直找不到重复值（因为没记录地址），但实际上因为 while(p) 不会终止，会无限循环。同时 p 永远不会为 nullptr，这会导致超时或崩溃。；

### 11.2 正确解法1 -- 哈希表
代码如下：
```cpp
class Solution {
public:
    bool hasCircle(ListNode *head) {
        unordered_set<ListNode*> visited;       // 用集合存结点指针(地址)
        ListNode *p = head;
        while (p) {
            if (visited.count(p)) {             // 查找集合中是否有等于 p 的值(这里为地址)，只可能为 0 或 1(集合不允许有重复值)，这里也可以用visited.find(p) != visited.end()
                return true;
            }
            visited.insert(p);
            p = p->next;
        }
        return false;
    }
};
```

- **知识点(关键词)**：**哈希表(unordered_set)**

- **思路**：判断链表是否有环，唯一可靠的标准是**节点的内存地址是否重复出现**；

- **时间复杂度 -- $O(n)$，空间复杂度 -- $O(n)$**

### 11.3 正确解法2 -- 快慢指针
```cpp
class Solution {
public:
    bool hasCircle(ListNode *head) {
        ListNode *slow = head, *fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) {
                return true;
            }
        }
        return false;
    }
};
```

- **知识点**：**快慢指针**，**Floyd 判圈法**，**龟兔赛跑算法**

- **思路**：**`fast` 永远跑不到终点，反而会在环内兜圈，一直循环，总会有和 `slow` 相遇的时候**

- **时间复杂度 -- O(n)，空间复杂度 -- O(1)**

> **说明**：
>
> 1.**无环时**，`fast` 每次走两步，大约需要 n/2 次循环，`slow` 也走 n/2 步，总共走 1.5 步，**循环次数 n/2 次**；
> 2.**有环时**，
> 
> **最坏情况**：**慢指针入环后，需要在环里走很远才能被快指针追上**。
> 
> **发生的条件**：
> 
> 1.**环入口离链表头部非常远**（接近 n，这里 n 是节点总数）。
> 
> 2.并且 **慢指针入环时，快指针刚好领先它 1 步**（或者更广义地说，慢指针需要跑**几乎**一整圈(环长度为 b，实际走 b-1 次)环才能被追上）。
> 
> 此时 `slow` 走约 n 步，`fast` 走约 2n 步，总共走约 3n 步，**循环次数 n-1 次**；

- **可优化**：
1. **提前判断**：若只有 0 或 1 个结点，必定无环；
2. **初始化优化**：在初始化时就让 `fast` 领先 `slow` 一步；**循环次数 - 1**；

### 11.4 最终优化版本(考虑边界条件)
```cpp
class Solution {
public:
    bool hasCircle(ListNode *head) {
        if (!head || !head->next) {
            return false;
        }
        ListNode *slow = head, *fast = head->next;
        while (fast && fast->next) {
            if (slow == fast) {
                return true;
            } else {
                slow = slow->next;
                fast = fast->next->next;
            }
        }
        return false;
    }
};
```

## 12. 题号：160 -- 只出现一次的数字（简单）
### 12.1 正确解法(自己写的)
```cpp
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        if (!headA || !headB) {
            return nullptr;
        }
        ListNode *p = headA;
        ListNode *q = headB;
        unordered_set<ListNode*> setA;
        while (p) {
            setA.insert(p);
            p = p->next;
        }
        while (q) {
            if (setA.count(q)) {
                return q;
            }
            q = q->next;
        }
        return nullptr;
    }
};
```

- **思路**：遍历链表 A，把其所有地址加入哈希表中，然后遍历链表 B，在哈希表中查看是否有 B 的各个元素的地址，有则返回该结点的地址，若遍历完 B 仍然哈希表仍然没有找到，则返回 `nullptr`;

- **时间复杂度 -- $O(m + n)$，空间复杂度 -- $O(m)$(可以看 m 和 n 谁更小适当调整)**

### 12.2 正确解法二(更优)
代码如下：
```cpp
if (headA == nullptr || headB == nullptr) {
            return nullptr;
        }
        ListNode *pA = headA, *pB = headB;
        while (pA != pB) {
            pA = (pA == nullptr) ? headB : pA->next;
            pB = (pB == nullptr) ? headA : pB->next;
        }
        // 要么相交必相遇 pA = pB = 交点地址，要么不相交 pA = pB = nullptr
        return pA;
```

- **知识点**：**双指针**

- **思路**：假设链表 `A` 的长度为 `a`，链表 `B` 的长度为 `b`，它们相交部分的长度为 `c`（不相交则 c=0），**`pA` 和 `pB` 走的步数相同(相交必相遇)，要么走 `a + b` 步(不相交，两个指针均走完两个链表)，要么走 `a + b - c` 步(相交)**；

## 13. 题号：234 -- 回文链表（简单）
### 13.1 错误解法(自己写的)
```cpp
class Solution {
public:
    bool isPalindrome(ListNode *head) {
        if (!head || !head->next) {
            return false;
        }
        ListNode *head2 = new ListNode;
        ListNode *p = head->next, *pre = head, *q = head2->next;
        while (p) {
            // 头插法使回文链表前半段倒序
            ListNode *newNode = new ListNode(p->val, q);
            if (pre->val == p->val) {
                break;
            }
            q = newNode;
            p = p->next;
            pre = pre->next;
        }
        while (p) {
            if (q->val == p->val) {
                q = q->next;
                p = p->next;
            } else {
                return false;
            }
        }
        return true;
    }
};
```

- **思路**：通过**头插法**重新创建一个链表存储**半个回文链表的倒叙序列**，然后比较与剩下的半个序列逐元素比较是否相同;

- **错误点**：
1. **逻辑错误**：(1) **找中点的方式完全错误**(比如 `1 2 3 2 1` 是回文，但中间 `3` 没有配对，不会触发相等；)(2) **构建“反转前半部分”的过程遗漏了头节点**；
2. **语法错误**：(1) **内存泄露风险**：`new` 之后没有 `delete`；**其他错误略**

### 13.2 正确解法1 -- 数组拷贝
代码如下：
```cpp
class Solution {
public:
    bool isPalindrome(ListNode *head) {
        if (head == nullptr || head->next == nullptr) {
            return true;                // 空链表和单元素默认回文
        }
        vector<int> vals;                // 自动管理长度
        ListNode *curr = head;
        while (curr != nullptr) {
            vals.push_back(curr->val);
            curr = curr->next;
        }
        // 数组双指针判断是否回文
        int left = 0, right = (int)vals.size() - 1;
        while (left < right) {          // 循环终止条件是 left < right，不能写成 left != right(奇数满足，偶数不满足)
            if (vals[left] != vals[right]) {
                return false;
            }
            ++left;
            --right;
        }
        return true;
    }
};
```

- **知识点**：**数组拷贝**，**数组双指针**；

- **时间复杂度 -- $O(n)$，空间复杂度 -- $O(n)$**

- **思路**：遍历链表将值拷贝到数组中，然后用**在数组中利用双指针**判断数组是否为回文（用 `C 语言` 实现手动管理内存需**多遍历一次链表以求得数组的长度**）;

### 13.3 正确解法 2 -- 栈
代码如下：
```cpp
class Solution {
public:
    bool isPalindrome(ListNode *head) {
        if (head == nullptr || head->next == nullptr) {
            return true;                
        }
        stack<int> vals;
        ListNode *curr = head;
        while (curr != nullptr) {
            vals.push(curr->val);
            curr = curr->next;
        }
        curr = head;
        while (curr != nullptr) {
            int val = vals.top();
            vals.pop();                 // 出栈不返回值
            if (curr->val != val) {
                return false;
            }
            curr = curr->next;
        }
        return true;
    }
};
```

- **知识点**：**数组拷贝**，**数组双指针**；

- **时间复杂度 -- $O(n)$，空间复杂度 -- $O(n)$**

- **思路**：遍历链表将值拷贝到栈(**stack**)中，然后不断获取栈顶元素(**别忘了出栈**)与链表元素比较;

### 13.3 正确解法 3 -- 反转后段链表(最优解法)
代码如下：
```cpp
class Solution {
public:
    // 反转链表 -- 参考题 9 反转链表
    ListNode* reverseList(ListNode *head) {
        ListNode *prev = nullptr, *curr = head;
        while (curr != nullptr) {
            ListNode *tmp = curr->next;
            curr->next = prev;
            prev = curr;
            curr = tmp;
        }
        return prev;
    }

    bool isPalindrome(ListNode *head) {
        if (head == nullptr || head->next == nullptr) {
            return true;                
        }
        // 1.快慢指针找中点，参考题 11 环形链表 
        ListNode *slow = head, *fast = head;
        while (fast->next != nullptr && fast->next->next != nullptr) {          // 注意循环终止条件
            slow = slow->next;
            fast = fast->next->next;
        }
        // 2. 反转链表，slow->next 为后半段头结点
        ListNode *secondHalf = reverseList(slow->next);
        // 3.判断回文
        ListNode *p1 = head;
        ListNode *p2 = secondHalf;       //  防止头结点丢失并便于反转还原
        while (p2 != nullptr) {         //  后半段 可能少一个元素，用 p2 判断
            if (p1->val != p2->val) {
                return false;
            }
            p1 = p1->next;
            p2 = p2->next;
        }        
        // 4.还原(再次反转)后半段
        // slow->next = reverseList(secondHalf);
        return true;
    }
};

```

- **知识点**：**三指针反转**，**快慢指针找中点**

- **思路**：见代码；

> **注意**：**慢指针 奇数停留在唯一值的位置，偶数停留在前半段的末尾**，保证了 slow->next 是后半段的头结点(**奇数时反转后前半段的最后的唯一值不会比较(循环判断条件)**，因为前半段长度 n+1/2, 后半段长度 n-1/2，刚好差 1)

- **时间复杂度 -- $O(n)，空间复杂度 -- $O(1)$**

## 14. 题号：20 -- 有效的括号（简单）
### 14.2 正确解法(这题很简单，严书中有括号匹配(C实现)，但是这个用C++实现)
代码如下：
```cpp
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
```

- **知识点**：**栈**；

- **时间复杂度 -- $O(n)$，空间复杂度 -- $O(n)$**

- **思路**：左括号进栈，碰到右括号则取栈顶元素看是否匹配(**内层括号先匹配**);

- **注意**：
1. **两处对栈空的判断**：第一处：**避免只有左括号的情形**；第二处(返回)：**避免有多余右括号的情形**；
2. **==优化技巧==**：对左括号的进栈操作处，此时 `ch` 已经是左括号，没必要继续判断后面的是否为右括号，直接 `continue` 退出本轮循环，**避免不必要的判断**；

## 15. 题号：155 -- 最小栈（中等）
### 15.1 解法 1 -- vector(本题最优)
```cpp
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
```

- **知识点**：**vector容器**；

- **思路**：一个 `vector` 存数据，一个每次加入一个当前的最小值(**不维护一个**方便出栈管理);

- **注意**：
1. `vector` 在**非尾部的插入删除开销大**(如 `insert(pos, val)`, `erase(pos)`, `pop_front()`)，**`push_back`分摊 O(1)**，但**更省内存空间(用一个中控器（指针数组）管理多个固定大小的块)**

### 15.2 解法 2 -- stack
代码如下：
```cpp
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
```

- **知识点**：**stack容器**；

- **思路**：同上;

- **优化部分**：使用哨兵，提前放入 `INT_MAX`，后续元素 `val` 只可能小于等于 `INT_MAX`，这样就**不用每次判断栈空**了；（`vector` 可同理优化）；

### 15.3 解法 3 -- 差值栈 + long long min
代码如下：
```cpp
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
```

- **知识点**：**差值栈**，**long long**；

- **思路**：看代码;

- **优化部分**：**时间复杂度 -- $O(1)$，(额外)空间复杂度 -- $(1)$**

## 16. 题号：232 -- 用栈实现队列（简单）
### 16.1 解法
```cpp
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
```

- **知识点**：**两个栈实现队列**；

- **思路**：两个栈，**栈 `in` 用于进栈，栈 `out` 用于出栈**，关键点：不用每个元素进栈时都加入 `out` 栈，**只在需要使用到队头元素时(队头出队`pop`和取队头`peek`)把 `in` 中的元素倒装进 `out`**，然后用栈的 `pop` 和 `top` 即可完成相应操作;

- **注意点**：
1. **要用到队头时才把 `in` 中的元素倒序放入 `out`**;
2. **进入 `out` 的元素记得出 `in` (`in.pop()`)**;
3. **队判空时要检查两个栈：因为 `push` 时 `in` 中有元素而 `out` 为空，`pop` 时 `out` 中有元素而 `in` 为空**;

## 17. 题号：225 -- 用队列实现栈（简单）
### 17.1 错误解法(自己写的)
```cpp
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
```

- **思路**：同上;

- **错误点**：
1. **逻辑错误**：`push` 将元素放入 `in` 队列，`pop` 时调用 `in2out` 将 `in` 中所有元素按**原顺序转移**到 `out` 队列；
2. **语法错误**：`in2out` 只在 `out.empty()` 时执行。这会导致混合操作时出现更隐蔽的错误;

### 17.2 正确解法1 -- 两个队列实现(题目要求)
代码如下：
```cpp
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
```

- **知识点**：**栈的实现**，**两个队列**；

- **思路**：**q1 作主队列(即 ==栈== )，q2 用于记录最后进的元素，并在 q1 的基础上重新构造 q1**;

### 17.3 正确解法 2 (最优) -- 一个队列实现
代码如下:
```cpp
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
```

- **知识点**：**栈的实现**，**一个队列**；

- **思路**：直接让队列存储出栈顺序，**每次添加新元素时，要把新入队的元素 `x` 之前的所有元素排在 `x` 后面，以保证出栈顺序**，即旋转；

## 18. 题号：94 -- 二叉树的中序遍历（简单）

> **思路核心(三种解法同)**：**左子树->根->右子树**;

### 18.1 解法 1 -- 拷贝递归实现(同上)
```cpp
class Solution {
public:
    vector<int> inorderTraversal(TreeNode *root) {
        vector<int> res;
        if (root == nullptr) {              // 递归基
            return res;
        }
        // 左子树结果
        vector<int> left = inorderTraversal(root->left);
        res.insert(res.end(), left.begin(), left.end());        // 用 insert 而不用 push_back 时因为 left 是 vector 而不是 元素
        res.push_back(root->val);
        // 右子树结果
        vector<int> right = inorderTraversal(root->right);
        res.insert(res.end(), right.begin(), right.end());
        return res;
    }
};
```

- **知识点**：**拷贝递归**；

- **注意**：

1. 直接使用题目所给函数递归**有返回值，需拷贝最终添加到 `res` 中**;
2. 添加到 `res` 用 `insert` 而不用 `push_back` 是**因为 `left` 是 `vector` 而不是 元素(push_back)**;

### 18.2 解法 2 -- 借用辅助函数原地递归实现
代码如下：
```cpp
class Solution {
private:
    void inorder(TreeNode *root, vector<int> &res) {
        if (root == nullptr) {              // 递归基
            return ;
        }
        inorder(root->left, res);
        res.push_back(root->val);
        inorder(root->right, res);
    }
public:
    vector<int> inorderTraversal(TreeNode *root) {
        vector<int> res;
        inorder(root, res);
        return res;
    }
};
```

- **知识点**：**辅助函数+原地递归**；

> **思路清晰，写递归大推荐**

### 18.3 解法 3 -- 借助栈非递归迭代实现
代码如下：
```cpp
class Solution {
public:
    vector<int> inorderTraversal(TreeNode *root) {
        vector<int> res;
        stack<TreeNode*> s;
        TreeNode *curr = root;
        while (curr != nullptr || !s.empty()) {         // 注意：只有当 curr 为空且栈空时才退出循环
            // 1.沿着左子树走到底，并每走一步压栈
            while (curr != nullptr) {
                s.push(curr);
                curr = curr->left;
            }
            // 2.弹栈并加入 res
            curr = s.top();
            s.pop();
            res.push_back(curr->val);
            // 3.对右子树重复上述操作
            curr = curr->right;
        }
        return res;
    }
};
```

- **知识点**：**栈**，**非递归迭代**；

- **注意点**：循环退出条件：**只有同时满足 curr == nullptr 且 栈空 时才退出循环**;

## 19. 题号：144 -- 二叉树的前序遍历（简单）

> **思路核心(三种解法同)**：**根->左子树->右子树**;
> 
> **解法 1、2 同上**，**注意解法 3 的差异**

### 19.1 解法 1 -- 拷贝递归实现
```cpp
class Solution {
public:
    vector<int> preorderTraversal(TreeNode *root) {
        vector<int> res;
        if (root == nullptr) {
            return res;
        }
        res.push_back(root->val);
        vector<int> left = preorderTraversal(root->left);
        res.insert(res.end(), left.begin(), left.end());
        vector<int> right = preorderTraversal(root->right);
        res.insert(res.end(), right.begin(), right.end());
        return res;
    }
};
```

- **知识点**：**拷贝递归**；

### 19.2 解法 2 -- 借用辅助函数原地递归实现
代码如下：
```cpp
class Solution {
private:
    void preorder(TreeNode *root, vector<int> &res) {
        if (root == nullptr) {
            return ;
        }
        res.push_back(root->val);
        preorder(root->left, res);
        preorder(root->right, res);
    }
public:
    vector<int> preorderTraversal(TreeNode *root) {
        vector<int> res;
        preorder(root, res);
        return res;
    }
};
```

- **知识点**：**辅助函数+原地递归**；

> **思路清晰，写递归大推荐**

### 19.3 解法 3 -- 借助栈非递归迭代实现
代码如下：
```cpp
class Solution {
public:
    vector<int> preorderTraversal(TreeNode *root) {
        vector<int> res;
        stack<TreeNode*> s;
        if (root == nullptr) {
            return res;
        }
        s.push(root);                       // 这里要提前让 root 进栈，否则循环中 curr 为野指针
        while (!s.empty()) {
            TreeNode *curr = s.top();
            s.pop();
            // 1.先访问根结点
            res.push_back(curr->val);
            // 依次把右孩子、左孩子放入栈中(对应顺序左孩子、右孩子出栈)
            if (curr->right != nullptr) {
                s.push(curr->right);
            }
            if (curr->left != nullptr) {
                s.push(curr->left);
            }
        }
        return res;
    }
};
```

- **知识点**：**栈**，**迭代非递归**；

- **注意点**：

1. **首先判断根结点指针是否为空，为空直接返回空**；
2. **提前让 `root` 进栈，否则循环中 `curr` 为野指针**；
3. **先访问根节点**，**进栈顺序**：`右孩子->左孩子`;**出栈顺序**：`左孩子->右孩子`;

## 20. 题号：145 -- 二叉树的后序遍历（简单）

> **思路核心(三种解法同)**：**左子树->右子树->根**;
> 
> **解法 1、2 同上**，**注意解法 3、4 的差异**

### 20.1 解法 1 -- 拷贝递归实现
```cpp
class Solution {
public:
    vector<int> postorderTraversal(TreeNode *root) {
        vector<int> res;
        if (root == nullptr) {
            return res;
        }
        vector<int> left = postorderTraversal(root->left);
        res.insert(res.end(), left.begin(), left.end());
        vector<int> right = postorderTraversal(root->right);
        res.insert(res.end(), right.begin(), right.end());
        res.push_back(root->val);
        return res;
    }
};
```

- **知识点**：**拷贝递归**；

### 20.2 解法 2 -- 借用辅助函数原地递归实现
代码如下：
```cpp
class Solution {
private:
    void postorder(TreeNode *root, vector<int> &res) {
        if (root == nullptr) {
            return ;
        }
        postorder(root->left, res);
        postorder(root->right, res);
        res.push_back(root->val);
    }
public:
    vector<int> postorderTraversal(TreeNode *root) {
        vector<int> res;
        postorder(root, res);
        return res;
    }
};
```

- **知识点**：**辅助函数+原地递归**；

> **思路清晰，写递归大推荐**

### 20.3 解法 3 -- 非递归迭代实现 -- 前序遍历变种(根右左) + 反转
代码如下：
```cpp
class Solution {
public:
    vector<int> postorderTraversal(TreeNode *root) {
        vector<int> res;
        stack<TreeNode*> s;
        if (root == nullptr) {
            return res;
        }
        s.push(root);                       // 这里要提前让 root 进栈，否则循环中 curr 为野指针
        while (!s.empty()) {
            TreeNode *curr = s.top();
            s.pop();
            // 1.先访问根结点
            res.push_back(curr->val);
            // 进栈：左->右   出栈：右->左(即 根->右->左)
            if (curr->left != nullptr) {
                s.push(curr->left);
            }
            if (curr->right != nullptr) {
                s.push(curr->right);
            }
        }
        // 反转：根->右->左 ----> 左->右->根
        reverse(res.begin(), res.end());
        return res;
    }
};
```

- **知识点**：**栈**，**前序遍历变种(根右左)+反转**；

- **注意点**：
1. 此为前序遍历的变种，**原前序遍历：`根->左->右`，变种：`根->右->左`，以满足前序遍历变种反转之后为 `左->右->根`** (即为**后序遍历**)；
2. 实现时，**先访问根节点，再左子树入栈，最后右子树入栈**（**原前序遍历先右子树入栈，最后左子树入栈**）；
3. 使用 `reverse` 记得包含 `algorithm`；

### 20.4 解法 4 -- 正式非递归迭代实现
代码如下：
```cpp
class Solution {
public:
    vector<int> postorderTraversal(TreeNode *root) {
        vector<int> res;
        stack<TreeNode*> s;
        TreeNode *curr = root;
        TreeNode *lastVisited = nullptr;
        while (curr != nullptr || !s.empty()) {
            // 一路向左，压栈
            while (curr != nullptr) {
                s.push(curr);
                curr = curr->left;
            }
            // 此时 curr = nullptr
            TreeNode *top = s.top();            // 栈顶节点 top 是最左边的尚未被访问的节点（即栈空表示左子树已经处理完毕）
            // 如果右子树为空或右子树已访问，则访问当前节点
            if (top->right == nullptr || top->right == lastVisited) {
                res.push_back(top->val);
                s.pop();
                lastVisited = top;              // 标记为已访问
            } else {
                // 右子树不为空且未访问，转右子树
                curr = top->right;
            }
        }
        return res;
    }
};
```

- **知识点**：**栈**，**迭代**；

- **注意点**：
1. **左子树的访问完毕由栈空保证，右子树第二个被访问时在 `else` 分支中，只要有右子树就访问，没有或已经访问过右子树才访问根**
2. 左子树、根节点、右子树是在一个地方被访问（**均作为根结点被访问**）
3. `lastVisited` 用于标记是否访问

## 21. 题号： -- （简单）
### 21.1 解法
```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode *root) {
        vector<vector<int>> res;
        queue<TreeNode*> q;
        if (root == nullptr) {
            return res;
        }
        q.push(root);
        while (!q.empty()) {
            int levelSize = q.size();           // 关键点：根据当前层的结点数遍历
            vector<int> level;
            for (int i = 0; i < levelSize; ++i) {
                TreeNode *curr = q.front();
                q.pop();
                level.push_back(curr->val);
                // 将下一层要遍历的结点入队
                if (curr->left != nullptr) q.push(curr->left);
                if (curr->right != nullptr) q.push(curr->right);
            }
            res.push_back(move(level));           // 显式移动，避免拷贝
        }
        return res;
    }
};
```

- **知识点**：**层序遍历**、**BFS**、**队列**；
    
- **思路**：**借助队列实现，每次记录上一层遍历的结点的个数以遍历出队当前层**;

- **注意点**：
1. 使用**队列存储下一层的访问顺序**，保证广度优先搜索的访问顺序正确；
2. 每层遍历前，先**获取当前层的长度(核心技巧)，保证遍历时不会出队下一层的元素**；
3. **边遍历当前层的元素，边将下一层的元素入队**；
4. **关键优化点**：`res.push_back(move(level))` **使用 `move` 将左值改为右值，避免不必要的拷贝开销**；

> `move` 的作用：`res` 里的新 `vector` 会直接**接管 `level` 的内部数组指针，而 `level` 自身变空**。**成本从 $O(levelSize)$ 降到 $O(1)$**。

## 22. 题号：104 -- 二叉树的最大深度（简单）
### 22.1 解法 1 -- 递归解法(深度优先搜索 DFS)
```cpp
class Solution {
public:
    int maxDepth(TreeNode *root) {
        if (root == nullptr) {
            return 0;
        }
        int leftDepth = maxDepth(root->left);
        int rightDepth = maxDepth(root->right);
        return 1 + (leftDepth > rightDepth ? leftDepth : rightDepth);
    }
};
```

### 22.2 解法 2 -- 层序遍历解法(广度优先搜索 BFS)
代码如下：
```cpp
class Solution {
public:
    int maxDepth(TreeNode *root) {
        if (root == nullptr) return 0;
        int depth = 0;                  // 出队一层 + 1，故而初始化为 0
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int levelSize = q.size();
            for (int i = 0; i < levelSize; ++i) {
                TreeNode *curr = q.front();
                q.pop();
                if (curr->left != nullptr) q.push(curr->left);
                if (curr->right != nullptr) q.push(curr->right);
            }
            ++depth;
        }
        return depth;
    }
};
```

- **知识点**：**递归**、**层序遍历**；

- **对比**：**时间相同，空间理论上 DFS 对平衡树更优，BFS 对退化树更优，一般情况 DFS 更优(递归)，但极深数栈有溢出风险**；

## 23. 题号： -- （简单）
### 23.1 错误解法
```cpp
class Solution {
public:
    bool isSymmatric(TreeNode *root) {
        if (root == nullptr) return true;
        TreeNode *left = root->left, *right = root->right;
        queue<TreeNode*> q;
        q.push(root);
        while (left != nullptr && right != nullptr) {
            int q_size = q.size();
            for (int i = 0; i < q_size; ++i) {
                TreeNode *node_l = q.front();
                q.pop();
                TreeNode *node_r = q.front();
                q.pop();
                if (node_l->val != node_r->val) {
                    return false;
                }
                // 每次压入两个值 left->val, right->val
                if (left->left && right->left) {
                    q.push(left->left);
                    q.push(right->left);
                }
                if (left->right && right->right) {
                    q.push(left->right);
                    q.push(right->right);
                }
            }
        }
        return true;
    }
};
```

- **思路**：**双指针 + BFS + 单个队列**(没问题);

- **错误点**：
1. **逻辑错误**：

(1) 第一次进入循环时，队列只有一个元素 `root`，但循环内连续 `pop` 两次，**第二次 `pop` 会访问空队列**，导致未定义行为（通常崩溃）;

(2) `left` 和 `right` 是 `root->left` 和 `root->right` 的初始值，**在循环内从未被更新**。因此：

- 如果**根没有左孩子或没有右孩子，循环根本不会进入**，直接 return true（错误，例如 [1,2,null] 应返回 false）;

- 如果**左右孩子都存在，则条件永远为真，形成无限循环**（因为队列可能在某一轮变空，但外层 while 不会退出）;

(3) **`left` 和 `right` 一直指向根的孩子，而不是当前正在比较的节点 `node_l` / `node_r`**。这导致所有层都只比较根的孩子，完全不符合对称性要求;

(4) 代码只在两个子节点都存在时才入队。**如果有一个子节点为 null 而另一个非 `nullptr`**，这一对节点会被忽略，**不会触发 `false`**;

### 23.2 正确解法及分析
代码如下：
```cpp
class Solution {
public:
    bool isSymmetric(TreeNode *root) {
        if (root == nullptr) return true;
        queue<TreeNode*> q;
        q.push(root->left);
        q.push(root->right);
        while (!q.empty()) {
            TreeNode *l = q.front();
            q.pop();
            TreeNode *r = q.front();
            q.pop();
            if (l == nullptr && r == nullptr) continue;
            if (l == nullptr || r == nullptr) return false;
            if (l->val != r->val) return false;
            // 按照镜像顺序入队
            q.push(l->left);
            q.push(r->right);
            q.push(l->right);
            q.push(r->left);
        }
        return true;
    }
};
```

- **知识点**：**一个队列 + BFS + 双指针**；

- **时间复杂度 -- $O(n)$，空间复杂度 -- $O(2^k - 1)(最大)$**(n 为结点数)

- **注意点**：
1. **`root` 不用入队**，否则队元素个数不是都是，总有一次出队两个元素时，第二次出队是**未定义行为**；
2. **循环终止条件为队空**；
3. **先判断 `l` 和 `r` 是否为空(先排除 `l == nullptr && r == nullptr` 的情况)**，为空则退出本次循环，之后再 `if (l == nullptr || r == nullptr) return false` ，否则当 `l` 和 `r` 均为 `nullptr` 时也会返回 `false`，还有，别忘了 `l->val != r->val` 也返回 `false`;
4. 二叉树对称是 **镜像对称**（类似纸对折重合）即 `l->left = ->right` && `l->right = r->left`，**故按镜像顺序入轨**;
5. 这里的 BFS 不用 `levelSize`;

## 24. 题号：226 -- 翻转二叉树（简单）
### 24.1 错误解法
```cpp
class Solution {
public:
    TreeNode* invertTree(TreeNode *root) {
        if (root == nullptr) return root;
        queue<TreeNode*> q;
        q.push(root);
        q.push(root->left);
        q.push(root->right);
        while (!q.empty()) {
            TreeNode *r = q.front(); q.pop();
            TreeNode *lT = q.front(); q.pop();
            TreeNode *rT = q.front(); q.pop();
            r->left = rT;
            r->right = lT;
            q.push(lT);
            q.push(lT->left);
            q.push(lT->right);
            q.push(rT);
            q.push(rT->left);
            q.push(rT->right);
        }
        return root;
    }
};
```

- **思路**：队列实现层序遍历;

- **错误点**：
1. **逻辑错误**：；

(1) **空指针解引用（崩溃）**
- 当 `lT` 或 `rT` 为 `nullptr` 时，访问 `lT->left` 等会直接导致程序崩溃。
- 即便前一步判空跳过，后续从队列取出 `nullptr` 作为 `r` 时，`r->left = rT` 同样解引用空指针。

(2) **“三元组”入队模式在树不完全时必然错位**
- 若某个节点缺少左子或右子，入队时少 `push` 了对应的三元组（或 `push` 了 `nullptr`）。
- 下一轮循环取出的三个元素就不再是“父-左-右”的对应关系，**后续所有处理全部错位**，导致错误交换甚至崩溃。

(3) **重复处理同一节点导致无限循环的风险**
- 虽然在某些满二叉树情况下不会重复，但代码将 r（当前节点）作为三元组的首元素压回队列，若因错位等原因某些节点被多次压入，或因空指针跳过导致队列元素不足三个时，while(!q.empty()) 可能永远为真或提前异常终止。

(4) **交换的语义实现存在严重误导**
- `r->left = rT; r->right = lT;` 看上去像是交换左右孩子，但 `rT` 和 `lT` 是**旧的孩子**，而**新孩子入队时用的还是旧孩子，没有利用交换后的结构**。虽然巧合下能遍历完，但队列管理极为混乱，不属于正确的 BFS 或 DFS 模式。

### 24.2 正确解法 1 -- 迭代
代码如下：
```cpp
class Solution {
public:
    TreeNode* invertTree(TreeNode *root) {
        if (root == nullptr) return root;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode *curr = q.front();
            q.pop();
            swap(curr->left, curr->right);
            if (curr->left != nullptr) q.push(curr->left);
            if (curr->right != nullptr) q.push(curr->right);
        }
        return root;
    }
};
```

- **知识点**：**队列**，**层序遍历**；

- **注意**：如果**强制使用 C 语言**，`swap` 函数可以用 `TreeNode *tmp = curr->left; curr->left = curr->right; curr->right = tmp;` 实现；

### 24.3 正确解法 2 -- 递归解法
代码如下：
```
class Solution {
public:
    TreeNode* invertTree(TreeNode *root) {
        // 递归基
        if (root == nullptr) return root;
        // 先翻转左、右子树
        invertTree(root->left);
        invertTree(root->right);
        // 再交换左右孩子
        swap(root->left, root->right);
        return root;
    }
};
```

- **知识点**：**递归**；

- **注意**：**别忘记翻转左右子树后还要交换当前 `root` 的左右孩子**；

## 25. 题号：543 -- 二叉树的直径（简单）
### 25.1 错误解法
```cpp
private:
    int maxDepth(TreeNode *root) {
        if (root == nullptr) return 0;
        int leftDepth = maxDepth(root->left);
        int rightDepth = maxDepth(root->right);
        return 1 + (leftDepth > rightDepth ? leftDepth : rightDepth);
    }
public:
    int diameterOfBinatyTree(TreeNode *root) {
        if (root == nullptr) return 0;
        return 1 + maxDepth(root->left) + maxDepth(root->right);
    }
};
```

- **思路**：**二叉树直径 = 左子树最大深度 + 右子树最大深度 + 1(根结点)**;

- **错误点**：
1. **逻辑错误**：**二叉树的直径不一定经过根节点**，上述代码只计算了 `root->left` 的深度 + `root->right` 的深度（再加1），这**仅仅得到了经过根节点的那条路径的节点数，但直径可能出现在任意子树中，完全可能不经过根节点**，如下例直径为 `6->4->2->5->27`，没有经过根结点：

```text
        1
       / \
      2   3
     / \
    4   5
   /     \
  6       27
```

### 25.2 正确解法 1 -- 递归解法
代码如下：
```cpp
class Solution {
private:
    int diameter = 0;                       // 全局直径(全局最大边数)
    int depth(TreeNode *root) {
        if (root == nullptr) return 0;
        int L = depth(root->left);
        int R = depth(root->right);
        diameter = diameter > (L + R) ? diameter : L + R;   // L(左子树深度) + R(右子树深度) 即经过该结点的边数
        return 1 + (L > R ? L : R);     // 返回深度
    }
public:
    int diameterOfBinaryTree(TreeNode *root) {
        depth(root);
        return diameter;
    }
};
```

- **知识点**：**递归**；

- **思路**：每颗子树二叉树直径 = 左子树最大深度 + 右子树最大深度 + 1，**每颗子树二叉树直径中的最大值即为二叉树的直径**;

## 26. 题号：2704 -- 二分查找（简单）
### 26.1 正确解法 -- 二分查找
代码如下：
```cpp
class Solution {
public:
    int search(vector<int> &nums, int target) {
        int left = 0, right = nums.size() - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (nums[mid] == target) {
                return mid;
            }
            if (nums[mid] < target) {       // target 在右半边
                left = mid + 1;
            }
            if (nums[mid] > target) {       // target 在左半边
                right = mid - 1;
            }
        }
        return -1;
    }
};
```

- **知识点**：**二分查找**；

- **时间复杂度 -- $O(log n)$，空间复杂度 -- O(1)**；

- **思路**：学过数据结构的都知道，可以不看；没学过的看这里：每次取区间重点与 `target` 进行比较，**通过 `nums[mid]` 和 `target` 的大小关系不断更新区间，直至 `nums[mid] = target`(找到) 或 `left > right`(不存在数组中)**；

- **注意点**：
1. 循环终止条件为 `left <= right`，**等号不能掉**；
2. 更新 `mid` 时的 **`+ 1` 或 `- 1` 不能掉**；
3. 二分搜索的前提：**有序数组**；

## 27. 题号：35 -- 搜索插入位置（简单）
### 27.1 错误解法
```cpp
public:
    int searchInsert(vector<int> &nums, int target) {
        int left = 0, right = nums.size() - 1;
        int mid = 0;
        while (left <= right) {
            // 防止 left + right > INT_MAX 时溢出
            mid = left + (right - left) / 2;
            if (nums[mid] == target) {
                return mid;
            } else if (nums[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }   // mid = left = right;
        return mid;
    }
};
```

- **错误点**：**插入位置判断错误**；

### 27.2 正确解法及分析
代码如下：
```cpp
class Solution {
public:
    int searchInsert(vector<int> &nums, int target) {
        int left = 0, right = nums.size() - 1;
        int pos = nums.size();      // 默认插入位置在末尾(所有元素 < target)
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] >= target) {
                pos = mid;          // mid 为候选插入位置，先往前找是否有等于 nums[mid] 的重复值
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        return pos;
    }
};
```

- **知识点**：**二分搜索变种**；

- **时间复杂度 -- $O(log n)$，空间复杂度 -- $O(1)$**

- **注意点**：
1. 关键 -- **查找第一个大于等于 `target` 的位置（即 C++ 中 `std::lower_bound` 的实现）**；
2. **当 `nums[mid] == target` 时不立即返回，而是继续往左搜索（因为可能有重复元素，我们找的是第一个满足条件的位置）**，所以 `nums[mid] == target` 的 情况和 `nums[mid] > target` 的情况合并，均向左搜索；
3. **插入位置永远为 `mid` 的位置**：
- `target` 没找到 且 `target < mid` 时，插入 `mid` 前面，即当前 `mid` 的位置，`mid` 及后面所有数后移；
- `nums[mid] == target` 时，重复值可以插入 `mid` 前面(即当前 `mid` 的位置)或后面；
- **综上取 `mid` 之前(即当前 `mid` 的位置)**； 

## 28. 题号：509 -- 斐波那契数（简单）
### 28.1 解法 1 -- 递归解法
```cpp
class Solution {
public:
    int fib(int n) {
        f (n <= 1) {
            return n;
        } else {
            return fib(n - 1) + fib(n - 2);
        }
    }
};
```

- **知识点**：**递归**；

- **时间复杂度 -- $O(2^n)$，空间复杂度(栈开销) -- $O(n)$**；

> **关于时间复杂度为 $O(2^n)$ 的解释：**

1. **通过二叉树的结构理解**：每次递归调用都伸出两个分支 `fib(n - 1)` 和 `fib(n - 2)`，类似二叉树的左右子树，但是**左右子树的深度不同，故此二叉树非完全二叉树**(一个为 `n - 1`，一个为 `n - 2`)，所以**最终的时间复杂度接近 $O(2^n)$ 但是小于 $O(2^n)$(做了剪枝操作)**；
2. **精确推导**：

设 T(n) 为计算 fib(n) 所需的基本操作次数（或函数调用次数）。
由代码：
```cpp
if (n == 0) return 0;          // 常数时间 O(1)
else if (n == 1) return 1;     // O(1)
else return fib(n-1) + fib(n-2); // T(n-1) + T(n-2) + 常数
```
因此有类似的递推式子：
```text
T(n) = T(n-1) + T(n-2) + c
T(0) = d, T(1) = e   （常数）
```
它的**解为 $T(n) = Θ(φ^n)$(解法：常系数线性非齐次方程求解)，其中 $φ = (1+√5)/2 ≈ 1.618$**（黄金比例）。

**因为 $1.618^n$ 是指数级增长，我们用更宽松的上界 $2^n$ 来表示其指数复杂度，即 $T(n) = O(2^n)$**。

- **可优化**：上述代码 `fib(n - 1)` 和 `fib(n - 2)` 的计算每个都要**单独**递归计算，导致时间复杂度飙升至 $O(2^n)$，实际上，**可以存储计算过的斐波那契数便于直接使用，避免重复计算**；

### 28.2 解法 2 -- 记忆化搜索
代码如下：
```cpp
class Solution {
private:
    int fib_memo(int n, vector<int> &memo) {
        if (n <= 1) return n;
        // 如果已经计算过 memo[n]，直接返回
        if (memo[n] != -1) return memo[n];      
        // 没有计算 memo[n] 则递归计算
        memo[n] = fib_memo(n - 1, memo) + fib_memo(n - 2, memo);
        return memo[n];
    }
public:
    int fib(int n) {
        vector<int> memo(n + 1, -1);        // fib(0) ~ fib(n)
        return fib_memo(n, memo);
    }
};
```

- **知识点**：**记忆化搜索**；

- **时间复杂度 -- $O(n)$，空间复杂度(多了`memo`数组，占用空间更大) -- $O(n)$**

### 28.3 解法 3 -- 迭代 DP(朴素版)-- 空间复杂度 O(n)
代码如下：
```cpp
class Solution {
public:
    int fib(int n) {
        if (n <= 1) return n;
        vector<int> dp(n + 1);
        dp[0] = 0;
        dp[1] = 1;
        for (int i = 2; i <= n; ++i) {
            dp[i] = dp[i - 1] + dp[i - 2];
        }
        return dp[n];
    }
};
```

- **知识点**：**迭代**，**动态规划(DP -- Dynamic Programming)**；

> **动态规划的两个核心特征**
> 1. **重叠子问题**：问题可以分解为子问题，且这些子问题会被重复计算。
> 2. **最优子结构**（这里更准确是“递推结构”）：当前问题的解可以由子问题的解组合而成。

- **时间复杂度 -- $O(n)$，空间复杂度 -- $O(n)$**

- **空间可优化**：实际上，**计算 `dp[i]` 只需要用到 `dp[i-1]` 和 `dp[i-2]`，其他的数不需要保存**；

### 28.4 解法 4 - 滚动数组(迭代 DP 空间优化) -- 空间复杂度 O(1)
优化版本代码如下：
```cpp
class Solution {
public:
    int fib(int n) {
        if (n <= 1) return n;
        int pre2 = 0;               // 前两个位置
        int pre1 = 1;               // 前一个位置
        int curr;
        for (int i = 2; i <= n; ++i) {
            curr = pre2 + pre1;
            pre2 = pre1;
            pre1 = curr;
        }
        return curr;
    }
};
```

> **推荐该种解法**

### 28.5 解法 5 -- 矩阵快速幂
代码如下：
```cpp
class Solution {
private:
    // 2x2 矩阵乘法
    vector<vector<long long>> multiply(const vector<vector<long long>> &A, const vector<vector<long long>> &B) {
        return {
            {   // 第一行
                A[0][0] * B[0][0] + A[0][1] * B[1][0],      // 第一行第一列
                A[0][0] * B[0][1] + A[0][1] * B[1][1]       // 第一行第二列
            }, 
            {   // 第二行
                A[1][0] * B[0][0] + A[1][1] * B[1][0],      // 第二行第一列
                A[1][0] * B[0][1] + A[1][1] * B[1][1]       // 第二行第二列
            }
        };
    }
    // 矩阵快速幂 -- 位运算实现
    vector<vector<long long>> matrixPower(const vector<vector<long long>> &M, int exp) {
        vector<vector<long long>> res = {{1, 0}, {0, 1}};
        vector<vector<long long>> base = M;
        while (exp > 0) {
            // 指数为奇数，提前面的 M
            if (exp & 1) {          // 等价于 exp % 2 == 0
                res = multiply(res, base);
            }
            // 指数为偶数次，进行 M = M^2
            base = multiply(base, base);
            // 位运算更新 exp
            exp >>= 1;              // 右移一位，等价于 exp %= 2;
        }
        return res;
    }
public:
    int fib(int n) {
        if (n <= 1) return n;
        vector<vector<long long>> M = {{1, 1}, {1, 0}};
        // M^(n-1) 的左上角元素即为 F(n)
        vector<vector<long long>> power = matrixPower(M, n - 1);
        return static_cast<int>(power[0][0]);
    }
};
```

- **知识点**：**矩阵快速幂**；

- **时间复杂度 -- $O(log n)$，空间复杂度 -- O(1)**；

- **思路**：

矩阵快速幂是求解斐波那契数列的**对数级**方法，时间复杂度 **O(log n)**。它的核心思想是把递推关系转化为矩阵乘法，再利用快速幂高效计算矩阵的 n 次方。

---

#### 1. 递推关系的矩阵形式
斐波那契数列的递推：
$$
F(n) = F(n-1) + F(n-2)
$$
可以写成矩阵乘法：
$$
\begin{bmatrix}
F(n) \\
F(n-1)
\end{bmatrix}
=
\begin{bmatrix}
1 & 1 \\
1 & 0
\end{bmatrix}
\cdot
\begin{bmatrix}
F(n-1) \\
F(n-2)
\end{bmatrix}
$$
设矩阵 $ M = \begin{bmatrix} 1 & 1 \\ 1 & 0 \end{bmatrix} $，则有：
$$
\begin{bmatrix}
F(n) \\
F(n-1)
\end{bmatrix}
= M \cdot
\begin{bmatrix}
F(n-1) \\
F(n-2)
\end{bmatrix}
= M^2 \cdot
\begin{bmatrix}
F(n-2) \\
F(n-3)
\end{bmatrix}
= \dots = M^{n-1} \cdot
\begin{bmatrix}
F(1) \\
F(0)
\end{bmatrix}
$$
已知 $ F(1) = 1, F(0) = 0 $，所以：
$$
\begin{bmatrix}
F(n) \\
F(n-1)
\end{bmatrix}
= M^{n-1} \cdot
\begin{bmatrix}
1 \\
0
\end{bmatrix}
$$

最终 $ F(n) $ 就是结果矩阵的**第一行第一列元素**（或直接取列向量的第一个分量）。

---

#### 2. 快速幂（二分求幂）
计算 $M^n$ 不需要做 n 次乘法，可以用快速幂将复杂度降到 $O(log n)$：

- 若指数为偶数：$ M^k = (M^{k/2})^2 $
- 若指数为奇数：$ M^k = M \cdot (M^{(k-1)/2})^2 $

每次指数减半，只需 $O(log n)$ 次矩阵乘法。

```cpp
vector<vector<long long>> matrixPower(const vector<vector<long long>> &M, int exp) {
        vector<vector<long long>> res = {{1, 0}, {0, 1}};
        vector<vector<long long>> base = M;
        while (exp > 0) {
            // 指数为奇数，提前面的 M
            if (exp & 1) {          // 等价于 exp % 2 == 0
                res = multiply(res, base);
            }
            // 指数为偶数次，进行 M = M^2
            base = multiply(base, base);
            // 位运算更新 exp
            exp >>= 1;              // 右移一位，等价于 exp %= 2;
        }
        return res;
    }
```

- **理解矩阵快速幂的代码**:
具体例子：**计算 $2^13$**（用矩阵类比）

1. **传统描述**：  
$2^{13} = 2 \cdot (2^2)^6$ （13 是奇数，先提一个 2）  
然后 $2^2$ 平方得 $2^4$，指数 6 是偶数，继续平方……  
最终把遇到的奇数次乘上去。

2. **二进制实现**：  
13 的二进制是 `1101`（从右到左：1、0、1、1）。  
- 最低位=1（奇数），`res *= base`（乘 2^1），`base` 平方成 2^2，指数右移为 `110` (6)。
- 下一位=0（偶数），不乘，`base` 平方成 2^4，指数右移为 `11` (3)。
- 下一位=1（奇数），`res *= base`（乘 2^4），`base` 平方成 2^8，指数右移为 `1` (1)。
- 下一位=1（奇数），`res *= base`（乘 2^8），`base` 平方成 2^16，指数右移为 `0`，退出。

`res` 先后乘了 $2^1$、$2^4$、$2^8$，乘积正好是 $2^{13}$。这完美对应 “若奇则先乘后平方，若偶则只平方” 的规则，只是换了个更直接的实现视角。

- **关于位运算(硬件层面一定更高效)**：
1. **`exp & 1`：等价于 `exp %2 == 1`，但是更高效**；计算过程：1 的二进制除了最低位是 1，其余所有高位全是 0。所以 `exp & 1` 实际上是**把 `exp` 的所有高位清零，只保留最低位的那一位，然后进行与运算**；
2. **`exp >>= 1`：右移一位，等价于 `exp %= 2`(实际编译器会该指令自动转化为右移运算)**；
---

- **关于代码实现**：
1. **`long long` 的使用原因**：
- `int` 通常只有 32 位，能表示的最大值大约 21 亿（$2^{31}-1$），而 `fib(46) ≈ 1.836 亿`，还能塞进 `int`；`fib(47) ≈ 2.97 亿`也还行，但到 **`fib(48)` 就溢出**了。
- `long long` 至少 64 位，能表示的范围大得多（约 9e18），可以安全计算到 `fib(92)` 左右。

2. **`const` 的使用**：**允许接收临时对象** -- 如果调用时传入一个**右值**（比如函数返回的临时矩阵），只有 `const`引用才能绑定到它。去掉 `const` 就绑不上了，会增加拷贝开销或编译错误。

3. **`static_cast<int>` 的使用**：C++ 的**显示类型转换**，类似 C 的 `(int)power[0][0]` ；

## 29. 题号：70 -- 爬楼梯（简单）
### 29.1 错误解法
```cpp
class Solution {
private:
    // 辗转相除法/欧几里得算法 求 最大公约数用于之后组合数的约分(Greatest Common Divisor)
    long long gcd(long long a, long long b) {       // a >= b
        if (b != 0) {
            long long tmp = b;
            b = a % b;
            a = tmp;
        }
        return a;
    }
    // long long + 求组合数 C(n, k) 时边乘边除最大公约数约分的技巧防止溢出
    long long combination(int n, int k) {
        if (k < 0 || k > n) return 0;
        // 利用对称性 C(n, k) = C(n, n - k) 挑选 k 和 n - k 的较小值减少循环次数
        if (k > n - k) k = n - k;
        long long numerator = 1;        // 分子
        long long denominator = 1;      // 分母
        for (int i = k; i >= 1; ++i) {
            numerator *= i;
            denominator *= (n - k + i);
            // 核心优化：边乘边约分
            long long g = gcd(denominator, numerator);      // 注意 a >= b，这里的传值顺序要对应
            numerator /= g;
            denominator /= g;
        }
        return numerator / denominator;
    }
public:
    int climbStairs(int n) {
        int totalWay = 0;
        for (int i = 0; i <= n / 2; ++i) {
            totalWay += combination(n, i);
        }
        return totalWay;
    }
};
```

- **思路**：**讨论数字 2 的不同个数 + 相应个数的数字 2 怎么在 n 中排列组合**;

- **错误点**：**超时**；

- **注意点**：
1. 题目中 `n` 最大可以达到 45，$C_n^{k}$ 这个组合数会非常大（约 $4.1 \times 10^{12}$），这个值已经远远超出了 `int` 类型的范围，**必须使用 `long long` 类型来存放中间结果防止溢出**；
2. **边乘边约分防止溢出**；
3. **辗转相除法(欧几里得算法)原理**：求**最大公约数(`Greatest Common Divisor`)**
(1) **原理推导**

设我们要计算 `gcd(a, b)`，且 `a ≥ b > 0`。  
根据带余除法，存在唯一整数商 `q` 和余数 `r`，使得：
$$
a = b \cdot q + r, \quad 0 \le r < b
$$

**关键性质**：  
- 如果 `d` 能同时整除 `a` 和 `b`（即 `d | a` 且 `d | b`），则 `d` 必定也能整除 `r`（因为 `r = a - bq`）。  
- 反之，如果 `d` 能整除 `b` 和 `r`，则 `d` 也一定能整除 `a`。

因此，**`a` 和 `b` 的公约数集合与 `b` 和 `r` 的公约数集合完全相同**，所以它们的 **最大公约数也相等**：

$$
\gcd(a, b) = \gcd(b, r)
$$

其中 `r = a \b mod b`。

(2) **高效性**

- 每迭代一次，余数至少减半（更严格地，余数 `r < b/2` 或下一次迭代会快速减小），因此迭代次数为 `O(log min(a,b))`。
- 只需要整数除法和取模运算，非常适合编程实现。

(3) **特殊情况处理**

- 如果 `b = 0`，则 `gcd(a, 0) = a`（按定义，任何数整除 0）。
- 如果 `a = b = 0`，通常定义 GCD 为 0（视具体需求）。

> **大一一直不懂辗转相除法的原理，看很久都不懂，现在一会就看懂了。**
> 
> **我想说的是：不必纠结于一时的怅惘，不妨向前走，回首处，也无风雨也无晴！**

### 29.2 正确解法及分析
代码如下：
```cpp
class Solution {
public:
    int climbStairs(int n) {
        if (n <= 2) return n;
        int prev = 1;           // 与 fibonacci 不同的是前两个值
        int curr = 2;    
        for (int i = 3; i <= n; ++i) {
            int next = prev + curr;
            prev = curr;
            curr = next;
        }
        return curr;
    }
};
```

- **知识点**：**斐波那契数列**，**DP**，**滚动数组**；

- **时间复杂度 -- $O(n)$，空间复杂度 -- $O(1)$**

- **思路**：关键在于发现 **状态转移方程**：

`爬到第 n 阶的方法数 = 爬到第 n-1 阶的方法数（再爬 1 步） + 爬到第 n-2 阶的方法数（再爬 2 步）`;

即：`dp[n] = dp[n-1] + dp[n-2]`，初始条件 `dp[1] = 1`, `dp[2] = 2`，所以 **该题为斐波那契数列的“马甲题”**。

- **注意点(与 `Fibonacci` 数列稍有不同)**：
1. **`n <= 2`(`fib` 是 `n <= 1`) 直接返回 `n`**；
2. **初始化的两个值** 是 1 和 2 (`fib` 是 0 和 1)；
3. **`i` 从 3 开始** (`fib` 是从 2 开始)；

## 30. 题号：121 -- 买卖股票的最佳时机（简单）
### 30.1 错误解法 1 -- 暴力枚举(超时)
```cpp
class Solution {
public:
    int maxProfit(vector<int> prices) {
        int max = 0;
        for (int i = 0; i < prices.size() - 1; ++i) {
            for (int j = i + 1; j < prices.size(); ++j) {
                if (max < prices[j] - prices[i]) {
                    max = prices[j] - prices[i];
                }
            }
        }
        return max;
    }
};
```

- **思路**：问题等价于：**在数组中找两个位置 `i < j`，使得 `prices[j] - prices[i]` 最大**；

### 30.2 解法 2 -- 贪心
代码如下：
```cpp
class Solution {
public:
    int maxProfit(vector<int> prices) {
        int min = prices[0], maxProfit = 0;
        for (int i = 1; i < prices.size(); ++i) {
            maxProfit = maxProfit < (prices[i] - min) ? (prices[i] - min) : maxProfit;
            min = min > prices[i] ? prices[i] : min;
        }
        return maxProfit;
    }
};
```

- **知识点**：**贪心**；

- **时间复杂度 -- $O(n)$，空间复杂度 -- $O(1)$**

- **思路**：**在遍历每一天时，把当天当做“潜在卖出日”，根据前面出现过的最低价格（即最佳买入价），求出到这一天的最大利润，遍历完数组(所有天)，即求得题目要的最大利润**;

### 30.3 解法 3 -- DP / Kadane 算法变体
代码如下：
```cpp
class Solution {
public:
    int maxProfit(vector<int> prices) {
        if (prices.empty()) return 0;
        int currProfit = 0;         // 当前累计利润
        int maxProfit = 0;
        for (int i = 1; i < prices.size(); ++i) {
            currProfit = (currProfit + prices[i] - prices[i - 1]) > 0 ? (currProfit + prices[i] - prices[i - 1]) : 0;
            maxProfit = maxProfit > currProfit ? maxProfit : currProfit;
        }
        return maxProfit;
    }
};
```

- **知识点**：**DP**，**Kadane 算法变体**；

> `Kadane` 算法是一种用于 **求解最大子数组和**（Maximum Subarray Sum）问题的经典 **动态规划** 算法。
> 
> 它的核心思想非常简洁：遍历数组，维护两个变量 -- **当前子数组的最大和**，以及 以**当前位置结尾的最大子数组和**。

- **时间复杂度 -- $O(n)$，空间复杂度 -- $O(1)$**

- **思路**：同上，只是上一种算法用 `min` 存当前最小值，该算法用 `currProfit` 存当前利润;

---
