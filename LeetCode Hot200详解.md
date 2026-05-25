# LeetCode Hot200 刷题文档 — 阶段一：入门（30题）

---

## 1. 两数之和 (Two Sum) — 题号 1

**难度**: 简单
**标签**: 数组、哈希表

### 题目大意

给定一个整数数组 `nums` 和一个整数目标值 `target`，请你在该数组中找出和为目标值 `target` 的那两个整数，并返回它们的数组下标。假设每种输入只会对应一个答案，且同一个元素不能使用两遍。

### 思路

拿到这道题，最朴素的想法是两层循环枚举所有数对 `(i, j)`，检查 `nums[i] + nums[j] == target`。但这样复杂度是 O(n^2)，在 n 较大时不可接受。

优化的关键洞察是：当我们遍历到位置 `i` 时，我们想知道"前面已经遍历过的数中，是否存在一个数等于 `target - nums[i]`"。这是一个典型的"查找某元素是否存在"的问题——哈希表正是处理 O(1) 查找的最佳工具。因此，我们用一个哈希表（C++ 中的 `unordered_map`）存储"数值到下标"的映射，遍历时在哈希表中查找 `target - nums[i]`，若找到则立即返回，否则将当前值存入哈希表。这就是"空间换时间"的经典应用。

### 核心知识点

- 哈希表 `unordered_map` 的 O(1) 查找
- 空间换时间思想
- 一遍遍历（而非两遍，减少常数因子）

### 复杂度

- **时间复杂度**: O(n) — 只需遍历一次数组
- **空间复杂度**: O(n) — 最坏情况下哈希表存储 n 个元素

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<int> twoSum(vector<int>& nums, int target) {
    // 哈希表：数值 -> 下标
    unordered_map<int, int> mp;
    // 一遍遍历：边查边存
    for (int i = 0; i < nums.size(); i++) {
        int need = target - nums[i]; // 我需要配对的数
        // 如果这个数已经出现过，直接返回答案
        if (mp.find(need) != mp.end()) {
            return {mp[need], i};
        }
        // 否则把当前数记录到哈希表中，供后续使用
        mp[nums[i]] = i;
    }
    return {}; // 题目保证有解，此行不会执行
}
```

### 常见错误

- 错误1：先将所有元素全部存入哈希表再查找，这会导致当 `nums[i] == target - nums[i]`（即自身配对，如 target=6, 元素=3）时，可能取到自己的下标。正确做法是边遍历边查找，保证查找的是"前面的"元素。
- 错误2：使用 `map` 而不是 `unordered_map`，导致查找变成 O(log n)，虽然也能通过但效率较低。
- 错误3：忘记在找不到答案时返回空数组（虽然题目保证有解，但编译器可能对无返回值代码报 warning）。

### 面试陷阱

- 陷阱1：面试官追问"如果要求返回所有可能的数对，且不允许重复使用元素，怎么做？"——这变成了组合求和问题，需要用排序+双指针或回溯法，复杂度不再是 O(n)。
- 陷阱2：面试官问"如果数组有序，有没有更优解法？"——有序数组可以用双指针（一头一尾往中间夹），时间复杂度 O(n)，空间复杂度 O(1)。
- 陷阱3：面试官追问"哈希冲突严重时复杂度退化到 O(n) 怎么办？"——在 C++ 中 `unordered_map` 通常使用链地址法处理冲突，最坏情况确实退化为 O(n)，但均摊仍是 O(1)。可以提到如果数据规模极大，可以用平衡树（`map`）保证 O(log n)。

---

## 2. 移动零 (Move Zeroes) — 题号 283

**难度**: 简单
**标签**: 数组、双指针

### 题目大意

给定一个整数数组 `nums`，将所有 0 移动到数组的末尾，同时保持非零元素的相对顺序。要求原地操作，不能拷贝额外的数组。

### 思路

直观想法：遇到一个 0，就把它后面的所有元素往前移一位，然后把 0 放到末尾。但这样每个 0 都要移动 O(n) 次，总复杂度 O(n^2)。

换一个角度想：我们关心的不是"0 在哪里"，而是"非零元素应该在哪里"。如果忽略所有的 0，剩下的非零元素按顺序紧凑排列在数组前部即可。于是引入**快慢指针**：
- 慢指针 `slow` 指向"下一个非零元素应该放的位置"；
- 快指针 `fast` 遍历数组，遇到非零元素就交换到 `slow` 位置，然后 `slow++`。

这样，每次交换最多涉及一个非零元素和 slow 位置的 0，保证了非零元素的相对顺序不变。

### 核心知识点

- 双指针之快慢指针
- 原地操作、交换技巧
- "保持相对顺序"的通用套路

### 复杂度

- **时间复杂度**: O(n) — 一次遍历
- **空间复杂度**: O(1) — 只用了两个指针

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

void moveZeroes(vector<int>& nums) {
    int slow = 0; // 慢指针：下一个非零元素要放置的位置
    // 快指针遍历整个数组
    for (int fast = 0; fast < nums.size(); fast++) {
        if (nums[fast] != 0) {
            // 将非零元素交换到 slow 位置，保证顺序不变
            swap(nums[slow], nums[fast]);
            slow++; // slow 后移，指向下一个"空位"
        }
    }
    // 循环结束后，slow 及其后面的位置自然都是 0
}
```

### 常见错误

- 错误1：写成覆盖+填充两遍的方式（先把非零移到前面，再把后面全赋 0），虽然正确但多了一次遍历。交换法只需一次遍历。
- 错误2：当 slow 和 fast 指向同一位置时也执行交换，虽然不影响正确性但做了无用功（可用 `if (slow != fast)` 优化，但非必须）。
- 错误3：遍历方向错误——从后往前遍历会破坏相对顺序。

### 面试陷阱

- 陷阱1：面试官追问"如果不允许交换操作，只允许赋值，怎么做？"——那只能用两遍覆盖方式：快指针收集非零元素填充前面，最后把剩余位置置零。
- 陷阱2：面试官问"如果要求 0 移到前面而不是后面？"——思路对称，快慢指针，只是把非零替换为零即可。
- 陷阱3：追问"如果数组中不是 0，而是一个特定值的多个重复元素，要求按此移动？"——将判断条件从 `!= 0` 改为 `!= target` 即可，思路完全一致。

---

## 3. 删除有序数组中的重复项 (Remove Duplicates from Sorted Array) — 题号 26

**难度**: 简单
**标签**: 数组、双指针

### 题目大意

给定一个升序排列的数组 `nums`，原地删除所有重复出现的元素，使每个元素只出现一次，并返回移除后数组的新长度。不要求改变数组长度，只需保证前 k 个元素是去重后的结果。

### 思路

数组已经**有序**，这是关键条件。有序意味着相同的元素一定相邻。所以问题转化为"保留每个不相等的元素"。

因为要去重并保持顺序，经典的快慢指针再次适用：
- 慢指针 `slow` 指向"已去重部分"的最后一个位置；
- 快指针 `fast` 从第二个元素开始遍历，每当遇到一个与 `nums[slow]` 不同的元素时，说明发现了一个新的唯一值，把它放到 `++slow` 的位置。

为什么慢指针初始化为 0？因为第一个元素始终保留，从第二个元素开始判断是否重复。

### 核心知识点

- 双指针之快慢指针（覆盖式）
- 有序数组的去重模式
- 原地修改、返回逻辑长度

### 复杂度

- **时间复杂度**: O(n) — 一次遍历
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

int removeDuplicates(vector<int>& nums) {
    if (nums.empty()) return 0;
    int slow = 0; // 慢指针：已去重部分的最后一个位置
    // 快指针从 1 开始，去发现新的不重复元素
    for (int fast = 1; fast < nums.size(); fast++) {
        // 只要当前元素与慢指针元素不同，就把它"纳入"去重队列
        if (nums[fast] != nums[slow]) {
            slow++;                // 扩展去重区域
            nums[slow] = nums[fast]; // 放置新元素
        }
    }
    // 返回的是数量，slow 是下标（从0开始），所以 +1
    return slow + 1;
}
```

### 常见错误

- 错误1：忘记判断空数组，导致 `nums[0]` 越界。
- 错误2：直接在原数组上 `erase` 元素，vector 的 `erase` 是 O(n) 的，导致总复杂度变成 O(n^2)。
- 错误3：返回 `slow` 而不是 `slow + 1`。`slow` 是下标索引，元素数量需要 `+1`。

### 面试陷阱

- 陷阱1：面试官追问"如果要求每个元素最多出现两次呢？"——这是 LeetCode 80，判断条件变为 `nums[fast] != nums[slow - 1]`（与前两个已保留位置的最后一个比较）。
- 陷阱2：面试官问"如果数组无序怎么办？"——那需要先用哈希表记录元素出现情况，或者先排序再双指针（排序 O(n log n)），无法 O(n) 原地。
- 陷阱3：面试官追问"你能证明该算法不会丢失任何不重复元素吗？"——核心是数组有序保证了重复元素相邻，快指针不会跳过任何新值。

---

## 4. 移除元素 (Remove Element) — 题号 27

**难度**: 简单
**标签**: 数组、双指针

### 题目大意

给定一个数组 `nums` 和一个值 `val`，原地移除所有数值等于 `val` 的元素，并返回移除后数组的新长度。元素的顺序可以改变，不考虑超出新长度后面的元素。

### 思路

本题和"移动零"几乎一样：把所有非 `val` 的元素移到前面。思路完全对称——快慢指针，快指针遍历，慢指针记录放置位置。快指针遇到非 `val` 的元素就覆盖慢指针位置，然后慢指针后移。

但还有一个更巧妙的优化：题目说"元素的顺序可以改变"，这意味着我们可以用**首尾双指针**：用尾部的非 val 元素"填补"头部遇到的 val 空缺。每次找到头部的 val，就用尾部的非 val 元素覆盖它，然后收缩尾部。这样当 val 很少时，只需很少的交换次数。

### 核心知识点

- 双指针之快慢指针 / 首尾指针
- 原地覆盖技巧
- "顺序可改变"带来的优化思路

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法一：快慢指针（保持顺序）
int removeElement_v1(vector<int>& nums, int val) {
    int slow = 0;
    for (int fast = 0; fast < nums.size(); fast++) {
        if (nums[fast] != val) {
            nums[slow] = nums[fast];
            slow++;
        }
    }
    return slow;
}

// 解法二：首尾双指针（不保持顺序，但减少赋值次数）
int removeElement_v2(vector<int>& nums, int val) {
    int left = 0, right = nums.size() - 1;
    while (left <= right) {
        if (nums[left] == val) {
            // 用尾部的非 val 元素覆盖
            nums[left] = nums[right];
            right--;
        } else {
            left++;
        }
    }
    return left; // left 指向第一个"无效"位置，恰好等于新长度
}
```

### 常见错误

- 错误1：首尾指针版本中，覆盖后没有再次检查 `nums[left]`（因为覆盖过来的尾部元素也可能是 val），正确做法是 `left` 不变，下一轮循环会重新检查。所以此处 `left++` 只在 `!= val` 时执行。
- 错误2：返回 `left + 1`（快慢指针版本中返回 slow 即可，因为 slow 维护的就是"个数"）。
- 错误3：循环条件用 `left < right` 而非 `left <= right`，导致最后一个元素未被检查。

### 面试陷阱

- 陷阱1：面试官追问"如果必须保持相对顺序，哪种写法更优？"——快慢指针保持顺序，首尾指针会打乱顺序。
- 陷阱2：面试官问"如果 val 很少，哪种写法赋值次数更少？"——首尾指针版本：只有遇到 val 时才赋值，而非 val 元素原封不动。
- 陷阱3：追问"对于 C++ STL 的 `std::remove` 你了解吗？它和这道题有什么关系？"——`std::remove` 用的就是快慢指针的双指针算法，它返回新末尾的迭代器（搭配 `erase` 真正删除）。

---

## 5. 合并两个有序数组 (Merge Sorted Array) — 题号 88

**难度**: 简单
**标签**: 数组、双指针

### 题目大意

给定两个升序整数数组 `nums1` 和 `nums2`，将 `nums2` 合并到 `nums1` 中，使 `nums1` 成为一个升序数组。`nums1` 的初始长度为 `m + n`，其中前 `m` 个元素是有效数据，后面 `n` 个位置是 0（占位符，用于存放 `nums2`）。

### 思路

如果像常规归并那样从前往后遍历，将较小值放入 `nums1` 前面，就需要频繁移动 `nums1` 的已有元素，效率低下。

**关键洞察**：`nums1` 的后半部分全是空的占位符，这天然适合**从后往前填充**。我们从两个数组的有效末尾（分别指向 `m-1` 和 `n-1`）开始比较，将较大值放到 `nums1` 的末尾（`m+n-1` 位置），依次向前填充。这样每个元素只需移动一次，完全避免了额外空间和数组元素的后移开销。

为什么能想到"从后往前"？因为题目特别给了 `nums1` 后面 n 个空位，这正是暗示"从尾部构建结果"——前面有数据堵着，但后面是空的，自然从后往前。

### 核心知识点

- 逆向双指针 / 从后往前归并
- 数组尾部空间利用
- 归并排序的合并思想

### 复杂度

- **时间复杂度**: O(m + n) — 每个元素访问一次
- **空间复杂度**: O(1) — 原地合并

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
    // p: nums1 有效部分的末尾; q: nums2 的末尾
    int p = m - 1, q = n - 1;
    int tail = m + n - 1; // 结果数组的末尾（即 nums1 的实际末尾）
    // 从后往前归并
    while (p >= 0 && q >= 0) {
        if (nums1[p] > nums2[q]) {
            nums1[tail] = nums1[p];
            p--;
        } else {
            nums1[tail] = nums2[q];
            q--;
        }
        tail--;
    }
    // 如果 nums2 还有剩余，拷贝到 nums1 的前部
    // （如果是 nums1 有剩余，则无需操作，因为已经在正确位置）
    while (q >= 0) {
        nums1[tail] = nums2[q];
        tail--;
        q--;
    }
}
```

### 常见错误

- 错误1：试图从前往后填充，结果覆盖了 `nums1` 中尚未使用的有效数据。
- 错误2：两个 while 循环条件写反——`nums1` 剩余时不需要处理（它已经在前面位置），但 `nums2` 剩余时必须拷贝。
- 错误3：索引计算错误，`tail` 初始值写成 `m + n`（越界），正确是 `m + n - 1`。

### 面试陷阱

- 陷阱1：面试官追问"为什么 `nums2` 剩余时才需要拷贝，而 `nums1` 剩余时不需要？"——因为结果数组在 `nums1` 上原地构建，`nums1` 的剩余元素本身就在正确位置（数组的前部）。
- 陷阱2：面试官追问"如果给你两个链表，怎么做？"——链表没有"尾部空位"的优势，需要新建链表进行归并（或修改指针进行原地归并），复杂度不变但实现完全不同。
- 陷阱3：面试官追问"时间复杂度和空间复杂度能进一步优化吗？"——已经是理论最优，每个元素必须至少被读取一次。

---

## 6. 存在重复元素 (Contains Duplicate) — 题号 217

**难度**: 简单
**标签**: 数组、哈希表

### 题目大意

给定一个整数数组，判断是否存在重复元素。如果存在任一值在数组中至少出现两次，返回 `true`；如果数组中每个元素各不相同，返回 `false`。

### 思路

最朴素的想法：对每一对元素做比较，O(n^2)。

稍微优化：先排序 O(n log n)，然后相邻元素比较 O(n)，总 O(n log n)。

进一步优化：如果可以在 O(1) 时间内判断一个元素是否已经出现过，就可以一遍扫描得到答案。这就是哈希表。遍历数组，如果元素已在集合中则返回 `true`，否则将其插入集合。C++ 中使用 `unordered_set`。

### 核心知识点

- `unordered_set` 的插入与查找均为 O(1)
- 去重判断的标准哈希表用法
- 排序+相邻比较的备选方案

### 复杂度

- **时间复杂度**: O(n)（哈希表）/ O(n log n)（排序）
- **空间复杂度**: O(n)（哈希表）/ O(1) 或 O(log n)（排序，取决于排序算法）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

bool containsDuplicate(vector<int>& nums) {
    unordered_set<int> seen; // 记录已经见过的元素
    for (int x : nums) {
        // 如果之前已经见过，说明有重复
        if (seen.find(x) != seen.end()) {
            return true;
        }
        seen.insert(x);
    }
    return false;
}

// 备选写法：一行流（利用 set 的 insert 返回 pair<iterator, bool>）
bool containsDuplicate_v2(vector<int>& nums) {
    unordered_set<int> s;
    for (int x : nums) {
        // insert 返回的 pair 的 second 为 false 表示插入失败（已存在）
        if (!s.insert(x).second) return true;
    }
    return false;
}
```

### 常见错误

- 错误1：使用 `unordered_set.insert()` 后没有检查返回值，而是再次调用 `find`，多做了一次哈希查找。
- 错误2：在哈希表中存入 `pair<int, int>` 之类的复杂类型但没有提供自定义哈希函数。
- 错误3：误用 `set`（有序集合，O(log n) 插入），也能通过但效率略低。

### 面试陷阱

- 陷阱1：面试官追问"如果数据量非常大（数十亿），内存放不下所有元素怎么办？"——可以使用布隆过滤器（Bloom Filter），它是一种概率型数据结构，可能出现假阳性但不会出现假阴性，空间效率极高。
- 陷阱2：面试官追问"如果要求找出所有重复元素，而非仅判断是否存在？"——哈希表存计数，遍历后筛选出 `count > 1` 的元素。
- 陷阱3：追问"排序法和哈希表法在什么场景下各有优势？"——排序法空间 O(1)，适合内存紧张场景；哈希表法时间 O(n)，适合追求速度且内存充裕场景。

---

## 7. 只出现一次的数字 (Single Number) — 题号 136

**难度**: 简单
**标签**: 数组、位运算、哈希表

### 题目大意

给定一个非空整数数组，除了某个元素只出现一次以外，其余每个元素均出现两次。找出那个只出现了一次的元素。要求线性时间复杂度，且只使用常量额外空间。

### 思路

如果用哈希表统计频率，时间 O(n)、空间 O(n) 可以轻松解决。但题目要求常量空间，哈希表不符合。

**关键洞察**：所有出现两次的元素需要某种方式"抵消"掉。什么样的运算能满足 `a ☆ a = 0`（两个相同值运算后消失），且 `a ☆ 0 = a`（零和任何值运算后不变），且满足交换律和结合律？

答案是**异或 XOR**：
- `a ⊕ a = 0`
- `a ⊕ 0 = a`
- 交换律：`a ⊕ b = b ⊕ a`
- 结合律：`(a ⊕ b) ⊕ c = a ⊕ (b ⊕ c)`

因为所有出现两次的元素都会成对抵消为 0，最终剩下的就是那个唯一出现一次的元素。这个思路非常精巧，前提是你能联想到 XOR 的自反性。

### 核心知识点

- 异或 Xor 的自反性：`a ⊕ a = 0`
- 位运算解决问题的一般套路
- 交换律和结合律保证抵消与顺序无关

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

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
```

### 常见错误

- 错误1：忘记了异或满足交换律和结合律，误以为异或顺序会影响结果。实际上不会，因为 XOR 是可交换的结合操作。
- 错误2：初始值设为 `nums[0]` 然后从下标 1 开始遍历。虽然正确但不如 `result = 0` 然后从头遍历简洁（`0 ^ a = a`）。
- 错误3：错误地认为此方法适用于出现奇数次的元素。实际上此方法仅适用于"恰好一个出现一次，其余出现偶数次"。

### 面试陷阱

- 陷阱1：面试官追问"如果有两个元素各出现一次呢？"——这是 LeetCode 260，需要用分组异或法：先全部异或得到这两个数的异或值，然后根据某一位为 1 将数组分成两组，每组分别异或。
- 陷阱2：面试官追问"如果某个元素出现三次，其余出现一次呢？"——LeetCode 137，XOR 不再适用，需要按位统计每个 bit 出现的次数（模 3），或者使用有限状态自动机。
- 陷阱3：面试官追问"有没有其他位运算也可以做到？"——本质上需要满足群的性质（每个元素是自己的逆元），异或是唯一简单且高效的选择。

---

## 8. 多数元素 (Majority Element) — 题号 169

**难度**: 简单
**标签**: 数组、摩尔投票

### 题目大意

给定一个大小为 n 的数组，找到其中的多数元素。多数元素是指在数组中出现次数大于 n/2 的元素。假设数组非空，且多数元素一定存在。

### 思路

哈希表统计频率—— O(n) 时间 + O(n) 空间，可以解决。

排序后取中间元素—— O(n log n) 时间 + O(1) 空间，也可解决（因为多数元素一定占据 n/2+1 个位置，排序后必然在中间位置）。

**摩尔投票法（Boyer-Moore Voting）**：这是本题的最优解。核心直觉：多数元素比所有其他元素加起来还多。如果我们将"多数元素"和"非多数元素"两两配对抵消，最后能剩下来的只有多数元素（因为它数量最多）。

具体实现：维护一个 `candidate`（候选元素）和一个 `count`（票数）。遍历数组：
- 若 `count == 0`，更换 `candidate` 为当前元素；
- 若当前元素等于 `candidate`，`count++`；
- 若当前元素不等于 `candidate`，`count--`（相当于一个多数元素和一个非多数元素"同归于尽"）。

最终 `candidate` 就是多数元素。注意：题目保证多数元素一定存在，否则需要第二轮验证。

为什么能想到这种解法？"出现次数超过一半"等价于"它比其他所有元素加起来还多"，这自然引出配对抵消的思想。

### 核心知识点

- 摩尔投票算法（Boyer-Moore Voting）
- 多数元素的数学性质：出现次数 > n/2
- 抵消思想

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

int majorityElement(vector<int>& nums) {
    int candidate = 0; // 候选人
    int count = 0;     // 票数
    // 摩尔投票：多数元素最终不会被抵消掉
    for (int x : nums) {
        if (count == 0) {
            candidate = x; // 重新推选候选人
        }
        // 当前元素和候选人相同时加票，不同时减票（抵消一对）
        count += (x == candidate) ? 1 : -1;
    }
    // 题目保证多数元素一定存在，因此 candidate 就是答案
    // 如果不保证，需要再遍历一次验证 candidate 的出现次数 > n/2
    return candidate;
}
```

### 常见错误

- 错误1：在 count == 0 时更新 candidate 后忘记将 count 设为 1（后面 `count += ...` 包括了这种情况，但如果写法不同需注意）。
- 错误2：认为摩尔投票法对所有"找出现次数最多的元素"问题都适用。它只适用于"多数元素（出现次数 > n/2）"的情况，对其他频率问题不适用。
- 错误3：在题目没有保证多数元素一定存在时，遗漏了第二轮验证。

### 面试陷阱

- 陷阱1：面试官追问"如果要求找出所有出现次数超过 n/3 的元素呢？"——这是 LeetCode 229，扩展摩尔投票：维护两个候选人和两个计数器，最后至多有两个答案。
- 陷阱2：面试官追问"摩尔投票法的正确性证明？"——核心是多数元素比其他所有元素之和还多，所以即使每次都有一对"多数+其他"被抵消，多数元素依然能有剩余。严格证明可以用数学归纳法。
- 陷阱3：面试官追问"如果数组是流式输入（不能一次性全部读取），摩尔投票还能用吗？"——可以，摩尔投票本身就是 O(1) 空间、单遍扫描，天然适合流式数据。

---

## 9. 反转链表 (Reverse Linked List) — 题号 206

**难度**: 简单
**标签**: 链表、双指针

### 题目大意

给定单链表的头节点 `head`，将链表反转，并返回反转后的头节点。

### 思路

反转链表的本质是将每个节点的 `next` 指针反向指回前驱节点。但反转后你会丢失原来的后继节点，所以需要一个变量保存它。

**迭代法**：使用三个指针——`prev`（前驱）、`curr`（当前）、`nxt`（后继）。每一步：
1. 先保存 `curr->next` 到 `nxt`（防止断链）
2. 将 `curr->next` 指向 `prev`（反转）
3. `prev` 和 `curr` 都向前移动一位

为什么能想到三指针？反转操作需要两两之间改向，"保存后继"是修改指针链表操作中的常见前置动作。

**递归法**（扩展）：递归到最后一个节点作为新头，然后在回溯过程中逐层将 `head->next->next = head`、`head->next = nullptr`。

### 核心知识点

- 链表指针操作的基本安全模式：先保存再修改
- 三指针滑动窗口
- 迭代 vs 递归的权衡

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(1)（迭代）/ O(n)（递归栈）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr; // 前驱节点，初始为空（反转后原头节点指向空）
    ListNode* curr = head;    // 当前节点
    // 三指针迭代反转
    while (curr != nullptr) {
        ListNode* nxt = curr->next; // 1. 先保存后继，防止断链
        curr->next = prev;          // 2. 反转指针
        prev = curr;                // 3. prev 向前移
        curr = nxt;                 // 4. curr 向前移
    }
    // 循环结束时 curr == nullptr，prev 指向原链表的最后一个节点，即新头
    return prev;
}

// 递归写法（扩展）
ListNode* reverseListRecursive(ListNode* head) {
    // 空链表或只有一个节点时，不需要反转
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    ListNode* newHead = reverseListRecursive(head->next); // 递归反转后续
    head->next->next = head; // 让后继节点指回自己
    head->next = nullptr;    // 自己的 next 置空（等待上一层处理）
    return newHead;
}
```

### 常见错误

- 错误1：在修改 `curr->next = prev` 之前忘记保存 `curr->next`，导致链表断裂、无法继续遍历。
- 错误2：`prev` 初始化为 `nullptr` 而非 `head`，导致反转后原头节点指向自己形成环。
- 错误3：递归版本中忘记将 `head->next` 置为 `nullptr`，导致链表成环（原第一个和第二个节点互相指向）。

### 面试陷阱

- 陷阱1：面试官追问"反转链表的前 K 个节点怎么做？"——需要先切断第 K 个节点的 `next`（保存后半段），反转前 K 个，然后重新连接。
- 陷阱2：面试官追问"如何反转链表的一部分（区间 [m, n]）？"——LeetCode 92，需要定位到第 m-1 个节点，然后用头插法或三指针反转区间。
- 陷阱3：面试官追问"数组和链表反转的根本区别是什么？"——数组随机访问反转容易（双指针交换），链表需要改指针方向，操作的是 `next` 而非值。

---

## 10. 合并两个有序链表 (Merge Two Sorted Lists) — 题号 21

**难度**: 简单
**标签**: 链表、双指针

### 题目大意

将两个升序链表合并为一个新的升序链表。新链表是通过拼接给定的两个链表的所有节点组成的。

### 思路

类似于合并两个有序数组（题 88），但链表天然适合"挑选较小节点并连接"的操作——不需要像数组那样担心空间分配。

我们用一个**哑节点（dummy node）**作为新链表的头前哨兵，然后用一个 `tail` 指针指向新链表的末尾。每次比较两个链表当前节点的值，将较小的那个接到 `tail` 后面，然后 `tail` 和该链表的指针都后移一位。循环直到其中一个链表为空，最后把剩余链表直接拼接到 `tail` 后面。

为什么使用哑节点？因为我们需要处理"新链表的第一个节点如何确定"的问题——如果没有哑节点，第一个节点需要特殊处理（判断是取 l1 还是 l2）。哑节点统一了逻辑，使代码简洁。

### 核心知识点

- 哑节点（dummy node）简化链表操作
- 双指针归并
- 链表的尾插法

### 复杂度

- **时间复杂度**: O(m + n) — 遍历两个链表的所有节点
- **空间复杂度**: O(1) — 只使用了常量个额外指针

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    // 哑节点：简化头节点的处理，避免判断"第一个节点从哪来"
    ListNode dummy(0);
    ListNode* tail = &dummy; // tail 始终指向新链表的最后一个节点
    // 两个链表都还有节点时，取较小者接入新链表
    while (l1 != nullptr && l2 != nullptr) {
        if (l1->val <= l2->val) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next; // tail 后移，指向新末尾
    }
    // 将剩余链表直接拼接到尾部（最多只有一个链表非空）
    tail->next = (l1 != nullptr) ? l1 : l2;
    // dummy.next 才是真正的头节点
    return dummy.next;
}
```

### 常见错误

- 错误1：忘记 `tail = tail->next`，导致 `tail` 始终指向哑节点，新节点不断被覆盖。
- 错误2：使用 `new` 创建新节点复制值，而不是直接复用原链表节点。这浪费了空间且不符合"拼接"的语义。
- 错误3：返回 `&dummy` 或 `dummy.next` 写错——应返回 `dummy.next`（真正的头节点），`dummy` 本身是栈上的局部变量，函数结束后失效，但它的 `next` 指针仍然有效。

### 面试陷阱

- 陷阱1：面试官追问"递归写法怎么写？"——递归版的思路更简洁：`if (l1->val < l2->val) l1->next = mergeTwoLists(l1->next, l2); return l1;`。但递归可能导致栈溢出，迭代更适合链表很长的情况。
- 陷阱2：面试官追问"合并 K 个有序链表怎么做？"——LeetCode 23，可以用优先队列（小顶堆）或分治法。
- 陷阱3：面试官追问"为什么合并链表用哑节点比合并数组更常见？"——数组有索引可以直接访问，而链表没有索引，处理"第一个元素"时需要特殊处理，哑节点消除了这种特殊处理。

---

## 11. 环形链表 (Linked List Cycle) — 题号 141

**难度**: 简单
**标签**: 链表、双指针、哈希表

### 题目大意

给定一个链表，判断链表中是否有环。如果链表中有某个节点，可以通过连续跟踪 `next` 指针再次到达，则链表中存在环。

### 思路

**方法一（哈希表）**：遍历链表，用哈希表记录访问过的节点。如果遇到已经在哈希表中的节点，说明有环。时间 O(n)，空间 O(n)。

**方法二（快慢指针 / Floyd 判圈算法）**：这是空间 O(1) 的经典解法。设置两个指针，慢指针每次走一步，快指针每次走两步。如果链表中有环，快指针最终会追上慢指针（在环中相遇）；如果没有环，快指针会先到达 `nullptr`。

为什么快指针每次走两步？如果走一步，快慢指针速度相同永远不会相遇；如果走两步，在环中快指针相对于慢指针的速度差为 1，最多在环长步内一定能相遇（每次步进追 1 步）。如果走三步或更多，可能会跳过慢指针，需要更多判断。

为什么能想到快慢指针？"判断环"的问题本质是"在同一路径上，速度不同的两个对象是否会相遇"，这来自现实中的跑道追及问题。

### 核心知识点

- Floyd 判圈算法（龟兔赛跑算法）
- 快慢指针的步长选择：步长差 = 1 保证相遇
- 哈希表判重

### 复杂度

- **时间复杂度**: O(n)（快慢指针最多走 n + k 步，其中 k 是环长）
- **空间复杂度**: O(1)（快慢指针）/ O(n)（哈希表）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

bool hasCycle(ListNode *head) {
    // 空链表或单节点无环链表不可能有环
    if (head == nullptr || head->next == nullptr) {
        return false;
    }
    ListNode* slow = head;       // 慢指针，一次走一步
    ListNode* fast = head->next; // 快指针，一次走两步
    // 如果快指针追上慢指针，说明有环
    while (slow != fast) {
        // 快指针到达末尾，说明无环
        if (fast == nullptr || fast->next == nullptr) {
            return false;
        }
        slow = slow->next;       // 慢指针走一步
        fast = fast->next->next; // 快指针走两步
    }
    // slow == fast，相遇了，说明有环
    return true;
}
```

### 常见错误

- 错误1：`slow` 和 `fast` 初始都设为 `head`，然后循环条件用 `while (fast && fast->next)`。这样一开始就 `slow == fast`，循环直接退出返回 true。正确做法要么初始错开，要么用 do-while。
- 错误2：在 `fast->next->next` 之前只检查了 `fast` 非空，没有检查 `fast->next` 非空，导致空指针访问。
- 错误3：快指针走三步或更多，导致在环中错过慢指针，判断逻辑变复杂。

### 面试陷阱

- 陷阱1：面试官追问"不仅判断是否有环，还要找出环的入口节点，怎么做？"——LeetCode 142，当快慢指针相遇后，让一个新指针从头开始走（和慢指针同速），相遇点就是环入口。这可以通过数学推导证明。
- 陷阱2：面试官追问"为什么快指针最多比慢指针多走 n 步就能相遇？"——因为慢指针进入环时，快指针已经在环中某个位置。环长最多为 n，快指针相对于慢指针每次追 1 步，因此最多 n 步相遇。
- 陷阱3：面试官追问"能否用哈希表的方式判断，并比较两种方法的使用场景？"——哈希表简单直观但费空间；快慢指针省空间但需要理解原理。实际工程中如果链表不长，哈希表更不容易出错。

---

## 12. 相交链表 (Intersection of Two Linked Lists) — 题号 160

**难度**: 简单
**标签**: 链表、双指针

### 题目大意

给定两个单链表的头节点 `headA` 和 `headB`，找出并返回两个链表相交的起始节点。如果两个链表没有交点，返回 `null`。

### 思路

**方法一（哈希表）**：遍历链表 A，将所有节点存入哈希表。然后遍历链表 B，检查每个节点是否在哈希表中。第一个命中即为交点。

**方法二（双指针，浪漫相遇）**：这是空间 O(1) 的巧妙解法。设置两个指针 `pA` 和 `pB` 分别从 `headA` 和 `headB` 出发：
- 若 `pA` 到达末尾，则将其重定位到 `headB`；
- 若 `pB` 到达末尾，则将其重定位到 `headA`。

两个指针每次都前进一步。如果两个链表相交，它们一定会在交点相遇；如果不相交，它们会同时到达 `nullptr`。

**为什么能相遇？** 设链表 A 的非公共部分长度为 a，链表 B 的非公共部分长度为 b，公共部分长度为 c。`pA` 走过的路径是 `a + c + b`，`pB` 走过的路径是 `b + c + a`，两者路径长度相等，因此会同时到达交点。

**为什么能想到？** 消除长度差是关键。两个链表长度不同，但如果让每个指针把两个链表都走一遍，它们走过的总路程就一样了，自然会在公共交点相遇。

### 核心知识点

- 双指针消除长度差
- 路径等价性：`a + c + b = b + c + a`
- 链表指针重定位技巧

### 复杂度

- **时间复杂度**: O(m + n)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
    if (headA == nullptr || headB == nullptr) return nullptr;
    ListNode *pA = headA, *pB = headB;
    // 两个指针分别走 A+B 和 B+A，路径长度相等
    while (pA != pB) {
        pA = (pA == nullptr) ? headB : pA->next; // A 走完就跳到 B 的头
        pB = (pB == nullptr) ? headA : pB->next; // B 走完就跳到 A 的头
    }
    // pA == pB 时退出循环，要么是交点，要么都是 nullptr（不相交）
    return pA;
}
```

### 常见错误

- 错误1：`pA` 和 `pB` 每次到达 `nullptr` 后跳转到另一链表头，但只跳转一次导致无限循环。正确做法是持续跳转，直到两者相遇。
- 错误2：忘记处理两个链表不相交的情况。不加保护可能无限循环——但本算法通过同时到达 `nullptr` 自然终止，不会无限循环。
- 错误3：在跳转前先 `break` 或添加多余标志位，破坏了算法的简洁性。

### 面试陷阱

- 陷阱1：面试官追问"如果链表有环，这个算法还能用吗？"——不能。需要先用快慢指针找到环入口，再分情况讨论。带环的相交链表是更复杂的变体。
- 陷阱2：面试官追问"这个算法的最坏循环次数是多少？"——每个指针最多走 `m + n` 步，因为两个指针都会走完 A+B 或 B+A 的总路程。
- 陷阱3：面试官追问"如果要求 O(1) 空间但不允许修改链表结构，还有别的方法吗？"——可以先计算两个链表的长度，然后让较长的链表先走差值步，再同步前进，也能找到交点。

---

## 13. 回文链表 (Palindrome Linked List) — 题号 234

**难度**: 简单
**标签**: 链表、双指针、栈

### 题目大意

给定一个单链表的头节点 `head`，判断该链表是否为回文链表（即正序和逆序遍历结果相同）。

### 思路

**方法一（数组拷贝）**：遍历链表将值拷贝到数组中，然后用双指针判断数组是否为回文。时间 O(n)，空间 O(n)。

**方法二（栈）**：利用栈的后进先出特性。先遍历一遍把所有值压入栈，再遍历一遍和栈顶比较。时间 O(n)，空间 O(n)。

**方法三（反转后半段链表）**：空间 O(1) 的最优解：
1. 使用快慢指针找到链表中点；
2. 反转后半段链表；
3. 将前半段和反转后的后半段逐节点比较；
4. （可选）恢复链表原样。

为什么能想到找中点+反转？回文的本质是"前半段 = 逆序的后半段"。如果能反转后半段，就可以直接和前半段比较。找中点用快慢指针（题 141 的延伸），反转用三指针（题 206），是两个基础技巧的组合。

### 核心知识点

- 快慢指针找链表中点
- 反转链表
- 回文的"中点对称"性质
- 技巧组合（题 141 + 题 206）

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(1)（反转后半段方法）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
};

// 反转链表（题 206 的迭代解法）
ListNode* reverseList(ListNode* head) {
    ListNode *prev = nullptr, *curr = head;
    while (curr) {
        ListNode* nxt = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nxt;
    }
    return prev;
}

bool isPalindrome(ListNode* head) {
    if (head == nullptr || head->next == nullptr) return true;
    // 1. 快慢指针找中点（慢指针停在中间或中间偏左）
    ListNode *slow = head, *fast = head;
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    // 2. 反转后半段链表
    ListNode* secondHalf = reverseList(slow->next);
    // 3. 比较前半段和反转后的后半段
    ListNode* p1 = head;
    ListNode* p2 = secondHalf;
    bool ok = true;
    while (p2) { // 后半段可能少一个节点（奇数长度），用 p2 判断
        if (p1->val != p2->val) {
            ok = false;
            break;
        }
        p1 = p1->next;
        p2 = p2->next;
    }
    // 4. （可选）恢复链表原样
    slow->next = reverseList(secondHalf);
    return ok;
}
```

### 常见错误

- 错误1：快慢指针找中点时的终止条件写错。`while (fast->next && fast->next->next)` 保证慢指针停在中间偏左（偶数长度）或正中间（奇数长度）。
- 错误2：反转后半段后忘记断开前半段与后半段的连接（`slow->next` 仍然指向原后半段头），导致比较时出错。
- 错误3：比较时用 `for` 循环统计长度做边界，实际上用 `while (p2)` 即可（后半段长度一定 ≤ 前半段）。

### 面试陷阱

- 陷阱1：面试官追问"如果不允许修改链表结构（不能反转），怎么做？"——只能用数组或栈（空间 O(n)），或者用递归回溯（空间 O(n) 递归栈）。
- 陷阱2：面试官追问"如果要判断一个字符串是否回文，和链表回文的区别是什么？"——字符串支持随机访问，双指针从两端向中间即可 O(1) 空间。链表不支持随机访问，因此需要借助反转或额外空间。
- 陷阱3：面试官追问"快慢指针找中点的两种常见写法有什么区别？"——`while(fast && fast->next)` 在奇数时慢指针停在右中点；`while(fast->next && fast->next->next)` 停在左中点。本题用后者更合适，因为我们需要反转后半段。

---

## 14. 有效的括号 (Valid Parentheses) — 题号 20

**难度**: 简单
**标签**: 栈、哈希表

### 题目大意

给定一个只包含 `'('`、`')'`、`'{'`、`'}'`、`'['`、`']'` 的字符串 s，判断字符串是否有效。有效条件：左括号必须有相同类型的右括号闭合，且左括号必须以正确的顺序闭合。

### 思路

括号匹配是栈的"教科书级别"应用。核心观察：右括号必须与**最近出现的、尚未配对的**左括号匹配（即后进先出的顺序）。这正是栈的特性。

遍历字符串：
- 遇到左括号，压入栈中；
- 遇到右括号，检查栈顶是否为对应的左括号：
  - 如果栈为空或栈顶不匹配，返回 `false`；
  - 如果匹配，弹出栈顶。

遍历结束后，如果栈为空，说明所有左括号都被匹配了，返回 `true`；否则返回 `false`。

为什么能想到栈？"括号嵌套"天然是 LIFO（后进先出）结构——最内层的括号最先闭合。任何需要"最近未匹配"的场景都应考虑栈。

### 核心知识点

- 栈的后进先出（LIFO）
- 括号匹配的栈模型
- 哈希表建立括号对应关系

### 复杂度

- **时间复杂度**: O(n) — 每个字符处理一次
- **空间复杂度**: O(n) — 最坏情况下（全是左括号）栈存储 n 个字符

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

bool isValid(string s) {
    // 右括号到左括号的映射，方便查找对应的左括号
    unordered_map<char, char> pairs = {
        {')', '('},
        {']', '['},
        {'}', '{'}
    };
    stack<char> st;
    for (char c : s) {
        // 如果是右括号（在映射的键中）
        if (pairs.count(c)) {
            // 栈为空或栈顶不匹配，说明括号无效
            if (st.empty() || st.top() != pairs[c]) {
                return false;
            }
            st.pop(); // 匹配成功，弹出左括号
        } else {
            // 左括号直接入栈
            st.push(c);
        }
    }
    // 栈为空说明所有括号都匹配完毕
    return st.empty();
}
```

### 常见错误

- 错误1：没有检查"栈为空"就直接调用 `st.top()`，导致空栈访问错误（如输入 `"]"`）。
- 错误2：用多个 if-else 而不是哈希表，代码冗长且容易漏写某种括号。
- 错误3：忘记最后检查栈是否为空——如输入 `"(("` 只入栈不出栈，如果最后不判断 `st.empty()` 会错误返回 `true`。

### 面试陷阱

- 陷阱1：面试官追问"如果括号种类不只三种，而是用户自定义的成对符号，代码如何适应？"——将映射表抽取为函数参数，代码逻辑不变。
- 陷阱2：面试官追问"如果不仅判断是否匹配，还要求找出最长有效括号子串长度？"——LeetCode 32，需要使用栈+索引或动态规划，栈中存储下标而非字符。
- 陷阱3：面试官追问"用递归能解决括号匹配吗？"——理论上可以，但递归本质上也是用调用栈模拟，不如显式栈直观。对于嵌套深度极大的情况递归还可能栈溢出。

---

## 15. 最小栈 (Min Stack) — 题号 155

**难度**: 中等
**标签**: 栈、设计

### 题目大意

设计一个支持 `push`、`pop`、`top` 操作，并能在**常数时间**内检索到最小元素的栈。即实现 `MinStack` 类：`push(val)`、`pop()`、`top()`、`getMin()` 均需 O(1) 时间。

### 思路

普通栈只有一个维度——元素值。现在需要额外维护一个维度——当前栈中的最小值。

当某个元素 `x` 被压入栈时，栈的最小值可能会更新（如果 `x` 小于之前的最小值）。但问题是：如果 `x` 后来被弹出，我们需要知道"x 被弹出后的最小值"——也就是 `x` 入栈之前的旧最小值。

**核心洞察**：最小值的变化只在 `push` 时发生，`pop` 时可能需要恢复到前一个最小值。因此我们可以再维护一个"最小值栈" `minSt`，与主栈同步操作：
- `push(x)`：主栈压入 x；`minSt` 压入 `min(x, minSt.top())`（即当前最小值）。
- `pop()`：两个栈都弹出。
- `getMin()`：返回 `minSt.top()`（即当前栈中最小值）。

为什么能想到双栈？"需要在 O(1) 时间内获取某种历史状态"的通用模式就是维护一个辅助栈，记录每个时刻的关键信息快照。

### 核心知识点

- 辅助栈（双栈）设计模式
- 空间换时间
- 状态快照的思想

### 复杂度

- **时间复杂度**: 所有操作 O(1)
- **空间复杂度**: O(n) — 两个栈各存 n 个元素

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class MinStack {
private:
    stack<int> dataSt; // 主栈：存所有元素
    stack<int> minSt;  // 辅助栈：存每个状态下的最小值
public:
    MinStack() {
        // minSt 预置一个最大值，简化空栈判断
        minSt.push(INT_MAX);
    }

    void push(int val) {
        dataSt.push(val);
        // 当前最小值 = min(新值, 之前的最小值)
        minSt.push(min(val, minSt.top()));
    }

    void pop() {
        dataSt.pop();
        minSt.pop(); // 同步弹出，恢复到上一个状态的最小值
    }

    int top() {
        return dataSt.top();
    }

    int getMin() {
        return minSt.top();
    }
};
```

### 常见错误

- 错误1：`pop` 时只弹出了主栈，忘记弹出辅助栈，导致 `getMin` 返回错误值。
- 错误2：`minSt` 初始为空，第一次 `push` 时调用 `minSt.top()` 出错。解决方案是构造时预置 `INT_MAX` 或特殊处理第一次 push。
- 错误3：使用单个变量记录最小值，`pop` 后无法恢复到旧最小值。这是面试中最常见的错误——要意识到最小值必须对应栈的每个状态。

### 面试陷阱

- 陷阱1：面试官追问"能否只用一个栈实现，空间优化到不用双栈？"——可以在栈中存入 `pair<val, curMin>` 而非两个独立的栈，或者在压入比当前最小值还小的元素时，先压入旧最小值再压入新值（差值法）。
- 陷阱2：面试官追问"如果要求实现 O(1) 的 `getMax()`，怎么做？"——再加一个辅助栈记录最大值即可，完全对称。
- 陷阱3：面试官追问"如果要求支持 `getMin()` 且原栈支持各种操作（包括随机删除），还能 O(1) 吗？"——不能，随机删除后最小值维护变复杂（需线段树或有序集合）。

---

## 16. 用栈实现队列 (Implement Queue using Stacks) — 题号 232

**难度**: 简单
**标签**: 栈、队列、设计

### 题目大意

使用两个栈实现一个队列（FIFO）。实现 `push`、`pop`、`peek`、`empty` 操作。

### 思路

栈是 LIFO（后进先出），队列是 FIFO（先进先出）。用栈模拟队列的关键是**两次反转**。

使用两个栈：
- `inStack`（输入栈）：接收 push 的元素；
- `outStack`（输出栈）：负责 pop/peek 操作。

当需要出队时，如果 `outStack` 为空，就把 `inStack` 中的所有元素逐个弹出并压入 `outStack`。这个过程相当于将元素顺序反转，使得最早进入 `inStack` 的元素出现在 `outStack` 的栈顶（即先进先出）。

**为什么能想到两个栈？** 单个栈反转后就能模拟队列，但不能同时支持 push 和 pop（因为你只能在栈顶操作）。两个栈分工协作：一个负责入队（只管 push），一个负责出队（反转后的顺序即 FIFO），在需要时才将元素从入栈转移到出栈。

### 核心知识点

- 两个栈模拟 FIFO 队列
- 数据反转实现 LIFO -> FIFO
- 均摊 O(1) 分析

### 复杂度

- **时间复杂度**: push O(1)，pop/peek 均摊 O(1)（每个元素最多被转移一次，从 inStack 到 outStack）
- **空间复杂度**: O(n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class MyQueue {
private:
    stack<int> inStack;  // 输入栈：只负责接收 push
    stack<int> outStack; // 输出栈：负责 pop/peek，顺序已反转
public:
    MyQueue() {}

    void push(int x) {
        inStack.push(x); // 直接压入输入栈
    }

    int pop() {
        // 如果输出栈为空，将输入栈的全部元素转移到输出栈（反转顺序）
        if (outStack.empty()) {
            while (!inStack.empty()) {
                outStack.push(inStack.top());
                inStack.pop();
            }
        }
        int val = outStack.top();
        outStack.pop();
        return val;
    }

    int peek() {
        // 和 pop 逻辑相同，但不弹出元素
        if (outStack.empty()) {
            while (!inStack.empty()) {
                outStack.push(inStack.top());
                inStack.pop();
            }
        }
        return outStack.top();
    }

    bool empty() {
        // 两个栈都为空时队列才为空
        return inStack.empty() && outStack.empty();
    }
};
```

### 常见错误

- 错误1：在每次 `push` 时都做转移操作，导致 `push` 变成 O(n)。正确做法是惰性转移（只在需要 pop/peek 且 outStack 为空时转移）。
- 错误2：`peek` 中调用 `pop()` 获取值后又 push 回去，这虽然正确但多余——直接用 `top()` 即可。
- 错误3：判断 `empty()` 时只检查一个栈，遗漏另一个栈。

### 面试陷阱

- 陷阱1：面试官追问"能否证明 pop 操作的均摊时间复杂度为 O(1)？"——每个元素人队 push 一次（入 inStack），转移一次（inStack -> outStack），出队 pop 一次（出 outStack）。每个元素总操作 3 次，均摊 O(1)。
- 陷阱2：面试官追问"用一个栈和一个递归能否实现队列？"——递归本质是用调用栈模拟第二个栈，是可行的但不可取（递归深度受限）。
- 陷阱3：面试官追问"用两个队列实现栈（下一题的反向操作）怎么做？"——核心在于 push 时把新元素压入非空队列，然后将另一个队列的所有元素全部转移到该队列后面。

---

## 17. 用队列实现栈 (Implement Stack using Queues) — 题号 225

**难度**: 简单
**标签**: 队列、栈、设计

### 题目大意

使用两个队列实现一个栈（LIFO）。实现 `push`、`pop`、`top`、`empty` 操作。

### 思路

本题和上一题对称但方向相反：用 FIFO 模拟 LIFO。

**方法一（两个队列）**：每次 `push` 新元素时，先将其放入一个临时队列，然后把主队列的所有元素转移到临时队列后面，最后交换两个队列。这样新元素总是位于队列头部，实现后进先出。

**方法二（一个队列）**：更简洁！每次 `push` 新元素后，将队列中该元素之前的所有元素依次弹出并重新入队。效果是把新元素"顶到"队首位置。

为什么能想到？队列只有首部可以出队、尾部可以入队。要让最后入队的元素最先出队，唯一的办法就是让新元素始终放在队首。而做到这一点的方式就是"push 后旋转队列"。

### 核心知识点

- 单队列/双队列模拟 LIFO
- push 时旋转队列
- 与"用栈实现队列"的思路对比

### 复杂度

- **时间复杂度**: push O(n)（旋转 n-1 个元素），pop/peek O(1)
- **空间复杂度**: O(n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class MyStack {
private:
    queue<int> q; // 只需一个队列
public:
    MyStack() {}

    void push(int x) {
        q.push(x);
        // 旋转：将新入队的元素之前的所有元素移到队尾
        // 效果：新元素变为队首，即"后进"的元素在"先出"的位置
        int sz = q.size();
        for (int i = 0; i < sz - 1; i++) {
            q.push(q.front()); // 队首移动到队尾
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

### 常见错误

- 错误1：旋转次数写为 `sz` 而不是 `sz - 1`，导致新元素被旋转到队尾而非队首。
- 错误2：使用双队列时，转移完毕后忘记交换两个队列的引用，导致后续操作在错误的队列上进行。
- 错误3：在旋转过程中使用 `q.size()` 作为循环次数，但 `size()` 在 push 后已经包含了新元素——如果代码逻辑不同需谨慎。

### 面试陷阱

- 陷阱1：面试官追问"你能比较一下'用栈实现队列'和'用队列实现栈'在思路上的根本区别吗？"——前者是"出队时反转"（惰性），后者是"入队时反转"（即时）。原因在于前者可以用两个栈分工（in + out），而队列的反转只能在 push 时做。
- 陷阱2：面试官追问"如果改用双端队列（deque）实现栈？"——直接用 deque 的 `push_back` 和 `pop_back` 即可，但这就失去了"用队列模拟栈"的意义。
- 陷阱3：追问"push 的 O(n) 能否优化为均摊 O(1)？"——不能做到 push O(1) 同时 pop O(1)，因为栈和队列的本质不同。但如果接受 pop 为 O(n) 也可以反过来（懒处理）。

---

## 18. 二叉树的中序遍历 (Binary Tree Inorder Traversal) — 题号 94

**难度**: 简单
**标签**: 二叉树、DFS、栈

### 题目大意

给定一个二叉树的根节点 `root`，返回它的**中序遍历**（左子树 -> 根节点 -> 右子树）。

### 思路

中序遍历是二叉树最基础的遍历方式之一。思路天然递归：先遍历左子树，再访问根节点，最后遍历右子树。

**递归法**：代码极简，但面试中几乎都会被要求写迭代版。

**迭代法**：递归的本质是系统调用栈，我们可以用显式栈模拟。中序迭代的核心是"一路向左，走到尽头再回溯"：
1. 从根节点开始，一直沿左子树往下走，沿途把所有节点压入栈；
2. 走到最左（nullptr）时，弹出栈顶节点，访问它；
3. 然后转向其右子树，重复步骤 1。

为什么能想到这个迭代过程？中序"左-根-右"的顺序意味着：最左边的节点最先被访问。而"一路向左走到底"正是找到最左节点，过程中用栈记录"回溯路径"（根节点和未处理的右子树）。

### 核心知识点

- 递归与迭代的转换
- 栈模拟递归
- 中序遍历的"左链入栈"模式

### 复杂度

- **时间复杂度**: O(n) — 每个节点访问一次
- **空间复杂度**: O(h) — h 为树高，栈最大深度为树高（最坏 O(n)，平衡树 O(log n)）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 递归解法
void inorderRecursive(TreeNode* root, vector<int>& result) {
    if (root == nullptr) return;
    inorderRecursive(root->left, result);  // 先左
    result.push_back(root->val);           // 再根（中）
    inorderRecursive(root->right, result); // 最后右
}

// 迭代解法（栈模拟）
vector<int> inorderTraversal(TreeNode* root) {
    vector<int> result;
    stack<TreeNode*> st;
    TreeNode* curr = root;
    // 迭代中序：一路向左，走到尽头再回溯
    while (curr != nullptr || !st.empty()) {
        // 1. 沿着左子树一路走到底，沿途压栈
        while (curr != nullptr) {
            st.push(curr);
            curr = curr->left;
        }
        // 2. 左子树走到头了，弹出栈顶（最近一个"根"），访问它
        curr = st.top();
        st.pop();
        result.push_back(curr->val);
        // 3. 转向右子树，对右子树重复上述过程
        curr = curr->right;
    }
    return result;
}
```

### 常见错误

- 错误1：迭代法中，`while` 条件只写了 `!st.empty()`，忘记 `curr != nullptr`，导致初始时（栈空、curr 指向根）直接跳过循环。
- 错误2：访问根节点后忘记将 `curr` 设为 `curr->right`，导致循环停在当前节点无限重复。
- 错误3：递归法中遍历左右子树的顺序写错——中序是"左根右"，前序是"根左右"，后序是"左右根"。

### 面试陷阱

- 陷阱1：面试官追问"Morris 遍历了解吗？能否做到 O(1) 空间？"——Morris 遍历利用叶子节点的空闲指针（Threaded Binary Tree），实现 O(1) 空间的中序遍历，但会临时修改树结构。
- 陷阱2：面试官追问"如果树的深度很大（如 100000），递归和迭代哪个更优？"——迭代更优，递归可能栈溢出。
- 陷阱3：追问"三种遍历（前/中/后序）的迭代写法，哪个最复杂？为什么？"——后序遍历最复杂。因为需要区分"刚从左边回来"还是"刚从右边回来"，通常需要一个额外变量记录上一次访问的节点。

---

## 19. 二叉树的前序遍历 (Binary Tree Preorder Traversal) — 题号 144

**难度**: 简单
**标签**: 二叉树、DFS、栈

### 题目大意

给定一个二叉树的根节点 `root`，返回它的**前序遍历**（根节点 -> 左子树 -> 右子树）。

### 思路

前序遍历顺序：根-左-右。递归法非常简单。

**迭代法**：前序遍历是三种遍历中迭代写法最简单的。访问顺序是"根左右"，而栈是后进先出的，所以入栈时要**先右后左**（这样弹出时才会先左后右）：
1. 将根节点压入栈；
2. 弹出栈顶，访问它；
3. 先压入右孩子（如果有），再压入左孩子（如果有）；
4. 重复 2-3 直到栈空。

为什么能想到？前序的特点是"根先被访问"，所以遇到一个节点直接访问，然后把它的孩子按"右左"顺序入栈即可。

### 核心知识点

- 栈模拟递归
- 前序遍历的简单迭代模式
- 入栈顺序：先右后左（与访问顺序相反）

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(h)（最坏 O(n)）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 迭代解法
vector<int> preorderTraversal(TreeNode* root) {
    vector<int> result;
    if (root == nullptr) return result;
    stack<TreeNode*> st;
    st.push(root);
    while (!st.empty()) {
        TreeNode* node = st.top();
        st.pop();
        result.push_back(node->val); // 先访问根
        // 栈是 LIFO，为了让左孩子先出栈，先压入右孩子
        if (node->right) st.push(node->right);
        if (node->left) st.push(node->left);
    }
    return result;
}

// 递归写法（对比用）
void preorderRecursive(TreeNode* root, vector<int>& result) {
    if (root == nullptr) return;
    result.push_back(root->val);          // 根
    preorderRecursive(root->left, result);  // 左
    preorderRecursive(root->right, result); // 右
}
```

### 常见错误

- 错误1：入栈顺序写成"先左后右"，导致遍历变成"根-右-左"。
- 错误2：忘记判断 root 为空的情况，导致对空栈 `push(nullptr)` 后出现异常。
- 错误3：试图用中序迭代的模式（一路向左）来写前序迭代——虽然也可以（沿左链一路走一路访问，同时把右孩子压栈），但不如通用栈法直观。

### 面试陷阱

- 陷阱1：面试官追问"前序遍历和前序序列化二叉树的关系？"——前序序列化恰好是前序遍历时记录空节点（如用 `#`），可以唯一确定一棵二叉树。
- 陷阱2：面试官追问"N 叉树的前序遍历怎么做？"——和二叉树一样，只是入栈时按孩子从右到左依次压入。
- 陷阱3：追问"前序迭代的两种写法（通用栈法 vs 左链入栈法）的使用场景？"——通用栈法思路最清晰；左链入栈法空间复杂度更优（不需要同时存储左右孩子）。

---

## 20. 二叉树的后序遍历 (Binary Tree Postorder Traversal) — 题号 145

**难度**: 简单
**标签**: 二叉树、DFS、栈

### 题目大意

给定一个二叉树的根节点 `root`，返回它的**后序遍历**（左子树 -> 右子树 -> 根节点）。

### 思路

后序是三种遍历中最难的迭代写法，因为访问一个节点前必须确保其左右子树都已访问完毕。

**技巧法（前序变形）**：后序是"左右根"，前序是"根左右"。如果把前序改成"根右左"（交换左右孩子的入栈顺序），然后将结果反转，就变成了"左右根"——恰好是后序！

**正统迭代法**：用栈模拟"左-右-根"，需要一个额外变量 `lastVisited` 记录上次访问的节点，用于判断右子树是否已访问完毕。

这里我们展示技巧法，因为它在面试中更容易清晰表达。

### 核心知识点

- 后序 = 前序变形 + 结果反转
- 遍历顺序的转换技巧
- 栈模拟递归

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(h)（最坏 O(n)）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 技巧法：前序（根右左）变种 + 反转
vector<int> postorderTraversal(TreeNode* root) {
    vector<int> result;
    if (root == nullptr) return result;
    stack<TreeNode*> st;
    st.push(root);
    // 按"根-右-左"的顺序访问（先左后右入栈的镜像：先右后左改成先左后右）
    while (!st.empty()) {
        TreeNode* node = st.top();
        st.pop();
        result.push_back(node->val); // 访问根
        // 为了让右孩子先被访问（因为后面要反转），先压左、后压右
        // 弹出时就是：根 -> 右 -> 左
        if (node->left) st.push(node->left);
        if (node->right) st.push(node->right);
    }
    // 反转结果：将"根右左"变成"左右根"（即后序）
    reverse(result.begin(), result.end());
    return result;
}

// 正统迭代法（了解即可）
vector<int> postorderTraversal_v2(TreeNode* root) {
    vector<int> result;
    stack<TreeNode*> st;
    TreeNode* curr = root;
    TreeNode* lastVisited = nullptr; // 记录上一次访问的节点
    while (curr || !st.empty()) {
        // 一路向左，压栈
        while (curr) {
            st.push(curr);
            curr = curr->left;
        }
        TreeNode* top = st.top();
        // 如果右子树为空，或右子树已访问，则访问当前节点
        if (top->right == nullptr || top->right == lastVisited) {
            result.push_back(top->val);
            st.pop();
            lastVisited = top; // 标记为已访问
        } else {
            // 转向右子树
            curr = top->right;
        }
    }
    return result;
}
```

### 常见错误

- 错误1：在技巧法中，入栈顺序写成"先右后左"（和前序一样），导致结果变成"根左右"的反转即"右左根"，而不是"左右根"。
- 错误2：正统迭代法中忘记设置 `lastVisited`，导致右子树被重复处理。
- 错误3：在正统迭代法中，`if (top->right == nullptr || top->right == lastVisited)` 两个条件的顺序不重要，但必须同时检查。

### 面试陷阱

- 陷阱1：面试官追问"三种遍历的迭代写法，哪个最简单？为什么？"——前序最简单（直接访问即可），中序次之（需要"左链入栈"模式），后序最复杂（需要区分左右子树状态或使用反转技巧）。
- 陷阱2：面试官追问"技巧法的空间复杂度是多少？"——O(h) 的栈空间 + O(n) 的结果数组空间。反转操作本身 O(n) 时间，O(1) 额外空间。
- 陷阱3：追问"能否不反转结果数组，直接按后序输出？"——这就是正统迭代法的目的，但代码复杂度显著增加。

---

## 21. 二叉树的层序遍历 (Binary Tree Level Order Traversal) — 题号 102

**难度**: 中等
**标签**: 二叉树、BFS、队列

### 题目大意

给定一个二叉树的根节点 `root`，返回其节点值的**层序遍历**（即逐层地，从左到右访问所有节点）。结果是一个二维数组，每个子数组代表一层的节点值。

### 思路

层序遍历 = BFS（广度优先搜索），天然适合用队列实现。

基本流程：
1. 将根节点入队；
2. 当队列非空时，处理当前层的所有节点（通过记录当前队列大小 `levelSize` 来区分层）；
3. 对于每层的每个节点，出队并访问，然后将其左右孩子入队（供下一层处理）。

为什么用队列？BFS 要求"先入先出"——先遇到的层的节点先被处理。队列的 FIFO 特性恰好满足。这里的关键技巧是**在每层开始时记录当前队列大小**，因为后续入队的孩子属于下一层，不能在这一层处理。

### 核心知识点

- BFS（层序遍历）= 队列
- 使用 `levelSize` 区分层
- 二维结果的组织方式

### 复杂度

- **时间复杂度**: O(n) — 每个节点入队出队一次
- **空间复杂度**: O(n) — 队列最多存储最宽一层的节点数（约 n/2）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> result;
    if (root == nullptr) return result;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        int levelSize = q.size(); // 当前层的节点数（核心技巧！）
        vector<int> level;
        // 处理当前层的所有节点
        for (int i = 0; i < levelSize; i++) {
            TreeNode* node = q.front();
            q.pop();
            level.push_back(node->val);
            // 将下一层的节点入队（只属于下一层，不会在当前层循环中被处理）
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        result.push_back(level); // 当前层处理完毕，加入结果
    }
    return result;
}
```

### 常见错误

- 错误1：没有使用 `levelSize` 来区分层，而是直接 `while (!q.empty())` 然后逐个处理，结果变成一维数组而非二维。
- 错误2：将 `levelSize = q.size()` 写在 `for` 循环内部或遗漏，导致层与层之间混淆。
- 错误3：对于每层的节点，`for` 循环的条件写成 `i < q.size()`，但每次 pop 后 size 会变，导致少处理节点或死循环。

### 面试陷阱

- 陷阱1：面试官追问"锯齿形层序遍历（Zigzag Level Order）怎么做？"——LeetCode 103，在普通层序遍历的基础上，对偶数层（从 0 开始计数）的结果做反转，或使用双端队列。
- 陷阱2：面试官追问"自底向上的层序遍历怎么做？"——LeetCode 107，先正常层序遍历，最后将 result 数组反转即可。
- 陷阱3：面试官追问"层序遍历能否用 DFS（递归）实现？"——可以！递归遍历时记录节点深度，按深度将节点值放入对应层的数组中。这实际上是"DFS 按层收集"。

---

## 22. 二叉树的最大深度 (Maximum Depth of Binary Tree) — 题号 104

**难度**: 简单
**标签**: 二叉树、DFS、BFS

### 题目大意

给定一个二叉树，找出其最大深度。二叉树的深度为根节点到最远叶子节点的最长路径上的节点数。

### 思路

**DFS（递归）**：一棵树的最大深度 = `1 + max(左子树最大深度, 右子树最大深度)`。递归基：空树深度为 0。这是最直观的解法，一行代码即可。

**BFS（层序遍历）**：用层序遍历（题 102），每遍历一层计数 +1，直到遍历完毕。层数即为最大深度。

**为什么能想到递归？** 树问题天然适合递归——树的结构是递归定义的（左右子树也是树）。求整棵树的最大深度可以规约为"求左右子树的最大深度并取较大值加一"。

### 核心知识点

- DFS 自底向上求深度
- BFS 层序统计层数
- 递归的"大问题化小问题"思想

### 复杂度

- **时间复杂度**: O(n) — 每个节点访问一次
- **空间复杂度**: O(h)（递归栈）/ O(n)（BFS 队列最宽层）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// DFS 递归解法（最简洁）
int maxDepth(TreeNode* root) {
    if (root == nullptr) return 0;
    // 当前树的最大深度 = 1（根节点） + max(左子树深度, 右子树深度)
    return 1 + max(maxDepth(root->left), maxDepth(root->right));
}

// BFS 层序遍历解法
int maxDepth_BFS(TreeNode* root) {
    if (root == nullptr) return 0;
    queue<TreeNode*> q;
    q.push(root);
    int depth = 0;
    while (!q.empty()) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; i++) {
            TreeNode* node = q.front();
            q.pop();
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        depth++; // 每处理完一层，深度 +1
    }
    return depth;
}
```

### 常见错误

- 错误1：递归时忘记空节点返回 0，导致空指针解引用。
- 错误2：BFS 解法中 `depth++` 放在 `for` 循环内部而非外部，导致每处理一个节点就加一次深度。
- 错误3：将深度理解为"边数"而非"节点数"，导致结果差 1（本题深度定义为节点数）。

### 面试陷阱

- 陷阱1：面试官追问"如果树非常深（如退化成链表），递归法有什么风险？"——递归栈可能溢出。此时应改用 BFS（队列）或 DFS 迭代（栈）来避免。
- 陷阱2：面试官追问"最小深度怎么求？"——LeetCode 111，注意最小深度的定义是到最近叶子节点的节点数。需要特殊处理单侧为空的情况（`1 + min(left, right)` 仅在两侧都不为空时有效）。
- 陷阱3：面试官追问"N 叉树的最大深度怎么求？"——`1 + max(所有孩子的深度)`，遍历所有孩子取最大值即可。

---

## 23. 对称二叉树 (Symmetric Tree) — 题号 101

**难度**: 简单
**标签**: 二叉树、DFS、BFS

### 题目大意

给定一个二叉树，检查它是否是镜像对称的（即，以根节点的中心线为轴，左右子树互为镜像）。

### 思路

对称的核心是：**根节点的左子树和右子树互为镜像**。

两个树互为镜像的条件：
1. 它们的根节点值相等；
2. 树 A 的左子树与树 B 的右子树互为镜像；
3. 树 A 的右子树与树 B 的左子树互为镜像。

这是递归的自然定义。因此，我们可以写一个辅助函数 `isMirror(t1, t2)`，判断两棵树是否镜像对称。

**DFS 递归法**：按上述条件递归判断。

**BFS 迭代法**：使用队列成对存储需要比较的节点。每次取出两个节点比较，然后按"左-右 vs 右-左"的顺序将其孩子成对入队。

为什么能想到"两个指针"的方法？把一棵树对称性地"拆成"两棵树，让两棵树按镜像方式比较——左子树走左边时右子树走右边，反之亦然。

### 核心知识点

- 树对称的递归定义
- 双指针 + 镜像比较
- DFS vs BFS 两种判断方式

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(h) / O(n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 辅助函数：判断两棵树是否镜像对称
bool isMirror(TreeNode* t1, TreeNode* t2) {
    // 都为空，说明对称
    if (t1 == nullptr && t2 == nullptr) return true;
    // 一个空一个非空，说明不对称
    if (t1 == nullptr || t2 == nullptr) return false;
    // 根节点值相等，
    // 且 t1 的左子树与 t2 的右子树镜像对称，
    // 且 t1 的右子树与 t2 的左子树镜像对称
    return (t1->val == t2->val)
        && isMirror(t1->left, t2->right)
        && isMirror(t1->right, t2->left);
}

bool isSymmetric(TreeNode* root) {
    if (root == nullptr) return true;
    // 将根节点的左右子树进行镜像比较
    return isMirror(root->left, root->right);
}

// BFS 迭代写法（使用队列）
bool isSymmetric_BFS(TreeNode* root) {
    if (root == nullptr) return true;
    queue<TreeNode*> q;
    q.push(root->left);
    q.push(root->right);
    while (!q.empty()) {
        TreeNode* t1 = q.front(); q.pop();
        TreeNode* t2 = q.front(); q.pop();
        if (t1 == nullptr && t2 == nullptr) continue;
        if (t1 == nullptr || t2 == nullptr) return false;
        if (t1->val != t2->val) return false;
        // 成对入队：t1的左 vs t2的右，t1的右 vs t2的左
        q.push(t1->left);
        q.push(t2->right);
        q.push(t1->right);
        q.push(t2->left);
    }
    return true;
}
```

### 常见错误

- 错误1：只比较了 `root->left` 和 `root->right` 的值，没有递归检查子树。对称不等于"左右孩子值相等"。
- 错误2：递归时写成了 `isMirror(t1->left, t2->left)` 和 `isMirror(t1->right, t2->right)`——这判断的是两棵树"相等"而非"镜像"。
- 错误3：迭代法中成对入队的顺序写错，把 `(t1->left, t2->right)` 写成 `(t1->left, t2->left)`。

### 面试陷阱

- 陷阱1：面试官追问"判断两棵树是否完全相同（而非镜像对称）怎么做？"——把镜像比较的条件改成 `isSame(t1->left, t2->left) && isSame(t1->right, t2->right)` 即可。
- 陷阱2：面试官追问"如果要求判断一棵树是否是自己的镜像（即对称），和判断两棵树是否互为镜像有什么区别？"——对称 = `isMirror(root->left, root->right)`，互为镜像 = `isMirror(rootA, rootB)`。前者是后者的特例。
- 陷阱3：追问"N 叉树如何判断对称？"——N 叉树的对称需要孩子数组从左到右和从右到左互为镜像，需要额外处理孩子数量不等的情况。

---

## 24. 翻转二叉树 (Invert Binary Tree) — 题号 226

**难度**: 简单
**标签**: 二叉树、DFS

### 题目大意

翻转一棵二叉树（即左右子树交换）。返回翻转后的根节点。

### 思路

"翻转"的定义就是：每个节点的左右孩子互换。这是一个典型的自底向上或自顶向下的递归操作。

**递归法**：
1. 递归翻转左子树；
2. 递归翻转右子树；
3. 交换当前节点的左右孩子。

**为什么能想到递归？** 每个节点都需要做相同的操作（交换左右孩子），而子树也是同样的树结构——这是递归的天然舞台。

注意：本题由于 Homebrew 作者 Max Howell 在 Google 面试中被问到但没做出而著名，实际上它非常简单。

### 核心知识点

- 树的递归操作模式
- 自底向上翻转
- 前序/后序 vs 中序（中序会导致某些节点被翻转两次）

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(h)（递归栈）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 递归解法
TreeNode* invertTree(TreeNode* root) {
    if (root == nullptr) return nullptr;
    // 先翻转左右子树
    invertTree(root->left);
    invertTree(root->right);
    // 再交换当前节点的左右孩子
    swap(root->left, root->right);
    return root;
}

// 迭代解法（使用队列，层序方式翻转）
TreeNode* invertTree_BFS(TreeNode* root) {
    if (root == nullptr) return nullptr;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode* node = q.front();
        q.pop();
        // 交换左右孩子
        swap(node->left, node->right);
        // 将孩子入队继续处理
        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
    return root;
}
```

### 常见错误

- 错误1：只交换了 `left` 和 `right` 指针，但没有递归处理子节点。这样只有根节点的孩子被交换了。
- 错误2：使用中序遍历方式翻转——先翻转左子树，然后交换左右孩子，然后再翻转"右子树"（实际是原来的左子树），导致某些节点被翻转两次变回原样。
- 错误3：`swap` 前没有判断子节点是否为空——实际上 `swap` 两个空指针也没问题，但如果有额外操作需要注意空指针。

### 面试陷阱

- 陷阱1：面试官追问"前序、中序、后序三种翻转方式有什么不同？"——前序和后序都可以正确翻转。中序不行，因为交换后原本的右子树变成了左子树，会被再次递归处理。
- 陷阱2：面试官追问"翻转二叉树和镜像二叉树（题 101 对称判断）的关系？"——翻转 = 将树变为自己的镜像。判断对称 = 检查左子树的翻转是否等于右子树。
- 陷阱3：追问"这道题被认为是 Google 面试的经典笑话题，你觉得面试官真正在考察什么？"——考察基础数据结构素养、递归思维、代码简洁性以及沟通能力（是否能快速理解需求并写出 bug-free 代码）。

---

## 25. 二叉树的直径 (Diameter of Binary Tree) — 题号 543

**难度**: 简单
**标签**: 二叉树、DFS

### 题目大意

给定一棵二叉树，计算它的直径长度。直径长度是任意两个节点路径长度中的最大值，这条路径可能穿过也可能不穿过根节点。路径长度由两个节点之间边的数目表示。

### 思路

"直径"可以理解为：对于树中的每个节点，经过该节点的最长路径 = 左子树高度 + 右子树高度（边数）。直径 = 所有节点的 `(左高 + 右高)` 的最大值。

为什么能想到这个转化？"两个节点的最长路径"等价于"以某个节点为转折点，分别向左右走的最长路径之和"。这个转折点就是该路径上最高的节点（LCA 概念）。

实现时，我们需要一个全局变量 `maxDiameter` 来记录遍历过程中遇到的最大 `(左高+右高)`。在计算每个节点高度的同时，顺便更新直径。

**DFS 后序遍历**：
1. 递归计算左子树高度；
2. 递归计算右子树高度；
3. 更新 `maxDiameter = max(maxDiameter, 左高 + 右高)`；
4. 返回当前节点的高度 = `1 + max(左高, 右高)`。

为什么需要全局变量？因为直径不一定经过根节点，我们需要在遍历过程中持续比较所有节点的"左右子树高度之和"。

### 核心知识点

- 树的高度与直径的关系
- DFS 后序遍历 + 后处理
- 全局变量/引用参数维护最值

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(h)（递归栈）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
    int maxDiameter = 0; // 全局变量记录最大直径

    // 返回以 node 为根子树的高度（边数），同时更新全局直径
    int dfs(TreeNode* node) {
        if (node == nullptr) return 0;
        int leftH = dfs(node->left);   // 左子树高度
        int rightH = dfs(node->right);  // 右子树高度
        // 经过当前节点的路径长度为 leftH + rightH
        // 更新全局最大直径
        maxDiameter = max(maxDiameter, leftH + rightH);
        // 返回当前节点的高度供父节点使用
        return 1 + max(leftH, rightH);
    }

public:
    int diameterOfBinaryTree(TreeNode* root) {
        maxDiameter = 0;
        dfs(root);
        return maxDiameter;
    }
};
```

### 常见错误

- 错误1：返回 `dfs(root)` 而不是 `maxDiameter`。`dfs` 返回的是高度，不是直径。
- 错误2：认为直径一定经过根节点，只计算了 `左高 + 右高` 而没有在递归过程中更新最大值。
- 错误3：对高度和直径的定义混淆——高度是节点数还是边数？本题中路径长度用边数，所以空节点返回 0，叶子节点的高度为 1（如果高度定义为节点数，则左右高之和恰好是边数，公式不变）。

### 面试陷阱

- 陷阱1：面试官追问"为什么直径需要全局变量，而最大深度不需要？"——最大深度一定经过根节点，可直接用递归返回值。直径不一定经过根节点，需要在递归过程中持续比较所有节点的贡献。
- 陷阱2：面试官追问"`dfs` 返回的高度是节点数还是边数？"——代码中空节点返回 0，叶子节点返回 1，所以高度 = 节点数。但 `leftH + rightH` 恰好等于路径的边数（因为两个节点数的高度之和 = 两段边的数量之和），所以公式依然正确。
- 陷阱3：追问"N 叉树的直径怎么求？"——需要记录当前节点所有子树中最大的两个高度（前二高度之和），而不仅仅是两个子树的高度和。

---

## 26. 二分查找 (Binary Search) — 题号 704

**难度**: 简单
**标签**: 数组、二分查找

### 题目大意

给定一个 n 个元素有序的（升序）整型数组 `nums` 和一个目标值 `target`，写一个函数搜索 `nums` 中的 `target`，如果目标值存在返回下标，否则返回 -1。

### 思路

二分查找是最基础的搜索算法之一，前提是**数组有序**。核心思想是：每次取中间位置 `mid` 的元素，与 `target` 比较：
- 若 `nums[mid] == target`，找到目标，返回 `mid`；
- 若 `nums[mid] < target`，说明 target 在右半部分，收缩左边界；
- 若 `nums[mid] > target`，说明 target 在左半部分，收缩右边界。

**区间模型**非常重要，推荐使用 `[left, right]` 闭区间模型（而非左闭右开 `[left, right)`）：
- `left = 0, right = nums.size() - 1`
- 循环条件 `while (left <= right)`（闭区间中 left > right 时区间为空）
- `right = mid - 1`，`left = mid + 1`

为什么二分每次能排除一半？因为数组有序，当前 `mid` 的值和 `target` 的关系决定了 `target` 只能出现在一侧。利用有序性来剪枝是二分查找的核心思维。

### 核心知识点

- 二分查找的区间模型（闭区间 vs 左闭右开）
- `mid` 的防溢出写法：`left + (right - left) / 2`
- 有序数组的"剪枝"思想

### 复杂度

- **时间复杂度**: O(log n) — 每次搜索区间减半
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

int search(vector<int>& nums, int target) {
    // 闭区间模型：[left, right]
    int left = 0, right = nums.size() - 1;
    while (left <= right) {
        // 防溢出写法，等价于 (left + right) / 2
        int mid = left + (right - left) / 2;
        if (nums[mid] == target) {
            return mid; // 命中
        } else if (nums[mid] < target) {
            left = mid + 1; // target 在右半部分
        } else {
            right = mid - 1; // target 在左半部分
        }
    }
    return -1; // 未找到
}
```

### 常见错误

- 错误1：`mid` 计算使用 `(left + right) / 2`，当 `left + right` 超过 `int` 范围时会溢出。应改为 `left + (right - left) / 2`。
- 错误2：循环条件写成 `while (left < right)`（左闭右开模型），但区间初始化为 `right = nums.size() - 1`，导致最后一个元素可能漏检。
- 错误3：更新时 `left = mid` 或 `right = mid`，当区间只剩两个元素时可能导致死循环（mid 永远等于 left）。

### 面试陷阱

- 陷阱1：面试官追问"二分查找的变种——找第一个等于 target 的位置（有重复元素时），怎么写？"——当 `nums[mid] == target` 时不立即返回，而是 `right = mid - 1` 继续左探。这是 lower_bound 的核心逻辑。
- 陷阱2：面试官追问"二分查找适用于链表吗？"——不直接适用，因为链表不能 O(1) 随机访问。但跳表（Skip List）就是基于二分思想的链表增强结构。
- 陷阱3：追问"二分查找和二叉搜索树的查找有什么本质联系？"——本质上都在利用"有序"和"大小比较"来缩小搜索空间。BST 的中序遍历就是有序的，二分查找相当于在 BST 的平衡形态上做查找。

---

## 27. 搜索插入位置 (Search Insert Position) — 题号 35

**难度**: 简单
**标签**: 数组、二分查找

### 题目大意

给定一个排序数组和一个目标值，在数组中找到目标值，并返回其索引。如果目标值不存在于数组中，返回它将会被按顺序插入的位置（即第一个大于等于 target 的元素位置）。

### 思路

本题是二分查找的经典变种——查找**第一个大于等于 target 的位置**（即 C++ 中 `std::lower_bound` 的实现）。

核心思路依然是二分，但当 `nums[mid] == target` 时不立即返回，而是继续往左搜索（因为可能有重复元素，我们找的是第一个满足条件的位置）。换句话说：
- 若 `nums[mid] >= target`，`ans = mid`，然后 `right = mid - 1` 继续搜索左半部分；
- 若 `nums[mid] < target`，`left = mid + 1`。

为什么这样做？我们需要找到"第一个满足 `>= target` 的位置"。当 `nums[mid] >= target` 时，mid 是一个候选答案，但左边可能还有更小的候选。收缩右边界继续搜索即可。

### 核心知识点

- 二分查找变种：`lower_bound` 的实现
- "第一个 >= target" 的二分模式
- 候选答案 + 继续收缩

### 复杂度

- **时间复杂度**: O(log n)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

int searchInsert(vector<int>& nums, int target) {
    int left = 0, right = nums.size() - 1;
    int ans = nums.size(); // 默认插入位置在末尾（所有元素都小于 target）
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] >= target) {
            ans = mid;          // mid 是候选答案
            right = mid - 1;    // 继续向左搜索，找更前的候选
        } else {
            left = mid + 1;     // 当前值太小，搜索右半部分
        }
    }
    return ans;
}
```

### 常见错误

- 错误1：`ans` 初始化为 `0` 而非 `nums.size()`。如果所有元素都小于 target，答案应该是最末尾位置 `nums.size()`。
- 错误2：在 `nums[mid] == target` 时直接返回 `mid`，没有考虑重复元素的情况（虽然本题可以通过，但不符合"第一个>=target"的严格语义）。
- 错误3：搜索区间写错——当 `nums[mid] >= target` 时用 `right = mid` 而非 `right = mid - 1`，可能导致死循环。

### 面试陷阱

- 陷阱1：面试官追问"找最后一个严格小于 target 的位置，怎么写？"——对称，`if (nums[mid] < target) { ans = mid; left = mid + 1; } else { right = mid - 1; }`。
- 陷阱2：面试官追问"找第一个严格大于 target 的位置（upper_bound）怎么写？"——条件改为 `nums[mid] > target` 时记录候选并左探。
- 陷阱3：追问"手写 lower_bound 和直接调用 STL 的 `std::lower_bound`，面试时该怎么做？"——面试中应先手写展示理解，然后可以提 STL 版本。实际工作中建议用 STL，代码更不易出错。

---

## 28. 斐波那契数 (Fibonacci Number) — 题号 509

**难度**: 简单
**标签**: DP、递归

### 题目大意

斐波那契数 `F(n)` 由以下递推关系定义：`F(0) = 0, F(1) = 1`，`F(n) = F(n-1) + F(n-2)`（n >= 2）。给定 n，计算 F(n)。

### 思路

这是动态规划的入门题，但它完美展示了"从递归到 DP"的思维进化：

**暴力递归**：直接按定义递归，每个 F(n) 调用两个子问题 — 导致大量重复计算，指数级复杂度 O(2^n)。

**记忆化搜索（自顶向下 DP）**：在递归中加入缓存数组 `memo`，避免重复计算。时间降到 O(n)，空间 O(n)（递归栈 + memo 数组）。

**迭代 DP（自底向上）**：用数组 `dp[i]` 存储 F(i)，从小到大填充。`dp[i] = dp[i-1] + dp[i-2]`，时间 O(n)，空间 O(n)。

**滚动数组优化**：发现每次只需要前两个值，用两个变量 `prev` 和 `curr` 滚动更新，空间优化到 O(1)。

**为什么这个思路是 DP 的入门？** 斐波那契展现了 DP 的两个核心性质：重叠子问题（同一个 F(k) 被多次计算）和最优子结构（F(n) 由 F(n-1) 和 F(n-2) 决定）。

### 核心知识点

- 暴力递归 -> 记忆化搜索 -> 迭代 DP -> 滚动数组优化的完整思维链
- DP 的重叠子问题和最优子结构
- 滚动数组（状态压缩）

### 复杂度

| 方法 | 时间 | 空间 |
|------|------|------|
| 暴力递归 | O(2^n) | O(n) |
| 记忆化搜索 | O(n) | O(n) |
| 迭代 DP | O(n) | O(n) |
| 滚动数组 | O(n) | O(1) |

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法一：滚动数组优化（空间 O(1)，推荐）
int fib(int n) {
    if (n <= 1) return n;
    int prev = 0, curr = 1; // F(0) 和 F(1)
    // 从 F(2) 开始迭代
    for (int i = 2; i <= n; i++) {
        int next = prev + curr; // F(i) = F(i-2) + F(i-1)
        prev = curr;
        curr = next;
    }
    return curr;
}

// 解法二：记忆化搜索
int fib_memo_helper(int n, vector<int>& memo) {
    if (n <= 1) return n;
    if (memo[n] != -1) return memo[n]; // 如果已计算过，直接返回
    memo[n] = fib_memo_helper(n - 1, memo) + fib_memo_helper(n - 2, memo);
    return memo[n];
}

int fib_memo(int n) {
    vector<int> memo(n + 1, -1);
    return fib_memo_helper(n, memo);
}

// 解法三：矩阵快速幂（扩展，O(log n)）
// 本质上 Fibonacci 可以用矩阵乘法加速到对数时间
```

### 常见错误

- 错误1：滚动数组中 `prev = curr; curr = next;` 的顺序写反，导致数据覆盖错误。
- 错误2：DP 数组大小开成 `n` 而不是 `n+1`（F(0) 到 F(n) 共 n+1 个值），导致 n=0 时数组为空或越界。
- 错误3：记忆化搜索中 memo 数组初始化为 0，但 F(0) = 0，导致每次都在重复计算 F(0)（应初始化为 -1 或其他不合法值）。

### 面试陷阱

- 陷阱1：面试官追问"能否在 O(log n) 时间内求解？"——可以使用矩阵快速幂：`[[F(n)],[F(n-1)]] = [[1,1],[1,0]]^n * [[F(1)],[F(0)]]`。或者使用通项公式（涉及无理数精度问题）。
- 陷阱2：面试官追问"如果 n 极大（如 n = 10^18），要求取模结果怎么做？"——使用矩阵快速幂 + 取模，O(log n) 即可。
- 陷阱3：追问"递归、记忆化搜索、迭代 DP 分别在什么场景下使用？"——递归最直观但最慢；记忆化搜索适合"不是所有子问题都会被用到"的场景（如自顶向下不必计算所有 dp 状态）；迭代 DP 适合需要全部子问题的场景且代码更简洁。

---

## 29. 爬楼梯 (Climbing Stairs) — 题号 70

**难度**: 简单
**标签**: DP、滚动数组

### 题目大意

假设你正在爬楼梯，需要 n 阶才能到达楼顶。每次你可以爬 1 或 2 个台阶。你有多少种不同的方法可以爬到楼顶？

### 思路

这道题是斐波那契的"马甲题"。关键在于发现状态转移方程：

爬到第 n 阶的方法数 = 爬到第 n-1 阶的方法数（再爬 1 步） + 爬到第 n-2 阶的方法数（再爬 2 步）。

即：`dp[n] = dp[n-1] + dp[n-2]`，初始条件 `dp[1] = 1, dp[2] = 2`。

**为什么能想到 DP？** "求方法总数"、"每次只能走 1 或 2 步"——这类"决策累加"问题天然适合 DP。最后一步有两种可能：要么从 n-1 阶走 1 步，要么从 n-2 阶走 2 步。将这两种情况的方案数相加即可。

**为什么是斐波那契？** `dp[1]=1, dp[2]=2, dp[n]=dp[n-1]+dp[n-2]`，本质是平移一格的斐波那契数列。

**滚动数组优化**：由于 `dp[n]` 只依赖前两个状态，用两个变量滚动即可，空间 O(1)。

### 核心知识点

- 动态规划的状态转移方程推导
- "最后一步"分析法
- 爬楼梯 = 斐波那契的发现（归纳能力）
- 滚动数组空间优化

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(1)（滚动数组）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

int climbStairs(int n) {
    if (n <= 2) return n; // n=1 -> 1种; n=2 -> 2种
    int prev = 1, curr = 2; // dp[1] 和 dp[2]
    // 从 dp[3] 开始迭代
    for (int i = 3; i <= n; i++) {
        int next = prev + curr; // dp[i] = dp[i-2] + dp[i-1]
        prev = curr;
        curr = next;
    }
    return curr;
}
```

### 常见错误

- 错误1：初始条件写错——`dp[1] = 1, dp[2] = 2`，不要写成 `dp[1] = 1, dp[2] = 1`（后者是斐波那契的初始值）。
- 错误2：认为 `dp[0] = 1` 然后 `dp[1] = 1`，这个映射也可以但容易混淆。我们的 `dp[i]` 定义为"爬到第 i 阶的方法数"，`i` 从 1 开始。
- 错误3：循环从 `i = 2` 开始而非法从 `i = 3` 开始，导致 `prev` 和 `curr` 的初始值含义混乱。

### 面试陷阱

- 陷阱1：面试官追问"如果每次可以爬 1、2、3 阶呢？"——`dp[n] = dp[n-1] + dp[n-2] + dp[n-3]`，滚动数组需要三个变量。
- 陷阱2：面试官追问"如果每次可以爬 1 到 k 阶（k 可变），怎么做？"——`dp[n] = sum(dp[n-1], ..., dp[n-k])`，可用滑动窗口（前缀和优化）或维护最近 k 个状态。
- 陷阱3：追问"完全背包问题和爬楼梯问题的区别？"——爬楼梯中 1,2 和 2,1 是不同的方案（顺序重要），而完全背包中相同硬币组合视为一种方案（顺序无关）。本题强调走台阶的顺序，因此是排列问题。

---

## 30. 买卖股票的最佳时机 (Best Time to Buy and Sell Stock) — 题号 121

**难度**: 简单
**标签**: 数组、贪心、DP

### 题目大意

给定一个数组 `prices`，其中 `prices[i]` 表示某支股票第 i 天的价格。你只能选择某一天买入这只股票，并选择在未来的某一个不同的日子卖出该股票。计算你所能获取的最大利润。如果你不能获取任何利润，返回 0。

### 思路

本题只能买卖一次，因此问题等价于：在数组中找两个位置 `i < j`，使得 `prices[j] - prices[i]` 最大。

**暴力法**：枚举所有 `i < j`，计算差值，O(n^2)。

**贪心优化**：遍历数组时，如果我们知道"到当前为止之前的最小价格"，那么今天卖出能获得的最大利润 = `prices[i] - minPriceSoFar`。我们在遍历过程中持续更新 `minPriceSoFar` 和 `maxProfit`。

为什么能想到贪心？"只能买卖一次"意味着买卖是独立事件——我们希望买入价尽可能低、卖出价尽可能高。但买入必须在卖出之前。因此，在遍历每一天时，把当天当作"潜在卖出日"，我们需要知道前面出现过的最低价格（即最佳买入价）。维护一个"前缀最小值"完美对应这个需求。

从 DP 角度理解：`dp[i]` 表示"截至第 i 天的最大利润"（必须第 i 天卖出），则 `dp[i] = max(dp[i-1] + prices[i] - prices[i-1], 0)`，即"延续之前的利润"或"从今天重新开始（利润归零）"。

### 核心知识点

- 贪心：前缀最小值 + 最大差值
- DP：最大子数组和（Kadane 算法）的变体
- "只能买卖一次"的简化性质

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法一：贪心（维护前缀最小值）
int maxProfit(vector<int>& prices) {
    if (prices.empty()) return 0;
    int minPrice = prices[0]; // 到目前为止的最低价格（最佳买入点）
    int maxProfit = 0;         // 最大利润
    for (int i = 1; i < prices.size(); i++) {
        // 如果今天卖出，能获得的利润 = 今天价格 - 历史最低价
        maxProfit = max(maxProfit, prices[i] - minPrice);
        // 更新历史最低价
        minPrice = min(minPrice, prices[i]);
    }
    return maxProfit;
}

// 解法二：DP / Kadane 变体（将价格序列转为"每日变化"序列）
int maxProfit_DP(vector<int>& prices) {
    if (prices.empty()) return 0;
    int curProfit = 0;  // 当前累计利润
    int maxProfit = 0;
    for (int i = 1; i < prices.size(); i++) {
        // 每天的变化：diff = prices[i] - prices[i-1]
        // 将利润累加，如果累加成负数则归零（重新选择买入点）
        curProfit = max(0, curProfit + prices[i] - prices[i - 1]);
        maxProfit = max(maxProfit, curProfit);
    }
    return maxProfit;
}
```

### 常见错误

- 错误1：`minPrice` 初始化为 0 而非 `prices[0]`，导致所有价格大于 0 时 `prices[i] - 0` 给出错误值。
- 错误2：遍历从 `i = 0` 开始而非 `i = 1`（第一天只能买不能卖，利润必为 0）。
- 错误3：DP 解法中 `curProfit = max(0, curProfit + diff)`，忘记与 0 取 max——相当于扔掉前面的"负贡献"积累，重新开始。

### 面试陷阱

- 陷阱1：面试官追问"如果可以多次买卖（同一时间只能持有一股），最大利润怎么算？"——LeetCode 122，贪心：收集所有正的相邻差值（`prices[i] - prices[i-1]` 的正值之和）。
- 陷阱2：面试官追问"如果最多可以交易两次呢？"——LeetCode 123，需要三维 DP（第几天、交易了几次、当前是否持有股票）。
- 陷阱3：面试官追问"如果最多交易 K 次呢？"——LeetCode 188，通用 DP 框架：`dp[i][k][0/1]` 表示第 i 天、交易了 k 次、是否持有股票时的最大利润。这是买卖股票的终极分析框架。
- 陷阱4：追问"股价全是递减的（如 `[5,4,3,2,1]`），算法返回什么？"——返回 0（无法获利），这是正确的处理。

---

# LeetCode Hot200 刷题文档 — 阶段二a：基础（前半，22题）

---

## 31. 三数之和 (3Sum) — 题号 15

**难度**: 中等
**标签**: 数组、双指针、排序

### 题目大意

给定一个整数数组 `nums`，找出所有和为 0 且不重复的三元组 `[nums[i], nums[j], nums[k]]`，满足 `i != j != k`。

### 思路

从暴力 O(n³) 出发思考优化：

1. **暴力三重循环**：枚举所有 i, j, k 组合，复杂度 O(n³)，且去重非常麻烦——即使用 set 去重，O(n³) 也无法通过 3000 长度的数据。

2. **两数之和降维**：固定一个数 a，问题变成找 b + c = -a，即 Two Sum。Two Sum 可以用哈希表 O(n) 解决，整体 O(n²)。但哈希表方案要处理去重，需要在结果层面用 set，常数大且不优雅。

3. **排序 + 双指针（最终方案）**：排序后，固定第一个数 nums[i]（最外层循环），内层对区间 [i+1, n-1] 做双指针：
   - 若 `sum = nums[i] + nums[L] + nums[R] == 0`，收集结果
   - 若 sum < 0，L 右移；若 sum > 0，R 左移
   - 排序后相同的数相邻，天然方便去重

**去重是本题最容易出错的地方，分三层**：
- **第一层**：`if (i > 0 && nums[i] == nums[i-1]) continue;` — 跳过相同的第一个数
- **第二层**：收集到结果后，`while (L < R && nums[L] == nums[L+1]) L++;` — 跳过相同的左指针值
- **第三层**：收集到结果后，`while (L < R && nums[R] == nums[R-1]) R--;` — 跳过相同的右指针值

为什么去重放在找到答案之后而不是之前？因为放在之前可能漏掉第一次出现的合法组合。

### 核心知识点

- 排序 + 双指针将 O(n³) 降为 O(n²)
- 三层去重技巧：固定数去重 + 左右指针各自去重
- 剪枝优化：nums[i] > 0 直接 break（排序后最小的数都 >0 则不可能和为 0）

### 复杂度

- **时间复杂度**: O(n²) — 排序 O(n log n)，双指针 O(n²)
- **空间复杂度**: O(1) — 不计返回结果

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> threeSum(vector<int>& nums) {
    vector<vector<int>> res;
    int n = nums.size();
    sort(nums.begin(), nums.end());
    
    for (int i = 0; i < n - 2; i++) {
        // 剪枝：最小的数 > 0，不可能有三数之和为0
        if (nums[i] > 0) break;
        // 第一层去重：跳过相同的第一个数
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        
        int L = i + 1, R = n - 1;
        while (L < R) {
            int sum = nums[i] + nums[L] + nums[R];
            if (sum == 0) {
                res.push_back({nums[i], nums[L], nums[R]});
                // 第二层去重：跳过重复的左指针值
                while (L < R && nums[L] == nums[L + 1]) L++;
                // 第三层去重：跳过重复的右指针值
                while (L < R && nums[R] == nums[R - 1]) R--;
                L++; R--;
            } else if (sum < 0) {
                L++;
            } else {
                R--;
            }
        }
    }
    return res;
}

int main() {
    vector<int> nums1 = {-1, 0, 1, 2, -1, -4};
    // 期望输出: [[-1,-1,2], [-1,0,1]]
    auto res = threeSum(nums1);
    for (auto& v : res) {
        cout << "[";
        for (int i = 0; i < v.size(); i++) {
            cout << v[i] << (i + 1 < v.size() ? "," : "");
        }
        cout << "] ";
    }
    cout << endl;
    
    // 测试去重: [0,0,0,0]
    vector<int> nums2 = {0, 0, 0, 0};
    res = threeSum(nums2);
    // 期望输出: [[0,0,0]] 仅一个
    cout << "nums2 size: " << res.size() << endl; // 应为1
    return 0;
}
```

### 常见错误

- **错误1**：去重条件写成 `nums[i] == nums[i+1]` 而非 `nums[i] == nums[i-1]`。区别在于：前者会跳过"后面相同"的数，导致第一次出现的值被跳过从而漏解；后者跳过"前面已经处理过"的相同值，第一次出现时正常处理。
- **错误2**：找到 sum == 0 后忘记同时移动 L 和 R，导致死循环。或者只移动 L 不移 R（反之亦然），sum 不变继续重复收集。
- **错误3**：剪枝写成 `nums[i] >= 0` 直接 break。当数组为 `[0,0,0]` 时 nums[i]=0，三数之和确实可为 0，不应剪掉。

### 面试陷阱

- **陷阱1**：面试官会问"为什么不用哈希表？"考察你是否理解排序+双指针在去重方面的优势——哈希表方案需要处理重复组合（如有多组相同的 a、b、c），去重逻辑麻烦且常数大。
- **陷阱2**：面试官追问"如果不止三个数，而是 k 个数之和怎么办？"考察能否推广到 kSum 递归降维框架。

---

## 32. 盛最多水的容器 (Container With Most Water) — 题号 11

**难度**: 中等
**标签**: 数组、贪心、双指针

### 题目大意

数组 `height` 的每个元素代表垂直线的高度，找出两条线与 x 轴构成的容器能容纳的最大水量。容器不能倾斜，面积 = 底边宽度 × 两条线中较短的高度。

### 思路

1. **暴力枚举**：枚举所有左右边界的组合 O(n²)，计算面积取最大值。n 可达 10⁵，O(n²) 必定超时。

2. **双指针贪心（最终方案）**：初始化 L=0, R=n-1（底边最宽）。每次计算当前面积并更新最大值。关键决策：**移动较矮的那一侧的指针**。为什么？

   - 面积由"底边宽度"和"较短高度"共同决定。当前状态底边最宽，要获得更大面积，只能寄希望于提高"短板高度"。
   - 如果移动较高的那一侧：底边变窄，且高度仍受限于原本的矮板（或更矮）。面积只会变小。
   - 如果移动较矮的那一侧：虽然底边变窄，但可能遇到更高的板，弥补甚至超过宽度损失。
   - 这本质是一个贪心：每次丢弃"已经不可能成为最优解中短板"的那一侧。

3. 为什么这样做不会漏掉最优解？用"状态空间消除"论证：对任意最优解 (i, j)，当指针从两端向中间移动时，较矮一侧会先被淘汰——在被淘汰前，最优解的状态已被计算过。

### 核心知识点

- 双指针从两端向中间收缩
- 贪心策略：永远移动较矮指针
- 面积由短板决定，因此"提升短板"是唯一改进方向

### 复杂度

- **时间复杂度**: O(n) — 每个元素被访问一次
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

int maxArea(vector<int>& height) {
    int L = 0, R = height.size() - 1;
    int ans = 0;
    while (L < R) {
        // 计算当前容器的面积
        int area = min(height[L], height[R]) * (R - L);
        ans = max(ans, area);
        // 核心贪心：移动较矮一侧，期望遇到更高的板
        if (height[L] < height[R]) {
            L++;
        } else {
            R--;
        }
    }
    return ans;
}

int main() {
    vector<int> h1 = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    cout << maxArea(h1) << endl; // 期望: 49 (板高7和8, 宽度7)
    
    vector<int> h2 = {1, 1};
    cout << maxArea(h2) << endl; // 期望: 1
    
    vector<int> h3 = {4, 3, 2, 1, 4};
    cout << maxArea(h3) << endl; // 期望: 16
    return 0;
}
```

### 常见错误

- **错误1**：移动条件写反——移动较高的那一侧。这是最常见的错误，等价于把贪心方向搞反，会漏解。
- **错误2**：只用高度乘指针差计算面积，忘了取 min(height[L], height[R])。容器高度由短板决定，不是高板也不是平均。
- **错误3**：for 循环用 `for (int i=0; i<n; i++) for (int j=i+1; j<n; j++)` 暴力枚举。面试中写暴力不会加分，直接给最优解。

### 面试陷阱

- **陷阱1**：面试官追问"为什么移动矮板一定对？给出严格证明。"考察状态空间消除思想——对于当前 (L,R)，假设 h[L] < h[R]，那么 (L, R-1), (L, R-2), ..., (L, L+1) 这些 L 为左边界的情况面积都小于等于当前面积（底边更窄，高度受限于 h[L]），所以 L 作为左边界已不可能成为最优解，可以安全丢弃。
- **陷阱2**：追问"如果两根线一样高怎么办？"答案：移动哪一侧都可以，因为两边都不可能是更优解的短板（任意一侧移动后，新的组合底边更窄，高度受限于当前高度，不可能更优）。

---

## 33. 接雨水 (Trapping Rain Water) — 题号 42

**难度**: 困难
**标签**: 数组、双指针、单调栈、DP

### 题目大意

给定非负整数数组 `height`，表示柱子高度。下雨后柱子之间能接多少雨水？

### 思路

**解法一：双指针（对撞指针）—— 最优解 O(n) 时间 O(1) 空间**

直观理解：每一列能接多少水？取决于它**左边最高柱**和**右边最高柱**中较矮的那个，再减去自身高度：
```
water[i] = min(leftMax[i], rightMax[i]) - height[i]
```
如果 leftMax 和 rightMax 用 DP 预处理，需要 O(n) 空间。如何做到 O(1) 空间？

双指针 L=0, R=n-1，维护 `leftMax`（从左到当前位置见过的最高柱）和 `rightMax`（从右到当前位置见过的最高柱）。核心洞察：

- 如果 `leftMax < rightMax`：位置 L 的接水量由 leftMax 决定（因为 rightMax 至少比 leftMax 大，min(leftMax, rightMax) = leftMax），我们已经有确切信息，可以安全计算 `water[L] = leftMax - height[L]`，然后 L++。
- 如果 `leftMax >= rightMax`：同理，位置 R 的接水量由 rightMax 决定，计算 `water[R] = rightMax - height[R]`，R--。

**解法二：单调栈（递减栈）—— O(n) 时间 O(n) 空间**

维护一个**单调递减栈**（栈底大、栈顶小，存下标）。遍历时：
- 当 `height[i] <= height[stk.top()]`，当前高度不高于栈顶，直接入栈（坑还没形成）。
- 当 `height[i] > height[stk.top()]`，当前高度高于栈顶，说明栈顶元素是一个"坑底"：
  - 弹出坑底 top，如果栈为空说明坑底左边无边界，无法蓄水
  - 取新的栈顶作为左边界 left，当前 i 为右边界
  - 宽度 = i - left - 1，高度 = min(height[left], height[i]) - height[top]
  - 累加蓄水量
  - 继续循环判断新的栈顶

通俗理解：单调栈方法在遇到"右墙比左墙高"时，一层一层地填坑。

### 核心知识点

- 双指针对撞法：通过比较左右最大高度，确定当前位置的蓄水量
- 单调栈法：层层填坑，宽度 × 高度差计算蓄水量
- DP 预处理法：`leftMax[i] = max(leftMax[i-1], height[i])`，`rightMax` 同理

### 复杂度

**双指针法**：
- 时间复杂度: O(n)
- 空间复杂度: O(1)

**单调栈法**：
- 时间复杂度: O(n)
- 空间复杂度: O(n)

### C++ 代码 (ACM风格)

#### 解法一：双指针（O(1) 空间）

```cpp
#include <bits/stdc++.h>
using namespace std;

int trap(vector<int>& height) {
    int n = height.size();
    if (n <= 2) return 0; // 少于3根柱子无法蓄水
    
    int L = 0, R = n - 1;
    int leftMax = 0, rightMax = 0;
    int ans = 0;
    
    while (L < R) {
        leftMax = max(leftMax, height[L]);   // 更新左侧所见最高
        rightMax = max(rightMax, height[R]); // 更新右侧所见最高
        
        if (leftMax < rightMax) {
            // leftMax 较小，L处的水量由 leftMax 决定
            ans += leftMax - height[L];
            L++;
        } else {
            // rightMax 较小或相等，R处的水量由 rightMax 决定
            ans += rightMax - height[R];
            R--;
        }
    }
    return ans;
}

int main() {
    vector<int> h1 = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    cout << "解法一: " << trap(h1) << endl; // 期望: 6
    
    vector<int> h2 = {4, 2, 0, 3, 2, 5};
    cout << "解法二: " << trap(h2) << endl; // 期望: 9
    return 0;
}
```

#### 解法二：单调栈

```cpp
#include <bits/stdc++.h>
using namespace std;

int trapStack(vector<int>& height) {
    int n = height.size();
    if (n <= 2) return 0;
    
    stack<int> stk; // 单调递减栈，存下标
    int ans = 0;
    
    for (int i = 0; i < n; i++) {
        // 当前高度 > 栈顶高度，说明栈顶是"坑底"
        while (!stk.empty() && height[i] > height[stk.top()]) {
            int bottom = stk.top(); // 坑底
            stk.pop();
            if (stk.empty()) break; // 左边无墙，无法蓄水
            
            int left = stk.top();          // 左边界
            int w = i - left - 1;          // 宽度
            int h = min(height[left], height[i]) - height[bottom]; // 蓄水高度
            ans += w * h;                  // 累加
        }
        stk.push(i);
    }
    return ans;
}

int main() {
    vector<int> h1 = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    cout << "单调栈: " << trapStack(h1) << endl; // 期望: 6
    
    vector<int> h2 = {4, 2, 0, 3, 2, 5};
    cout << "单调栈: " << trapStack(h2) << endl; // 期望: 9
    return 0;
}
```

### 常见错误

- **错误1**：双指针法先更新 leftMax/rightMax 再比较，还是先比较再更新？正确顺序是**先更新再比较**，因为当前位置的蓄水量需要使用当前柱子作为"可能的边界"，leftMax 应包含 height[L]。
- **错误2**：单调栈法中计算高度时写成 `min(height[left], height[i]) - height[i]`，正确是用坑底高度 height[bottom] 而非当前高度 height[i]。
- **错误3**：忘记处理 n <= 2 的边界情况（少于 3 根柱子无法蓄水），导致越界或错误结果。

### 面试陷阱

- **陷阱1**：面试官追问"双指针法和单调栈法分别适用于什么场景？"双指针法适合快速求总量，单调栈适合需要知道每个凹坑具体形状和蓄水分布时（如需要知道每个位置的水量）。
- **陷阱2**：追问"能否在像素/二维版接雨水中使用同样思路？"二维接雨水（LeetCode 407）需要 Dijkstra 或优先队列，一维的双指针思路是特殊情况的简化。

---

## 34. 两数之和 II - 输入有序数组 (Two Sum II) — 题号 167

**难度**: 中等
**标签**: 数组、双指针、二分

### 题目大意

给定已升序排列的数组 `numbers`，找到两个数使它们的和等于目标值 `target`，返回两个数的下标（1-indexed）。保证有唯一解。

### 思路

1. **暴力 O(n²)**：双层循环枚举所有组合。n 可达 3×10⁴，太慢。

2. **二分查找 O(n log n)**：固定一个数 `numbers[i]`，在 `[i+1, n-1]` 中二分查找 `target - numbers[i]`。利用了数组有序的性质，但时间复杂度不如双指针。

3. **哈希表 O(n)**：直接用 unordered_map 做，但不利用"有序"这个条件，空间 O(n)。面试中写哈希表不会错，但也不加分。

4. **双指针 O(n) O(1)（最优）**：L=0, R=n-1，计算 sum = numbers[L] + numbers[R]：
   - sum == target → 找到答案
   - sum < target → 需要更大的和，L 右移（数组递增，加大较小的加数）
   - sum > target → 需要更小的和，R 左移（减小较大的加数）
   
   这是利用有序性最直接的方法，且达到最优时空。

### 核心知识点

- 有序数组利用双指针，O(n) 时间 O(1) 空间
- 二分查找作为备选方案
- 与第一题 Two Sum (LeetCode 1) 的对比：未排序版本用哈希表；已排序版本双指针更优

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<int> twoSum(vector<int>& numbers, int target) {
    int L = 0, R = numbers.size() - 1;
    while (L < R) {
        int sum = numbers[L] + numbers[R];
        if (sum == target) {
            return {L + 1, R + 1}; // 1-indexed
        } else if (sum < target) {
            L++;
        } else {
            R--;
        }
    }
    return {};
}

// 二分查找版本（备选）
vector<int> twoSumBinary(vector<int>& numbers, int target) {
    int n = numbers.size();
    for (int i = 0; i < n; i++) {
        int need = target - numbers[i];
        int L = i + 1, R = n - 1;
        while (L <= R) {
            int mid = L + (R - L) / 2;
            if (numbers[mid] == need) return {i + 1, mid + 1};
            else if (numbers[mid] < need) L = mid + 1;
            else R = mid - 1;
        }
    }
    return {};
}

int main() {
    vector<int> nums1 = {2, 7, 11, 15};
    auto res1 = twoSum(nums1, 9);
    cout << res1[0] << " " << res1[1] << endl; // 期望: 1 2
    
    vector<int> nums2 = {2, 3, 4};
    auto res2 = twoSum(nums2, 6);
    cout << res2[0] << " " << res2[1] << endl; // 期望: 1 3
    
    // 测试含负数的情况
    vector<int> nums3 = {-1, 0};
    auto res3 = twoSum(nums3, -1);
    cout << res3[0] << " " << res3[1] << endl; // 期望: 1 2
    return 0;
}
```

### 常见错误

- **错误1**：返回下标时忘记使用 1-indexed，直接返回了 L 和 R（0-indexed）。题目明确要求下标从 1 开始。
- **错误2**：双指针条件写成 `L < R` 但循环内没有正确处理 sum==target 的结束，导致死循环。
- **错误3**：二分查找版本中，`L = i + 1` 写成 `L = i`，可能找到同一个元素被使用两次（但题目要求两个不同元素）。

### 面试陷阱

- **陷阱1**：面试官追问"如果数组有重复元素，双指针法还能用吗？"可以。重复元素不影响双指针逻辑，因为题目保证有唯一解。如果有多个解，双指针仍然能找到其中之一。
- **陷阱2**：追问"如果有多个解，返回所有解怎么做？"此时找到一组解后，不能直接返回——需要 L++, R-- 继续搜索，同时跳过重复元素（类似三数之和的去重逻辑）。

---

## 35. 四数之和 (4Sum) — 题号 18

**难度**: 中等
**标签**: 数组、双指针、排序

### 题目大意

给定数组 `nums` 和目标值 `target`，找出所有不重复的四元组 `[a, b, c, d]` 使 `a+b+c+d = target`。

### 思路

这是 3Sum 的推广。核心是将 kSum 问题递归降维为双指针：

1. **排序数组**，使得双指针可行、去重方便。
2. **固定前两个数**：双层循环枚举 `i` 和 `j`（`j > i`）。
3. **对剩余区间双指针**：L = j+1, R = n-1，找 `nums[L] + nums[R] = target - nums[i] - nums[j]`。
4. **去重同样分三层**：i 去重、j 去重、L/R 各自去重。

值得注意的是，可以额外添加**剪枝**加速：
- `nums[i] + nums[i+1] + nums[i+2] + nums[i+3] > target` → 最小的四个都超过 target，后续只会更大，break
- `nums[i] + nums[n-3] + nums[n-2] + nums[n-1] < target` → 当前 i 配上最大三个仍不足，continue

### 核心知识点

- kSum 降维框架：递归将 kSum 转化为 (k-1)Sum，最终归约为双指针
- 四层去重
- 剪枝优化思想

### 复杂度

- **时间复杂度**: O(n³) — 排序 O(n log n)，三重循环 O(n³)
- **空间复杂度**: O(1) — 不计返回结果

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> fourSum(vector<int>& nums, int target) {
    vector<vector<int>> res;
    int n = nums.size();
    if (n < 4) return res;
    
    sort(nums.begin(), nums.end());
    
    for (int i = 0; i < n - 3; i++) {
        // 剪枝1：当前最小的四个数之和 > target
        if ((long long)nums[i] + nums[i+1] + nums[i+2] + nums[i+3] > target) break;
        // 剪枝2：当前 i 配上最大的三个数 < target
        if ((long long)nums[i] + nums[n-3] + nums[n-2] + nums[n-1] < target) continue;
        // i 去重
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        
        for (int j = i + 1; j < n - 2; j++) {
            // 剪枝：当前 i,j 配上最小的两个 > target
            if ((long long)nums[i] + nums[j] + nums[j+1] + nums[j+2] > target) break;
            // 剪枝：当前 i,j 配上最大的两个 < target
            if ((long long)nums[i] + nums[j] + nums[n-2] + nums[n-1] < target) continue;
            // j 去重
            if (j > i + 1 && nums[j] == nums[j - 1]) continue;
            
            int L = j + 1, R = n - 1;
            // 转为 long long 避免 4 个 int 相加溢出
            long long need = (long long)target - nums[i] - nums[j];
            while (L < R) {
                int sum = nums[L] + nums[R];
                if (sum == need) {
                    res.push_back({nums[i], nums[j], nums[L], nums[R]});
                    while (L < R && nums[L] == nums[L + 1]) L++;
                    while (L < R && nums[R] == nums[R - 1]) R--;
                    L++; R--;
                } else if (sum < need) {
                    L++;
                } else {
                    R--;
                }
            }
        }
    }
    return res;
}

int main() {
    vector<int> nums1 = {1, 0, -1, 0, -2, 2};
    auto res1 = fourSum(nums1, 0);
    // 期望: [[-2,-1,1,2], [-2,0,0,2], [-1,0,0,1]]
    for (auto& v : res1) {
        cout << "[";
        for (int i = 0; i < 4; i++) cout << v[i] << (i < 3 ? "," : "] ");
    }
    cout << endl;
    
    vector<int> nums2 = {2, 2, 2, 2, 2};
    auto res2 = fourSum(nums2, 8);
    cout << "res2 size: " << res2.size() << endl; // 期望: 1 ([2,2,2,2])
    return 0;
}
```

### 常见错误

- **错误1**：没有用 long long 处理中间加法，4 个 int 相加可能溢出。如 `nums = {1000000000, 1000000000, 1000000000, 1000000000}`，`target = -294967296`。
- **错误2**：j 去重条件写错——写成 `nums[j] == nums[j+1]` 而不是 `nums[j] == nums[j-1]`。与 3Sum 同理，应该跳过"前面已处理过的"相同值。
- **错误3**：剪枝条件写反——把 break 和 continue 的条件弄混。break 是"最小的都超过 target"，continue 是"最大的都不到 target"。

### 面试陷阱

- **陷阱1**：面试官追问"4Sum 能否用哈希表做？"可以——存两两之和的哈希表，O(n²) 空间，但去重逻辑比双指针复杂得多，容易出现重复组合。
- **陷阱2**：追问"推广到 kSum 怎么设计？"用递归函数 `kSum(k, start, target)`，当 k==2 时走双指针，否则递归固定一个数并调用 kSum(k-1, ...)。

---

## 36. 无重复字符的最长子串 (Longest Substring Without Repeating Characters) — 题号 3

**难度**: 中等
**标签**: 哈希表、滑动窗口、字符串

### 题目大意

给定字符串 `s`，找出不含重复字符的最长子串的长度。

### 思路

1. **暴力 O(n²)**：枚举所有子串，判断是否有重复，总复杂度 O(n³) 或 O(n²) 取决于判重方法。

2. **滑动窗口 O(n)（最优）**：维护窗口 [L, R] 内无重复字符。用哈希表 `pos[256]` 或 `unordered_map<char,int>` 记录每个字符最近出现的位置：
   - 右指针 R 逐步扩展，遇到字符 c
   - 如果 c 在当前窗口内出现过（`pos[c] >= L`），则将 L 跳到 `pos[c] + 1`（跳过重复字符）
   - 更新 `pos[c] = R`，更新答案 `ans = max(ans, R - L + 1)`

   窗口长度任何时候都在变大或保持不变，重来不会回退（L 只增不减），所以每个字符最多被访问两次（R 一次，L 一次），时间复杂度 O(n)。

### 核心知识点

- 滑动窗口模板：右指针扩展、左指针收缩
- 哈希表存最近位置实现 O(1) 查重
- 窗口有效性条件：窗口内所有字符唯一

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(1) — 字符集最多 256（或 ASCII 128 / 扩展 ASCII）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

int lengthOfLongestSubstring(string s) {
    vector<int> pos(128, -1); // ASCII 字符最近出现位置，-1 表示未出现
    int L = 0, ans = 0;
    
    for (int R = 0; R < s.length(); R++) {
        char c = s[R];
        // 如果 c 在当前窗口内出现过（pos[c] >= L），收缩左边界
        if (pos[c] >= L) {
            L = pos[c] + 1;
        }
        // 更新 c 的最新位置
        pos[c] = R;
        // 更新最长长度
        ans = max(ans, R - L + 1);
    }
    return ans;
}

int main() {
    cout << lengthOfLongestSubstring("abcabcbb") << endl; // 期望: 3 ("abc")
    cout << lengthOfLongestSubstring("bbbbb") << endl;    // 期望: 1 ("b")
    cout << lengthOfLongestSubstring("pwwkew") << endl;   // 期望: 3 ("wke")
    cout << lengthOfLongestSubstring("") << endl;         // 期望: 0
    cout << lengthOfLongestSubstring("aab") << endl;      // 期望: 2 ("ab")
    return 0;
}
```

### 常见错误

- **错误1**：L 跳到 `pos[c]` 而不是 `pos[c] + 1`。如果跳到重复字符本身，窗口仍包含该字符，无法达到无重复的要求。
- **错误2**：用 `count()` 判断是否存在而不用位置信息。如果字符出现过但在窗口左侧（pos[c] < L），仍应视为窗口内不存在。
- **错误3**：更新答案的时机放在 L 收缩之前，导致可能包含重复字符时的长度被记录。

### 面试陷阱

- **陷阱1**：面试官追问"如果字符集是 Unicode 怎么办？"答案把 `vector<int>(128)` 换成 `unordered_map<char, int>`，但这只是实现细节，滑动窗口思想不变。
- **陷阱2**：追问"能否用队列或双端队列模拟窗口替代 pos 数组？"可以但效率更低：deque 需要 O(k) 时间找到重复元素并弹出。哈希表 O(1) 更优。

---

## 37. 找到字符串中所有字母异位词 (Find All Anagrams) — 题号 438

**难度**: 中等
**标签**: 哈希表、滑动窗口、字符串

### 题目大意

给定字符串 `s` 和 `p`，找到 `s` 中所有 `p` 的字母异位词（Anagram）的起始索引。Anagram 指字母组成完全相同但顺序可不同的子串。

### 思路

1. **暴力**：枚举每个位置，截取长度等于 `p` 的子串，排序比较。每次 O(k log k)（k 是 p 的长度），总共 O(n · k log k)，太慢。

2. **滑动窗口 + 计数比较 O(26n)**：维护一个固定大小（等于 p.length()）的滑动窗口，用长度为 26 的数组统计窗口内各字母出现频率，每滑动一次比较窗口频率与 p 的频率。O(26n)，常数较大但可接受。

3. **滑动窗口 + 差值计数 O(n)（最优）**：用 `diff` 变量记录窗口与 p 之间有多少种字母频率不匹配。每次滑动只需更新被移出和移入的两个字母，O(1) 判断。具体：
   - `need[26]`：p 中每个字母的需求量
   - 窗口每滑一步：移除左边字符 → 如果移除后该字符频率从"匹配"变为"不匹配"，diff 变化；添加右边字符同理
   - 当 diff == 0 时窗口是 Anagram

### 核心知识点

- 定长滑动窗口模板
- 频率数组统计 Anagram
- 差值计数将比较的 O(26) 降为 O(1)

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(1) — 固定 26 字母

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<int> findAnagrams(string s, string p) {
    vector<int> res;
    int n = s.length(), k = p.length();
    if (n < k) return res;
    
    // need: p 中每个字母的需求计数
    // window: 当前窗口中每个字母的计数
    vector<int> need(26, 0), window(26, 0);
    for (char c : p) need[c - 'a']++;
    
    // 初始化第一个窗口（前 k 个字符）
    for (int i = 0; i < k; i++) window[s[i] - 'a']++;
    if (need == window) res.push_back(0);
    
    // 滑动窗口：移出 s[i]，移入 s[i+k]
    for (int i = 0; i < n - k; i++) {
        window[s[i] - 'a']--;           // 移出左边界
        window[s[i + k] - 'a']++;       // 移入右边界
        if (need == window) res.push_back(i + 1);
    }
    return res;
}

int main() {
    auto print = [](vector<int>& v) {
        for (int x : v) cout << x << " ";
        cout << endl;
    };
    
    vector<int> r1 = findAnagrams("cbaebabacd", "abc");
    print(r1); // 期望: 0 6
    
    vector<int> r2 = findAnagrams("abab", "ab");
    print(r2); // 期望: 0 1 2
    
    vector<int> r3 = findAnagrams("aa", "bb");
    print(r3); // 期望: (空)
    return 0;
}
```

### 常见错误

- **错误1**：窗口大小不固定，误用可变滑动窗口模板。Anagram 的窗口长度必须等于 p.length()，不能伸缩。
- **错误2**：for 循环边界 `i < n - k` 写成 `i < n`，导致下标 i+k 越界。
- **错误3**：忘记初始化第一个窗口就直接进入滑动循环，漏掉起始位置。

### 面试陷阱

- **陷阱1**：面试官追问"如果比较 need 和 window 不用每次 O(26)，如何做到 O(1)？"引入 diff 计数变量——每次滑动只改变两个字母的计数，维护 diff 增量，判 diff==0。
- **陷阱2**：追问"如果 p 包含大写字母或其他字符怎么办？"将数组大小扩展到 128 或 256，或使用 unordered_map。

---

## 38. 最小覆盖子串 (Minimum Window Substring) — 题号 76

**难度**: 困难
**标签**: 哈希表、滑动窗口、字符串

### 题目大意

给定字符串 `s` 和 `t`，找出 `s` 中包含 `t` 所有字符的最短子串。如果不存在返回空串。

### 思路

**解法一：滑动窗口 + 计数（标准解法）**

这是可变大小滑动窗口的经典模板题：

1. **need[128]** 记录 t 中每个字符的需求量，**window[128]** 记录当前窗口各字符计数。
2. **valid** 变量记录已满足需求的字符种类数。`needCount` 是 t 中不同字符的种类数。
3. 右指针 R 扩展窗口：
   - 当前字符 c 加入窗口，若 `window[c] == need[c]`，valid++
4. 当 valid == needCount（窗口已覆盖所有需求）：
   - 更新最短子串
   - 收缩左指针：移出 s[L]，若 `window[s[L]] == need[s[L]] - 1`，valid--
   - L++，继续检查
5. 直到 R 遍历完整个 s。

**为什么这样做是对的？** 滑动窗口保证了每个可能的覆盖区间都被检查。窗口只扩张或收缩，左指针总在追赶右指针，每个字符被访问 O(1) 次。

### 核心知识点

- 可变大小滑动窗口模板
- valid 变量将"窗口是否覆盖需求"的判断从 O(26) 降到 O(1)
- 窗口收缩条件：valid == needCount

### 复杂度

- **时间复杂度**: O(n + m)，n = len(s)，m = len(t)
- **空间复杂度**: O(1) — 固定 128 ASCII 字符

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

string minWindow(string s, string t) {
    if (s.empty() || t.empty()) return "";
    
    vector<int> need(128, 0);
    int needCount = 0; // t 中有多少种不同字符
    for (char c : t) {
        if (need[c] == 0) needCount++;
        need[c]++;
    }
    
    vector<int> window(128, 0);
    int L = 0, valid = 0;
    int start = 0, minLen = INT_MAX;
    
    for (int R = 0; R < s.length(); R++) {
        char c = s[R];
        window[c]++;
        if (need[c] > 0 && window[c] == need[c]) {
            valid++; // 字符 c 达到需求数量
        }
        
        // 当窗口覆盖所有需求时，尝试收缩左边界找最短
        while (valid == needCount) {
            // 更新最短子串
            if (R - L + 1 < minLen) {
                minLen = R - L + 1;
                start = L;
            }
            // 移出左边界字符
            char leftChar = s[L];
            window[leftChar]--;
            if (need[leftChar] > 0 && window[leftChar] == need[leftChar] - 1) {
                valid--; // 字符 leftChar 不再满足需求
            }
            L++;
        }
    }
    return minLen == INT_MAX ? "" : s.substr(start, minLen);
}

int main() {
    cout << minWindow("ADOBECODEBANC", "ABC") << endl; // 期望: "BANC"
    cout << minWindow("a", "a") << endl;               // 期望: "a"
    cout << minWindow("a", "aa") << endl;              // 期望: ""
    cout << minWindow("ab", "b") << endl;              // 期望: "b"
    return 0;
}
```

### 常见错误

- **错误1**：窗口收缩时 `valid--` 的条件写成 `window[leftChar] < need[leftChar]` 而非 `window[leftChar] == need[leftChar] - 1`。valid 应该在"刚好从满足变为不满足"时减 1，不是任何时候都不满足。
- **错误2**：`needCount` 计算错误——直接用 t.length()。如果 t 中有重复字符（如 "AA"），t.length() 是 2，但不同字符只有 1 种，导致 valid 永远达不到 needCount。
- **错误3**：更新 minLen 放在 while 循环外面，会漏掉某些窗口下的更优解（收缩后的窗口可能也是合法的且更短）。

### 面试陷阱

- **陷阱1**：面试官追问"如果字符集是 Unicode 怎么办？"把 `vector<int>(128)` 换成 `unordered_map<char, int>`，时间仍是 O(n+m)，空间 O(字符种类)。
- **陷阱2**：追问"能否不用 valid 变量，每次检查所有字母？"可以但效率低——每次检查 O(52~128)，整体 O(128n) 可能在某些评测机上超时。

---

## 39. 滑动窗口最大值 (Sliding Window Maximum) — 题号 239

**难度**: 困难
**标签**: 滑动窗口、单调队列、堆

### 题目大意

给定数组 `nums` 和窗口大小 `k`，窗口从左向右滑动，返回每个窗口的最大值。

### 思路

**解法一：单调递减队列 O(n)（最优解）**

维护一个**双端队列** `deque`，存下标，队列内元素对应的值单调递减（队头最大）：

1. 遍历数组，对于当前元素 nums[i]：
   - **清理过期元素**：如果队头下标 ≤ i - k（已滑出窗口），弹出队头
   - **维护单调性**：如果队尾元素对应值 ≤ nums[i]，弹出队尾（这些旧元素比当前小且更靠左，永远不可能成为之后任何窗口的最大值）
   - 将 i 入队尾
   - 当 i ≥ k-1（第一个窗口形成），记录结果 deq.front()

每个元素入队一次、出队一次，均摊 O(n)。

**解法二：优先队列（大顶堆）O(n log k)**

存 pair<值, 下标>，大顶堆。每次滑动：
- 将新元素入堆
- 如果堆顶下标 ≤ i - k，弹出堆顶（已过期）
- 堆顶就是当前窗口最大值

比单调队列慢一个 log 级别，但代码简单直观。

### 核心知识点

- 单调队列：在 O(1) 均摊时间内获取滑动窗口的最值
- 双端队列 push_back / pop_front / pop_back 混用
- 过期元素清理 + 单调性维护

### 复杂度

**单调队列：**
- 时间复杂度: O(n)
- 空间复杂度: O(k)

**优先队列：**
- 时间复杂度: O(n log k)
- 空间复杂度: O(n)

### C++ 代码 (ACM风格)

#### 解法一：单调队列（推荐）

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    vector<int> res;
    deque<int> dq; // 存下标，对应值单调递减
    
    for (int i = 0; i < nums.size(); i++) {
        // 1. 清理过期元素：队头已滑出窗口
        if (!dq.empty() && dq.front() <= i - k) {
            dq.pop_front();
        }
        // 2. 维护单调递减：弹出所有比 nums[i] 小或等的队尾
        while (!dq.empty() && nums[dq.back()] <= nums[i]) {
            dq.pop_back();
        }
        // 3. 当前元素入队
        dq.push_back(i);
        // 4. 窗口形成后记录结果
        if (i >= k - 1) {
            res.push_back(nums[dq.front()]);
        }
    }
    return res;
}

int main() {
    vector<int> nums1 = {1, 3, -1, -3, 5, 3, 6, 7};
    auto res1 = maxSlidingWindow(nums1, 3);
    // 期望: [3, 3, 5, 5, 6, 7]
    for (int x : res1) cout << x << " ";
    cout << endl;
    
    vector<int> nums2 = {1, -1};
    auto res2 = maxSlidingWindow(nums2, 1);
    // 期望: [1, -1]
    for (int x : res2) cout << x << " ";
    cout << endl;
    return 0;
}
```

#### 解法二：优先队列

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<int> maxSlidingWindowHeap(vector<int>& nums, int k) {
    // 大顶堆：pair<值, 下标>，默认按 first 降序
    priority_queue<pair<int, int>> pq;
    vector<int> res;
    
    for (int i = 0; i < nums.size(); i++) {
        pq.push({nums[i], i});
        // 窗口形成后
        if (i >= k - 1) {
            // 弹出所有过期元素（下标不在 [i-k+1, i] 范围内的）
            while (pq.top().second <= i - k) {
                pq.pop();
            }
            res.push_back(pq.top().first);
        }
    }
    return res;
}

int main() {
    vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
    auto res = maxSlidingWindowHeap(nums, 3);
    for (int x : res) cout << x << " ";
    cout << endl; // 期望: 3 3 5 5 6 7
    return 0;
}
```

### 常见错误

- **错误1**：单调队列中弹出条件写成 `nums[dq.back()] < nums[i]` 而非 `<=`。当有多个相等的最大值进入窗口时，如果不用 `<=`，旧的小值可能留在队列中导致结果错误（因为等值也应该被更新为更靠右的那个）。
- **错误2**：清理过期元素的判断写成 `dq.front() < i - k` 而非 `<=`。当窗口右边界为 i 时，有效下标范围是 `[i-k+1, i]`，因此 `dq.front() == i - k` 时正好是过期元素（它在窗口左边界之外）。
- **错误3**：优先队列方法中忘了清理过期元素，堆顶可能是上个窗口留下来的。

### 面试陷阱

- **陷阱1**：面试官追问"单调队列为什么是均摊 O(1) 而不是 O(n)？"每个元素只入队一次、出队一次，总操作次数 ≤ 2n，均摊每次 O(1)。
- **陷阱2**：追问"如果需要窗口的最小值和最大值同时查询怎么办？"用两个单调队列分别维护最大值和最小值即可。

---

## 40. 和为 K 的子数组 (Subarray Sum Equals K) — 题号 560

**难度**: 中等
**标签**: 数组、哈希表、前缀和

### 题目大意

给定整数数组 `nums` 和整数 `k`，统计和为 k 的连续子数组的个数。

### 思路

1. **暴力 O(n²)**：枚举所有子数组区间 [i, j]，累计和。n 可达 2×10⁴，O(n²) = 4×10⁸，会超时。

2. **前缀和 + 哈希表 O(n)（最优）**：
   - 定义 `preSum[i]` = nums[0..i-1] 之和，则子数组 [i, j] 的和 = `preSum[j+1] - preSum[i]`
   - 我们要找 `preSum[j+1] - preSum[i] = k`，即 `preSum[i] = preSum[j+1] - k`
   - 遍历数组，维护当前前缀和 `sum`，用哈希表 `cnt` 存之前各前缀和出现的次数
   - 对于当前 sum，查 `cnt[sum - k]` 就是以前面的某个位置为左边界、当前位置为右边界的满足条件的子数组个数
   - 更新 `cnt[sum]++`

   本质是把"两数之差等于 k"的问题转化为了哈希表查询。时间复杂度 O(n)。

### 核心知识点

- 前缀和将区间和问题转为两数之差
- 哈希表存前缀和频率实现 O(1) 查询
- 类似 Two Sum 的变体（Two Sum 求和等于 target，这里是差等于 k）

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

int subarraySum(vector<int>& nums, int k) {
    unordered_map<int, int> cnt; // 前缀和 -> 出现次数
    cnt[0] = 1; // 空前缀和为0，处理从头开始的子数组
    int sum = 0, ans = 0;
    
    for (int x : nums) {
        sum += x; // 当前前缀和
        // 查询有多少个前面的前缀和等于 sum - k
        if (cnt.find(sum - k) != cnt.end()) {
            ans += cnt[sum - k];
        }
        // 更新当前前缀和计数
        cnt[sum]++;
    }
    return ans;
}

int main() {
    vector<int> nums1 = {1, 1, 1};
    cout << subarraySum(nums1, 2) << endl; // 期望: 2  ([1,1], [1,1])
    
    vector<int> nums2 = {1, 2, 3};
    cout << subarraySum(nums2, 3) << endl; // 期望: 2  ([1,2], [3])
    
    vector<int> nums3 = {-1, -1, 1};
    cout << subarraySum(nums3, 0) << endl; // 期望: 1  ([-1,-1,1])
    
    vector<int> nums4 = {1, -1, 0};
    cout << subarraySum(nums4, 0) << endl; // 期望: 3
    return 0;
}
```

### 常见错误

- **错误1**：哈希表初始没有 `cnt[0] = 1`，导致从数组头开始的子数组（即 sum == k 的情况）被漏掉。
- **错误2**：先 `cnt[sum]++` 再查 `cnt[sum-k]`，导致当 k=0 时把当前空子数组也计入（当前位置自己不算一个子数组）。
- **错误3**：忽略了负数的情况。前缀和可能重复出现多次（因为有负数），因此 cnt 中存的是次数而非布尔值。

### 面试陷阱

- **陷阱1**：面试官追问"如果数组中全是正数怎么办？"滑窗 O(n) 空间 O(1)，比哈希表更优。但前缀和 + 哈希表是通用解法。
- **陷阱2**：追问"和为 K 的子数组，改成小于等于 K 的子数组个数？"用前缀和 + 二分/有序容器（如 TreeMap/Fenwick Tree），复杂度 O(n log n)。

---

## 41. 长度最小的子数组 (Minimum Size Subarray Sum) — 题号 209

**难度**: 中等
**标签**: 数组、滑动窗口、前缀和、二分

### 题目大意

给定正整数数组 `nums` 和目标值 `target`，找出和 >= target 的长度最小的连续子数组，返回其长度。不存在则返回 0。

### 思路

1. **暴力 O(n²)**：枚举所有区间，O(n²) 超时（n 可达 10⁵）。

2. **滑动窗口 O(n)（最优）**：
   - 因为元素都是正数，子数组越长和越大（单调性保证）
   - 右指针 R 扩展，累加 sum；当 sum >= target 时，收缩左指针 L，同时维护最短长度
   - 每个元素被加入和移出各一次，O(n)

   注意：数组有负数的情况下滑动窗口失效，因为窗口扩大不一定增加和。

3. **前缀和 + 二分 O(n log n)（备选）**：
   - 计算前缀和数组 `preSum`（元素为正，preSum 递增）
   - 枚举左边界 i，在 `[i+1, n]` 中二分查找第一个使 `preSum[mid] - preSum[i] >= target` 的位置

### 核心知识点

- 可变长滑动窗口：单调性保证（正数）
- 前缀和 + 二分查找作为通用备选

### 复杂度

**滑动窗口：**
- 时间复杂度: O(n)
- 空间复杂度: O(1)

**前缀和 + 二分：**
- 时间复杂度: O(n log n)
- 空间复杂度: O(n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法一：滑动窗口（正数最优）
int minSubArrayLen(int target, vector<int>& nums) {
    int n = nums.size();
    int L = 0, sum = 0;
    int ans = INT_MAX;
    
    for (int R = 0; R < n; R++) {
        sum += nums[R];
        // 满足条件时尽量收缩窗口
        while (sum >= target) {
            ans = min(ans, R - L + 1);
            sum -= nums[L];
            L++;
        }
    }
    return ans == INT_MAX ? 0 : ans;
}

// 解法二：前缀和 + 二分查找
int minSubArrayLenBinary(int target, vector<int>& nums) {
    int n = nums.size();
    vector<int> preSum(n + 1, 0); // preSum[i] = nums[0...i-1] 之和
    for (int i = 0; i < n; i++) preSum[i + 1] = preSum[i] + nums[i];
    
    int ans = INT_MAX;
    for (int i = 0; i < n; i++) {
        // 二分查找第一个 >= target + preSum[i] 的位置
        int need = target + preSum[i];
        auto it = lower_bound(preSum.begin(), preSum.end(), need);
        if (it != preSum.end()) {
            int len = it - preSum.begin() - i; // 区间长度
            ans = min(ans, len);
        }
    }
    return ans == INT_MAX ? 0 : ans;
}

int main() {
    vector<int> nums1 = {2, 3, 1, 2, 4, 3};
    cout << minSubArrayLen(7, nums1) << endl; // 期望: 2 ([4,3])
    
    vector<int> nums2 = {1, 4, 4};
    cout << minSubArrayLen(4, nums2) << endl; // 期望: 1 ([4])
    
    vector<int> nums3 = {1, 1, 1, 1};
    cout << minSubArrayLen(11, nums3) << endl; // 期望: 0
    return 0;
}
```

### 常见错误

- **错误1**：窗口收缩时忘记更新 ans——收缩后的窗口可能仍是合法且更短的解。更新 ans 应在 `while (sum >= target)` 循环内部。
- **错误2**：边界条件 `ans == INT_MAX` 时返回 0 漏掉——不存在满足条件的子数组。
- **错误3**：在二分查找版本中，`lower_bound` 查找的是 `preSum.begin()` 到 `preSum.end()` 的全区间，起点应为 `preSum.begin() + i + 1`（右边界必须在左边界右侧）。

### 面试陷阱

- **陷阱1**：面试官追问"如果数组中有负数，滑动窗口还能用吗？"不能。因为前缀和不再单调，窗口扩大和可能变小。此时只能用前缀和 + 有序数据结构（如平衡二叉搜索树）。
- **陷阱2**：追问"如果要找恰好等于 target 的最小子数组呢？"正数时滑动窗口改为 sum == target 时记录；有负数时前缀和 + 哈希表（类似题 560）。

---

## 42. 最大子数组和 (Maximum Subarray) — 题号 53

**难度**: 中等
**标签**: 数组、DP、分治

### 题目大意

给定整数数组 `nums`，找和最大的连续子数组，返回最大和。

### 思路

**解法一：动态规划 / Kadane 算法 O(n)**

定义 `dp[i]` = 以 nums[i] 结尾的最大子数组和。状态转移：
```
dp[i] = max(nums[i], dp[i-1] + nums[i])
```
即：要么另起炉灶（nums[i] 单独成段），要么接在前面最大子数组后面。

答案 = max of all dp[i]。空间可以优化为 O(1)：用 pre 代替 dp[i-1]。

**直觉理解**：如果前面的累积和 pre 是负数，那它对后面的贡献是"拖累"——舍弃它另起炉灶更优。这就是 Kadane 算法的核心。

**解法二：分治 O(n log n)**

将数组分成左右两半，最大子数组要么：
- 完全在左半部分
- 完全在右半部分
- 跨越中点（从中点向左扩展 + 从中点向右扩展）

递归求解，合并时 O(n) 计算跨越中点的最大和。

分治法虽然不如 Kadane 高效，但展示了分治思想的可推广性——当问题扩展到二维时（最大子矩阵），分治很有用。

### 核心知识点

- Kadane 算法：`dp[i] = max(num, dp[i-1]+num)`
- 状态定义：以 i 结尾（而非以 i 开头或以 [0,i] 结尾）
- 分治法的合并逻辑

### 复杂度

**DP：**
- 时间复杂度: O(n)
- 空间复杂度: O(1)

**分治：**
- 时间复杂度: O(n log n)
- 空间复杂度: O(log n) — 递归栈

### C++ 代码 (ACM风格)

#### 解法一：Kadane 算法（推荐）

```cpp
#include <bits/stdc++.h>
using namespace std;

int maxSubArray(vector<int>& nums) {
    int pre = 0;    // dp[i-1]
    int ans = nums[0];
    
    for (int x : nums) {
        pre = max(x, pre + x); // 要么另起炉灶，要么接前面
        ans = max(ans, pre);
    }
    return ans;
}

int main() {
    vector<int> nums1 = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    cout << maxSubArray(nums1) << endl; // 期望: 6 ([4,-1,2,1])
    
    vector<int> nums2 = {1};
    cout << maxSubArray(nums2) << endl; // 期望: 1
    
    vector<int> nums3 = {5, 4, -1, 7, 8};
    cout << maxSubArray(nums3) << endl; // 期望: 23 (整个数组)
    return 0;
}
```

#### 解法二：分治法

```cpp
#include <bits/stdc++.h>
using namespace std;

// 跨越中点的最大子数组和
int crossSum(vector<int>& nums, int L, int mid, int R) {
    // 从中点向左扩展的最大和
    int leftSum = INT_MIN, sum = 0;
    for (int i = mid; i >= L; i--) {
        sum += nums[i];
        leftSum = max(leftSum, sum);
    }
    // 从中点+1向右扩展的最大和
    int rightSum = INT_MIN;
    sum = 0;
    for (int i = mid + 1; i <= R; i++) {
        sum += nums[i];
        rightSum = max(rightSum, sum);
    }
    return leftSum + rightSum;
}

int divide(vector<int>& nums, int L, int R) {
    if (L == R) return nums[L];
    int mid = L + (R - L) / 2;
    int leftMax = divide(nums, L, mid);
    int rightMax = divide(nums, mid + 1, R);
    int crossMax = crossSum(nums, L, mid, R);
    return max({leftMax, rightMax, crossMax});
}

int maxSubArrayDivide(vector<int>& nums) {
    return divide(nums, 0, nums.size() - 1);
}

int main() {
    vector<int> nums = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    cout << maxSubArrayDivide(nums) << endl; // 期望: 6
    return 0;
}
```

### 常见错误

- **错误1**：初始化 `ans = 0` 而非 `ans = nums[0]`。如果数组全是负数（如 [-1]），ans=0 是错的（虽然子数组不能为空，但最大和应该是 -1）。LeetCode 要求至少选一个元素。
- **错误2**：分治法中跨越中点的计算方向写反——向左扩展时应该是从中点往左，写成从中点往右。
- **错误3**：Kadane 算法中 `pre = max(x, pre + x)` 写成 `pre = max(0, pre + x)`，后者允许空子数组（和为 0），与题目要求不符。

### 面试陷阱

- **陷阱1**：面试官追问"如果允许空子数组（和为 0），代码怎么改？"初始化 ans=0, pre=0，`pre = max(0, pre + x)`。
- **陷阱2**：追问"最大子矩阵和怎么做？"枚举上下边，压缩成一维后用 Kadane，O(m²n)。分治思想可以直接推广。

---

## 43. 除自身以外数组的乘积 (Product of Array Except Self) — 题号 238

**难度**: 中等
**标签**: 数组、前缀和

### 题目大意

给定数组 `nums`，返回数组 `answer`，其中 `answer[i]` = `nums` 中除 `nums[i]` 外所有元素的乘积。要求不使用除法，O(n) 时间。

### 思路

1. **如果允许除法**：先求总乘积，然后 `answer[i] = total / nums[i]`。但题目禁止除法，且需处理 0 的情况。

2. **左右乘积 O(n) 时间 O(n) 空间**：
   - `left[i]` = nums[0] * ... * nums[i-1]
   - `right[i]` = nums[i+1] * ... * nums[n-1]
   - `answer[i] = left[i] * right[i]`

3. **O(1) 额外空间优化（不计返回数组）**：
   - 先用 answer 数组存左乘积（`answer[i] = nums[0] * ... * nums[i-1]`）
   - 再从右向左遍历，用一个变量 `right` 累积右乘积：`answer[i] *= right; right *= nums[i]`

### 核心知识点

- 前缀积 / 后缀积
- O(1) 空间优化技巧（输出数组不计入额外空间）
- 不依赖除法

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(1) — 不计返回数组

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<int> productExceptSelf(vector<int>& nums) {
    int n = nums.size();
    vector<int> ans(n, 1);
    
    // 第一遍从左到右：ans[i] = nums[0] * ... * nums[i-1]
    int leftProduct = 1;
    for (int i = 0; i < n; i++) {
        ans[i] = leftProduct;
        leftProduct *= nums[i];
    }
    
    // 第二遍从右到左：ans[i] *= nums[i+1] * ... * nums[n-1]
    int rightProduct = 1;
    for (int i = n - 1; i >= 0; i--) {
        ans[i] *= rightProduct;
        rightProduct *= nums[i];
    }
    
    return ans;
}

int main() {
    vector<int> nums1 = {1, 2, 3, 4};
    auto res1 = productExceptSelf(nums1);
    // 期望: [24, 12, 8, 6]
    for (int x : res1) cout << x << " ";
    cout << endl;
    
    vector<int> nums2 = {-1, 1, 0, -3, 3};
    auto res2 = productExceptSelf(nums2);
    // 期望: [0, 0, 9, 0, 0]
    for (int x : res2) cout << x << " ";
    cout << endl;
    return 0;
}
```

### 常见错误

- **错误1**：左右遍历顺序写错。左遍历时 ans[i] 应该等于 nums[0..i-1] 的积（不含 nums[i]），不要写成 `ans[i] *= leftProduct`（应为 `ans[i] = leftProduct`，因为 ans 初始全 1）。
- **错误2**：右遍历时更新 `rightProduct *= nums[i]` 写在 `ans[i] *= rightProduct` 之前，导致 ans[i] 乘上了 nums[i]（右边乘积包含了自身）。
- **错误3**：认为 O(1) 空间就是连返回数组都不分配——题目说的"额外空间"不包括输出数组。

### 面试陷阱

- **陷阱1**：面试官追问"如果有多个 0 会怎样？"两个以上的 0 会导致所有结果都是 0。代码不用特殊处理也能正确。
- **陷阱2**：追问"如果要求用 O(1) 空间且不能修改输入，能否用更小的额外变量？"当前解法已经是最优——额外只用了 leftProduct 和 rightProduct 两个变量。

---

## 44. 轮转数组 (Rotate Array) — 题号 189

**难度**: 中等
**标签**: 数组、双指针

### 题目大意

将数组 `nums` 向右轮转 `k` 步（k >= 0），原地修改。

### 思路

1. **额外数组**：新开一个数组，`new[(i+k) % n] = nums[i]`，然后复制回原数组。O(n) 时间 O(n) 空间。不符合"原地"要求。

2. **三次反转法 O(n) 时间 O(1) 空间（最优）**：
   这是原地轮转的经典技巧：
   ```
   原数组: [1, 2, 3, 4, 5, 6, 7], k = 3
   第1步: 整体反转 → [7, 6, 5, 4, 3, 2, 1]
   第2步: 反转前 k 个 → [5, 6, 7, 4, 3, 2, 1]
   第3步: 反转后 n-k 个 → [5, 6, 7, 1, 2, 3, 4]
   ```
   注意 k 可能大于 n，先取模 `k %= n`。

   直观理解：轮转后，后 k 个元素移到前面。反转整个数组让后 k 个到前面但顺序反了，再分别反转两段恢复顺序。

3. **环状替换**：从位置 0 开始，每个元素跳到 `(i+k) % n`，沿着环走。当回到起点且遍历过所有元素时停止。

### 核心知识点

- 三次反转法实现原地轮转
- 先取模防止 k > n
- 环状替换作为备选

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

void rotate(vector<int>& nums, int k) {
    int n = nums.size();
    k %= n; // 处理 k > n 的情况
    if (k == 0) return;
    
    // 三次反转
    reverse(nums.begin(), nums.end());         // 整体反转
    reverse(nums.begin(), nums.begin() + k);   // 反转前 k 个
    reverse(nums.begin() + k, nums.end());     // 反转后 n-k 个
}

// 环状替换法（备选）
void rotateCyclic(vector<int>& nums, int k) {
    int n = nums.size();
    k %= n;
    int count = 0; // 已处理的元素数
    for (int start = 0; count < n; start++) {
        int cur = start;
        int prevVal = nums[start];
        do {
            int next = (cur + k) % n;
            int temp = nums[next];
            nums[next] = prevVal;
            prevVal = temp;
            cur = next;
            count++;
        } while (cur != start); // 回到起点，环结束
    }
}

int main() {
    vector<int> nums1 = {1, 2, 3, 4, 5, 6, 7};
    rotate(nums1, 3);
    for (int x : nums1) cout << x << " ";
    cout << endl; // 期望: 5 6 7 1 2 3 4
    
    vector<int> nums2 = {-1, -100, 3, 99};
    rotate(nums2, 2);
    for (int x : nums2) cout << x << " ";
    cout << endl; // 期望: 3 99 -1 -100
    
    // 测试 k > n
    vector<int> nums3 = {1, 2, 3};
    rotate(nums3, 5); // 等价于 k=2
    for (int x : nums3) cout << x << " ";
    cout << endl; // 期望: 2 3 1
    return 0;
}
```

### 常见错误

- **错误1**：忘记 `k %= n`，当 k > n 时 `reverse(nums.begin(), nums.begin() + k)` 越界。
- **错误2**：三次反转顺序写错。正确顺序是：整体 → 前 k → 后 n-k。如果顺序错（如先反前 k 再反后 n-k 再总体），结果不对。
- **错误3**：环状替换法中忘记 `while(cur != start)` 的退出条件，导致死循环。或 count 没递增导致环之间不跳转。

### 面试陷阱

- **陷阱1**：面试官追问"向左轮转怎么做？"向右轮转 k 步等价于向左轮转 n-k 步。直接用三次反转（或者改变 k 的方向）。
- **陷阱2**：追问"如果数组是链表怎么轮转？"找到第 n-k-1 个节点，断开并重连（LeetCode 61 旋转链表），用快慢指针定位。

---

## 45. 缺失的第一个正数 (First Missing Positive) — 题号 41

**难度**: 困难
**标签**: 数组、哈希表、原地哈希

### 题目大意

给定未排序的整数数组，找出其中没有出现的最小正整数。要求 O(n) 时间、O(1) 空间。

### 思路

**解法一：原地哈希（最优 O(n) O(1)）**

核心洞察：对于一个长度为 n 的数组，答案范围是 [1, n+1]。因为如果 1 到 n 全部出现，缺失的就是 n+1。

因此，我们可以**用数组本身作为哈希表**：
1. 遍历数组，对于范围在 [1, n] 内的数 nums[i]，将其"归位"到索引 `nums[i] - 1` 处。通过交换实现：不断交换 nums[i] 和 nums[nums[i] - 1]，直到 nums[i] 不在 [1, n] 范围内或已经归位（nums[i] == nums[nums[i] - 1]）。
2. 再次遍历：第一个 `nums[i] != i + 1` 的位置 i，答案就是 i + 1。
3. 如果全部归位，答案是 n + 1。

**解法二：标记法（备选）**

1. 将所有 <= 0 的数替换为 n+1（一个不影响判断的哨兵值）。
2. 对于每个数 x = abs(nums[i])，如果 1 <= x <= n，将 nums[x-1] 标记为负数（表示 x 出现过）。
3. 第一个正数的下标 + 1 就是答案。

### 核心知识点

- 答案范围 [1, n+1] 的洞察是解题关键
- 原地哈希：用数组下标做哈希键，交换实现
- 均摊 O(n)：每个数最多被交换一次，while 循环总次数 O(n)

### 复杂度

- **时间复杂度**: O(n) — 每个元素最多被交换一次
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

#### 解法一：原地哈希（交换法）

```cpp
#include <bits/stdc++.h>
using namespace std;

int firstMissingPositive(vector<int>& nums) {
    int n = nums.size();
    
    // 将每个在 [1,n] 范围的数放到对应位置
    for (int i = 0; i < n; i++) {
        // while 而非 if：交换后新来的数可能也需要归位
        while (nums[i] >= 1 && nums[i] <= n && nums[i] != nums[nums[i] - 1]) {
            swap(nums[i], nums[nums[i] - 1]);
        }
    }
    
    // 找第一个不在正确位置上的数
    for (int i = 0; i < n; i++) {
        if (nums[i] != i + 1) return i + 1;
    }
    return n + 1; // 1~n 全齐
}

int main() {
    vector<int> nums1 = {1, 2, 0};
    cout << firstMissingPositive(nums1) << endl; // 期望: 3
    
    vector<int> nums2 = {3, 4, -1, 1};
    cout << firstMissingPositive(nums2) << endl; // 期望: 2
    
    vector<int> nums3 = {7, 8, 9, 11, 12};
    cout << firstMissingPositive(nums3) << endl; // 期望: 1
    
    vector<int> nums4 = {1, 1};
    cout << firstMissingPositive(nums4) << endl; // 期望: 2
    return 0;
}
```

#### 解法二：标记法

```cpp
#include <bits/stdc++.h>
using namespace std;

int firstMissingPositiveMark(vector<int>& nums) {
    int n = nums.size();
    
    // 第1步：将所有非正数替换为 n+1
    for (int i = 0; i < n; i++) {
        if (nums[i] <= 0) nums[i] = n + 1;
    }
    
    // 第2步：用负号标记出现过的数字
    for (int i = 0; i < n; i++) {
        int x = abs(nums[i]);
        if (x >= 1 && x <= n) {
            // 将 nums[x-1] 标记为负数（如果还不是负数）
            if (nums[x - 1] > 0) nums[x - 1] = -nums[x - 1];
        }
    }
    
    // 第3步：找第一个正数
    for (int i = 0; i < n; i++) {
        if (nums[i] > 0) return i + 1;
    }
    return n + 1;
}

int main() {
    vector<int> nums = {3, 4, -1, 1};
    cout << firstMissingPositiveMark(nums) << endl; // 期望: 2
    return 0;
}
```

### 常见错误

- **错误1**：交换时用 if 而非 while。如果 nums[i] 归位后新换来的数也在 [1, n] 范围且未归位，需要继续交换，只用 if 会漏掉。
- **错误2**：交换条件缺少 `nums[i] != nums[nums[i] - 1]`。如果目标位置已经有正确的数（如 [1, 1]），不加这个条件会导致死循环。
- **错误3**：标记法中，标记时直接 `nums[x-1] = -nums[x-1]` 而不判断是否已经是负数。如果重复标记，双重取负会变回正数，导致漏标记。

### 面试陷阱

- **陷阱1**：面试官追问"为什么 while 循环嵌套 for 循环还是 O(n)？"均摊分析：每个数被放到正确位置后就不会再动，总共最多执行 n 次交换。
- **陷阱2**：追问"如果允许 O(n) 空间，怎么做？"简单：把所有正数放进 unordered_set，然后从 1 开始逐个检查。但这失去了本题"原地哈希"考察的意义。

---

## 46. 最长连续序列 (Longest Consecutive Sequence) — 题号 128

**难度**: 中等
**标签**: 并查集、哈希表

### 题目大意

给定未排序的整数数组，找出最长连续序列的长度。要求 O(n) 时间。

### 思路

1. **排序 O(n log n)**：排序后线性扫描连续递增段。不满足 O(n) 要求。

2. **哈希集合 O(n)（最优）**：
   - 将所有数存入 unordered_set
   - 遍历每个数 x：如果 x-1 不在集合中（说明 x 是某个连续序列的起点），则从 x 开始向后扩展：x+1, x+2, ...，直到不在集合中为止，更新最大长度
   - 每个数只被作为"起点"访问一次，作为"后续"被检查一次，均摊 O(n)

   关键优化：只从序列起点（x-1 不存在）开始扩展，避免重复计算。如果每个数都无脑前后扩展，会退化到 O(n²)。

3. **并查集（备选）**：将相邻的数值 union 在一起，维护每个集合的大小。

### 核心知识点

- 哈希集合 + 只从起点扩展的优化
- 将"寻找连续序列"转化为"寻找连续链的起点"
- 并查集也可以处理区间连续性

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

int longestConsecutive(vector<int>& nums) {
    unordered_set<int> st(nums.begin(), nums.end());
    int ans = 0;
    
    for (int x : st) {
        // 只有当 x-1 不存在时，x 才是连续序列的起点
        if (st.find(x - 1) == st.end()) {
            int cur = x;
            int len = 1;
            // 从起点向后扩展
            while (st.find(cur + 1) != st.end()) {
                cur++;
                len++;
            }
            ans = max(ans, len);
        }
    }
    return ans;
}

// 并查集版本（备选）
class UnionFind {
public:
    unordered_map<int, int> parent, size;
    
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    
    void unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra != rb) {
            parent[ra] = rb;
            size[rb] += size[ra];
        }
    }
    
    void add(int x) {
        if (parent.count(x)) return;
        parent[x] = x;
        size[x] = 1;
        if (parent.count(x - 1)) unite(x, x - 1);
        if (parent.count(x + 1)) unite(x, x + 1);
    }
};

int longestConsecutiveUF(vector<int>& nums) {
    UnionFind uf;
    for (int x : nums) uf.add(x);
    int ans = 0;
    for (auto& p : uf.size) ans = max(ans, p.second);
    return ans;
}

int main() {
    vector<int> nums1 = {100, 4, 200, 1, 3, 2};
    cout << longestConsecutive(nums1) << endl; // 期望: 4 ([1,2,3,4])
    
    vector<int> nums2 = {0, 3, 7, 2, 5, 8, 4, 6, 0, 1};
    cout << longestConsecutive(nums2) << endl; // 期望: 9
    
    vector<int> nums3 = {};
    cout << longestConsecutive(nums3) << endl; // 期望: 0
    return 0;
}
```

### 常见错误

- **错误1**：遍历时用了 `for (int x : nums)` 而非 `for (int x : st)`。如果数组有大量重复元素，每个重复的 x 都会被当作起点检查一次，退化。
- **错误2**：扩展循环中 `while (st.count(cur + 1))` 不断 `cur++`，但没有更新 ans。ans 应该在 while 之后根据 len 更新。
- **错误3**：并查集版本中，union 时没有判断 x-1 和 x+1 是否存在就直接 union，会导致 `count` 访问空键。

### 面试陷阱

- **陷阱1**：面试官追问"如果要返回最长序列本身，怎么改？"从起点扩展时同时记录序列元素，用 vector 存储。额外 O(n) 空间。
- **陷阱2**：追问"如果数据量极大不能用哈希表（内存不够）怎么办？"可以排序后线性扫描，牺牲时间换空间。也考虑用位图或外部排序。

---

## 47. 字母异位词分组 (Group Anagrams) — 题号 49

**难度**: 中等
**标签**: 哈希表、字符串、排序

### 题目大意

给定字符串数组 `strs`，将字母异位词（即字母组成相同但顺序不同的词）分到同一组。

### 思路

核心问题：如何为每个 Anagrams 组构造一个唯一的 key？

1. **排序做 key（最直观）**：对每个字符串排序，排序后的字符串作为 key（如 "eat" → "aet"）。同一组的排序结果相同。

2. **计数编码做 key（更优）**：统计 26 个字母出现次数，编码成类似 `"0#1#0#2#..."` 的字符串作为 key。当字符串很长时，计数法比排序快（O(m) vs O(m log m)）。

两种方法本质相同：找到 Anagrams 的不变量作为哈希键。

### 核心知识点

- 排序后字符串作为 Anagrams 的签名
- 字母计数编码作为备选签名
- 哈希表分组

### 复杂度

- **时间复杂度**: O(n · m log m)，n 是字符串数量，m 是最大字符串长度（排序法）/ O(n · m)（计数法）
- **空间复杂度**: O(n · m)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法一：排序做 key
vector<vector<string>> groupAnagrams(vector<string>& strs) {
    unordered_map<string, vector<string>> groups;
    for (string& s : strs) {
        string key = s;
        sort(key.begin(), key.end());
        groups[key].push_back(s);
    }
    vector<vector<string>> res;
    for (auto& p : groups) {
        res.push_back(p.second);
    }
    return res;
}

// 解法二：计数编码做 key
vector<vector<string>> groupAnagramsCount(vector<string>& strs) {
    unordered_map<string, vector<string>> groups;
    for (string& s : strs) {
        vector<int> cnt(26, 0);
        for (char c : s) cnt[c - 'a']++;
        // 编码成类似 "1#0#2#..." 的字符串
        string key;
        for (int i = 0; i < 26; i++) {
            key += to_string(cnt[i]) + "#";
        }
        groups[key].push_back(s);
    }
    vector<vector<string>> res;
    for (auto& p : groups) res.push_back(p.second);
    return res;
}

int main() {
    vector<string> strs1 = {"eat", "tea", "tan", "ate", "nat", "bat"};
    auto res1 = groupAnagrams(strs1);
    // 期望: [["bat"], ["nat","tan"], ["ate","eat","tea"]] (顺序可不同)
    for (auto& g : res1) {
        cout << "[";
        for (int i = 0; i < g.size(); i++) cout << g[i] << (i+1<g.size()?",":"");
        cout << "] ";
    }
    cout << endl;
    
    vector<string> strs2 = {""};
    auto res2 = groupAnagrams(strs2);
    cout << "Empty string groups: " << res2.size() << endl; // 期望: 1
    
    vector<string> strs3 = {"a"};
    auto res3 = groupAnagrams(strs3);
    cout << "Single char groups: " << res3.size() << endl; // 期望: 1
    return 0;
}
```

### 常见错误

- **错误1**：对空字符串 `""` 排序或编码时越界。空字符串 sort 结果是空串，计数数组全 0，应该能正确处理。
- **错误2**：计数编码时忘记分隔符 `#`，如 `"aabb"` 编码成 `"22..."` 和 `"ab"` 不同但可能碰撞（虽然概率极低）。
- **错误3**：计数法中字符范围假设只有小写字母，如果输入包含大写或其他字符，应扩展数组大小。

### 面试陷阱

- **陷阱1**：面试官追问"如果字符串非常长（如 10⁶），排序法和计数法哪个好？"计数法 O(m) 完胜排序法的 O(m log m)。极端情况下应该用计数编码。
- **陷阱2**：追问"能否不用额外字符串排序，直接在原数组上操作？"可以——但没必要。排序 key 法已经足够清晰高效。

---

## 48. 有效的字母异位词 (Valid Anagram) — 题号 242

**难度**: 简单
**标签**: 哈希表、字符串、排序

### 题目大意

判断字符串 `s` 和 `t` 是否互为字母异位词（字母组成和频率完全相同，顺序可不同）。

### 思路

1. **排序比较**：将两个字符串排序，比较排序后是否相等。O(n log n)。

2. **频率计数 O(n)**：用一个 26 大小的数组统计 s 中每个字母的计数，遍历 t 时递减，最后检查数组是否全 0。如果中间出现负数可提前返回 false。

### 核心知识点

- 计数数组做 Anagrams 判定
- 提前返回优化

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(1) — 固定 26 大小

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

bool isAnagram(string s, string t) {
    if (s.length() != t.length()) return false;
    
    vector<int> cnt(26, 0);
    for (char c : s) cnt[c - 'a']++;
    for (char c : t) {
        cnt[c - 'a']--;
        // 如果某字母 t 比 s 多，可提前退出
        if (cnt[c - 'a'] < 0) return false;
    }
    return true; // 长度相同且无负数，必然全0
}

int main() {
    cout << isAnagram("anagram", "nagaram") << endl; // 期望: 1 (true)
    cout << isAnagram("rat", "car") << endl;         // 期望: 0 (false)
    cout << isAnagram("a", "ab") << endl;            // 期望: 0 (false)
    cout << isAnagram("aacc", "ccac") << endl;       // 期望: 0 (false)
    return 0;
}
```

### 常见错误

- **错误1**：忘记先判断长度是否相同。如果长度不同，一定不是 Anagrams，直接返回 false 节省计算。
- **错误2**：将 `cnt` 用于统一计数（s 计数 +1，t 计数 +1 然后比较），导致两倍空间或遍历 26 检查的逻辑变复杂。
- **错误3**：只判断了 s 是否包含 t 的所有字符，忽略了频率也要相同。如 s="aab", t="abb"，会误判为 true。

### 面试陷阱

- **陷阱1**：面试官追问"如果字符集是 Unicode（非仅 26 字母）怎么办？"用 `unordered_map<char, int>` 替代数组，时间 O(n)，空间 O(字符种类数)。
- **陷阱2**：追问"能否 O(1) 额外空间完成？"排序法在原串上排序（如果可以修改输入）是 O(1) 额外空间，但时间 O(n log n)。计数法已经是用 O(1) 固定大小数组。

---

## 49. 赎金信 (Ransom Note) — 题号 383

**难度**: 简单
**标签**: 哈希表、字符串、计数

### 题目大意

给定 `ransomNote` 和 `magazine` 两个字符串，判断 `ransomNote` 能否由 `magazine` 中的字母构成（每个字母各用一次）。

### 思路

这是 Anagrams 问题的简化版：不是要求完全相等，而是 magazine 的字母频率 >= ransomNote 的字母频率。

1. **频率计数最直接**：统计 magazine 中每个字母的出现次数，遍历 ransomNote 逐个扣除，如果不够（计数 < 0）返回 false。

### 核心知识点

- 频率计数判断子集关系
- 与 Anagrams 的区别：本题是"子集包含"而非"完全相等"

### 复杂度

- **时间复杂度**: O(n + m)
- **空间复杂度**: O(1) — 固定 26 字母

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

bool canConstruct(string ransomNote, string magazine) {
    if (ransomNote.length() > magazine.length()) return false;
    
    vector<int> cnt(26, 0);
    for (char c : magazine) cnt[c - 'a']++;
    for (char c : ransomNote) {
        cnt[c - 'a']--;
        if (cnt[c - 'a'] < 0) return false;
    }
    return true;
}

int main() {
    cout << canConstruct("a", "b") << endl;       // 期望: 0 (false)
    cout << canConstruct("aa", "ab") << endl;     // 期望: 0 (false)
    cout << canConstruct("aa", "aab") << endl;    // 期望: 1 (true)
    cout << canConstruct("", "a") << endl;        // 期望: 1 (true)
    cout << canConstruct("fihjjjjei", "hjibagacbhadfaefdjaeaebgi") << endl; // LeetCode 样例
    return 0;
}
```

### 常见错误

- **错误1**：把 magazine 和 ransomNote 的角色搞反——从 ransomNote 统计，从 magazine 扣除。magazine 是供给方，应该先统计供给再看需求够不够。
- **错误2**：没有在扣除时立即判断 < 0 提前退出，而是遍历完再检查 26 个值。虽然复杂度相同但前者可以更早返回。
- **错误3**：忽略长度判断——如果 ransomNote 比 magazine 长，直接返回 false。

### 面试陷阱

- **陷阱1**：面试官追问"如果 magazine 中的字母可以被重复利用呢？"那只需要判断 magazine 中是否包含 ransomNote 的所有不同字符（每个字符至少出现一次），用集合而非计数。
- **陷阱2**：追问"如果需要构造多个 ransomNotes，怎么高效批量处理？"预处理 magazine 的计数，批量与各个 ransomNote 比较。

---

## 50. 快乐数 (Happy Number) — 题号 202

**难度**: 简单
**标签**: 哈希表、双指针、数学

### 题目大意

判断一个正整数是否是快乐数：不断将数字替换为各位数字的平方和，如果最终变成 1 则是快乐数；如果陷入循环（永远变不成 1）则不是。

### 思路

**解法一：哈希表检测循环**

不断迭代计算平方和，用 unordered_set 记录见过的数。如果遇到 1 返回 true，如果遇到见过的数（循环）返回 false。

**解法二：快慢指针（Floyd 判圈法）O(1) 空间**

将迭代过程视为一个隐式链表，用快慢指针检测循环：
- slow 每次迭代一步，fast 每次迭代两步
- 如果 fast 变成 1，是快乐数
- 如果 slow == fast（在非 1 处相遇），说明有循环

**为什么一定会终止（不会无限增长）？**
- 对于 n 位数的数字，平方和最多为 n×81（每位最大 9²=81）
- 对于 ≥ 4 位数，平方和会小于原数（9999 → 324）；对于 3 位数，平方和 ≤ 243
- 所以最终一定进入 243 以内的小循环，要么到 1，要么陷入已知非快乐循环

### 核心知识点

- 循环检测：哈希表 vs Floyd 判圈
- 数学性质：平方和不会无限增长
- Floyd 快慢指针在不显式链表中的应用

### 复杂度

- **时间复杂度**: O(log n) — 每次迭代位数减少或稳定在有限范围
- **空间复杂度**: O(1) — Floyd 判圈法 / O(log n) — 哈希表法

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 计算各位数字的平方和
int getNext(int n) {
    int sum = 0;
    while (n > 0) {
        int d = n % 10;
        sum += d * d;
        n /= 10;
    }
    return sum;
}

// 解法一：哈希表检测循环
bool isHappy(int n) {
    unordered_set<int> seen;
    while (n != 1 && seen.find(n) == seen.end()) {
        seen.insert(n);
        n = getNext(n);
    }
    return n == 1;
}

// 解法二：快慢指针（Floyd 判圈）—— O(1) 空间
bool isHappyFloyd(int n) {
    int slow = n;
    int fast = getNext(n);
    // fast 到达 1 说明是快乐数；slow==fast 说明有循环
    while (fast != 1 && slow != fast) {
        slow = getNext(slow);
        fast = getNext(getNext(fast));
    }
    return fast == 1;
}

int main() {
    cout << isHappy(19) << endl;  // 期望: 1 (true)
    // 19 -> 82 -> 68 -> 100 -> 1
    
    cout << isHappy(2) << endl;   // 期望: 0 (false)
    // 2 -> 4 -> 16 -> 37 -> 58 -> 89 -> 145 -> 42 -> 20 -> 4 (循环)
    
    cout << isHappyFloyd(19) << endl; // 期望: 1
    cout << isHappyFloyd(2) << endl;  // 期望: 0
    return 0;
}
```

### 常见错误

- **错误1**：Floyd 判圈法中 fast 初始化为 n 而非 getNext(n)，导致一开始 slow == fast，循环直接退出误判为有环。
- **错误2**：HashMap 法中循环条件写成 `while (n != 1)` 而没有检测重复，导致对非快乐数（如 2）死循环。
- **错误3**：快指针跳两步时有 null/越界风险——但本题没有 null，因为函数总是有定义的输出。

### 面试陷阱

- **陷阱1**：面试官追问"Floyd 判圈法为什么一定能检测到循环？"因为如果链表有环，快指针每次比慢指针多走一步，在环内一定能追上。
- **陷阱2**：追问快乐数的已知非快乐循环有哪些？答案是 `4 → 16 → 37 → 58 → 89 → 145 → 42 → 20 → 4`，可以利用这个"硬编码"提前终止优化。

---

## 51. LRU 缓存 (LRU Cache) — 题号 146

**难度**: 中等
**标签**: 哈希表、双向链表、设计

### 题目大意

设计一个 LRU（最近最少使用）缓存，支持 `get(key)` 和 `put(key, value)`。缓存容量有限，满时淘汰最久未使用的键。要求 O(1) 时间复杂度。

### 思路

**数据结构设计**：`哈希表 + 双向链表`

- **哈希表** `unordered_map<int, Node*>`：存 `key -> 链表节点指针`，实现 O(1) 查找
- **双向链表**：按使用时间排序。最近使用的在头部（或尾部），最久未使用的在另一端
  - 每次 get 或 put 更新时，将对应节点移到头部
  - put 新键时：如果容量满，删除尾部节点（最久未使用）

**实现细节**：
- 使用**虚拟头尾节点**（dummy head/tail）简化边界处理：插入/删除时不需要判空
- put 已存在的 key 时，更新 value 并移到头部
- 删除节点时要同时从哈希表中删除

**为什么必须双向链表？**
单向链表删除指定节点需要找到前驱，O(n)。双向链表可以在 O(1) 内删除任意节点。

### 核心知识点

- 哈希表 + 双向链表实现 O(1) 的 LRU
- 虚拟头尾节点简化边界
- 设计模式：封装 moveToHead、removeNode、addToHead 等原子操作

### 复杂度

- **时间复杂度**: O(1) — get 和 put 都是 O(1)
- **空间复杂度**: O(capacity)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class LRUCache {
private:
    struct Node {
        int key, val;
        Node* prev;
        Node* next;
        Node(int k, int v) : key(k), val(v), prev(nullptr), next(nullptr) {}
    };
    
    int cap;
    unordered_map<int, Node*> cache;
    Node* head; // 虚拟头节点
    Node* tail; // 虚拟尾节点
    
    // 将 node 从链表中移除
    void removeNode(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    
    // 将 node 添加到头部（最近使用）
    void addToHead(Node* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }
    
    // 将 node 移到头部 = 删除 + 添加
    void moveToHead(Node* node) {
        removeNode(node);
        addToHead(node);
    }
    
    // 删除尾部节点（最久未使用），返回 key 用于从哈希表删除
    Node* removeTail() {
        Node* node = tail->prev;
        removeNode(node);
        return node;
    }
    
public:
    LRUCache(int capacity) : cap(capacity) {
        head = new Node(0, 0);
        tail = new Node(0, 0);
        head->next = tail;
        tail->prev = head;
    }
    
    int get(int key) {
        if (cache.find(key) == cache.end()) {
            return -1;
        }
        Node* node = cache[key];
        moveToHead(node); // 标记为最近使用
        return node->val;
    }
    
    void put(int key, int value) {
        if (cache.find(key) != cache.end()) {
            // key 已存在：更新值并移到头部
            Node* node = cache[key];
            node->val = value;
            moveToHead(node);
        } else {
            // key 不存在：创建新节点
            Node* newNode = new Node(key, value);
            cache[key] = newNode;
            addToHead(newNode);
            
            // 容量超限：删除最久未使用的
            if (cache.size() > cap) {
                Node* removed = removeTail();
                cache.erase(removed->key);
                delete removed;
            }
        }
    }
};

int main() {
    LRUCache lru(2);
    lru.put(1, 1);           // 缓存: {1=1}
    lru.put(2, 2);           // 缓存: {1=1, 2=2}
    cout << lru.get(1) << endl; // 返回 1, 缓存: {2=2, 1=1}
    lru.put(3, 3);           // 淘汰 key=2, 缓存: {1=1, 3=3}
    cout << lru.get(2) << endl; // 返回 -1 (不存在)
    lru.put(4, 4);           // 淘汰 key=1, 缓存: {3=3, 4=4}
    cout << lru.get(1) << endl; // 返回 -1
    cout << lru.get(3) << endl; // 返回 3
    cout << lru.get(4) << endl; // 返回 4
    return 0;
}
```

### 常见错误

- **错误1**：put 更新已存在的 key 时，只更新了 Node::val，但**忘了调用 moveToHead**。这导致该 key 的使用时间没有被更新，可能被错误淘汰。
- **错误2**：删除节点时只从链表移除，忘了 `cache.erase(removed->key)`，导致哈希表增长，且 get 时可能返回已被删除的节点（悬空指针）。
- **错误3**：put 新节点时先判断容量再插入，顺序应该是：先插入、再判断容量、如果超了再删除。反过来写（先判断容量再插入）逻辑更复杂且容易出错。
- **错误4**：没有使用虚拟头尾节点，导致在头或尾操作时要额外判断 null，代码冗长且容易出 bug。

### 面试陷阱

- **陷阱1**：面试官追问"为什么用双向链表，单向链表 + 哈希表存前驱行不行？"不行。删除中间节点时需要更新前驱的 next 指针，单向链表中即使哈希表存了前驱，前驱的 next 好更新，但后继的 prev 无法在 O(1) 内获取（如果还需要 prev 的话）。如果只是单向链表且不需要反向遍历，可以用 `unordered_map<key, iter>` 指向 list 的迭代器。
- **陷阱2**：追问"C++ STL 中是否有现成的？"可以用 `std::list<pair<int,int>>` + `unordered_map<int, list::iterator>`。`list::splice` 可以在 O(1) 内移动节点。

---

## 52. LFU 缓存 (LFU Cache) — 题号 460

**难度**: 困难
**标签**: 哈希表、双向链表、设计

### 题目大意

设计 LFU（最不经常使用）缓存：淘汰使用频率最低的键；若频率相同，淘汰最久未使用的（LRU 策略作为 tie-breaker）。要求 O(1) 时间复杂度。

### 思路

**解法一：双哈希表 + 频率链表（O(1)）**

核心数据结构：
- `key2node`：`unordered_map<key, Node*>` — O(1) 查找节点
- `freq2list`：`unordered_map<freq, DoubleLinkedList>` — 每个频率对应一个双向链表，存该频率的所有节点（LRU 顺序）
- `minFreq`：记录当前最小频率，淘汰时 O(1) 定位

每个 Node 存：key, val, freq, prev, next。

**get(key)**:
1. key 不存在返回 -1
2. 通过 key2node 找到节点，增加其频率：从当前频率链表中删除，加入 freq+1 的链表
3. 如果当前频率链表变空且 freq == minFreq，则 minFreq++

**put(key, value)**:
1. 如果 key 已存在，更新值并增加频率（同 get）
2. 如果 key 不存在：
   - 如果容量满：从 minFreq 对应的链表中淘汰最后一个节点（LRU 顺序，即链表尾）
   - 创建新节点，频率=1，加入 freq2list[1]。minFreq 重置为 1

**为什么用频率作为粒度而非全局一个 LRU 链表？** LFU 需要按频率分层，每个频率层内按 LRU 排序。

**解法二：优先队列 + 惰性删除（O(log n)）**

用 `priority_queue` 按 (freq, timestamp, key) 排序。淘汰时弹出堆顶，如果堆顶的 freq 已经过期（实际频率更高），则丢弃继续弹。惰性删除思路简单但 get 需要更新频率——优先队列不支持快速更新，所以 get 时只能插入新记录，旧记录标记为过期。导致堆中有冗余数据。

### 核心知识点

- 频率分层 + 每层 LRU（双哈希表架构）
- minFreq 变量避免遍历所有频率找最小
- Node 同时属于哈希表 key2node 和频率链表

### 复杂度

**双哈希表法：**
- 时间复杂度: O(1) — get 和 put 都是 O(1)
- 空间复杂度: O(capacity)

**优先队列法：**
- 时间复杂度: O(log n) — get 和 put 的堆操作
- 空间复杂度: O(capacity) + 惰性删除的冗余

### C++ 代码 (ACM风格)

#### 解法一：双哈希表 + 频率链表（O(1)）

```cpp
#include <bits/stdc++.h>
using namespace std;

class LFUCache {
private:
    struct Node {
        int key, val, freq;
        Node* prev;
        Node* next;
        Node(int k, int v) : key(k), val(v), freq(1), prev(nullptr), next(nullptr) {}
    };
    
    struct DoublyList {
        Node* head; // 虚拟头
        Node* tail; // 虚拟尾
        int size;
        
        DoublyList() {
            head = new Node(0, 0);
            tail = new Node(0, 0);
            head->next = tail;
            tail->prev = head;
            size = 0;
        }
        
        void addToHead(Node* node) {
            node->next = head->next;
            node->prev = head;
            head->next->prev = node;
            head->next = node;
            size++;
        }
        
        void removeNode(Node* node) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            size--;
        }
        
        Node* removeLast() {
            Node* node = tail->prev;
            removeNode(node);
            return node;
        }
    };
    
    int cap;
    int minFreq; // 当前最小频率
    unordered_map<int, Node*> key2node;  // key -> Node*
    unordered_map<int, DoublyList*> freq2list; // freq -> 该频率的LRU链表
    
    // 更新节点的频率（从当前链表移到更高频率链表）
    void updateFreq(Node* node) {
        int oldFreq = node->freq;
        DoublyList* oldList = freq2list[oldFreq];
        oldList->removeNode(node);
        
        // 如果旧频率链表变空且是最小频率，minFreq++
        if (oldFreq == minFreq && oldList->size == 0) {
            minFreq++;
        }
        
        // 加入新频率链表
        node->freq++;
        int newFreq = node->freq;
        if (freq2list.find(newFreq) == freq2list.end()) {
            freq2list[newFreq] = new DoublyList();
        }
        freq2list[newFreq]->addToHead(node);
    }
    
public:
    LFUCache(int capacity) : cap(capacity), minFreq(0) {}
    
    int get(int key) {
        if (key2node.find(key) == key2node.end()) return -1;
        Node* node = key2node[key];
        updateFreq(node);
        return node->val;
    }
    
    void put(int key, int value) {
        if (cap == 0) return;
        
        if (key2node.find(key) != key2node.end()) {
            // key 已存在，更新值并增加频率
            Node* node = key2node[key];
            node->val = value;
            updateFreq(node);
        } else {
            // key 不存在，需要新建
            if (key2node.size() >= cap) {
                // 淘汰：从 minFreq 链表中删除最后一个（最久未使用）
                DoublyList* minList = freq2list[minFreq];
                Node* removed = minList->removeLast();
                key2node.erase(removed->key);
                delete removed;
            }
            
            // 创建新节点，频率=1
            Node* newNode = new Node(key, value);
            key2node[key] = newNode;
            if (freq2list.find(1) == freq2list.end()) {
                freq2list[1] = new DoublyList();
            }
            freq2list[1]->addToHead(newNode);
            minFreq = 1; // 新节点加入，最小频率必定是1
        }
    }
};

int main() {
    LFUCache lfu(2);
    lfu.put(1, 1);  // {1=1}
    lfu.put(2, 2);  // {1=1, 2=2}
    cout << lfu.get(1) << endl; // 返回 1, 1的freq变成2
    lfu.put(3, 3);  // 淘汰 key=2 (freq最小且最久), {1=1, 3=3}
    cout << lfu.get(2) << endl; // 返回 -1
    cout << lfu.get(3) << endl; // 返回 3, 3的freq=2
    lfu.put(4, 4);  // 淘汰 key=1 (freq=2但比3更久), {3=3, 4=4}
    cout << lfu.get(1) << endl; // 返回 -1
    cout << lfu.get(3) << endl; // 返回 3
    cout << lfu.get(4) << endl; // 返回 4
    return 0;
}
```

#### 解法二：优先队列 + 惰性删除（备选，非O(1)）

```cpp
#include <bits/stdc++.h>
using namespace std;

class LFUCachePQ {
private:
    struct Item {
        int key, val, freq, time;
        // 优先队列默认大顶堆，我们需要小顶堆 by freq then time
        bool operator<(const Item& other) const {
            if (freq != other.freq) return freq > other.freq; // 频率小的优先
            return time > other.time; // 时间早的优先
        }
    };
    
    int cap, timer;
    unordered_map<int, Item> cache; // key -> {key, val, freq, time}
    priority_queue<Item> pq;        // 按(freq, time)排序
    
public:
    LFUCachePQ(int capacity) : cap(capacity), timer(0) {}
    
    int get(int key) {
        if (cache.find(key) == cache.end()) return -1;
        Item& item = cache[key];
        item.freq++;
        item.time = ++timer;
        pq.push(item); // 插入新记录，旧记录惰性删除
        return item.val;
    }
    
    void put(int key, int value) {
        if (cap == 0) return;
        
        if (cache.find(key) != cache.end()) {
            Item& item = cache[key];
            item.val = value;
            item.freq++;
            item.time = ++timer;
            pq.push(item);
        } else {
            if (cache.size() >= cap) {
                // 惰性删除：弹出堆中已过期的记录
                while (!pq.empty()) {
                    Item top = pq.top(); pq.pop();
                    if (cache.find(top.key) != cache.end() && 
                        cache[top.key].freq == top.freq &&
                        cache[top.key].time == top.time) {
                        // 是最新的记录，确实需要淘汰
                        cache.erase(top.key);
                        break;
                    }
                }
            }
            Item newItem = {key, value, 1, ++timer};
            cache[key] = newItem;
            pq.push(newItem);
        }
    }
};

int main() {
    LFUCachePQ lfu(2);
    lfu.put(1, 1); lfu.put(2, 2);
    cout << lfu.get(1) << endl;
    lfu.put(3, 3);
    cout << lfu.get(2) << endl;
    cout << lfu.get(3) << endl;
    lfu.put(4, 4);
    cout << lfu.get(1) << endl;
    cout << lfu.get(3) << endl;
    cout << lfu.get(4) << endl;
    return 0;
}
```

### 常见错误

- **错误1**：更新频率后忘记更新 minFreq。如果旧频率链表变空且 freq 恰好是 minFreq，需要 minFreq++。否则淘汰时会找错频率层。
- **错误2**：put 新节点时容量判断写反——先判断 `if (cache.size() >= cap)` 再插入，还是先插入再判断？正确：插入后判断 `cache.size() > cap` 再淘汰；或插入前判断然后淘汰后再插入。两者等价但插入前判断需要注意 `cap == 0` 的边界。
- **错误3**：更新已有 key 时忘了将其从旧频率链表移到新频率链表，导致一个节点同时存在于两个链表中（或频率不正确）。
- **错误4**：优先队列版本中惰性删除条件不严谨——仅比较 key 存在是不够的，还需要比较频率和时间戳是否与缓存中的一致，因为可能有多个同 key 的不同频率版本在堆中。

### 面试陷阱

- **陷阱1**：面试官追问"LFU 和 LRU 的根本区别是什么？什么时候用 LFU？"LRU 按"最近使用时间"淘汰，适合短期热点；LFU 按"历史使用频率"淘汰，适合长期热点。LFU 会出现"缓存污染"——过去频繁使用但现在已经不用的数据难以淘汰；LRU 则会被大量冷数据冲刷掉热数据。
- **陷阱2**：追问"如何改进 LFU 避免历史热数据长期占坑？"可以引入时间衰减（如定期将所有频率乘以 0.9），或者使用 Window-LFU（只统计最近一段时间窗口内的频率）。

# LeetCode Hot200 刷题文档 — 阶段二b：基础（后半，23题）

---

## 53. 两两交换链表中的节点 — 题号 24

**难度**: 中等
**标签**: 链表、递归

### 题目大意

给定一个链表，两两交换其中相邻的节点，并返回交换后的链表。不能只修改节点内部的值，必须实际交换节点。

示例：`1 -> 2 -> 3 -> 4` 变为 `2 -> 1 -> 4 -> 3`

### 思路

**暴力思路**：遍历链表，每次取两个节点，交换它们的值。但题目要求不能只改值，必须交换节点本身。

**优化思路（迭代）**：引入虚拟头节点 `dummy`，统一处理头节点交换时的边界情况。核心是维护 `prev` 指针指向当前要交换的两个节点的前驱。交换顺序极其关键：

1. 先让 `prev->next` 指向第二个节点（否则第一个节点就丢了）
2. 再让第一个节点指向第二个节点的 `next`
3. 再让第二个节点指向第一个节点
4. 最后 `prev` 前进两步

指针操作顺序错一步就会丢失链表。

**递归思路**：每次递归处理两个节点。先保存当前头节点的下下个节点 `nextHead`，然后把第二个节点翻转指向第一个节点，第一个节点指向递归处理 `nextHead` 的结果。递归终止条件：为空或只剩一个节点。

### 核心知识点

- 虚拟头节点统一边界处理
- 链表指针操作顺序至关重要
- 递归"甩锅"思想：处理好当前层，剩下的递归处理

### 复杂度

- **时间复杂度**: O(n) — 每个节点访问一次
- **空间复杂度**: O(1)（迭代）/ O(n)（递归栈）

### C++ 代码 (ACM风格) — 迭代写法

```cpp
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// 迭代写法：虚拟头节点 + 三指针
ListNode* swapPairs(ListNode* head) {
    ListNode dummy(0);
    dummy.next = head;
    ListNode* prev = &dummy;  // prev指向待交换两个节点的前驱

    while (prev->next && prev->next->next) {
        ListNode* first = prev->next;       // 第一个待交换节点
        ListNode* second = first->next;     // 第二个待交换节点

        // 关键：指针交换顺序不能乱
        prev->next = second;         // 步骤1：前驱先接上second
        first->next = second->next;  // 步骤2：first接上second后面的
        second->next = first;        // 步骤3：second翻转指向first

        prev = first;                // 步骤4：prev前进两步（first已变成后一个）
    }

    return dummy.next;
}

// 递归写法
ListNode* swapPairsRecursive(ListNode* head) {
    // 终止条件：空节点或只剩一个节点
    if (!head || !head->next) return head;

    ListNode* second = head->next;
    ListNode* nextHead = second->next;  // 保存下一组的头

    second->next = head;                     // second指向first
    head->next = swapPairsRecursive(nextHead); // first接上递归结果

    return second;  // second是新的头
}

// 构建链表辅助函数
ListNode* buildList(vector<int>& arr) {
    ListNode dummy(0);
    ListNode* cur = &dummy;
    for (int v : arr) {
        cur->next = new ListNode(v);
        cur = cur->next;
    }
    return dummy.next;
}

void printList(ListNode* head) {
    while (head) {
        cout << head->val;
        if (head->next) cout << " -> ";
        head = head->next;
    }
    cout << endl;
}

int main() {
    vector<int> arr = {1, 2, 3, 4};
    ListNode* head = buildList(arr);
    cout << "原链表: ";
    printList(head);

    head = swapPairs(head);
    cout << "交换后: ";
    printList(head);

    // 递归测试
    vector<int> arr2 = {1, 2, 3, 4, 5};
    head = buildList(arr2);
    cout << "原链表: ";
    printList(head);
    head = swapPairsRecursive(head);
    cout << "递归交换后: ";
    printList(head);

    return 0;
}
```

### 常见错误

- **错误1：指针交换顺序颠倒**。先把 `first->next = second->next` 再 `prev->next = second` 会丢失 `second` 节点。正确顺序必须先让前驱接上 `second`。
- **错误2：忘记处理奇数长度**。循环条件是 `prev->next && prev->next->next`，少写一个会越界。还剩一个节点时不做交换但不报错（递归写法靠终止条件自然处理）。
- **错误3：递归返回错误的头**。递归应该返回 `second`（新的头节点），而不是 `head`。

### 面试陷阱

- **陷阱1**：面试官问"虚拟头节点到底解决了什么问题？"——解决头节点也需要交换的情况。如果没有 `dummy`，当头节点和第二个节点交换时，需要特判更新返回值。虚拟头让所有情况统一为"交换 `prev` 后面的两个节点"。
- **陷阱2**：面试官追问"递归的栈溢出风险和迭代怎么选择？"——链表较长（>10^4）时递归可能栈溢出，工程中选迭代；面试中递归更简洁但要说清楚两种的区别。

---

## 54. 删除链表的倒数第 N 个结点 — 题号 19

**难度**: 中等
**标签**: 链表、双指针

### 题目大意

给定一个链表，删除倒数第 `n` 个节点，并返回链表的头节点。

示例：`1 -> 2 -> 3 -> 4 -> 5, n = 2`，删除倒数第2个（即4），返回 `1 -> 2 -> 3 -> 5`

### 思路

**暴力思路**：先遍历一遍得长度 L，再遍历到第 L-n 个节点删除。需要两次遍历。

**优化思路（快慢指针/双指针）**：一次遍历解决。快指针先走 n+1 步，然后快慢一起走。当快指针到末尾（null），慢指针刚好在待删除节点的前驱。为什么是 n+1？因为我们需要慢指针指向待删节点的前驱，才能执行删除操作。

关于虚拟头节点：必须用！因为可能删除的就是头节点（n=链表长度），没有 `dummy` 就要特判。虚拟头节点后，删除头节点变成删除 `dummy->next`，统一操作。

### 核心知识点

- 快慢指针（一次遍历定位倒数第 N 个）
- 虚拟头节点处理头节点被删的边界情况
- 快指针先走 n+1 步的设计原因

### 复杂度

- **时间复杂度**: O(n) — 一次遍历
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode dummy(0);
    dummy.next = head;
    ListNode* fast = &dummy;
    ListNode* slow = &dummy;

    // 快指针先走 n+1 步，使得快慢相差 n+1
    // 这样当fast到null时，slow正好在待删节点的前一个位置
    for (int i = 0; i <= n; i++) {
        fast = fast->next;
    }

    // 快慢一起移动
    while (fast) {
        fast = fast->next;
        slow = slow->next;
    }

    // 删除 slow 后面的节点
    ListNode* toDelete = slow->next;
    slow->next = slow->next->next;
    delete toDelete;

    return dummy.next;
}

ListNode* buildList(vector<int>& arr) {
    ListNode dummy(0);
    ListNode* cur = &dummy;
    for (int v : arr) {
        cur->next = new ListNode(v);
        cur = cur->next;
    }
    return dummy.next;
}

void printList(ListNode* head) {
    while (head) {
        cout << head->val;
        if (head->next) cout << " -> ";
        head = head->next;
    }
    cout << endl;
}

int main() {
    vector<int> arr = {1, 2, 3, 4, 5};
    ListNode* head = buildList(arr);
    cout << "原链表: ";
    printList(head);

    head = removeNthFromEnd(head, 2);
    cout << "删除倒数第2个后: ";
    printList(head);

    // 测试删除头节点
    head = removeNthFromEnd(head, 4); // 只剩3个节点，删倒数第4个即头节点
    cout << "再删除倒数第4个（删头）: ";
    printList(head);

    return 0;
}
```

### 常见错误

- **错误1：快指针只走 n 步而不是 n+1 步**。这样 slow 最终指向待删节点本身，无法执行删除（需要前驱）。走 n+1 步才能让 slow 停在前驱位置。
- **错误2：忘记虚拟头节点**。当 n 等于链表长度时，要删除头节点，但没有前驱，直接操作会出错，必须加特判。用 `dummy` 统一处理。
- **错误3：不 delete 被删节点**。内存泄漏，虽然 OJ 不扣分但面试会扣分。

### 面试陷阱

- **陷阱1**：面试官问"如果 n 可能大于链表长度怎么办？"——需要先验证，在快指针前进 n+1 步时一旦中间遇到 null 就说明 n 不合法，直接返回原链表。
- **陷阱2**：面试官问"能不能用栈做？"——可以。先全部入栈，弹出 n 个，栈顶就是待删节点的前驱。但需要 O(n) 额外空间，不如双指针 O(1)。

---

## 55. 两数相加 — 题号 2

**难度**: 中等
**标签**: 链表、数学

### 题目大意

两个非空链表表示两个非负整数，每位数字以逆序存储（个位在头）。将两数相加，返回同样格式的链表。

示例：`2 -> 4 -> 3`（342）+ `5 -> 6 -> 4`（465）= `7 -> 0 -> 8`（807）

### 思路

**暴力思路**：把链表转成整数，相加后再转回链表。但题目可能给超长数字（超过 64 位），会溢出。

**标准思路（模拟竖式加法）**：同时遍历两个链表，逐位相加，维护进位 `carry`。核心公式：`sum = a + b + carry`，当前位 = `sum % 10`，新进位 = `sum / 10`。

注意：两个链表可能不等长，短的当作 0 处理。全部遍历完后，如果还有进位，需要额外创建一个节点。

虚拟头节点简化结果链表的构建。

### 核心知识点

- 大数加法（逐位模拟）
- 进位处理（尤其是最后的越界进位）
- 不等长处理（短链表补零）
- 虚拟头节点构建结果链表

### 复杂度

- **时间复杂度**: O(max(n, m))
- **空间复杂度**: O(max(n, m)) — 结果链表

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* cur = &dummy;
    int carry = 0;

    // 只要还有节点没处理，或还有进位，就继续
    while (l1 || l2 || carry) {
        int a = l1 ? l1->val : 0;  // 短链表补0
        int b = l2 ? l2->val : 0;
        int sum = a + b + carry;

        carry = sum / 10;
        cur->next = new ListNode(sum % 10);
        cur = cur->next;

        if (l1) l1 = l1->next;
        if (l2) l2 = l2->next;
    }

    return dummy.next;
}

ListNode* buildList(vector<int>& arr) {
    ListNode dummy(0);
    ListNode* cur = &dummy;
    for (int v : arr) {
        cur->next = new ListNode(v);
        cur = cur->next;
    }
    return dummy.next;
}

void printList(ListNode* head) {
    while (head) {
        cout << head->val;
        if (head->next) cout << " -> ";
        head = head->next;
    }
    cout << endl;
}

int main() {
    vector<int> a = {2, 4, 3};  // 342
    vector<int> b = {5, 6, 4};  // 465
    ListNode* l1 = buildList(a);
    ListNode* l2 = buildList(b);

    ListNode* res = addTwoNumbers(l1, l2);
    cout << "342 + 465 = ";
    printList(res);  // 7 -> 0 -> 8

    // 测试不等长 + 最后进位
    vector<int> a2 = {9, 9, 9};    // 999
    vector<int> b2 = {1};          // 1
    l1 = buildList(a2);
    l2 = buildList(b2);
    res = addTwoNumbers(l1, l2);
    cout << "999 + 1 = ";
    printList(res);  // 0 -> 0 -> 0 -> 1

    return 0;
}
```

### 常见错误

- **错误1：最后忘记检查进位**。如 999 + 1 = 1000，三个节点走完但 carry=1 还没处理。while 循环必须包含 `carry` 条件。
- **错误2：不等长时忘记判空**。访问 `l1->val` 前没有判 `l1 != nullptr`，会段错误。
- **错误3：混淆数字存储顺序**。题目是逆序（个位在头），如果思维转换成顺序存储就会全错。

### 面试陷阱

- **陷阱1**：面试官问"如果数字是顺序存储（高位在头）怎么做？"——需要先反转链表（或借助栈），然后按同样逻辑逐位相加，最后再反转结果。或者用栈（后进先出，自然从低位开始处理）。
- **陷阱2**：面试官问"怎么优化空间？"——可以复用较长链表中的节点，直接在原链表上修改，减少 new 操作。

---

## 56. 环形链表 II — 题号 142

**难度**: 中等
**标签**: 链表、双指针、哈希表

### 题目大意

给定一个链表，返回链表开始入环的第一个节点。如果链表无环，返回 null。

### 思路

**暴力思路（哈希表）**：遍历链表，用 `unordered_set` 记录访问过的节点。第一个重复访问的节点就是环入口。空间 O(n)。

**优化思路（快慢指针 / Floyd 判圈算法）**：
1. 快指针每次走两步，慢指针每次走一步，若相遇则说明有环。
2. 相遇后，一个指针回到 head，两个指针同时每次走一步，再次相遇点就是环入口。

数学证明：设头到环入口距离 a，环入口到相遇点距离 b，相遇点到环入口距离 c（环长 b+c）。
- 慢指针走 `a + b` 步被快指针追上
- 快指针走 `a + b + k(b+c)` 步（多绕了 k 圈）
- 快指针路程是慢指针两倍：`2(a+b) = a + b + k(b+c)` => `a = (k-1)(b+c) + c`
- 即：从头走 a 步 = 从相遇点走 c 步再绕若干圈，两个指针会同时到达环入口

### 核心知识点

- Floyd 判圈算法（快慢指针）
- 环入口的数学推导
- 哈希表 vs 双指针的时空权衡

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(1)（快慢指针）/ O(n)（哈希表）

### C++ 代码 (ACM风格) — 双指针法

```cpp
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode* detectCycle(ListNode* head) {
    ListNode *slow = head, *fast = head;

    // 第一步：判断是否有环
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {  // 相遇，有环
            // 第二步：找环入口
            ListNode* ptr = head;
            while (ptr != slow) {
                ptr = ptr->next;
                slow = slow->next;
            }
            return ptr;  // 或 slow，两者此时指向同一节点
        }
    }

    return nullptr;  // 无环
}

// 哈希表写法（更容易理解，空间换时间）
ListNode* detectCycleHash(ListNode* head) {
    unordered_set<ListNode*> visited;
    while (head) {
        if (visited.count(head)) return head;  // 第一个重复的就是入口
        visited.insert(head);
        head = head->next;
    }
    return nullptr;
}

int main() {
    // 构建测试：1 -> 2 -> 3 -> 4 -> 5 -> 3(环)
    ListNode* n1 = new ListNode(1);
    ListNode* n2 = new ListNode(2);
    ListNode* n3 = new ListNode(3);
    ListNode* n4 = new ListNode(4);
    ListNode* n5 = new ListNode(5);
    n1->next = n2; n2->next = n3; n3->next = n4; n4->next = n5; n5->next = n3;

    ListNode* entry = detectCycle(n1);
    if (entry) cout << "环入口值: " << entry->val << endl;  // 3

    return 0;
}
```

### 常见错误

- **错误1：快指针判空不完整**。只写了 `fast != nullptr` 没写 `fast->next != nullptr`，当 fast 是尾节点时 `fast->next->next` 空指针访问崩溃。
- **错误2：哈希表版本错判入口**。应该是"第一次重复访问的节点"就是入口，而不是任意在环中的节点。因为哈希表按遍历顺序记录，第一个 hit 的就是最早进入环的节点。
- **错误3：数学推导混淆**。快慢相遇后，忘了把其中一个指针移回 head，直接继续用两个指针走——那样找到的不是入口。

### 面试陷阱

- **陷阱1**：面试官问"为什么快指针每次走两步，走三步行不行？"——三步可能导致在环内永远不相遇（如环长为2时，三步每次跳过对方）。两步能保证在大多数环长下都能相遇，这是 Floyd 算法的关键设计。
- **陷阱2**：面试官问"如果同时有环检测和找入口的需求，哈希表不是一样 O(n) 吗？"——哈希表确实 O(n) 时间但 O(n) 空间。在大链表场景（如 10^6 节点），空间差距巨大，且哈希表有碰撞开销。

---

## 57. K 个一组翻转链表 — 题号 25

**难度**: 困难
**标签**: 链表、递归

### 题目大意

每 k 个节点一组进行翻转，如果剩余节点不足 k 个则保持原样。不能只改值，必须交换节点。要求 O(1) 额外空间。

### 思路

**问题拆解**：翻转链表的子问题（翻转前 k 个） + 递归处理剩余部分 + 连接结果。

**递归思路**：
1. 检查剩余长度是否 >= k：从当前 head 出发走 k 步，走不到就返回 head（不足 k 个不翻转）。
2. 翻转前 k 个节点（经典的链表反转子问题）。
3. 递归处理剩余链表，把当前组的尾（原来的 head）连接到递归结果。
4. 返回当前组的新头（原来的第 k 个节点）。

**迭代思路**：
1. 虚拟头节点 `dummy`，`prev` 指向待翻转组的前一个节点。
2. 每次检查剩余是否 >= k。
3. 翻转 [prev->next, prev->next+k) 区间的节点。
4. 翻转后重新连接：`prev->next` 指向新头，原来的头指向剩余部分，`prev` 移动到翻转后的尾。
5. 循环直到不足 k 个。

两种写法的核心差异：递归"自上而下"地决定哪些组要翻转，迭代"自下而上"（按遍历顺序）地逐组处理。

### 核心知识点

- 分组链表翻转（整组反转 + 组间连接）
- 递归的分治思想：当前组翻转 + 递归处理剩余
- 迭代的统一框架：虚拟头 + prev 指针维护组间关系

### 复杂度

- **时间复杂度**: O(n) — 每个节点被访问常数次
- **空间复杂度**: O(1)（迭代）/ O(n/k)（递归栈）

### C++ 代码 (ACM风格) — 递归写法

```cpp
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// 翻转head的前k个节点，返回新头；把翻转后的尾节点通过引用返回
ListNode* reverseFirstK(ListNode* head, int k) {
    ListNode *prev = nullptr, *cur = head;
    while (k--) {
        ListNode* nxt = cur->next;
        cur->next = prev;
        prev = cur;
        cur = nxt;
    }
    // prev是新头，head变成尾（next指向cur即剩余部分）
    head->next = cur;
    return prev;
}

ListNode* reverseKGroup(ListNode* head, int k) {
    // 检查剩余长度是否>=k
    ListNode* cur = head;
    int count = 0;
    while (cur && count < k) {
        cur = cur->next;
        count++;
    }
    if (count < k) return head;  // 不足k个，不翻转

    // 翻转前k个
    ListNode* newHead = reverseFirstK(head, k);

    // head现在指向翻转后的尾节点，递归处理剩余
    head->next = reverseKGroup(head->next, k);

    return newHead;
}

// ===== 迭代写法 =====
ListNode* reverseKGroupIterative(ListNode* head, int k) {
    ListNode dummy(0);
    dummy.next = head;
    ListNode* prev = &dummy;  // prev指向待翻转组的前驱

    while (true) {
        // 检查剩余是否>=k
        ListNode* check = prev;
        int count = 0;
        while (check->next && count < k) {
            check = check->next;
            count++;
        }
        if (count < k) break;

        // 翻转 [prev->next, 第k个]
        ListNode* cur = prev->next;
        ListNode* groupTail = cur;  // 翻转后cur会变成尾
        ListNode* pre = nullptr;
        for (int i = 0; i < k; i++) {
            ListNode* nxt = cur->next;
            cur->next = pre;
            pre = cur;
            cur = nxt;
        }
        // pre是新头，cur是下一组的头
        prev->next = pre;        // 前驱接新头
        groupTail->next = cur;   // 翻转后的尾接下一组
        prev = groupTail;        // prev前进到翻转后的尾
    }

    return dummy.next;
}

ListNode* buildList(vector<int>& arr) {
    ListNode dummy(0);
    ListNode* cur = &dummy;
    for (int v : arr) {
        cur->next = new ListNode(v);
        cur = cur->next;
    }
    return dummy.next;
}

void printList(ListNode* head) {
    while (head) {
        cout << head->val;
        if (head->next) cout << " -> ";
        head = head->next;
    }
    cout << endl;
}

int main() {
    vector<int> arr = {1, 2, 3, 4, 5};
    ListNode* head = buildList(arr);
    cout << "原链表: ";
    printList(head);

    head = reverseKGroup(head, 2);
    cout << "k=2 翻转后: ";
    printList(head);  // 2 -> 1 -> 4 -> 3 -> 5

    // 测试迭代写法
    vector<int> arr2 = {1, 2, 3, 4, 5, 6, 7, 8};
    head = buildList(arr2);
    head = reverseKGroupIterative(head, 3);
    cout << "k=3 迭代翻转后: ";
    printList(head);  // 3 -> 2 -> 1 -> 6 -> 5 -> 4 -> 7 -> 8

    return 0;
}
```

### 常见错误

- **错误1：翻转后忘记重新连接组间关系**。翻转完前 k 个后，tail（原 head）的 next 还指向旧的下一个，必须手动接到 `cur`（剩余部分）或递归结果。
- **错误2：迭代写法中 prev 更新错误**。翻转后 prev 应该移动到本组翻转后的尾部（原来是 head，翻转后变成尾），而不是新头。如果移到新头，下一组的前驱就接错了。
- **错误3：不足 k 个的边界**。检查长度的循环中，如果中途遇到 null 且 count < k，说明不足 k 个，直接返回原链表不翻转。忘记这个直接翻转会越界。

### 面试陷阱

- **陷阱1**：面试官问"递归写法和迭代写法实际工程怎么选？"——递归写法更清晰，但 k 很小链表很长时栈深度 O(n/k) 可接受；k 很大（如 k=n）时栈深度很小。迭代写法稳定 O(1) 空间但代码更长易错。对于本题建议掌握两种以展示全面性。
- **陷阱2**：面试官问"能不能不用检查长度，直接在翻转过程中发现不足 k 个再恢复？"——可以，但 "先翻转再恢复" 需要翻两次，不如先检查一次，总体时间复杂度相同但多写了很多代码。先检查更直观。

---

## 58. 排序链表 — 题号 148

**难度**: 中等
**标签**: 链表、归并排序、双指针

### 题目大意

对链表进行升序排序。要求 O(n log n) 时间复杂度和 O(1) 空间复杂度。

### 思路

**为什么不能用快速排序？**——快排对链表不友好（随机访问成本高），且最坏 O(n^2) 不稳定。**为什么归并排序？**——链表天然适合归并：找到中点 O(n)，合并 O(n)，且不需要额外数组空间（只是指针操作），总 O(n log n)。

**自顶向下（递归）归并**：
1. 快慢指针找到链表中点（slow 停在中点，fast 到末尾）
2. 从中点断开，递归排序左右两部分
3. 合并两个有序链表

**自底向上（迭代）归并**（真正 O(1) 空间）：
1. 按步长 1, 2, 4, 8... 逐步合并
2. 每次取两个长度为 step 的片段，合并后连接到结果
3. 步长翻倍直到 >= 链表长度

递归写法更容易理解，空间来自递归栈 O(log n)。迭代写法严格 O(1) 空间。

### 核心知识点

- 链表归并排序（找中点 + 合并有序链表）
- 自底向上归并（迭代 + 步长翻倍）
- 排序算法选择：为什么链表用归并，数组用快排

### 复杂度

- **时间复杂度**: O(n log n)
- **空间复杂度**: O(log n)（递归栈）/ O(1)（迭代自底向上）

### C++ 代码 (ACM风格) — 递归写法

```cpp
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// 找中点（快慢指针），并从中点前断开
ListNode* findMiddleAndCut(ListNode* head) {
    if (!head || !head->next) return head;
    ListNode *slow = head, *fast = head, *prev = nullptr;
    while (fast && fast->next) {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }
    prev->next = nullptr;  // 关键：断开，左半部分自成链表
    return slow;           // slow是右半部分的头
}

// 合并两个有序链表
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* cur = &dummy;
    while (l1 && l2) {
        if (l1->val < l2->val) {
            cur->next = l1;
            l1 = l1->next;
        } else {
            cur->next = l2;
            l2 = l2->next;
        }
        cur = cur->next;
    }
    cur->next = l1 ? l1 : l2;  // 接上剩余
    return dummy.next;
}

ListNode* sortList(ListNode* head) {
    if (!head || !head->next) return head;

    ListNode* mid = findMiddleAndCut(head);  // head是左半，mid是右半
    ListNode* left = sortList(head);
    ListNode* right = sortList(mid);

    return mergeTwoLists(left, right);
}

// ===== 自底向上迭代写法（真正 O(1) 空间）=====
ListNode* sortListIterative(ListNode* head) {
    if (!head || !head->next) return head;

    // 计算链表长度
    int n = 0;
    ListNode* cur = head;
    while (cur) { n++; cur = cur->next; }

    ListNode dummy(0);
    dummy.next = head;

    // 步长逐步翻倍
    for (int step = 1; step < n; step *= 2) {
        ListNode* prev = &dummy;
        cur = dummy.next;

        while (cur) {
            // 分割出第一段长度为step的链表
            ListNode* h1 = cur;
            ListNode* tail1 = cur;
            for (int i = 1; i < step && tail1->next; i++)
                tail1 = tail1->next;

            ListNode* h2 = tail1->next;
            tail1->next = nullptr;  // 断开第一段

            // 分割出第二段长度为step的链表
            ListNode* tail2 = h2;
            if (tail2) {
                for (int i = 1; i < step && tail2->next; i++)
                    tail2 = tail2->next;
                cur = tail2->next;
                tail2->next = nullptr;  // 断开第二段
            } else {
                cur = nullptr;
            }

            // 合并两段，并连接到结果
            prev->next = mergeTwoLists(h1, h2);

            // prev移到合并后的尾部
            while (prev->next) prev = prev->next;
        }
    }

    return dummy.next;
}

ListNode* buildList(vector<int>& arr) {
    ListNode dummy(0);
    ListNode* cur = &dummy;
    for (int v : arr) {
        cur->next = new ListNode(v);
        cur = cur->next;
    }
    return dummy.next;
}

void printList(ListNode* head) {
    while (head) {
        cout << head->val;
        if (head->next) cout << " -> ";
        head = head->next;
    }
    cout << endl;
}

int main() {
    vector<int> arr = {4, 2, 1, 3, 5};
    ListNode* head = buildList(arr);
    cout << "排序前: ";
    printList(head);

    head = sortList(head);
    cout << "排序后: ";
    printList(head);

    // 测试迭代写法
    vector<int> arr2 = {3, 1, 4, 1, 5, 9, 2, 6};
    head = buildList(arr2);
    head = sortListIterative(head);
    cout << "迭代排序后: ";
    printList(head);

    return 0;
}
```

### 常见错误

- **错误1：找中点时忘记断开链表**。如果没有 `prev->next = nullptr`，merge 函数会把整个链表当作两个重叠的部分来合并，造成死循环或结果错误。
- **错误2：快慢指针找中点时初始值错误**。如果 slow 和 fast 都从 head 开始，偶数长度时 slow 会停在中间偏后，导致分割不均（左多右少）。一般偶数长度选左半多一个或少一个都可以，但一定要断开。
- **错误3：迭代归并中指针更新混乱**。合并两段后，prev 需要移到合并后的链表尾部，为下一次合并做准备。忘记移动会导致下一组合并时连接错乱。

### 面试陷阱

- **陷阱1**：面试官问"为什么数组用快排，链表用归并？"——数组快排就地交换 O(1) 空间且缓存友好；但链表找 pivot 需要遍历而交换也需要修改指针，效率低。归并排序对链表刚好是 O(1) 额外空间（只需指针重连），且合并两个有序链表是 O(n) 的天然操作。
- **陷阱2**：面试官追问"自底向上的归并为什么是 O(1) 空间，你能证明吗？"——除了虚拟头节点和几个指针变量，没有递归栈、没有额外分配数组。所有操作都在原链表上通过指针重连完成。

---

## 59. 平衡二叉树 — 题号 110

**难度**: 简单
**标签**: 二叉树、DFS

### 题目大意

判断一棵二叉树是否是高度平衡的。平衡定义：任意节点的左右子树高度差不超过 1。

### 思路

**暴力思路（自顶向下）**：对每个节点，分别计算左右子树高度，判断差值<=1，再递归判断左右子树是否平衡。问题：大量重复计算高度，O(n^2)。

**优化思路（自底向上，后序遍历）**：DFS 返回两个信息合一——如果子树不平衡则返回 -1，否则返回子树高度。这样每个节点只被访问一次。父节点收到 -1 后直接向上传递，不必继续计算。

核心设计：让 `height` 函数返回 `int`，用 -1 作为"不平衡"的信号。这是 C++ 中常用的 sentinel 技巧。

### 核心知识点

- 自底向上的后序遍历（bottom-up）
- Sentinel 值（-1）标记异常状态，避免额外结构体
- 树的高度定义：空树高度 0，叶子高度 1

### 复杂度

- **时间复杂度**: O(n) — 每个节点访问一次
- **空间复杂度**: O(h) — 递归栈深度等于树高，最坏 O(n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 返回子树高度，如果不平衡返回 -1
int checkHeight(TreeNode* root) {
    if (!root) return 0;

    int leftH = checkHeight(root->left);
    if (leftH == -1) return -1;  // 左子树已不平衡，向上传递

    int rightH = checkHeight(root->right);
    if (rightH == -1) return -1;  // 右子树已不平衡

    if (abs(leftH - rightH) > 1) return -1;  // 当前节点不平衡

    return max(leftH, rightH) + 1;  // 返回正常高度
}

bool isBalanced(TreeNode* root) {
    return checkHeight(root) != -1;
}

// 构建测试树
TreeNode* buildTree() {
    //       3
    //      / \
    //     9  20
    //       /  \
    //      15   7
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(9);
    root->right = new TreeNode(20);
    root->right->left = new TreeNode(15);
    root->right->right = new TreeNode(7);
    return root;
}

int main() {
    TreeNode* root = buildTree();
    cout << "是否平衡: " << (isBalanced(root) ? "是" : "否") << endl;  // 是

    // 构造不平衡树
    TreeNode* root2 = new TreeNode(1);
    root2->left = new TreeNode(2);
    root2->left->left = new TreeNode(3);
    cout << "是否平衡: " << (isBalanced(root2) ? "是" : "否") << endl;  // 否

    return 0;
}
```

### 常见错误

- **错误1：混淆高度和深度**。平衡二叉树比较的是"高度"（从该节点到最深叶子的边数），而非"深度"（从根到该节点的边数）。在自底向上计算中，空节点高度返回 0（不是 -1）。
- **错误2：用自顶向下方法**。对每个节点调用 `height`，然后再递归调用 `isBalanced`，导致 O(n^2) 时间复杂度。面试官一眼看出不是最优。
- **错误3：忘记提前向上传递 -1**。左子树返回 -1 后，不能继续计算右子树高度，应直接返回 -1。

### 面试陷阱

- **陷阱1**：面试官问"能不能用栈实现迭代版避免递归栈溢出？"——可以，用后序遍历的迭代框架，同时维护每个节点的高度信息。但代码量很大，面试中如果没要求一般递归即可，但要主动提这个问题。
- **陷阱2**：面试官追问"AVL 树和红黑树的平衡条件有什么不同？"——AVL 树严格平衡（高度差 <=1），查找快但旋转频繁；红黑树宽松平衡（最长路径不超过最短路径两倍），写入性能更好。本题的条件是 AVL 级别的严格平衡。

---

## 60. 验证二叉搜索树 — 题号 98

**难度**: 中等
**标签**: 二叉树、DFS、中序遍历

### 题目大意

判断一棵二叉树是否是有效的二叉搜索树。有效条件：左子树所有节点 < 根节点 < 右子树所有节点（严格小于/大于，不能相等）。

### 思路

**误区**：不能只比较当前节点和左右孩子！比如 `[5,1,4,null,null,3,6]`，3 在 5 的右子树中但 3 < 5，违反了 BST 性质。

**方法一（范围递归，前序遍历）**：每个节点携带一个合法范围 `(minVal, maxVal)`，根节点范围 `(-inf, +inf)`。向左走时更新上界为当前节点值，向右走时更新下界为当前节点值。用 `long long` 防溢出。

**方法二（中序遍历）**：BST 的中序遍历结果严格递增。维护 `prev` 记录上一个访问的节点值，检查当前值 > prev。需要处理 `prev` 的初始状态（第一次访问前 prev 无效）。

### 核心知识点

- BST 的性质：左 < 根 < 右 是全局约束，不是局部约束
- 范围递归（传递上下界）
- 中序遍历的递增性质
- `long long` 处理边界值（INT_MIN / INT_MAX）

### 复杂度

- **时间复杂度**: O(n) — 每个节点访问一次
- **空间复杂度**: O(h) — 递归栈，最坏 O(n)

### C++ 代码 (ACM风格) — 两种写法

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// ===== 方法一：范围递归（前序遍历）=====
bool validateRange(TreeNode* root, long long minVal, long long maxVal) {
    if (!root) return true;

    // 当前节点必须严格在范围内
    if (root->val <= minVal || root->val >= maxVal) return false;

    // 左子树的上界变为当前节点值，右子树的下界变为当前节点值
    return validateRange(root->left, minVal, root->val) &&
           validateRange(root->right, root->val, maxVal);
}

bool isValidBST_Range(TreeNode* root) {
    return validateRange(root, LLONG_MIN, LLONG_MAX);
}

// ===== 方法二：中序遍历（检查是否严格递增）=====
bool isValidBST_Inorder(TreeNode* root) {
    long long prev = LLONG_MIN;  // 记录上一个节点的值
    bool valid = true;

    function<void(TreeNode*)> inorder = [&](TreeNode* node) {
        if (!node || !valid) return;

        inorder(node->left);

        // 中序位置：检查递增性
        if (node->val <= prev) {
            valid = false;
            return;
        }
        prev = node->val;

        inorder(node->right);
    };

    inorder(root);
    return valid;
}

int main() {
    // 构建合法BST：
    //       2
    //      / \
    //     1   3
    TreeNode* root = new TreeNode(2);
    root->left = new TreeNode(1);
    root->right = new TreeNode(3);
    cout << "方法一(范围): " << (isValidBST_Range(root) ? "是BST" : "不是BST") << endl;  // 是
    cout << "方法二(中序): " << (isValidBST_Inorder(root) ? "是BST" : "不是BST") << endl;  // 是

    // 构建非法BST：[5,1,4,null,null,3,6]
    TreeNode* root2 = new TreeNode(5);
    root2->left = new TreeNode(1);
    root2->right = new TreeNode(4);
    root2->right->left = new TreeNode(3);
    root2->right->right = new TreeNode(6);
    cout << "方法一(范围): " << (isValidBST_Range(root2) ? "是BST" : "不是BST") << endl;  // 不是
    cout << "方法二(中序): " << (isValidBST_Inorder(root2) ? "是BST" : "不是BST") << endl;  // 不是

    // 边界测试：INT_MIN/INT_MAX
    TreeNode* root3 = new TreeNode(INT_MIN);
    root3->right = new TreeNode(INT_MAX);
    cout << "INT_MIN/MAX BST: " << (isValidBST_Range(root3) ? "是BST" : "不是BST") << endl;  // 是

    return 0;
}
```

### 常见错误

- **错误1：只比较当前节点和左右孩子**。这是最常见的错误！BST 是全局约束。例如 `[5,1,4,null,null,3,6]`：4 是 5 的右子，4 的左子 3 是合法的（3<4），但 3 在 5 的右子树中（3<5 非法）。必须传递范围。
- **错误2：使用 int 而不是 long long**。当节点值恰好是 `INT_MIN` 或 `INT_MAX` 时，`minVal - 1` / `maxVal + 1` 会溢出。用 `LLONG_MIN/LLONG_MAX` 初始化范围。
- **错误3：中序遍历时 prev 的初始化和更新**。第一次访问的左下角的节点需要和 prev 比较，prev 必须初始化为比任何可能值都小的值。如果用 `int prev = INT_MIN` 且树中恰好有 `INT_MIN`，`<=` 判断会误判为不合法。所以要用 `long long`。

### 面试陷阱

- **陷阱1**：面试官问"如果树中有重复元素怎么办？"——BST 通常不允许重复（严格小于/大于）。如果允许重复，通常定义为"左 <= 根 <= 右"，此时中序遍历变成"非递减而非严格递增"。需要和面试官确认。
- **陷阱2**：面试官追问"递归传上下界和中序遍历各有什么适用场景？"——范围递归更直观，且可以灵活定义范围约束（如区间查询时）；中序遍历可以顺便拿到有序序列，适用于需要"第 K 小""范围求和"等衍生问题。如果面试官继续追问"中序能否不递归"——可以用栈做迭代中序，或 Morris 遍历。

---

## 61. 二叉树的最近公共祖先 — 题号 236

**难度**: 中等
**标签**: 二叉树、DFS

### 题目大意

在一棵普通二叉树中找到两个给定节点的最近公共祖先（LCA），即深度最大的公共祖先节点。

### 思路

**暴力思路**：分别记录从根到 p 和 q 的路径，然后比较两条路径，最后一个相同的节点就是 LCA。需要额外存储路径，空间 O(n)。

**优化思路（后序遍历，递归）**：
自底向上，如果当前节点是 p 或 q，返回当前节点。递归获取左右子树的搜索结果：
- 如果左右都返回非空（各找到一个）：说明 p 和 q 分布在左右子树，当前节点就是 LCA。
- 如果只有一边返回非空：p 和 q 都在那一边，向上传递那个结果。
- 如果两边都为空：当前子树不含 p 或 q，返回空。

这种设计非常巧妙，递归返回的语义是"当前子树中包含的 p 或 q（或 LCA）"。

### 核心知识点

- 后序遍历（自底向上收集信息）
- 递归返回值的多重语义（找到 p / 找到 q / 找到 LCA）
- 利用分治思想：左右都找到 → 当前为 LCA

### 复杂度

- **时间复杂度**: O(n) — 每个节点访问一次
- **空间复杂度**: O(h) — 递归栈

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    // 递归终止：到达空节点，或找到p/q之一
    if (!root || root == p || root == q) return root;

    TreeNode* left = lowestCommonAncestor(root->left, p, q);
    TreeNode* right = lowestCommonAncestor(root->right, p, q);

    // 左右各找到一个，说明当前root就是LCA
    if (left && right) return root;

    // 只有一边找到，向上传递找到的结果（p/q或已确定的LCA）
    return left ? left : right;
}

int main() {
    // 构建树：
    //       3
    //      / \
    //     5   1
    //    / \ / \
    //   6  2 0  8
    //     / \
    //    7   4
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(5);
    root->right = new TreeNode(1);
    root->left->left = new TreeNode(6);
    root->left->right = new TreeNode(2);
    root->left->right->left = new TreeNode(7);
    root->left->right->right = new TreeNode(4);
    root->right->left = new TreeNode(0);
    root->right->right = new TreeNode(8);

    TreeNode* p = root->left;           // 5
    TreeNode* q = root->left->right->right;  // 4
    TreeNode* lca = lowestCommonAncestor(root, p, q);
    cout << "LCA(5, 4) = " << lca->val << endl;  // 5

    p = root->left;              // 5
    q = root->right->right;      // 8
    lca = lowestCommonAncestor(root, p, q);
    cout << "LCA(5, 8) = " << lca->val << endl;  // 3

    // 边界：一个节点是另一个的祖先
    p = root;                    // 3
    q = root->left->left;       // 6
    lca = lowestCommonAncestor(root, p, q);
    cout << "LCA(3, 6) = " << lca->val << endl;  // 3

    return 0;
}
```

### 常见错误

- **错误1：递归终止条件写错**。`if (!root || root == p || root == q) return root;` — 必须把 `!root` 放在前面，否则空节点访问 `root->val` 会崩溃。
- **错误2：忘记 p 就是 LCA 的情况**。当 p 是 q 的祖先时，算法在遇到 p 时就返回 p（因为 `root == p` 直接返回），向上传递时不会遇到"左右都不空"的情况。这个处理是正确的，但初学者可能怀疑是不是漏了。
- **错误3：和多叉树或 BST 的 LCA 混淆**。本题是普通二叉树，不能利用 BST 的大小关系。如果是 BST，可以根据值的大小决定向左还是向右走，那是另一题（235）。

### 面试陷阱

- **陷阱1**：面试官问"如果 p 或 q 不在树中怎么处理？"——题目保证一定在，但可以讨论。如果可能不在，需要改变策略：先搜索验证两个节点都存在，再找 LCA。或者用一个额外的结构体返回"是否找到 p"和"是否找到 q"。
- **陷阱2**：面试官追问"这个方法的时间复杂度能否优化到小于 O(n)？"——不能，因为最坏情况需要遍历整棵树。但如果在同一棵树上多次查询不同节点对的 LCA，可以用倍增法或 Tarjan 离线算法预处理，达到 O(log n) 每次查询。

---

## 62. 从前序与中序遍历序列构造二叉树 — 题号 105

**难度**: 中等
**标签**: 二叉树、递归、哈希表

### 题目大意

给定前序遍历和中序遍历的结果（无重复元素），构造并返回原二叉树。

示例：前序 `[3,9,20,15,7]`，中序 `[9,3,15,20,7]` → 树 `[3,9,20,null,null,15,7]`

### 思路

**核心洞察**：前序的第一个元素是根节点。在中序中找到根节点的位置，左边是左子树的中序，右边是右子树的中序。根据左子树的大小，可以在前序中切出左子树的前序和右子树的前序。然后递归构建。

**索引计算是关键**：
- 前序 `[preStart ... preEnd]`，中序 `[inStart ... inEnd]`
- 根值 = `preorder[preStart]`
- 在中序中找根的位置 `rootIdx`
- 左子树大小 = `rootIdx - inStart`
- 左子树前序范围 = `[preStart+1 ... preStart+leftSize]`
- 右子树前序范围 = `[preStart+leftSize+1 ... preEnd]`

**优化：用哈希表加速查找**。每次递归扫描中序数组找根的位置是 O(n)，总体 O(n^2)。用哈希表 `unordered_map<值, 索引>` 将查找变为 O(1)。

### 核心知识点

- 前序+中序唯一确定二叉树（但前序+后序不能唯一确定）
- 递归划分子问题（分治）
- 哈希表加速查找（O(n^2) → O(n)）
- 索引计算：左子树大小是关键中间量

### 复杂度

- **时间复杂度**: O(n) — 哈希表查找 O(1)，每个节点构建一次
- **空间复杂度**: O(n) — 哈希表 + 递归栈

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

TreeNode* buildSubtree(vector<int>& preorder, int preStart, int preEnd,
                       vector<int>& inorder, int inStart, int inEnd,
                       unordered_map<int, int>& inMap) {
    if (preStart > preEnd || inStart > inEnd) return nullptr;

    // 前序第一个元素是根
    int rootVal = preorder[preStart];
    TreeNode* root = new TreeNode(rootVal);

    // 在中序中找到根的位置（O(1) via hashmap）
    int rootIdx = inMap[rootVal];
    int leftSize = rootIdx - inStart;  // 左子树节点数

    // 递归构建左右子树（索引计算是核心）
    root->left = buildSubtree(preorder, preStart + 1, preStart + leftSize,
                              inorder, inStart, rootIdx - 1, inMap);
    root->right = buildSubtree(preorder, preStart + leftSize + 1, preEnd,
                               inorder, rootIdx + 1, inEnd, inMap);

    return root;
}

TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
    unordered_map<int, int> inMap;
    for (int i = 0; i < inorder.size(); i++) {
        inMap[inorder[i]] = i;
    }
    return buildSubtree(preorder, 0, preorder.size() - 1,
                        inorder, 0, inorder.size() - 1, inMap);
}

// 后序遍历打印（验证用）
void postorder(TreeNode* root) {
    if (!root) return;
    postorder(root->left);
    postorder(root->right);
    cout << root->val << " ";
}

// 层序遍历打印（查看结构）
void levelOrder(TreeNode* root) {
    if (!root) return;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode* cur = q.front(); q.pop();
        if (cur) {
            cout << cur->val << " ";
            q.push(cur->left);
            q.push(cur->right);
        } else {
            cout << "null ";
        }
    }
    cout << endl;
}

int main() {
    vector<int> preorder = {3, 9, 20, 15, 7};
    vector<int> inorder = {9, 3, 15, 20, 7};

    TreeNode* root = buildTree(preorder, inorder);

    cout << "后序遍历（应等于原前序的特换验证）: ";
    postorder(root);
    cout << endl;  // 9 15 7 20 3

    cout << "层序遍历: ";
    levelOrder(root);

    return 0;
}
```

### 常见错误

- **错误1：索引计算错误**。左子树前序范围是 `[preStart+1, preStart+leftSize]`，不是 `[preStart+1, rootIdx]`。前者是基于前序索引，后者是基于中序索引，两者不能混用！右子树前序从 `preStart+leftSize+1` 开始，不是 `rootIdx+1`。
- **错误2：递归终止条件不完整**。需要同时检查 `preStart > preEnd` 和 `inStart > inEnd`，两者应该同步（因为分割的子数组大小相同），但双重检查更安全。
- **错误3：忘记用哈希表，每次线性查找**。如果直接在 inorder 中 `find` 或循环查找，时间复杂度退化到 O(n^2)。哈希表是 O(1) 查找的关键。

### 面试陷阱

- **陷阱1**：面试官问"如果树中有重复元素怎么办？"——前序+中序无法唯一确定有重复元素的二叉树。需要额外的信息，如添加"左子树优先"等约定，或在节点中增加唯一 ID。通常题目保证无重复元素。
- **陷阱2**：面试官追问"能不能只从中序和后序构造？"——可以，原理相同：后序的最后一个元素是根，在中序中找到根，切分左右。代码几乎一样，只是根的取值位置不同。前序+后序不能唯一确定（因为无法区分左右子树的分界）。

---

## 63. 路径总和 III — 题号 437

**难度**: 中等
**标签**: 二叉树、DFS、前缀和

### 题目大意

计算二叉树中路径和等于 `targetSum` 的路径总数。路径不需要从根开始，也不需要在叶子结束，但必须从上到下（父子关系，不能拐弯）。

### 思路

**暴力思路（双重递归）**：对每个节点作为起点，DFS 向下统计所有以该节点为起点的路径。外层递归遍历每个节点 O(n)，内层对每个节点 DFS O(n)，总 O(n^2)。

**优化思路（前缀和 + 哈希表）**：类似数组的子数组和问题！从上到下遍历时，维护从根到当前节点的前缀和。如果前缀和为 `curSum`，且之前有前缀和 `curSum - targetSum` 出现过，说明从那个时刻到当前节点的路径和为 `targetSum`。

关键实现细节：
- 哈希表记录"前缀和 → 出现次数"
- 进入节点时更新当前前缀和，检查 `curSum - target` 在哈希表中的次数
- 递归左右子树
- **离开节点时恢复现场**（前缀和出现次数 -1），因为兄弟子树不能使用当前路径的前缀和

### 核心知识点

- 前缀和思想从数组迁移到树（DFS 路径）
- 回溯恢复现场（离开节点时从哈希表中移除）
- 初始条件：`prefixSum[0] = 1`（空路径的前缀和为 0）

### 复杂度

- **时间复杂度**: O(n) — 每个节点访问一次
- **空间复杂度**: O(h) — 递归栈 + O(n) 哈希表

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

int pathSum(TreeNode* root, int targetSum) {
    unordered_map<long long, int> prefixCount;
    prefixCount[0] = 1;  // 空路径前缀和为0，很重要！
    int result = 0;

    function<void(TreeNode*, long long)> dfs = [&](TreeNode* node, long long curSum) {
        if (!node) return;

        curSum += node->val;

        // 检查有多少个以前的前缀和 == curSum - targetSum
        // 那些位置到当前节点的路径和就是 targetSum
        if (prefixCount.count(curSum - targetSum)) {
            result += prefixCount[curSum - targetSum];
        }

        // 进入当前节点：把当前前缀和加入哈希表
        prefixCount[curSum]++;

        // 递归左右子树
        dfs(node->left, curSum);
        dfs(node->right, curSum);

        // 离开当前节点：恢复现场（回溯）
        prefixCount[curSum]--;
    };

    dfs(root, 0);
    return result;
}

// ===== 暴力写法（O(n^2)，用于对比理解）=====
int pathSumBruteForce(TreeNode* root, int targetSum) {
    if (!root) return 0;

    // 以当前节点为起点的路径数（内层DFS）
    function<int(TreeNode*, long long)> countFrom = [&](TreeNode* node, long long curSum) {
        if (!node) return 0;
        int count = 0;
        curSum += node->val;
        if (curSum == targetSum) count++;
        count += countFrom(node->left, curSum);
        count += countFrom(node->right, curSum);
        return count;
    };

    // 外层递归：每个节点都作为起点尝试
    return countFrom(root, 0) +
           pathSumBruteForce(root->left, targetSum) +
           pathSumBruteForce(root->right, targetSum);
}

int main() {
    // 构建测试树：
    //       10
    //      /  \
    //     5   -3
    //    / \    \
    //   3   2    11
    //  / \   \
    // 3  -2   1
    TreeNode* root = new TreeNode(10);
    root->left = new TreeNode(5);
    root->right = new TreeNode(-3);
    root->left->left = new TreeNode(3);
    root->left->right = new TreeNode(2);
    root->right->right = new TreeNode(11);
    root->left->left->left = new TreeNode(3);
    root->left->left->right = new TreeNode(-2);
    root->left->right->right = new TreeNode(1);

    cout << "目标8的路径数(前缀和): " << pathSum(root, 8) << endl;  // 3
    cout << "目标8的路径数(暴力): " << pathSumBruteForce(root, 8) << endl;  // 3

    return 0;
}
```

### 常见错误

- **错误1：忘记 `prefixCount[0] = 1`**。这意味着"空路径的前缀和是0"。当 `curSum == targetSum` 时，`curSum - targetSum = 0`，需要一个计数。如果不初始化，从根开始的合法路径会被漏掉。
- **错误2：忘记回溯恢复现场**。`prefixCount[curSum]++` 后，如果在离开节点时不 `--`，兄弟子树的搜索会被当前路径的前缀和污染，导致错误计数。
- **错误3：使用 `int` 存前缀和可能溢出**。如果树很深且节点值较大，前缀和可能超过 int 范围。用 `long long` 更安全。

### 面试陷阱

- **陷阱1**：面试官问"如果树中有负值，前缀和还适用吗？"——适用。前缀和方法不要求路径和是单调的，也不要求节点值为正。哈希表中可能积累多个相同的前缀和值，这正是用 `count` 而非 `bool` 的原因。
- **陷阱2**：面试官追问"能不能扩展到任意方向的路径（不限于从上到下）？"——不可以。如果路径可以任意拐弯，就变成了图中的路径问题，复杂度会上升到 NP-hard（如找哈密顿路径）。题目限定"从上到下"是关键约束。

---

## 64. 二叉树的右视图 — 题号 199

**难度**: 中等
**标签**: 二叉树、BFS、DFS

### 题目大意

从二叉树的右侧看过去，返回能看到的节点值（从上到下）。

示例：`[1,2,3,null,5,null,4]` 右视图 = `[1,3,4]`

### 思路

**方法一（BFS 层序遍历）**：层序遍历，每层最后一个节点就是右视图看到的节点。最直观的解法。

**方法二（DFS 先右后左）**：维护一个 `depth` 参数。先遍历右子树再遍历左子树，如果当前深度等于 `result.size()`，说明这是该层第一个访问到的节点（即最右边的节点），将其加入结果。这利用了"深度等于已有结果数时就是新层的首个节点"的特性。

两种方法各有优劣：BFS 更好理解，DFS 无需维护队列。

### 核心知识点

- BFS 层序遍历（队列，每层最后一个节点）
- DFS 带深度参数（根右左遍历顺序）
- `depth == result.size()` 判断是否为该层第一个节点

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(w)（BFS 队列宽度）/ O(h)（DFS 递归栈）

### C++ 代码 (ACM风格) — 两种写法

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// ===== 方法一：BFS 层序遍历 =====
vector<int> rightSideViewBFS(TreeNode* root) {
    vector<int> result;
    if (!root) return result;

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int size = q.size();
        for (int i = 0; i < size; i++) {
            TreeNode* node = q.front(); q.pop();
            // 每层最后一个节点就是右视图看到的
            if (i == size - 1) result.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }
    return result;
}

// ===== 方法二：DFS 先右后左 =====
vector<int> rightSideViewDFS(TreeNode* root) {
    vector<int> result;
    function<void(TreeNode*, int)> dfs = [&](TreeNode* node, int depth) {
        if (!node) return;
        // 如果当前深度等于result的大小，说明这是该层第一个访问到的节点
        // 因为先遍历右子树，所以第一个一定是该层最右边的
        if (depth == result.size()) {
            result.push_back(node->val);
        }
        dfs(node->right, depth + 1);  // 先右！
        dfs(node->left, depth + 1);   // 后左
    };
    dfs(root, 0);
    return result;
}

int main() {
    // 构建测试树：[1,2,3,null,5,null,4]
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->right = new TreeNode(5);
    root->right->right = new TreeNode(4);

    vector<int> result1 = rightSideViewBFS(root);
    cout << "BFS右视图: ";
    for (int v : result1) cout << v << " ";
    cout << endl;  // 1 3 4

    vector<int> result2 = rightSideViewDFS(root);
    cout << "DFS右视图: ";
    for (int v : result2) cout << v << " ";
    cout << endl;  // 1 3 4

    return 0;
}
```

### 常见错误

- **错误1：DFS 中先遍历左子树**。如果先左后右，`depth == result.size()` 时添加的是该层最左侧的节点，就变成左视图了。必须"先右后左"才能保证最先访问到最右。
- **错误2：BFS 中用 `q.back()` 取最后一个节点**。`q.back()` 在每次 push 后会变，不是稳定的"该层最后一个"。应该在遍历每层时通过计数 `i == size - 1` 来判断。
- **错误3：DFS 中 depth 从 1 开始**。如果用 `depth=1` 初始化，会导致 `depth == result.size()` 的判断偏移一位。统一用 `depth=0` 更自然。

### 面试陷阱

- **陷阱1**：面试官问"如果让你实现左视图呢？"——DFS 改为先左后右即可；BFS 改为取每层第一个节点（`i == 0`）。
- **陷阱2**：面试官追问"DFS 方法中，如果某一层最右节点在很后面（树非常不平衡），BFS 和 DFS 哪个更好？"——BFS 需要存储整层节点，在宽树时内存大；DFS 只需要递归栈，在窄而深的树上栈深。实际场景根据树的形状选择。

---

## 65. 二叉树中的最大路径和 — 题号 124

**难度**: 困难
**标签**: 二叉树、DFS、DP

### 题目大意

二叉树中的路径被定义为任意节点出发、沿父子关系走到任意节点的序列，同一节点最多出现一次。求最大路径和。

### 思路

**核心**：路径有三种形态：
1. 只在左子树中
2. 只在右子树中
3. 跨越根节点（左子树贡献 + 当前节点 + 右子树贡献）

**后序遍历设计**：每个节点返回"以该节点为端点的最大单边路径和"（只选一边或都不选）。为什么？因为父节点需要知道从当前节点出发向上能贡献多少。

递归逻辑：
- `leftMax = max(dfs(root->left), 0)` — 如果子树贡献为负，不选
- `rightMax = max(dfs(root->right), 0)` — 同理
- 跨越当前节点的路径和 = `leftMax + root->val + rightMax`，更新全局最大值
- 返回给父节点的：`root->val + max(leftMax, rightMax)` — 只能选一边

全局变量 `maxSum` 初始化为 `INT_MIN`（或节点的最小值），处理全负数的情况。

### 核心知识点

- 后序遍历 + 全局最优
- "子树贡献可能为负"的处理（`max(0, val)` 剪枝）
- 区分"返回给父节点的值"和"跨越当前节点的值"
- 类似"最大子数组和"的树形推广

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(h) — 递归栈

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

int maxPathSum(TreeNode* root) {
    int maxSum = INT_MIN;  // 全局最大值，处理全负数情况

    // 返回以node为端点的最大单边路径和
    function<int(TreeNode*)> dfs = [&](TreeNode* node) -> int {
        if (!node) return 0;

        // 计算左右子树的最大贡献，如果为负则舍弃（不选）
        int leftMax = max(dfs(node->left), 0);
        int rightMax = max(dfs(node->right), 0);

        // 跨越当前节点的路径和（可能是最终答案的候选）
        int throughRoot = leftMax + node->val + rightMax;
        maxSum = max(maxSum, throughRoot);

        // 返回给父节点：只能选一边
        return node->val + max(leftMax, rightMax);
    };

    dfs(root);
    return maxSum;
}

int main() {
    // 测试1：[-10,9,20,null,null,15,7]
    TreeNode* root = new TreeNode(-10);
    root->left = new TreeNode(9);
    root->right = new TreeNode(20);
    root->right->left = new TreeNode(15);
    root->right->right = new TreeNode(7);
    cout << "最大路径和: " << maxPathSum(root) << endl;  // 42 (15+20+7)

    // 测试2：全负数
    TreeNode* root2 = new TreeNode(-3);
    root2->left = new TreeNode(-2);
    root2->right = new TreeNode(-1);
    cout << "全负数最大路径和: " << maxPathSum(root2) << endl;  // -1 (只选-1)

    // 测试3：单节点
    TreeNode* root3 = new TreeNode(5);
    cout << "单节点最大路径和: " << maxPathSum(root3) << endl;  // 5

    return 0;
}
```

### 常见错误

- **错误1：返回值与更新值混淆**。`dfs` 返回"以 node 为端点的最大单边路径和"给父节点用；但更新 `maxSum` 用的是"跨越 node 的最大路径和"（两边都选）。两者不同！
- **错误2：全负数时 maxSum 初始化为 0**。如果初始化为 0，全负数树的结果就是 0（空路径），不符合题意（路径至少包含一个节点）。必须初始化为 `INT_MIN`。
- **错误3：未处理子树负贡献**。不用 `max(0, ...)` 直接把负数加进来，会拉低结果。因为路径可以不包含某个子树上所有的节点（即从当前节点只往一个方向延伸，甚至不延伸）。

### 面试陷阱

- **陷阱1**：面试官问"为什么返回的是 `root->val + max(leftMax, rightMax)` 而不是 `leftMax + root->val + rightMax`？"——因为返回给父节点，父节点需要的是"以当前节点为端点、向上延伸"的最大值。路径不能分叉，只能选左右之一。
- **陷阱2**：面试官追问"如果路径可以包含兄弟节点（即路径不限于父子关系）呢？"——这就变成了一般图中的最长路径问题，是 NP-hard 的（哈密顿路径问题归约）。如果允许从任意节点到任意节点但仍是简单路径，可用树形 DP 的"最大子树贡献"扩展。

---

## 66. 二叉树的锯齿形层序遍历 — 题号 103

**难度**: 中等
**标签**: 二叉树、BFS

### 题目大意

层序遍历二叉树，但方向交替：第一层从左到右，第二层从右到左，第三层从左到右……依此类推。

示例：`[3,9,20,null,null,15,7]` → `[[3],[20,9],[15,7]]`

### 思路

在标准 BFS 层序遍历基础上，加一个 `direction` 标志位（或层数奇偶判断）：
- 奇数层（0-indexed 的偶数层）：正常从左到右，`push_back`
- 偶数层（0-indexed 的奇数层）：从右到左，可以 `reverse` 当前层，或使用 `deque` 按不同方向插入。

两种实现方式：
1. 每层 BFS 后，根据层数决定是否 `reverse`
2. 使用 `deque`，按方向决定 `push_back` 还是 `push_front`

方式 1 更简洁，方式 2 理论更优（省去 reverse 的 O(n) 但总复杂度相同）。

### 核心知识点

- BFS 层序遍历框架（queue + 层内循环）
- 方向标志位（bool 或 层数 % 2）
- `reverse` 的应用（或 `deque` 双向插入）

### 复杂度

- **时间复杂度**: O(n) — 每个节点访问一次（reverse 也是 O(该层节点数)，总 O(n)）
- **空间复杂度**: O(w) — 队列最宽时存储一层节点

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
    vector<vector<int>> result;
    if (!root) return result;

    queue<TreeNode*> q;
    q.push(root);
    bool leftToRight = true;  // 方向标志

    while (!q.empty()) {
        int size = q.size();
        vector<int> level(size);  // 提前分配，按索引插入

        for (int i = 0; i < size; i++) {
            TreeNode* node = q.front(); q.pop();

            // 根据方向决定插入位置
            int idx = leftToRight ? i : size - 1 - i;
            level[idx] = node->val;

            // 子节点正常入队
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }

        leftToRight = !leftToRight;  // 翻转方向
        result.push_back(level);
    }

    return result;
}

// 另一种写法：普通BFS + reverse
vector<vector<int>> zigzagLevelOrderV2(TreeNode* root) {
    vector<vector<int>> result;
    if (!root) return result;

    queue<TreeNode*> q;
    q.push(root);
    int level = 0;

    while (!q.empty()) {
        int size = q.size();
        vector<int> cur;
        for (int i = 0; i < size; i++) {
            TreeNode* node = q.front(); q.pop();
            cur.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        if (level % 2 == 1) reverse(cur.begin(), cur.end());
        result.push_back(cur);
        level++;
    }

    return result;
}

int main() {
    // 构建测试树：[3,9,20,null,null,15,7]
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(9);
    root->right = new TreeNode(20);
    root->right->left = new TreeNode(15);
    root->right->right = new TreeNode(7);

    vector<vector<int>> result = zigzagLevelOrder(root);
    cout << "锯齿遍历(索引法):" << endl;
    for (auto& level : result) {
        cout << "[";
        for (int i = 0; i < level.size(); i++) {
            cout << level[i];
            if (i < level.size() - 1) cout << ",";
        }
        cout << "] ";
    }
    cout << endl;  // [3] [20,9] [15,7]

    vector<vector<int>> result2 = zigzagLevelOrderV2(root);
    cout << "锯齿遍历(reverse法):" << endl;
    for (auto& level : result2) {
        cout << "[";
        for (int i = 0; i < level.size(); i++) {
            cout << level[i];
            if (i < level.size() - 1) cout << ",";
        }
        cout << "] ";
    }
    cout << endl;

    return 0;
}
```

### 常见错误

- **错误1：改变子节点入队顺序**。有人误以为偶数层应该"先右后左"入队，这是错的。子节点始终按"先左后右"入队，方向控制只影响当前层结果的读取顺序。如果改了入队顺序，下一层就全乱了。
- **错误2：索引计算用错层数判断**。如果用 `level % 2` 判断方向，注意 level 从 0 开始：第 0 层（根）从左到右，第 1 层从右到左。有些实现 level 从 1 开始，判断条件就变成了 `level % 2 == 0` 时翻转。
- **错误3：用 `push_front` 导致 O(n^2)**。`vector.push_front` 不存在，但用 `insert(begin())` 每次 O(k) 会累计 O(n^2)。要么用索引法预分配，要么用 `deque` 的 `push_front`。

### 面试陷阱

- **陷阱1**：面试官问"如果要用 DFS 实现呢？"——可以在 DFS 时记录深度，按深度插入到对应层。奇数层（0-indexed 偶数层）`push_back`，偶数层 `push_front`（用 `deque` 或预处理）。但 DFS 不是最自然的解法。
- **陷阱2**：面试官追问"索引法和 reverse 法哪个在实际工程中更好？"——reverse 法代码更简洁，但多了一次 reverse（虽然仍是 O(n)）。索引法需要预分配 vector 大小，在内存受限时更好。面试中写 reverse 法通常足够而且更不容易出错。

---

## 67. 二叉搜索树中第K小的元素 — 题号 230

**难度**: 中等
**标签**: 二叉搜索树、DFS、中序遍历

### 题目大意

在一棵 BST 中找到第 K 小的元素（1-indexed）。

### 思路

**核心洞察**：BST 的中序遍历是严格递增的，第 K 个访问的节点就是第 K 小。

**递归中序遍历**：维护计数器 `count`，中序位置每次 +1，当 `count == k` 时记录结果并提前终止（通过剪枝避免后续无用遍历）。

**迭代中序遍历（栈）**：用栈模拟中序遍历，最直观，可以在找到第 K 个后直接 `break`，不遍历剩余节点。

**进阶（如果经常插入删除且频繁查询）**：可以在节点中维护 `count`（子树节点数），在 O(log n) 时间内找到第 K 小。类似顺序统计树。

### 核心知识点

- BST 中序遍历 = 递增序列
- 递归中的提前终止（剪枝）
- 迭代中序遍历栈框架
- 进阶：节点维护子树大小（O(log n) 查询）

### 复杂度

- **时间复杂度**: O(h + k)，最坏 O(n)（当 k=n 时）
- **空间复杂度**: O(h)（递归栈 / 迭代栈）

### C++ 代码 (ACM风格) — 递归 + 迭代

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// ===== 方法一：递归中序（带剪枝）=====
int kthSmallestRecursive(TreeNode* root, int k) {
    int result = -1;
    int count = 0;

    function<void(TreeNode*)> inorder = [&](TreeNode* node) {
        if (!node || count >= k) return;  // 剪枝：已经找到就提前返回

        inorder(node->left);

        count++;
        if (count == k) {
            result = node->val;
            return;  // 找到后不再继续
        }

        inorder(node->right);
    };

    inorder(root);
    return result;
}

// ===== 方法二：迭代中序（栈）=====
int kthSmallestIterative(TreeNode* root, int k) {
    stack<TreeNode*> st;
    TreeNode* cur = root;

    while (cur || !st.empty()) {
        // 一路向左
        while (cur) {
            st.push(cur);
            cur = cur->left;
        }

        cur = st.top(); st.pop();
        k--;
        if (k == 0) return cur->val;

        cur = cur->right;  // 转向右子树
    }

    return -1;  // 不应该到这里
}

int main() {
    // 构建BST：
    //       5
    //      / \
    //     3   6
    //    / \
    //   2   4
    //  /
    // 1
    TreeNode* root = new TreeNode(5);
    root->left = new TreeNode(3);
    root->right = new TreeNode(6);
    root->left->left = new TreeNode(2);
    root->left->right = new TreeNode(4);
    root->left->left->left = new TreeNode(1);

    cout << "第1小(递归): " << kthSmallestRecursive(root, 1) << endl;  // 1
    cout << "第3小(递归): " << kthSmallestRecursive(root, 3) << endl;  // 3
    cout << "第3小(迭代): " << kthSmallestIterative(root, 3) << endl;  // 3
    cout << "第6小(迭代): " << kthSmallestIterative(root, 6) << endl;  // 6

    return 0;
}
```

### 常见错误

- **错误1：中序遍历时计数器递增位置错误**。`count++` 必须在访问节点值的位置（即左子树遍历完之后），而不是在函数入口或右子树之后。否则 `count` 和访问顺序不对应。
- **错误2：递归中未剪枝**。即使已经找到了第 K 小，仍然继续遍历剩余节点。应该检查 `count >= k` 提前返回，避免无效遍历。
- **错误3：迭代写法忘记转向右子树**。`cur = cur->right` 是必须的，否则会死循环在同一节点。

### 面试陷阱

- **陷阱1**：面试官问"如果这棵树频繁插入删除且要频繁查询第 K 小怎么办？"——在节点中添加 `size` 字段记录子树节点数。插入/删除时沿路径更新 size，查询时通过比较左子树的 size 和 k 决定走左边还是右边，O(log n) 时间。这就是顺序统计树（Order Statistic Tree）。
- **陷阱2**：面试官追问"如果 BST 退化成链（最坏 O(n)），如何保证 O(log n)？"——用平衡 BST，如 AVL 树或红黑树。C++ 中 `std::set` 不提供第 K 小查询，但可以通过 `__gnu_pbds::tree` 扩展，或者手写平衡树。

---

## 68. 二叉树的序列化与反序列化 — 题号 297

**难度**: 困难
**标签**: 二叉树、BFS、DFS、设计

### 题目大意

设计算法将二叉树序列化为字符串，并能将字符串反序列化回原二叉树。对格式没有限制，只要能还原即可。

### 思路

**方法一（BFS 层序遍历）**：用队列层序遍历，把节点的值（包括 null）按逗号分隔写入。null 用特殊标记（如 `"#"`）。反序列化时，同样用队列，按层序依次构建节点和它的左右孩子。

**方法二（DFS 前序遍历）**：先序遍历，null 标记为 `"#"`，用逗号分隔。反序列化时，用队列（或索引）依次消费序列化的值，按先序重建。递归写法非常简洁。

两种方法对比：
- BFS：更直观，但序列化字符串通常更长（存储了更多 null）
- DFS：代码更简洁，递归反序列化天然对应先序结构

### 核心知识点

- 序列化本质：将树的结构信息编码为线性序列
- null 节点的标记（`"#"`）是重建结构的关键
- 反序列化需要维护一个"消费指针"（队列或索引）
- `ostringstream` / `istringstream` 处理字符串分割

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(n) — 序列化字符串 + 反序列化时的队列/递归栈

### C++ 代码 (ACM风格) — BFS + DFS 两种写法

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// ==================== 方法一：BFS 层序遍历 ====================
class CodecBFS {
public:
    string serialize(TreeNode* root) {
        if (!root) return "";
        ostringstream oss;
        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            TreeNode* node = q.front(); q.pop();
            if (node) {
                oss << node->val << ",";
                q.push(node->left);
                q.push(node->right);
            } else {
                oss << "#,";  // null标记
            }
        }
        return oss.str();
    }

    TreeNode* deserialize(string data) {
        if (data.empty()) return nullptr;
        istringstream iss(data);
        string token;
        getline(iss, token, ',');
        TreeNode* root = new TreeNode(stoi(token));

        queue<TreeNode*> q;
        q.push(root);

        // 按层序逐个构建左右孩子
        while (!q.empty()) {
            TreeNode* node = q.front(); q.pop();

            // 左孩子
            if (getline(iss, token, ',')) {
                if (token != "#") {
                    node->left = new TreeNode(stoi(token));
                    q.push(node->left);
                }
            }
            // 右孩子
            if (getline(iss, token, ',')) {
                if (token != "#") {
                    node->right = new TreeNode(stoi(token));
                    q.push(node->right);
                }
            }
        }
        return root;
    }
};

// ==================== 方法二：DFS 前序遍历 ====================
class CodecDFS {
public:
    string serialize(TreeNode* root) {
        ostringstream oss;
        serializeDFS(root, oss);
        return oss.str();
    }

    TreeNode* deserialize(string data) {
        istringstream iss(data);
        queue<string> tokens;
        string token;
        while (getline(iss, token, ',')) {
            tokens.push(token);
        }
        return deserializeDFS(tokens);
    }

private:
    void serializeDFS(TreeNode* node, ostringstream& oss) {
        if (!node) {
            oss << "#,";
            return;
        }
        oss << node->val << ",";
        serializeDFS(node->left, oss);
        serializeDFS(node->right, oss);
    }

    // 按先序消费tokens，递归构建
    TreeNode* deserializeDFS(queue<string>& tokens) {
        string token = tokens.front(); tokens.pop();
        if (token == "#") return nullptr;

        TreeNode* root = new TreeNode(stoi(token));
        root->left = deserializeDFS(tokens);   // 先构建左子树
        root->right = deserializeDFS(tokens);  // 再构建右子树
        return root;
    }
};

int main() {
    // 构建测试树：[1,2,3,null,null,4,5]
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->right->left = new TreeNode(4);
    root->right->right = new TreeNode(5);

    // BFS版本
    CodecBFS codecBFS;
    string bfsData = codecBFS.serialize(root);
    cout << "BFS序列化: " << bfsData << endl;
    TreeNode* rootBFS = codecBFS.deserialize(bfsData);
    cout << "BFS反序列化后根值: " << rootBFS->val << endl;

    // DFS版本
    CodecDFS codecDFS;
    string dfsData = codecDFS.serialize(root);
    cout << "DFS序列化: " << dfsData << endl;
    TreeNode* rootDFS = codecDFS.deserialize(dfsData);
    cout << "DFS反序列化后根值: " << rootDFS->val << endl;

    // 验证：再序列化一次应该相同
    cout << "BFS再序列化: " << codecBFS.serialize(rootBFS) << endl;
    cout << "DFS再序列化: " << codecDFS.serialize(rootDFS) << endl;

    return 0;
}
```

### 常见错误

- **错误1：BFS 反序列化时子节点入队顺序错误**。构建一个节点的左孩子后，左孩子应该入队；构建右孩子后，右孩子入队。这和序列化时 push 的顺序一致。如果忘了入队，后续节点的孩子就无法构建。
- **错误2：DFS 反序列化时 tokens 的消费顺序**。先序序列化后，反序列化的消费顺序是"根 → 左子树(递归) → 右子树(递归)"。如果用了全局索引而非队列，索引前进的位置要保证左子树构建完才能构建右子树。用 `queue<string>` 的 pop 方式天然正确。
- **错误3：忘记处理空树**。序列化空树返回空字符串，反序列化时 `data.empty()` 返回 `nullptr`。BFS 需要处理输入为空的情况。

### 面试陷阱

- **陷阱1**：面试官问"BFS 和 DFS 序列化的字符串哪个更短？"——DFS 通常更短，因为 BFS 会存储很多中间层的 null 节点（尤其是完全二叉树中，最后一层所有 null 都要存）。但两者的理论长度都是 O(n)。
- **陷阱2**：面试官追问"如果不允许使用特殊标记（如 #），怎么序列化？"——可以用带长度信息的前序，如 `(size, val, left, right)` 的格式。或者用中序+前序（需要两次遍历且树中无重复元素）、或 BFS 层序 + 每层记录非 null 节点数。

---

## 69. 二叉树展开为链表 — 题号 114

**难度**: 中等
**标签**: 二叉树、DFS、栈

### 题目大意

将二叉树原地展开为一个"链表"：所有节点只有右孩子（`right` 作为链表 next），左孩子置为 `null`。展开后的顺序与前序遍历相同。

### 思路

**方法一（前序遍历 + 重建）**：先前序遍历把所有节点存入 vector，然后依次用 `right` 连接。需要 O(n) 额外空间。

**方法二（递归后序 / 反向前序）**：自底向上处理。对于每个节点：
1. 递归将左右子树展开
2. 把左子树插入到当前节点和右子树之间
3. 把左指针置为 null

具体操作（假设左右子树已展开完成）：
- 找到左子树展开后的最后一个节点（一路向右）
- 把右子树接到那个节点的右边
- 把左子树移到右子树位置
- 左指针置 null

**方法三（栈 / Morris 思想）**：利用"前序的逆"——右-左-根的顺序处理，维护 `prev` 指针记录上一个处理的节点。这样后处理的节点自然把 `right` 指向前一个。

### 核心知识点

- 树的原地操作（指针重连）
- 递归自底向上处理
- 利用前序遍历的特性重新排列
- 找链尾（一路向右）

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(h)（递归栈）/ O(1)（Morris 变体）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// ===== 方法一：递归自底向上 =====
void flatten(TreeNode* root) {
    if (!root) return;

    // 递归展开左右子树
    flatten(root->left);
    flatten(root->right);

    // 保存右子树，因为后面要覆盖掉
    TreeNode* rightSubtree = root->right;

    // 把左子树移到右边
    root->right = root->left;
    root->left = nullptr;

    // 找到当前"右子树"（原左子树）的最后一个节点
    TreeNode* tail = root;
    while (tail->right) {
        tail = tail->right;
    }

    // 把原右子树接在末尾
    tail->right = rightSubtree;
}

// ===== 方法二：栈（显式迭代，无需递归）=====
void flattenIterative(TreeNode* root) {
    if (!root) return;
    stack<TreeNode*> st;
    st.push(root);

    while (!st.empty()) {
        TreeNode* node = st.top(); st.pop();

        // 先压右再压左，保证左先出栈
        if (node->right) st.push(node->right);
        if (node->left) st.push(node->left);

        // 重连：当前节点的right指向栈顶（下一个前序节点）
        if (!st.empty()) {
            node->right = st.top();
        }
        node->left = nullptr;  // 左指针始终置null
    }
}

// 打印展开后的链表（只沿right走）
void printFlattened(TreeNode* head) {
    while (head) {
        cout << head->val;
        if (head->right) cout << " -> ";
        head = head->right;
    }
    cout << endl;
}

int main() {
    // 构建测试树：
    //     1
    //    / \
    //   2   5
    //  / \   \
    // 3   4   6
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(5);
    root->left->left = new TreeNode(3);
    root->left->right = new TreeNode(4);
    root->right->right = new TreeNode(6);

    cout << "递归展开: ";
    flatten(root);
    printFlattened(root);  // 1 -> 2 -> 3 -> 4 -> 5 -> 6

    // 重建树测试迭代方法
    TreeNode* root2 = new TreeNode(1);
    root2->left = new TreeNode(2);
    root2->right = new TreeNode(5);
    root2->left->left = new TreeNode(3);
    root2->left->right = new TreeNode(4);
    root2->right->right = new TreeNode(6);

    cout << "迭代展开: ";
    flattenIterative(root2);
    printFlattened(root2);

    return 0;
}
```

### 常见错误

- **错误1：忘记保存原右子树**。在 `root->right = root->left` 之前，如果不保存原来的 `right`，原右子树就永久丢失了。必须先保存再覆盖。
- **错误2：忘记将 `left` 置为 `null`**。展开后所有节点的左指针都应该是 null。如果忘了置 null，虽然不是功能性的 bug（遍历只沿 right），但不符合题目"原地展开为链表"的要求，面试会扣分。
- **错误3：找尾节点时踩空**。`while (tail->right)` 的前提是 `tail` 非空且至少有一个右节点。如果左子树为空，`root->right = root->left` 会让右指针变为 null，需要确保后续逻辑不依赖 `right` 非空。

### 面试陷阱

- **陷阱1**：面试官问"如果要求展开成按中序（或后序）排列的链表呢？"——核心思想相同，但需要调整"找尾"、"重连"的顺序和方向。比如中序展开，需要对每个节点，把左子树的最右节点和当前节点相连，把当前节点作为左子树的右孩子。难度大很多。
- **陷阱2**：面试官追问"能不能做到真正的 O(1) 空间？"——Morris 遍历可以。在遍历过程中通过线程指针（thread pointer）连接节点，避免使用栈。但代码复杂，面试中不一定要求写出来，说出思想即可。

---

## 70. 实现 Trie (前缀树) — 题号 208

**难度**: 中等
**标签**: Trie、哈希表、设计、字符串

### 题目大意

实现一个 Trie（前缀树/字典树），支持三个操作：
- `insert(word)`：插入单词
- `search(word)`：查找完整单词是否存在
- `startsWith(prefix)`：查找是否有以该前缀开头的单词

### 思路

Trie 的本质是"多叉树"，每个节点有：
1. 一个指向 26 个子节点的指针数组（或 `unordered_map` 来节省空间）
2. 一个 `isEnd` 标志位，表示当前节点是否是某个单词的结尾

插入：顺着单词的每个字母，遇到空子节点就创建，最后在最后一个字母的节点标记 `isEnd = true`。

查找：顺着字母走下去，中途遇空返回 false，走到最后检查 `isEnd`。

前缀匹配：和查找一样，但不需要检查 `isEnd`，只需要走到最后一个字母不遇空即可。

**关于子节点存储方案**：
- 固定 26 大小的数组：快速但耗空间（每个节点 26 个指针）
- `unordered_map<char, TrieNode*>`：节省空间但略慢
- 本题标准实现一般用固定数组

### 核心知识点

- Trie 的数据结构设计（节点 + isEnd 标志）
- 插入时"边走边创建"
- 区分 `search`（完整匹配）和 `startsWith`（前缀匹配）
- 多叉树的遍历

### 复杂度

- **时间复杂度**: O(k) — k 为单词长度（每次插入/查找/前缀匹配）
- **空间复杂度**: O(26^k) 理论最坏，实际 O(总字符数 * 26) 但很多节点共享

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Trie {
private:
    struct TrieNode {
        TrieNode* children[26];  // 26个小写字母
        bool isEnd;

        TrieNode() : isEnd(false) {
            // 所有子指针初始化为nullptr
            for (int i = 0; i < 26; i++) children[i] = nullptr;
        }
    };

    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(string word) {
        TrieNode* cur = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (!cur->children[idx]) {
                cur->children[idx] = new TrieNode();  // 节点不存在就创建
            }
            cur = cur->children[idx];
        }
        cur->isEnd = true;  // 标记单词结尾！容易忘记！
    }

    bool search(string word) {
        TrieNode* cur = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (!cur->children[idx]) return false;  // 路径断了
            cur = cur->children[idx];
        }
        return cur->isEnd;  // 必须检查是否为完整单词
    }

    bool startsWith(string prefix) {
        TrieNode* cur = root;
        for (char ch : prefix) {
            int idx = ch - 'a';
            if (!cur->children[idx]) return false;
            cur = cur->children[idx];
        }
        return true;  // 前缀匹配不需要isEnd
    }
};

int main() {
    Trie trie;
    trie.insert("apple");
    cout << "search apple: " << (trie.search("apple") ? "true" : "false") << endl;    // true
    cout << "search app: " << (trie.search("app") ? "true" : "false") << endl;        // false
    cout << "startsWith app: " << (trie.startsWith("app") ? "true" : "false") << endl; // true
    trie.insert("app");
    cout << "search app (after insert): " << (trie.search("app") ? "true" : "false") << endl; // true

    return 0;
}
```

### 常见错误

- **错误1：插入时忘记标记 `isEnd = true`**。单词插入完后，不标记结尾标志，导致 `search` 永远返回 false。这是 Trie 题目中最常见的错误！
- **错误2：`search` 和 `startsWith` 混淆**。`search` 必须检查 `isEnd`，`startsWith` 不需要。如果把 `startsWith` 写成 `return cur->isEnd`，就会漏掉"前缀是另一个单词的前半部分"的情况。
- **错误3：子节点数组未初始化**。`children[26]` 需要全部初始化为 `nullptr`，否则判断 `!cur->children[idx]` 时遇到野指针会出问题。

### 面试陷阱

- **陷阱1**：面试官问"如果字符集不是 26 个小写字母，而是任意 Unicode 字符怎么办？"——固定数组不行，要用 `unordered_map<char, TrieNode*>`。空间从 O(26^n) 变成实际存储的字符数。
- **陷阱2**：面试官追问"Trie 在工程中有什么实际应用？"——搜索引擎的自动补全（输入前缀→联想候选词）、拼写检查、IP 路由的最长前缀匹配、基因序列比对。Trie 的核心优势是前缀查询 O(k)，比哈希表的 O(k) 在功能上多了"按前缀遍历"的能力。

---

## 71. 全排列 — 题号 46

**难度**: 中等
**标签**: 回溯

### 题目大意

给定一个不含重复数字的数组，返回所有可能的全排列。

示例：`[1,2,3]` → `[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]`

### 思路

**回溯（DFS + 剪枝）**：每次选择一个尚未使用的数字加入当前排列，递归处理剩余数字。用 `used` 数组标记哪些数字已被使用。

回溯模板：
```
void backtrack(当前路径, 选择列表):
    if 路径长度够了:
        加入结果, return
    for 每个可选的数字:
        if 已被使用: continue
        做选择（标记used，加入路径）
        backtrack(路径, 选择列表)
        撤销选择（恢复used，从路径中弹出）  ← 极其重要！
```

**为什么必须恢复现场（pop + used=false）**？因为回溯的核心是"在一次递归调用返回后，状态必须恢复到进入这次调用之前"，否则上一层的其他分支会携带被污染的状态。

### 核心知识点

- 回溯算法框架：选择 → 递归 → 撤销
- `used` 数组标记已使用的元素
- 恢复现场的重要性（不恢复会导致结果遗漏或重复）
- 回溯 vs DFS 的关系：回溯 = DFS + 状态恢复

### 复杂度

- **时间复杂度**: O(n * n!) — n! 种排列，每种复制 O(n)
- **空间复杂度**: O(n) — 递归栈 + used 数组 + 当前路径

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> permute(vector<int>& nums) {
    vector<vector<int>> result;
    vector<int> current;
    vector<bool> used(nums.size(), false);

    function<void()> backtrack = [&]() {
        // 递归终止：路径长度够了
        if (current.size() == nums.size()) {
            result.push_back(current);
            return;
        }

        for (int i = 0; i < nums.size(); i++) {
            if (used[i]) continue;  // 已使用，跳过

            // 做选择
            used[i] = true;
            current.push_back(nums[i]);

            backtrack();  // 递归

            // 撤销选择（恢复现场）
            current.pop_back();
            used[i] = false;
        }
    };

    backtrack();
    return result;
}

int main() {
    vector<int> nums = {1, 2, 3};
    vector<vector<int>> result = permute(nums);

    cout << "全排列:" << endl;
    for (auto& perm : result) {
        cout << "[";
        for (int i = 0; i < perm.size(); i++) {
            cout << perm[i];
            if (i < perm.size() - 1) cout << ",";
        }
        cout << "] ";
    }
    cout << endl;
    cout << "总共 " << result.size() << " 种排列" << endl;  // 6

    return 0;
}
```

### 常见错误

- **错误1：忘记撤销选择（恢复现场）**。`current.pop_back()` 和 `used[i] = false` 缺一不可。如果忘了 pop，路径会不断增长；如果忘了 used=false，某些数字永不会被选到，导致结果大量遗漏。
- **错误2：回溯终止条件写错**。终止条件是 `current.size() == nums.size()`，不是在 for 循环结束后。如果写成循环结束才加入结果，会漏掉所有排列。
- **错误3：used 数组用局部变量的值**。如果在 `backtrack` 内部定义 used，那每次递归都是新的，失去了"标记全局已使用"的功能。必须在 lambda 外层定义。

### 面试陷阱

- **陷阱1**：面试官问"如果数组里有重复数字怎么办（全排列 II，题号 47）？"——需要先排序，然后在回溯中跳过"同一层的前一个相同数字未被使用"的情况。核心是 `if (i > 0 && nums[i] == nums[i-1] && !used[i-1]) continue;`。这个条件用来保证相同数字在结果中的相对顺序不变，从而去重。
- **陷阱2**：面试官追问"不用 used 数组，用 swap 交换法怎么做？"——每次固定一个位置，把当前位置和后面的每个位置交换，然后递归处理后面的。这样不需要额外空间，但不能保证输出顺序，且对于有重复数字的情况处理更复杂。

---

## 72. 子集 — 题号 78

**难度**: 中等
**标签**: 回溯、位运算

### 题目大意

给定一个不含重复元素的数组，返回所有可能的子集（幂集）。

示例：`[1,2,3]` → `[[],[1],[2],[3],[1,2],[1,3],[2,3],[1,2,3]]`

### 思路

**方法一（回溯）**：每个元素有两种选择：选或不选。从 index 0 开始，逐个决定：
```
void backtrack(start):
    加入当前路径到结果
    for i from start to n-1:
        选择 nums[i]
        backtrack(i+1)
        撤销选择
```

注意：和全排列不同，子集回溯不需要 `used` 数组，因为通过 `start` 索引自然保证不重复顺序。

**方法二（位运算枚举）**：对于 n 个元素，共有 2^n 个子集。用 0 到 2^n-1 的二进制位表示每个元素的选/不选，第 i 位为 1 表示选 `nums[i]`。一次循环枚举所有子集。

### 核心知识点

- 回溯选择模型：每个元素选/不选
- `start` 索引避免重复（不同于全排列的 `used` 数组）
- 位运算枚举幂集（n <= 31 时很方便）
- 加入结果的时机：每进入一次回溯函数就加入（不等长度够）

### 复杂度

- **时间复杂度**: O(n * 2^n) — 2^n 个子集，每个复制 O(n)
- **空间复杂度**: O(n) — 当前路径

### C++ 代码 (ACM风格) — 回溯 + 位运算

```cpp
#include <bits/stdc++.h>
using namespace std;

// ===== 方法一：回溯 =====
vector<vector<int>> subsetsBacktrack(vector<int>& nums) {
    vector<vector<int>> result;
    vector<int> current;

    function<void(int)> backtrack = [&](int start) {
        // 每次进入递归就把当前路径加入结果
        // 因为子集可以是任意长度（包括空集）
        result.push_back(current);

        for (int i = start; i < nums.size(); i++) {
            current.push_back(nums[i]);      // 选
            backtrack(i + 1);                // 递归后续元素
            current.pop_back();              // 撤销（恢复现场）
        }
    };

    backtrack(0);
    return result;
}

// ===== 方法二：位运算枚举 =====
vector<vector<int>> subsetsBitmask(vector<int>& nums) {
    int n = nums.size();
    int total = 1 << n;  // 2^n
    vector<vector<int>> result;

    for (int mask = 0; mask < total; mask++) {
        vector<int> subset;
        for (int i = 0; i < n; i++) {
            // 检查第i位是否为1
            if (mask & (1 << i)) {
                subset.push_back(nums[i]);
            }
        }
        result.push_back(subset);
    }

    return result;
}

int main() {
    vector<int> nums = {1, 2, 3};

    cout << "回溯法子集:" << endl;
    auto result1 = subsetsBacktrack(nums);
    for (auto& s : result1) {
        cout << "[";
        for (int i = 0; i < s.size(); i++) {
            cout << s[i];
            if (i < s.size() - 1) cout << ",";
        }
        cout << "] ";
    }
    cout << endl << "共 " << result1.size() << " 个" << endl;  // 8

    cout << "位运算法子集:" << endl;
    auto result2 = subsetsBitmask(nums);
    for (auto& s : result2) {
        cout << "[";
        for (int i = 0; i < s.size(); i++) {
            cout << s[i];
            if (i < s.size() - 1) cout << ",";
        }
        cout << "] ";
    }
    cout << endl << "共 " << result2.size() << " 个" << endl;  // 8

    return 0;
}
```

### 常见错误

- **错误1：回溯中加入结果的时机错误**。子集需要在每次进入回溯时（包括空集）都加入结果，而不是等 `start == n` 才加。前者的写法会自然包含空集和所有中间状态。
- **错误2：用全排列的 used 方式而不是 start 方式**。如果不用 `start` 索引而用 `used` 数组，会导致 `[1,2]` 和 `[2,1]` 都出现（子集中这是同一个集合），结果会有大量重复。
- **错误3：位运算版本的边界**。`1 << n` 当 n=31 时会溢出 `int`（因为 1<<31 在 32 位有符号 int 中是最小负数）。如果 n 可能达到 31，需要用 `1LL << n` 或 `1U << n`。

### 面试陷阱

- **陷阱1**：面试官问"如果数组中有重复数字（子集 II，题号 90），怎么去重？"——先排序，回溯时跳过"当前层不选但前一个相同数字没被选"的情况。条件类似于 `if (i > start && nums[i] == nums[i-1]) continue;`。
- **陷阱2**：面试官追问"位运算版本有什么局限性？"——只能处理 n <= 31（int）或 n <= 63（long long）的情况。如果 n 更大，必须用回溯。但在实际场景中 n 通常不会太大（子集数量本身是 2^n），太大也没有实际意义。

---

## 73. 组合 — 题号 77

**难度**: 中等
**标签**: 回溯

### 题目大意

给定 n 和 k，从 1~n 中选出 k 个数的所有组合。

示例：n=4, k=2 → `[[1,2],[1,3],[1,4],[2,3],[2,4],[3,4]]`

### 思路

**回溯 + 剪枝**：从 start 开始，依次选择数字加入当前路径。当路径长度等于 k 时加入结果。

**关键优化（剪枝）**：遍历时，如果剩余可选数字不够凑齐 k 个，直接终止循环。
- 当前还需要选 `k - current.size()` 个
- 从 `i` 到 `n` 一共有 `n - i + 1` 个可选
- 如果 `n - i + 1 < k - current.size()`，后面都不可行，`break`
- 即循环条件 `i <= n - (k - current.size()) + 1`

这个剪枝在 n 很大、k 很大时能显著减少分支数量。

### 核心知识点

- 组合 = 子集的长度约束版本
- `start` 索引保证顺序（避免重复组合如 [1,2] 和 [2,1]）
- 可行性剪枝（剩余不够 k 个直接终止）
- 组合 vs 排列：组合使用 `start`，排列使用 `used`

### 复杂度

- **时间复杂度**: O(k * C(n,k)) — C(n,k) 种组合，每种复制 O(k)
- **空间复杂度**: O(k) — 递归栈深度 + 当前路径

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> combine(int n, int k) {
    vector<vector<int>> result;
    vector<int> current;

    function<void(int)> backtrack = [&](int start) {
        // 终止条件：已选够k个
        if (current.size() == k) {
            result.push_back(current);
            return;
        }

        // 剪枝：i <= n - (还需选的个数) + 1
        // 如果从i开始，剩下的数不够凑k个，直接不搜了
        int need = k - current.size();
        for (int i = start; i <= n - need + 1; i++) {
            current.push_back(i);
            backtrack(i + 1);
            current.pop_back();  // 恢复现场
        }
    };

    backtrack(1);  // 从1开始，不是从0
    return result;
}

int main() {
    int n = 4, k = 2;
    auto result = combine(n, k);

    cout << "C(" << n << "," << k << ") = " << result.size() << " 种:" << endl;
    for (auto& combo : result) {
        cout << "[";
        for (int i = 0; i < combo.size(); i++) {
            cout << combo[i];
            if (i < combo.size() - 1) cout << ",";
        }
        cout << "] ";
    }
    cout << endl;

    // 测试大n小k的情况，验证剪枝
    n = 10; k = 3;
    result = combine(n, k);
    cout << "C(" << n << "," << k << ") = " << result.size() << " 种" << endl;  // 120

    return 0;
}
```

### 常见错误

- **错误1：`start` 从 0 开始**。题目是 1~n，start 应该从 1 开始。如果从 0 开始会多出 0 这个数，结果错误。
- **错误2：回溯中没写 `start` 递进**。`backtrack(i + 1)` 写成 `backtrack(start + 1)` 会导致一直选同一个数，或写成 `backtrack(start)` 导致重复选同一个。必须是 `i + 1`（下一个位置）。
- **错误3：剪枝条件写反**。`i <= n - need + 1` 中的 `need = k - current.size()`。如果写成了 `i <= k - need + 1` 或忘记 +1，剪枝条件就错了。理解这个公式：剩余至少要 `need` 个数，所以最后一个合法的起始位置是 `n - need + 1`。

### 面试陷阱

- **陷阱1**：面试官问"组合总和系列（39, 40, 216）和本题有什么关系？"——都是组合问题的变体。组合总和允许重复选同一个数（start 不 +1）或有重复元素需要去重。框架完全一样，只是剪枝条件和终止条件不同。掌握本题的框架能解决一整套题。
- **陷阱2**：面试官追问"这个剪枝到底节省了多少时间？"——可以做一个消融实验：不剪枝时，当 n=100, k=50 会生成 C(100,50) 级别的搜索树（虽然实际只输出 C(100,50) 个结果，但会多搜很多无效分支）。有剪枝后，无效分支被大幅提前剪掉，在大量组合题中这是必须的。

---

## 74. 括号生成 — 题号 22

**难度**: 中等
**标签**: 回溯、字符串、DP

### 题目大意

生成所有由 n 对括号组成的有效组合。有效定义为：括号正确配对，即任意位置左括号数 >= 右括号数。

示例：n=3 → `["((()))","(()())","(())()","()(())","()()()"]`

### 思路

**方法一（回溯）**：维护 `left`（还可以加多少个左括号）和 `right`（还可以加多少个右括号）。

约束条件：
1. 当 `left > 0` 时可以加左括号
2. 当 `right > left` 时可以加右括号（保证左括号不少于右括号）
3. 当 `left == 0 && right == 0` 时得到合法解

关键理解：约束 2 是核心——任何时候右括号数不能超过左括号数，所以只有当"剩余的左括号 < 剩余的右括号"（即已用的左括号 > 已用的右括号）时，才能加右括号。

**方法二（DP）**：这种生成类的题通常可以用递推。

dp[n] 的任何一个括号序列可以分解为：`(A)B`，其中 A 和 B 都是合法的括号序列，且 `len(A) + len(B) = 2(n-1)`。

遍历 `i` 从 0 到 n-1，`dp[n]` 由 `"(" + 所有dp[i] + ")" + 所有dp[n-1-i]` 组合而成。

DP 写法适合需要展示多种解法思路的面试场景，但回溯更通用。

### 核心知识点

- 回溯的约束设计（剩余左括号、剩余右括号的规则）
- 合法性判断：已用左括号 >= 已用右括号
- DP 的递推思路：`(A)B` 分解
- `left` 和 `right` 的递减语义

### 复杂度

- **时间复杂度**: O(4^n / sqrt(n)) — 卡特兰数，近似 O(4^n)
- **空间复杂度**: O(n) — 递归栈 + 当前字符串

### C++ 代码 (ACM风格) — 回溯 + DP

```cpp
#include <bits/stdc++.h>
using namespace std;

// ===== 方法一：回溯 =====
vector<string> generateParenthesis(int n) {
    vector<string> result;
    string current;

    // left: 还能加多少左括号, right: 还能加多少右括号
    function<void(int, int)> backtrack = [&](int left, int right) {
        // 递归终止：括号都用完了
        if (left == 0 && right == 0) {
            result.push_back(current);
            return;
        }

        // 加左括号：只要还有剩余
        if (left > 0) {
            current.push_back('(');
            backtrack(left - 1, right);
            current.pop_back();  // 恢复现场
        }

        // 加右括号：剩余的右括号 > 剩余的左括号，说明已用的左>右
        if (right > left) {
            current.push_back(')');
            backtrack(left, right - 1);
            current.pop_back();  // 恢复现场
        }
    };

    backtrack(n, n);
    return result;
}

// ===== 方法二：DP 递推 =====
vector<string> generateParenthesisDP(int n) {
    // dp[i] = 所有i对括号的合法序列
    vector<vector<string>> dp(n + 1);
    dp[0] = {""};  // 0对括号只有空串

    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < i; j++) {
            // dp[i] = "(" + dp[j] + ")" + dp[i-1-j]
            for (const string& inner : dp[j]) {
                for (const string& outer : dp[i - 1 - j]) {
                    dp[i].push_back("(" + inner + ")" + outer);
                }
            }
        }
    }

    return dp[n];
}

int main() {
    int n = 3;

    cout << "回溯法生成 " << n << " 对括号:" << endl;
    auto result1 = generateParenthesis(n);
    for (auto& s : result1) {
        cout << s << " ";
    }
    cout << endl << "共 " << result1.size() << " 个" << endl;  // 5

    cout << "DP法生成 " << n << " 对括号:" << endl;
    auto result2 = generateParenthesisDP(n);
    for (auto& s : result2) {
        cout << s << " ";
    }
    cout << endl << "共 " << result2.size() << " 个" << endl;  // 5

    // 比较两种方法的结果（顺序可能不同但集合相同）
    sort(result1.begin(), result1.end());
    sort(result2.begin(), result2.end());
    cout << "两种方法结果" << (result1 == result2 ? "一致" : "不一致") << endl;

    return 0;
}
```

### 常见错误

- **错误1：合法性判断条件写错**。加右括号的条件是 `right > left`（剩余右括号多 → 已用左括号 > 已用右括号），而不是 `right > 0`。如果只写 `right > 0`，会生成 `"())("` 这种违法序列。
- **错误2：恢复现场时只 pop 了括号但没恢复 count**。在回溯中，`left` 和 `right` 是通过参数传递的（值传递），不需要恢复；但如果用的是成员变量或引用，必须手动恢复 count。用函数参数是最安全的方式。
- **错误3：DP 中 `dp[0]` 未初始化为 `{""}`**。如果 dp[0] 为空，`dp[1] = "(" + "" + ")" + ""` 就无法生成。dp[0] 包含空串是递推的基础。

### 面试陷阱

- **陷阱1**：面试官问"第 n 个卡特兰数是多少？（结果数量）"——卡特兰数公式 C(2n, n) / (n+1)。对于 n=3，结果是 C(6,3)/4 = 20/4 = 5。这可以用于验证算法正确性。
- **陷阱2**：面试官追问"如果要求按字典序输出呢？"——回溯法中先递归左括号再递归右括号，天然保证了字典序（左括号 < 右括号）。DP 法则不能保证。面试官可能会进一步问"为什么回溯天然按字典序？"——因为回溯的搜索顺序决定了输出顺序。

---

## 75. 电话号码的字母组合 — 题号 17

**难度**: 中等
**标签**: 回溯、哈希表、字符串

### 题目大意

给定一个仅包含数字 2-9 的字符串，返回所有它能表示的字母组合（手机九键映射）。2→abc, 3→def, 4→ghi, 5→jkl, 6→mno, 7→pqrs, 8→tuv, 9→wxyz。

示例：`"23"` → `["ad","ae","af","bd","be","bf","cd","ce","cf"]`

### 思路

**回溯（组合型回溯）**：从 `index=0` 开始，处理第 `index` 个数字。获取该数字对应的所有字母，依次尝试每个字母，然后递归处理下一个数字。

这题的回溯和之前的不同在于：不需要 `used` 数组，不需要 `start` 参数跳过已选——因为每个位置是从不同的集合（不同数字对应的字母集）中选，天然不会重复。

回溯框架：
```
void backtrack(index):
    if index == digits.size():
        加入结果, return
    for 字母 in 数字对应的字母集:
        current.push_back(字母)
        backtrack(index + 1)    ← 处理下一个数字
        current.pop_back()
```

### 核心知识点

- 多集合的组合问题（每个位置从不同的候选集中选）
- 回溯的基本框架（index 参数）
- 映射表的设计（`unordered_map` 或字符串数组）
- 注意 7 和 9 对应 4 个字母，其他都是 3 个

### 复杂度

- **时间复杂度**: O(3^m * 4^n) — m 个 3 字母数字 + n 个 4 字母数字
- **空间复杂度**: O(k) — k = 输入数字串长度（递归栈 + 当前字符串）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<string> letterCombinations(string digits) {
    if (digits.empty()) return {};

    // 数字到字母的映射表（0和1占位不用）
    vector<string> mapping = {
        "", "", "abc", "def", "ghi", "jkl",
        "mno", "pqrs", "tuv", "wxyz"
    };

    vector<string> result;
    string current;

    function<void(int)> backtrack = [&](int index) {
        // 递归终止：所有数字都处理完了
        if (index == digits.size()) {
            result.push_back(current);
            return;
        }

        // 获取当前数字对应的字母集合
        int digit = digits[index] - '0';
        const string& letters = mapping[digit];

        for (char ch : letters) {
            current.push_back(ch);       // 选当前字母
            backtrack(index + 1);        // 处理下一个数字
            current.pop_back();          // 撤销选择（恢复现场）
        }
    };

    backtrack(0);
    return result;
}

int main() {
    string digits = "23";
    auto result = letterCombinations(digits);

    cout << "输入 \"" << digits << "\" 的字母组合:" << endl;
    for (auto& s : result) {
        cout << s << " ";
    }
    cout << endl << "共 " << result.size() << " 个" << endl;  // 9

    // 测试空输入
    digits = "";
    result = letterCombinations(digits);
    cout << "空输入结果数: " << result.size() << endl;  // 0

    // 测试包含4字母的数字
    digits = "79";  // pqrs + wxyz，共16种
    result = letterCombinations(digits);
    cout << "输入 \"79\" 共 " << result.size() << " 个组合" << endl;  // 16

    return 0;
}
```

### 常见错误

- **错误1：忘记处理空输入**。`digits` 为空时直接返回 `{}`，而不是 `{""}`。空输入不应该有结果，但如果不做特判，backtrack(0) 会在 `index == 0 == digits.size()` 时加入一个空字符串到结果中，这是错误的。
- **错误2：映射表索引错误**。`mapping` 应该用索引 2-9 存储，0 和 1 留空。如果用 `mapping[digit]` 但 digit 可能为 0 或 1，就会访问越界。可以通过 `if (digit < 2 || digit > 9) continue;` 防御。
- **错误3：把 `index` 参数误写成 `start` 语义**。本题 `index` 表示当前处理到第几个数字，不是一个"选择范围的起点"。递归时传 `index + 1`（处理下一个数字），而不是 `i + 1`。

### 面试陷阱

- **陷阱1**：面试官问"如果要求按字典序输出，当前写法满足吗？"——满足。因为 mapping 中的字母是按字母序排列的，回溯按顺序遍历，生成的自然按字典序（先按第一个字母排，再按第二个排...）。
- **陷阱2**：面试官追问"不用递归，用迭代怎么做？"——用队列（BFS 风格）：初始队列中放 `""`，每读一个数字，将队列中的每个字符串扩展为加上该数字对应的每个字母后的新字符串，放回队列。类似多叉树的层序遍历。迭代写法避免了递归栈溢出的风险，但代码更长一些。

# LeetCode Hot200 刷题文档 — 阶段三：提升（55题）

> 本阶段涵盖 **网格搜索、回溯、贪心、DP、单调栈、堆/快速选择** 六大核心模块，是机试从"能做"到"做快"的关键分水岭。

---

## 76. 岛屿数量 (Number of Islands) — 题号 200

**难度**: 中等
**标签**: DFS、BFS、并查集、网格

### 题目大意

给定一个 `m x n` 的二维网格 `grid`，`'1'` 表示陆地，`'0'` 表示水域。岛屿由相邻的陆地（水平或垂直方向）组成。求岛屿的数量。

### 思路

- **暴力/直觉**: 遍历每个格子，遇到陆地就计数+1，然后把与之相连的所有陆地"标记"掉（防止重复计数）。
- **为什么DFS/BFS**: 这是一个典型的连通分量计数问题。网格中的连通性天然适合图遍历。
- **沉岛法（Flood Fill）**: 用 DFS/BFS 将遍历过的陆地直接修改为 `'0'`（沉没），不用额外 `visited` 数组，空间 O(1)。
- **并查集视角**: 每个陆地是一个节点，相邻陆地 `union` 在一起，最后统计独立集合数。

### 核心知识点

- 网格 DFS 四方向模板：`int dx[4] = {0,0,1,-1}; int dy[4] = {1,-1,0,0};`
- 沉岛法修改原数组来替代 visited 标记
- BFS 队列写法（先入队再标记，避免重复入队）
- 并查集的二维坐标→一维索引映射：`id = i * n + j`

### 复杂度

- **时间复杂度**: O(m × n)，每个格子访问一次
- **空间复杂度**: DFS 最坏 O(m × n)（蛇形递归栈），沉岛法修改原数组则不需要额外 visited 空间

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// ============ 解法1: DFS 沉岛法 ============
class SolutionDFS {
public:
    int numIslands(vector<vector<char>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int count = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == '1') {
                    count++;
                    dfs(grid, i, j);
                }
            }
        }
        return count;
    }
private:
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    void dfs(vector<vector<char>>& grid, int x, int y) {
        // 越界或遇到水域则返回
        if (x < 0 || x >= grid.size() || y < 0 || y >= grid[0].size()) return;
        if (grid[x][y] != '1') return;
        grid[x][y] = '0';  // 沉岛：标记为已访问
        for (int d = 0; d < 4; d++) {
            dfs(grid, x + dx[d], y + dy[d]);
        }
    }
};

// ============ 解法2: BFS ============
class SolutionBFS {
public:
    int numIslands(vector<vector<char>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int count = 0;
        int dx[4] = {0, 0, 1, -1};
        int dy[4] = {1, -1, 0, 0};
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == '1') {
                    count++;
                    queue<pair<int,int>> q;
                    q.push({i, j});
                    grid[i][j] = '0';  // 入队即标记，防止重复入队
                    while (!q.empty()) {
                        auto [x, y] = q.front(); q.pop();
                        for (int d = 0; d < 4; d++) {
                            int nx = x + dx[d], ny = y + dy[d];
                            if (nx >= 0 && nx < m && ny >= 0 && ny < n && grid[nx][ny] == '1') {
                                q.push({nx, ny});
                                grid[nx][ny] = '0';  // 关键：入队时标记
                            }
                        }
                    }
                }
            }
        }
        return count;
    }
};

// ============ 解法3: 并查集 ============
class UnionFind {
    vector<int> parent, rank;
    int count;  // 独立集合数量
public:
    UnionFind(int n) : parent(n), rank(n, 0), count(n) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int find(int x) {
        // 路径压缩：让所有节点直接指向根
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    }
    void unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return;
        // 按秩合并：小树挂到大树下
        if (rank[rx] < rank[ry]) parent[rx] = ry;
        else if (rank[rx] > rank[ry]) parent[ry] = rx;
        else { parent[ry] = rx; rank[rx]++; }
        count--;  // 每次合并减少一个集合
    }
    int getCount() { return count; }
};

class SolutionUF {
public:
    int numIslands(vector<vector<char>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int total = m * n;
        UnionFind uf(total);
        int water = 0;  // 水域格子数
        int dx[4] = {0, 0, 1, -1};
        int dy[4] = {1, -1, 0, 0};
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == '0') {
                    water++;
                    continue;
                }
                // 只向右、下合并，避免重复合并
                for (int d = 0; d < 2; d++) {
                    int ni = i + dx[d], nj = j + dy[d];
                    if (ni >= 0 && ni < m && nj >= 0 && nj < n && grid[ni][nj] == '1') {
                        uf.unite(i * n + j, ni * n + nj);
                    }
                }
            }
        }
        // 陆地集合数 = 总集合数 - 水域集合数
        return uf.getCount() - water;
    }
};

// ============ 测试 ============
int main() {
    vector<vector<char>> grid = {
        {'1','1','0','0','0'},
        {'1','1','0','0','0'},
        {'0','0','1','0','0'},
        {'0','0','0','1','1'}
    };
    SolutionDFS s1;
    cout << "DFS: " << s1.numIslands(grid) << endl;  // 3

    vector<vector<char>> grid2 = {
        {'1','1','0','0','0'},
        {'1','1','0','0','0'},
        {'0','0','1','0','0'},
        {'0','0','0','1','1'}
    };
    SolutionBFS s2;
    cout << "BFS: " << s2.numIslands(grid2) << endl;

    vector<vector<char>> grid3 = {
        {'1','1','0','0','0'},
        {'1','1','0','0','0'},
        {'0','0','1','0','0'},
        {'0','0','0','1','1'}
    };
    SolutionUF s3;
    cout << "UF:  " << s3.numIslands(grid3) << endl;
    return 0;
}
```

### 常见错误

- **错误1**: BFS 中在 `pop()` 时才标记 visited，导致同一节点被重复入队，严重时 TLE。
- **错误2**: 并查集中直接用 `parent[x] = find(parent[x])` 但忘记调用 `find(x)` 触发路径压缩，导致链式退化。
- **错误3**: 二维坐标转一维时写成 `i * m + j`（应该是 `i * n + j`，`n` 是列数）。

### 面试陷阱

- **陷阱1**: 面试官问"如果网格很大不能修改原数组怎么办？"——用 `visited` 布尔数组，或用 BFS + 哈希表标记。
- **陷阱2**: "如果四个方向变成八个方向（包括对角）？"——修改 `dx/dy` 数组为8个方向即可，但只有四方向时才算一个岛屿，八方向连通性更强。

---

## 77. 岛屿的最大面积 (Max Area of Island) — 题号 695

**难度**: 中等
**标签**: DFS、BFS、网格

### 题目大意

给定 `m x n` 网格，`1` 为陆地，`0` 为水域。求最大的岛屿面积（岛屿是四方向连通的陆地集合）。

### 思路

- 在"岛屿数量"基础上，把"计数"改为"计算面积"。
- DFS 版本：递归函数返回当前格子出发能访问的陆地数量（1 + 四个方向的返回值之和）。
- BFS 版本：统计队列中弹出的元素个数。
- 仍用沉岛法避免 visited 数组。

### 核心知识点

- DFS 返回值累加模式：`return 1 + dfs(x+1,y) + dfs(x-1,y) + dfs(x,y+1) + dfs(x,y-1)`
- 全局最大值维护：每次 DFS 完一个岛屿后 `ans = max(ans, area)`

### 复杂度

- **时间复杂度**: O(m × n)
- **空间复杂度**: O(m × n)（递归栈）或 O(1)（沉岛法）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int ans = 0;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (grid[i][j] == 1)
                    ans = max(ans, dfs(grid, i, j));
        return ans;
    }
private:
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    int dfs(vector<vector<int>>& grid, int x, int y) {
        if (x < 0 || x >= grid.size() || y < 0 || y >= grid[0].size()) return 0;
        if (grid[x][y] != 1) return 0;
        grid[x][y] = 0;  // 沉岛
        int area = 1;
        for (int d = 0; d < 4; d++)
            area += dfs(grid, x + dx[d], y + dy[d]);
        return area;  // 返回本岛屿贡献的面积
    }
};

int main() {
    vector<vector<int>> grid = {
        {0,0,1,0,0,0,0,1,0,0,0,0,0},
        {0,0,0,0,0,0,0,1,1,1,0,0,0},
        {0,1,1,0,1,0,0,0,0,0,0,0,0},
        {0,1,0,0,1,1,0,0,1,0,1,0,0},
        {0,1,0,0,1,1,0,0,1,1,1,0,0},
        {0,0,0,0,0,0,0,0,0,0,1,0,0},
        {0,0,0,0,0,0,0,1,1,1,0,0,0},
        {0,0,0,0,0,0,0,1,1,0,0,0,0}
    };
    Solution s;
    cout << s.maxAreaOfIsland(grid) << endl;  // 6
    return 0;
}
```

### 常见错误

- **错误1**: DFS 返回值忘了加 `1`（当前格子本身），只返回四个方向之和，结果永远为 0 或偏小。
- **错误2**: 在 `if (grid[x][y] != 1) return 0` 之前就累加递归结果，导致无限递归（没有及时终止条件）。

### 面试陷阱

- **陷阱1**: 面试官问"如何不递归做？"——用 BFS 队列，每弹出一个节点面积+1。
- **陷阱2**: "如果网格非常大（如10000×10000）只有少量陆地？"——DFS 递归可能爆栈，此时必须用 BFS 或迭代栈。

---

## 78. 岛屿的周长 (Island Perimeter) — 题号 463

**难度**: 简单
**标签**: DFS、BFS、网格、数学

### 题目大意

网格中恰好有一个岛屿（无湖），求岛屿的周长。

### 思路

- **数学法（最优）**: 遍历每个陆地格子，周长 += 4，如果右边/下边也是陆地则各减 2（因为共享边被重复计算）。
- **DFS 法**: 从唯一陆地出发 DFS，遇到水域或边界就贡献 1 条边。
- 数学法 O(mn) 无需递归，代码最简洁，面试推荐。

### 核心知识点

- 公式：`perimeter = land*4 - adjacent*2`（每对相邻陆地扣除两条边）
- DFS 边界判断：越界 → 贡献 1 条边；遇水 → 贡献 1 条边

### 复杂度

- **时间复杂度**: O(m × n)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法1: 数学法（推荐）
class Solution {
public:
    int islandPerimeter(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int land = 0, adj = 0;  // 陆地数、相邻对数
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 0) continue;
                land++;
                if (i + 1 < m && grid[i+1][j] == 1) adj++;  // 下方相邻
                if (j + 1 < n && grid[i][j+1] == 1) adj++;  // 右方相邻
            }
        }
        return land * 4 - adj * 2;
    }
};

// 解法2: DFS（适合熟悉网格DFS模板）
class SolutionDFS {
public:
    int islandPerimeter(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (grid[i][j] == 1)
                    return dfs(grid, i, j);
        return 0;
    }
private:
    int dfs(vector<vector<int>>& grid, int x, int y) {
        // 越界 → 一条边
        if (x < 0 || x >= grid.size() || y < 0 || y >= grid[0].size()) return 1;
        // 水域 → 一条边
        if (grid[x][y] == 0) return 1;
        // 已访问过的陆地 → 不贡献
        if (grid[x][y] == 2) return 0;
        grid[x][y] = 2;  // 标记已访问
        int dx[4] = {0, 0, 1, -1};
        int dy[4] = {1, -1, 0, 0};
        int peri = 0;
        for (int d = 0; d < 4; d++)
            peri += dfs(grid, x + dx[d], y + dy[d]);
        return peri;
    }
};

int main() {
    vector<vector<int>> grid = {{0,1,0,0},{1,1,1,0},{0,1,0,0},{1,1,0,0}};
    Solution s;
    cout << s.islandPerimeter(grid) << endl;  // 16
    return 0;
}
```

### 常见错误

- **错误1**: 数学法中只检查右方或只检查下方，遗漏一个方向导致少扣边。
- **错误2**: DFS 中把访问过的陆地标为 `0`（和水域混淆了），应该用 `2` 或负值区分。

### 面试陷阱

- **陷阱1**: "如果有多个岛屿呢？"——数学法仍然适用，因为它按格子遍历，不过滤连通分量。
- **陷阱2**: "湖（岛屿内部的水域）算周长吗？"——不算，题目已说无湖；如果有湖，需要改 DFS 对封闭水域的处理。

---

## 79. 图像渲染 (Flood Fill) — 题号 733

**难度**: 简单
**标签**: DFS、BFS、网格

### 题目大意

给定用整数表示颜色的图像，从 `(sr, sc)` 开始，将与该位置颜色相同的所有四方向连通像素替换为 `newColor`。

### 思路

- 标准 Flood Fill，与"岛屿数量"的沉岛法几乎一样。
- 唯一注意：如果 `oldColor == newColor`，直接返回原图（否则无限递归/循环）。

### 核心知识点

- Flood Fill 模板（DFS/BFS）
- 边界条件优先级：越界 > 颜色不匹配 > 已染色

### 复杂度

- **时间复杂度**: O(m × n)
- **空间复杂度**: O(m × n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int newColor) {
        int oldColor = image[sr][sc];
        if (oldColor != newColor)  // 关键：否则无限递归
            dfs(image, sr, sc, oldColor, newColor);
        return image;
    }
private:
    void dfs(vector<vector<int>>& image, int x, int y, int oldColor, int newColor) {
        if (x < 0 || x >= image.size() || y < 0 || y >= image[0].size()) return;
        if (image[x][y] != oldColor) return;
        image[x][y] = newColor;
        int dx[4] = {0, 0, 1, -1};
        int dy[4] = {1, -1, 0, 0};
        for (int d = 0; d < 4; d++)
            dfs(image, x + dx[d], y + dy[d], oldColor, newColor);
    }
};

int main() {
    vector<vector<int>> image = {{1,1,1},{1,1,0},{1,0,1}};
    Solution s;
    auto res = s.floodFill(image, 1, 1, 2);
    for (auto& row : res) {
        for (int v : row) cout << v << " ";
        cout << endl;
    }
    return 0;
}
```

### 常见错误

- **错误1**: 忘记 `oldColor == newColor` 特判，导致两个颜色反复互相染，死循环。
- **错误2**: BFS 实现时在弹出时才染色（而非入队时），导致重复入队超时。

### 面试陷阱

- **陷阱1**: "DFS 和 BFS 哪个更好？"——此题一样，但如果图片很大且填充区域呈长条状，DFS 可能爆栈（递归深度过大），BFS 更安全。

---

## 80. 被围绕的区域 (Surrounded Regions) — 题号 130

**难度**: 中等
**标签**: DFS、BFS、并查集、网格

### 题目大意

给定 `m x n` 矩阵，`'X'` 和 `'O'`。将所有被 `'X'` 包围的 `'O'` 替换为 `'X'`。边界上的 `'O'` 以及与边界 `'O'` 连通的 `'O'` 不被包围，保持不变。

### 思路

- **正向思维（困难）**: 直接找被包围的 `'O'` 很难，因为需要判断"是否与边界连通"。
- **逆向思维（巧妙）**: 从边界出发，找到所有"没有被包围的 `'O'`"（即与边界连通的 `'O'`），标记为特殊字符（如 `'#'`），然后：
  - `'#'` → `'O'`（恢复）
  - `'O'` → `'X'`（被包围的）
- **并查集**: 增加一个虚拟节点作为"边界守护者"，所有边界 `'O'` 与它 union，最后遍历：与守护者不连通的 `'O'` 就是被包围的。

### 核心知识点

- 逆向思维：找"没被包围的"比"被包围的"容易
- 边界出发的 DFS/BFS
- 三阶段染色法：标记 → 替换 → 恢复

### 复杂度

- **时间复杂度**: O(m × n)
- **空间复杂度**: O(m × n)（递归栈）或 O(1)（修改原数组）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    void solve(vector<vector<char>>& board) {
        int m = board.size(), n = board[0].size();
        // 从四个边界出发 DFS，标记与边界连通的 'O' 为 '#'
        for (int i = 0; i < m; i++) {
            dfs(board, i, 0);       // 左边界
            dfs(board, i, n - 1);   // 右边界
        }
        for (int j = 0; j < n; j++) {
            dfs(board, 0, j);       // 上边界
            dfs(board, m - 1, j);   // 下边界
        }
        // 替换
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == '#') board[i][j] = 'O';      // 恢复
                else if (board[i][j] == 'O') board[i][j] = 'X'; // 吃掉
            }
        }
    }
private:
    void dfs(vector<vector<char>>& board, int x, int y) {
        if (x < 0 || x >= board.size() || y < 0 || y >= board[0].size()) return;
        if (board[x][y] != 'O') return;
        board[x][y] = '#';  // 标记：与边界连通
        int dx[4] = {0, 0, 1, -1};
        int dy[4] = {1, -1, 0, 0};
        for (int d = 0; d < 4; d++)
            dfs(board, x + dx[d], y + dy[d]);
    }
};

int main() {
    vector<vector<char>> board = {
        {'X','X','X','X'},
        {'X','O','O','X'},
        {'X','X','O','X'},
        {'X','O','X','X'}
    };
    Solution s;
    s.solve(board);
    for (auto& row : board) {
        for (char c : row) cout << c << " ";
        cout << endl;
    }
    return 0;
}
```

### 常见错误

- **错误1**: 忘记对四个边都做 DFS，只检查了部分边界。
- **错误2**: 替换顺序错误——先把 `'#'` 恢复成 `'O'`，然后再把 `'O'` 改成 `'X'`，导致刚恢复的也被改掉了。正确做法是互斥判断（`if/else if`）。

### 面试陷阱

- **陷阱1**: "为什么不能直接找被包围的？"——因为判断一个 `'O'` 是否被包围需要确认它是否被 `'X'` 完全环绕，这等价于确认它不连通到边界，正向判断需要对每个 `'O'` 做 DFS，复杂度高且容易出错。
- **陷阱2**: "并查集怎么做？"——设置虚拟节点 `dummy = m*n`，所有边界 `'O'` 与之 union，最后排查：`'O'` 且 `find(i*n+j) != find(dummy)` 的改为 `'X'`。

---

## 81. 腐烂的橘子 (Rotting Oranges) — 题号 994

**难度**: 中等
**标签**: BFS、网格、队列

### 题目大意

网格中 `0` 空格，`1` 新鲜橘子，`2` 腐烂橘子。每分钟腐烂橘子会感染四方向相邻的新鲜橘子。求所有橘子腐烂的最少分钟数，若不可能则返回 -1。

### 思路

- **为什么 BFS**: 这是一个多源最短路径问题，腐烂橘子是源点，传播速度相同（每分钟一层）。标准的层序 BFS（逐层扩散）。
- **过程**: (1) 初始将所有腐烂橘子入队，统计新鲜橘子数 (2) BFS 逐层扩展，每弹出腐烂一个新鲜橘子，`fresh--` (3) 每层结束 `minutes++` (4) 如果最后 `fresh > 0` 返回 -1。

### 核心知识点

- 多源 BFS：多个起点同时入队
- 层序遍历：记录当前队列大小，逐层处理
- 提前终止：新鲜橘子数为 0 即刻返回

### 复杂度

- **时间复杂度**: O(m × n)
- **空间复杂度**: O(m × n)（队列最坏情况）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        queue<pair<int,int>> q;
        int fresh = 0;
        // 初始：收集所有腐烂橘子和新鲜橘子
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 2) q.push({i, j});
                else if (grid[i][j] == 1) fresh++;
            }
        }
        if (fresh == 0) return 0;  // 没有新鲜橘子
        int minutes = 0;
        int dx[4] = {0, 0, 1, -1};
        int dy[4] = {1, -1, 0, 0};
        // 多源 BFS（层序遍历）
        while (!q.empty()) {
            int size = q.size();  // 当前层大小
            bool infected = false;
            for (int i = 0; i < size; i++) {
                auto [x, y] = q.front(); q.pop();
                for (int d = 0; d < 4; d++) {
                    int nx = x + dx[d], ny = y + dy[d];
                    if (nx >= 0 && nx < m && ny >= 0 && ny < n && grid[nx][ny] == 1) {
                        grid[nx][ny] = 2;  // 感染
                        q.push({nx, ny});
                        fresh--;
                        infected = true;
                    }
                }
            }
            if (infected) minutes++;  // 本层有感染才计时
        }
        return fresh == 0 ? minutes : -1;
    }
};

int main() {
    vector<vector<int>> grid = {{2,1,1},{1,1,0},{0,1,1}};
    Solution s;
    cout << s.orangesRotting(grid) << endl;  // 4
    return 0;
}
```

### 常见错误

- **错误1**: 忘记 `fresh == 0` 的初始特判，返回错误的 `minutes`（应当是 0 而非若干层循环后的值）。
- **错误2**: 不区分层地计数 `minutes`（每出队一次就 +1），导致结果远大于正确答案。
- **错误3**: 最后一层结束时也 `minutes++`，但已经没新鲜橘子了——应该用 `infected` 标志位控制。

### 面试陷阱

- **陷阱1**: "如果有多个腐烂橘子起点？"——多源 BFS 天然支持，初始全部入队即可。
- **陷阱2**: "是否可以并行感染？"——这正是 BFS 层序的意义，同一分钟所有腐烂橘子同时扩散。

---

## 82. 单词接龙 (Word Ladder) — 题号 127

**难度**: 困难
**标签**: BFS、哈希表、字符串

### 题目大意

给定 beginWord、endWord 和字典 wordList。每次只能改变一个字母，且中间单词必须在字典中。求从 beginWord 到 endWord 的最短转换序列长度，若不可达返回 0。

### 思路

- **暴力 BFS（对每个单词枚举所有字典单词找邻居）**: 每个单词与整个字典比对，O(L × N²)，N=5000 时超时。
- **优化——虚拟节点法**: 对每个单词的每个位置，用通配符（如 `h*t`）作为虚拟节点。例如 `hit` 与 `h*t`、`*it`、`hi*` 都连通，再通过 `h*t` 与 `hot` 连通。这样 BFS 图节点数 O(NL)，边数 O(NL × 26)进一步可优化。
- **优化——逐位替换法**: 对每个出队单词，逐位替换 a-z，查哈希表是否存在。复杂度 O(N × L × 26)，由于 26 是常数，实际上非常快。
- **双向 BFS**: 从 beginWord 和 endWord 同时 BFS，相遇时返回。将复杂度从 O(b^d) 降到 O(b^(d/2))。
- **为什么是"困难"**: 不仅考 BFS，还考建图策略和双向 BFS 优化；图不是显式给出的，需要自己"造邻居"。

### 核心知识点

- 图建模：单词是节点，相差一个字母的单词之间有边
- 逐位替换建邻居：`for each position: for c = 'a' to 'z': newWord`
- 双向 BFS：两个队列交替扩展，总步数相加
- 预过滤：wordList 转 unordered_set 加速查找

### 复杂度

- **时间复杂度**: O(N × L²)（逐位替换法）或 O(N × L × 26) ≈ O(NL)
- **空间复杂度**: O(N × L)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法1: 单向BFS + 逐位替换
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> dict(wordList.begin(), wordList.end());
        if (!dict.count(endWord)) return 0;  // endWord不在字典中
        queue<string> q;
        q.push(beginWord);
        int steps = 1;  // 序列长度，初始为1
        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; i++) {
                string word = q.front(); q.pop();
                if (word == endWord) return steps;
                // 尝试修改每一位
                for (int j = 0; j < word.size(); j++) {
                    char orig = word[j];
                    for (char c = 'a'; c <= 'z'; c++) {
                        if (c == orig) continue;
                        word[j] = c;
                        if (dict.count(word)) {
                            q.push(word);
                            dict.erase(word);  // 相当于visited，防止重复
                        }
                    }
                    word[j] = orig;  // 还原
                }
            }
            steps++;
        }
        return 0;
    }
};

// 解法2: 双向BFS（面试加分项）
class SolutionBiBFS {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> dict(wordList.begin(), wordList.end());
        if (!dict.count(endWord)) return 0;
        unordered_set<string> q1{beginWord}, q2{endWord};
        int steps = 1;
        while (!q1.empty() && !q2.empty()) {
            // 始终扩展较小的集合（关键优化）
            if (q1.size() > q2.size()) swap(q1, q2);
            unordered_set<string> nextLevel;
            for (string word : q1) {
                for (int j = 0; j < word.size(); j++) {
                    char orig = word[j];
                    for (char c = 'a'; c <= 'z'; c++) {
                        word[j] = c;
                        if (q2.count(word)) return steps + 1;  // 相遇！
                        if (dict.count(word)) {
                            nextLevel.insert(word);
                            dict.erase(word);
                        }
                    }
                    word[j] = orig;
                }
            }
            q1 = move(nextLevel);  // 交换：下一层
            steps++;
        }
        return 0;
    }
};

int main() {
    vector<string> wordList = {"hot","dot","dog","lot","log","cog"};
    Solution s;
    cout << "单向BFS: " << s.ladderLength("hit", "cog", wordList) << endl;  // 5
    SolutionBiBFS s2;
    vector<string> wordList2 = {"hot","dot","dog","lot","log","cog"};
    cout << "双向BFS: " << s2.ladderLength("hit", "cog", wordList2) << endl;  // 5
    return 0;
}
```

### 常见错误

- **错误1**: BFS 中忘记 `dict.erase(word)`（即 visited 标记），导致同一单词重复入队，指数级膨胀 TLE。
- **错误2**: 单向 BFS 在 endWord 不在字典时未提前返回 0，白白遍历。
- **错误3**: 双向 BFS 中 `nextLevel` 用 `vector` 而非 `unordered_set`，导致重复节点无法去重。

### 面试陷阱

- **陷阱1**: "为什么不用 Dijkstra？"——因为每步代价相同（都是1），BFS 即为最短路，不需要 Dijkstra 的优先队列。
- **陷阱2**: "字典很大（百万级）怎么办？"——逐位替换法 O(26L) 仍可行；但如果 L 很大（如100），改考虑双向 BFS + A* 启发式搜索。
- **陷阱3**: "所有单词长度都相同吗？"——是的，题目保证；若不保证，建图方式完全不同。

---

## 83. 打开转盘锁 (Open the Lock) — 题号 752

**难度**: 中等
**标签**: BFS、哈希表、字符串

### 题目大意

一个 4 位圆形密码锁，每位 0-9，每次可以旋转一位（向前或向后）。给定 deadends（不能出现的中间状态）和 target，求最少旋转次数。不可达返回 -1。

### 思路

- 本质就是 BFS 最短路：状态是 4 位数字串，邻居是 ±1 拨动某一位的结果（9 的下一状态是 0，0 的上一状态是 9，即模 10 运算）。
- 用 unordered_set 记录 deadends 和 visited。
- 起点 "0000" 如果在 deadends 中直接返回 -1。

### 核心知识点

- 状态 BFS：字符串作为状态，邻居通过操作生成
- 环处理：`(c - '0' + 1) % 10` 和 `(c - '0' - 1 + 10) % 10`
- 双向 BFS 同样适用

### 复杂度

- **时间复杂度**: O(10^4 × 4 × 2) = O(80000)，状态空间上界 10000
- **空间复杂度**: O(10000)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int openLock(vector<string>& deadends, string target) {
        unordered_set<string> dead(deadends.begin(), deadends.end());
        if (dead.count("0000")) return -1;
        unordered_set<string> visited{"0000"};
        queue<string> q;
        q.push("0000");
        int steps = 0;
        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; i++) {
                string cur = q.front(); q.pop();
                if (cur == target) return steps;
                // 枚举 4 个位置，每个位置 +1 或 -1
                for (int j = 0; j < 4; j++) {
                    for (int delta : {1, -1}) {
                        string nxt = cur;
                        int digit = (nxt[j] - '0' + delta + 10) % 10;  // 环处理
                        nxt[j] = '0' + digit;
                        if (!dead.count(nxt) && !visited.count(nxt)) {
                            visited.insert(nxt);
                            q.push(nxt);
                        }
                    }
                }
            }
            steps++;
        }
        return -1;
    }
};

int main() {
    vector<string> deadends = {"0201","0101","0102","1212","2002"};
    Solution s;
    cout << s.openLock(deadends, "0202") << endl;  // 6
    return 0;
}
```

### 常见错误

- **错误1**: 环处理写成 `(digit + 1) % 10` 和 `digit - 1`，忘记了 `0 - 1` 应该是 `9`（负数取模）。
- **错误2**: 没检查 "0000" 在 deadends 中的情况。
- **错误3**: 从 target 遍历到 deadends 中间无解时，死循环——因为 visited 没正确标记。

### 面试陷阱

- **陷阱1**: "如果锁有 N 位呢？"——状态空间变成 10^N，BFS 可能超空间；面试官在考察双向 BFS 或 A* 搜索。
- **陷阱2**: "如何减少 visited 存储？"——可以四位分别存储，或使用 bitset<10000>。

---

## 84. 最小基因变化 (Minimum Genetic Mutation) — 题号 433

**难度**: 中等
**标签**: BFS、哈希表、字符串

### 题目大意

基因序列是 8 个字符的字符串（字符集 `{'A','C','G','T'}`）。每次只允许改变一个字符，且变化结果必须在 `bank` 中。求从 `start` 到 `end` 的最少变化次数。

### 思路

与"单词接龙"完全同构：更小的字符集（4 个而非 26 个），固定长度 8，bank 是 wordList 的等价物。同样使用逐位替换法 BFS。因为没有 26 个字母，替换代价更小。

### 核心知识点

- BFS 状态搜索（同单词接龙模板）
- 字符集预处理：`char genes[4] = {'A','C','G','T'}`

### 复杂度

- **时间复杂度**: O(N × 8 × 4) = O(32N)
- **空间复杂度**: O(N)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minMutation(string start, string end, vector<string>& bank) {
        unordered_set<string> dict(bank.begin(), bank.end());
        if (!dict.count(end)) return -1;
        queue<string> q;
        q.push(start);
        int steps = 0;
        char genes[4] = {'A', 'C', 'G', 'T'};
        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; i++) {
                string cur = q.front(); q.pop();
                if (cur == end) return steps;
                for (int j = 0; j < 8; j++) {
                    char orig = cur[j];
                    for (char c : genes) {
                        if (c == orig) continue;
                        cur[j] = c;
                        if (dict.count(cur)) {
                            q.push(cur);
                            dict.erase(cur);  // 标记visited
                        }
                    }
                    cur[j] = orig;
                }
            }
            steps++;
        }
        return -1;
    }
};

int main() {
    vector<string> bank = {"AACCGGTA","AACCGCTA","AAACGGTA"};
    Solution s;
    cout << s.minMutation("AACCGGTT", "AAACGGTA", bank) << endl;  // 2
    return 0;
}
```

### 常见错误

- **错误1**: 用 start == end 时就返回 0，但忘记检查 bank 合法性（同单词接龙）。
- **错误2**: genes 数组只写了 3 个字符，漏了某个。

### 面试陷阱

- **陷阱1**: "和单词接龙有什么本质区别？"——字符集更小，但原理相同；采访官可能接着问"如果基因长度可变？"

---

## 85. 单词搜索 (Word Search) — 题号 79

**难度**: 中等
**标签**: DFS、回溯、网格

### 题目大意

给定 `m x n` 字符网格和一个单词 `word`，判断该单词是否存在于网格中（字母必须相邻，同一单元格不能重复使用）。

### 思路

- 这是典型的**回溯**问题：从每个格子出发，尝试匹配 word 的每个字符。
- DFS + 状态恢复（Backtracking）:
  1. 如果当前字符匹配，标记当前格子为已访问（如 `'#'`）
  2. 向四个方向递归搜索 word 的下一个字符
  3. 无论成功与否，**恢复现场**（这是回溯的核心！）
- 剪枝优化：先统计 word 中字符频率与 board 字符频率，若 board 中某字符不够则直接返回 false。

### 核心知识点

- 回溯模板：`标记 → 递归 → 恢复`
- 边界检查 + 字符匹配检查
- 剪枝优化（字符计数法）

### 复杂度

- **时间复杂度**: O(m × n × 4^L)，L 为 word 长度，4^L 为最坏搜索空间
- **空间复杂度**: O(L) 递归栈深度

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool exist(vector<vector<char>>& board, string word) {
        int m = board.size(), n = board[0].size();
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (dfs(board, word, 0, i, j))
                    return true;
        return false;
    }
private:
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    bool dfs(vector<vector<char>>& board, const string& word, int idx, int x, int y) {
        if (idx == word.size()) return true;  // 全部匹配完成
        if (x < 0 || x >= board.size() || y < 0 || y >= board[0].size()) return false;
        if (board[x][y] != word[idx]) return false;
        char tmp = board[x][y];
        board[x][y] = '#';  // 标记已访问
        for (int d = 0; d < 4; d++) {
            if (dfs(board, word, idx + 1, x + dx[d], y + dy[d]))
                return true;
        }
        board[x][y] = tmp;  // 回溯：恢复现场
        return false;
    }
};

int main() {
    vector<vector<char>> board = {
        {'A','B','C','E'},
        {'S','F','C','S'},
        {'A','D','E','E'}
    };
    Solution s;
    cout << s.exist(board, "ABCCED") << endl;  // 1 (true)
    cout << s.exist(board, "SEE") << endl;    // 1
    cout << s.exist(board, "ABCB") << endl;   // 0 (false)
    return 0;
}
```

### 常见错误

- **错误1**: 回溯后忘记恢复 `board[x][y] = tmp`，导致后续搜索路径被破坏。
- **错误2**: 先检查越界再检查字符匹配，但把越界判断放在字符判断之后——可能越界访问。
- **错误3**: 把 `board[x][y] = '#'` 写成 `board[x][y] = '0'` 之类的值，但 board 中有可能本身就包含这个字符，造成误判。

### 面试陷阱

- **陷阱1**: "空格子可以匹配多个字符吗？"——不可以，一个格子只能匹配一个字符，递归匹配下一个字符时位置必须移动到相邻格子。
- **陷阱2**: "如果 board 巨大但 word 很短？"——DFS 没问题；如果 word 极长（如100），DFS 深度太大可能爆栈，需要考虑迭代 DFS 或 BFS。

---

## 86. 单词搜索 II (Word Search II) — 题号 212

**难度**: 困难
**标签**: DFS、回溯、Trie（前缀树）

### 题目大意

给定 `m x n` 字符网格和一个单词列表 `words`，返回所有在网格中存在的单词（字母必须相邻，同一单元格不能重复使用）。

### 思路

- **暴力**: 对每个单词单独 DFS（即题79），单词数 K × O(mn×4^L)，K=30000 时必然 TLE。
- **优化——Trie 剪枝**: 将所有单词建成 Trie，DFS 时同时匹配多个单词的前缀。
  - 如果当前 Trie 节点没有匹配任何单词的前缀，立即回溯（剪枝）。
  - 当到达 Trie 的 `isEnd` 节点时，收集单词。
  - 关键：收集后把 `isEnd` 置 false 并删除叶节点（避免重复收集）。
- **为什么是"困难"**: 需要将回溯 + Trie + 剪枝结合，代码量较大；且需要处理重复单词、删除已找到的单词。

### 核心知识点

- Trie 节点的构建与剪枝删除
- 回溯 + Trie 双向匹配
- 标记清理：找到单词后从 Trie 中删除，避免重复输出和多余搜索

### 复杂度

- **时间复杂度**: O(m × n × 4^L)，L 为最长单词长度；但 Trie 剪枝后实际远小于此
- **空间复杂度**: O(所有单词字符数) Trie 空间

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TrieNode {
    string word = "";  // 到达该节点时保存完整单词，为空表示非单词结尾
    vector<TrieNode*> children;
    TrieNode() : children(26, nullptr) {}
};

class Solution {
    vector<string> ans;
public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        // 1. 建 Trie
        TrieNode* root = buildTrie(words);
        int m = board.size(), n = board[0].size();
        // 2. 从每个格子出发 DFS
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                dfs(board, i, j, root);
        return ans;
    }
private:
    TrieNode* buildTrie(vector<string>& words) {
        TrieNode* root = new TrieNode();
        for (string& w : words) {
            TrieNode* node = root;
            for (char c : w) {
                int idx = c - 'a';
                if (!node->children[idx])
                    node->children[idx] = new TrieNode();
                node = node->children[idx];
            }
            node->word = w;  // 在终点存储单词
        }
        return root;
    }
    void dfs(vector<vector<char>>& board, int x, int y, TrieNode* node) {
        if (x < 0 || x >= board.size() || y < 0 || y >= board[0].size()) return;
        char c = board[x][y];
        if (c == '#' || !node->children[c - 'a']) return;  // 已访问 或 无此前缀
        node = node->children[c - 'a'];
        if (!node->word.empty()) {  // 找到一个单词
            ans.push_back(node->word);
            node->word = "";  // 防止重复收集
        }
        board[x][y] = '#';  // 标记已访问
        int dx[4] = {0, 0, 1, -1};
        int dy[4] = {1, -1, 0, 0};
        for (int d = 0; d < 4; d++)
            dfs(board, x + dx[d], y + dy[d], node);
        board[x][y] = c;  // 回溯恢复
        // 剪枝优化：如果当前Trie节点是叶子（无子节点），可以删除
        // 这里为简洁省略了删除逻辑，面试时可提
    }
};

int main() {
    vector<vector<char>> board = {
        {'o','a','a','n'},
        {'e','t','a','e'},
        {'i','h','k','r'},
        {'i','f','l','v'}
    };
    vector<string> words = {"oath","pea","eat","rain"};
    Solution s;
    auto res = s.findWords(board, words);
    for (auto& w : res) cout << w << " ";  // eat oath
    cout << endl;
    return 0;
}
```

### 常见错误

- **错误1**: Trie 构建时忘记在 `isEnd` 存储完整单词，而是用 bool 标记，导致 DFS 中需要额外复原路径。
- **错误2**: 找到单词后没把 `node->word` 置空，导致同一单词被重复收集。
- **错误3**: char c = board[x][y] 在标记 '#' 之后读取——应该先保存再标记。

### 面试陷阱

- **陷阱1**: "Trie 删除节点怎么实现？"——DFS 返回后检查 node 的子节点是否全为空且自己不是终点，是则可向上删除（post-order 剪枝）。
- **陷阱2**: "如果 words 中有重复单词？"——建 Trie 时去重，或收集时用 set；推荐建 Trie 时去重（若相同单词只存一份）。

---

## 87. 分割回文串 (Palindrome Partitioning) — 题号 131

**难度**: 中等
**标签**: 回溯、字符串、DP

### 题目大意

将字符串 `s` 分割成若干子串，使每个子串都是回文串。返回所有可能的分割方案。

### 思路

- **暴力回溯**: 在 s 的每个位置尝试切割，如果 `s[start..i]` 是回文，则加入当前路径，递归处理 `i+1` 位置。
- **优化——DP 预处理**: 用 DP 预处理 `isPal[l][r]` 表示 `s[l..r]` 是否回文，避免回溯中重复判断。`isPal[l][r] = (s[l]==s[r]) && (r-l<=2 || isPal[l+1][r-1])`。
- **高效写法**: 先 DP 再回溯，回溯 O(2^N × N) 降到 O(2^N)。

### 核心知识点

- 回溯模板：`for i in [start, n): if isPal[start][i]: path.push; dfs(i+1); path.pop`
- 回文 DP 预处理：区间 DP，`dp[i][j] = s[i]==s[j] && (len<=3 || dp[i+1][j-1])`
- 记忆化也可替代 DP：`unordered_map<int,vector<vector<string>>> memo`

### 复杂度

- **时间复杂度**: O(N × 2^N)——每个字符间可选切或不切，2^N 种方案
- **空间复杂度**: O(N²) DP 表 + O(N) 递归栈

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
    vector<vector<string>> ans;
    vector<string> path;
    vector<vector<bool>> isPal;
public:
    vector<vector<string>> partition(string s) {
        int n = s.size();
        // DP 预处理回文
        isPal = vector<vector<bool>>(n, vector<bool>(n, false));
        for (int len = 1; len <= n; len++) {
            for (int l = 0; l + len - 1 < n; l++) {
                int r = l + len - 1;
                if (s[l] == s[r]) {
                    isPal[l][r] = (len <= 3) || isPal[l+1][r-1];
                }
            }
        }
        dfs(s, 0);
        return ans;
    }
    void dfs(string& s, int start) {
        if (start == s.size()) {
            ans.push_back(path);
            return;
        }
        for (int i = start; i < s.size(); i++) {
            if (!isPal[start][i]) continue;
            path.push_back(s.substr(start, i - start + 1));
            dfs(s, i + 1);
            path.pop_back();  // 回溯恢复
        }
    }
};

int main() {
    Solution s;
    auto res = s.partition("aab");
    for (auto& v : res) {
        for (auto& str : v) cout << str << " ";
        cout << endl;
    }
    // [a a b] 和 [aa b]
    return 0;
}
```

### 常见错误

- **错误1**: DP 预处理时 `isPal[l][r]` 的转移方程忘记 `len <= 3` 的特判（因为 `l+1 > r-1` 时访问越界）。
- **错误2**: 回溯中 `substr(start, i - start + 1)` 的第二个参数写错（应该是长度，不是结束位置）。

### 面试陷阱

- **陷阱1**: "如果不预处理 DP 呢？"——每次判断回文 O(N)，回溯 O(2^N × N)，差了 N 倍；但 N ≤ 16 时也可过。
- **陷阱2**: "只求最少分割次数？"——变成 132 题，用 DP 求最小分割，不需要回溯。

---

## 88. N 皇后 (N-Queens) — 题号 51

**难度**: 困难
**标签**: 回溯

### 题目大意

在 N×N 棋盘上放置 N 个皇后，使它们互不攻击（不同行、不同列、不同对角线）。返回所有不同的放置方案。

### 思路

- **直觉**: 每行必须恰好放一个皇后（否则必有某行为空导致某行有多个）。于是我们逐行放置。
- **回溯**: `dfs(row)` 枚举该行的第 `col` 列放置皇后，检查是否与已放置的皇后冲突。
- **冲突判定——O(1) 技巧**:
  - `cols[col]` 标记列
  - `diag1[row + col]` 标记主对角线（左上到右下，特点是 `row+col` 为定值）
  - `diag2[row - col + N - 1]` 标记副对角线（右上到左下，特点是 `row-col` 为定值，加偏移防负）
- **为什么是"困难"**: 需要构造输出格式，对角线的判断技巧是关键，回溯本身不难但代码细节多。

### 核心知识点

- 对角线编号技巧：`row+col` 和 `row-col+N-1` 是 O(1) 判断的核心
- 回溯状态：`cols`、`diag1`、`diag2` 三个布尔数组
- 输出构造：`string(row, '.')` 然后修改 `col` 位置为 `'Q'`

### 复杂度

- **时间复杂度**: O(N!)，但实际上远小于此（对角线剪枝）
- **空间复杂度**: O(N)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
    vector<vector<string>> ans;
    vector<int> queens;  // queens[row] = col
    vector<bool> cols, diag1, diag2;
public:
    vector<vector<string>> solveNQueens(int n) {
        cols.assign(n, false);
        diag1.assign(2 * n - 1, false);
        diag2.assign(2 * n - 1, false);
        queens.assign(n, -1);
        dfs(0, n);
        return ans;
    }
    void dfs(int row, int n) {
        if (row == n) {
            // 构造输出
            vector<string> board;
            for (int i = 0; i < n; i++) {
                string line(n, '.');
                line[queens[i]] = 'Q';
                board.push_back(line);
            }
            ans.push_back(board);
            return;
        }
        for (int col = 0; col < n; col++) {
            int d1 = row + col;           // 主对角线编号
            int d2 = row - col + n - 1;   // 副对角线编号（偏移防负）
            if (cols[col] || diag1[d1] || diag2[d2]) continue;
            queens[row] = col;
            cols[col] = diag1[d1] = diag2[d2] = true;
            dfs(row + 1, n);
            cols[col] = diag1[d1] = diag2[d2] = false;  // 回溯
            queens[row] = -1;
        }
    }
};

int main() {
    Solution s;
    auto res = s.solveNQueens(4);
    for (auto& board : res) {
        for (auto& row : board) cout << row << endl;
        cout << "---" << endl;
    }
    return 0;
}
```

### 常见错误

- **错误1**: 对角线编号 `d2 = row - col + N - 1` 忘记 `-1` 导致数组越界（最大 `row-col = N-1`，需要 `+ N - 1` 保证范围 `[0, 2N-2]`）。
- **错误2**: 恢复现场时只恢复 `cols`，忘记恢复两个对角线标记。
- **错误3**: 输出构造 `line[queens[i]] = 'Q'` 的下标用错。

### 面试陷阱

- **陷阱1**: "如何优化到 O(1) 额外空间？"——用位运算（bit mask），每列/对角线用 int 的一位表示。
- **陷阱2**: "N-Queens II（计数版）怎么做？"——只需要计数，不需要存储 `queens` 和构造 board，return 计数即可。

---

## 89. 解数独 (Sudoku Solver) — 题号 37

**难度**: 困难
**标签**: 回溯、哈希表

### 题目大意

解 9×9 数独，填充空白格（`'.'`），使每行、每列、每个 3×3 宫包含数字 1-9 各一次。

### 思路

- **暴力回溯**: 逐个空格尝试 1-9，检查是否合法，合法则递归填下一个空格。
- **优化——预处理可用数字**: 用三个 `bool[9][10]` 数组分别记录每行、每列、每宫已使用的数字，O(1) 判断合法性。
- **进一步优化——选最少候选的空格**: 不按顺序填，优先填候选数最少的空格（启发式，类似人类的"唯一选择"策略），大幅减少分支。
- **为什么是"困难"**: 回溯 + 多维约束管理 + 索引映射（`box = i/3*3 + j/3`），且必须找到唯一解。

### 核心知识点

- 宫号映射：`box = (row/3) * 3 + col/3`
- 三个 `bool` 数组的维护
- 回溯提前返回：找到解后停止搜索（通过 bool 返回值）
- 启发式剪枝：最少候选优先（可选）

### 复杂度

- **时间复杂度**: 最坏 O(9^(81))，但实际远小于此（强剪枝）
- **空间复杂度**: O(1)（固定 9×9）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
    bool rows[9][10] = {false};
    bool cols[9][10] = {false};
    bool boxes[9][10] = {false};
public:
    void solveSudoku(vector<vector<char>>& board) {
        // 预处理：记录已有数字
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] != '.') {
                    int num = board[i][j] - '0';
                    rows[i][num] = cols[j][num] = boxes[i/3*3 + j/3][num] = true;
                }
            }
        }
        dfs(board, 0, 0);
    }
    bool dfs(vector<vector<char>>& board, int i, int j) {
        if (i == 9) return true;  // 全部填完
        if (j == 9) return dfs(board, i + 1, 0);  // 换行
        if (board[i][j] != '.') return dfs(board, i, j + 1);  // 已有数字，跳过
        int boxIdx = i / 3 * 3 + j / 3;
        for (int num = 1; num <= 9; num++) {
            if (rows[i][num] || cols[j][num] || boxes[boxIdx][num]) continue;
            // 放置
            board[i][j] = '0' + num;
            rows[i][num] = cols[j][num] = boxes[boxIdx][num] = true;
            if (dfs(board, i, j + 1)) return true;  // 找到解，提前返回
            // 回溯
            board[i][j] = '.';
            rows[i][num] = cols[j][num] = boxes[boxIdx][num] = false;
        }
        return false;  // 1-9 都不行
    }
};

int main() {
    vector<vector<char>> board = {
        {'5','3','.','.','7','.','.','.','.'},
        {'6','.','.','1','9','5','.','.','.'},
        {'.','9','8','.','.','.','.','6','.'},
        {'8','.','.','.','6','.','.','.','3'},
        {'4','.','.','8','.','3','.','.','1'},
        {'7','.','.','.','2','.','.','.','6'},
        {'.','6','.','.','.','.','2','8','.'},
        {'.','.','.','4','1','9','.','.','5'},
        {'.','.','.','.','8','.','.','7','9'}
    };
    Solution s;
    s.solveSudoku(board);
    for (auto& row : board) {
        for (char c : row) cout << c << " ";
        cout << endl;
    }
    return 0;
}
```

### 常见错误

- **错误1**: `boxIdx = i/3*3 + j/3` 写成 `i/3 + j/3`（忘记了 `*3`），导致宫号映射错误。
- **错误2**: 恢复现场时只恢复 `board[i][j] = '.'`，忘记恢复 `rows/cols/boxes` 三个数组。
- **错误3**: dfs 返回 bool 但上层调用时没检查返回值，导致找到解后继续回溯覆盖了结果。

### 面试陷阱

- **陷阱1**: "为什么不用迭代而用递归？"——数独的搜索空间天然适合递归回溯；迭代需要手动维护栈，代码复杂且无优势。
- **陷阱2**: "如果数独有多解怎么办？"——标准的做法是去重输出所有解（不提前 return true），但本题保证唯一解。

---

## 90. 组合总和 (Combination Sum) — 题号 39

**难度**: 中等
**标签**: 回溯、数组

### 题目大意

给定无重复元素的数组 `candidates` 和目标值 `target`，找出所有可以使数字和为 target 的组合。candidates 中的数字可以无限重复使用。

### 思路

- 标准回溯，但有**无限重复使用**的限制。
- 关键技巧：`dfs(i, remain)` — i 控制起始索引避免重复组合（如 `[2,3]` 和 `[3,2]` 视为同一组合）。
- 因为可重复使用，递归时索引**不变**（即 `i` 而非 `i+1`），继续从同一元素开始尝试。
- 剪枝：数组排序后，如果 `candidates[i] > remain` 则 break（因为后面更大）。

### 核心知识点

- 去重策略：索引递增限制
- 可重复使用：递归时索引不走 +1
- 排序剪枝：排序后 `candidates[i] > remain` 即可 break

### 复杂度

- **时间复杂度**: O(2^(T))，T 为 target 最小值归一化后的大小，实际远小于此
- **空间复杂度**: O(T/min) 递归深度

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
    vector<vector<int>> ans;
    vector<int> path;
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());  // 排序为了剪枝
        dfs(candidates, target, 0);
        return ans;
    }
    void dfs(vector<int>& nums, int remain, int start) {
        if (remain == 0) {
            ans.push_back(path);
            return;
        }
        for (int i = start; i < nums.size(); i++) {
            if (nums[i] > remain) break;  // 剪枝：后面都不用试了
            path.push_back(nums[i]);
            dfs(nums, remain - nums[i], i);  // 注意: i 不是 i+1，因为可以重复使用
            path.pop_back();
        }
    }
};

int main() {
    vector<int> cand = {2,3,6,7};
    Solution s;
    auto res = s.combinationSum(cand, 7);
    for (auto& v : res) {
        for (int x : v) cout << x << " ";
        cout << endl;
    }
    // [2 2 3] 和 [7]
    return 0;
}
```

### 常见错误

- **错误1**: `dfs(nums, remain - nums[i], i)` 写成 `i+1`，导致每个数只能用一次（变成组合总和 II 了）。
- **错误2**: 忘记排序导致无法有效剪枝。
- **错误3**: 把剪枝条件写成 `nums[i] >= remain`（应该是 `>`，因为等于 remain 时还需要加入 path）。

### 面试陷阱

- **陷阱1**: "如果数据量极大？"——排序 + 剪枝仍然可能超时；考虑 DP（完全背包）或 meet-in-the-middle。
- **陷阱2**: "如何按字典序输出？"——对 candidates 排序后回溯，自然就是字典序。

---

## 91. 组合总和 II (Combination Sum II) — 题号 40

**难度**: 中等
**标签**: 回溯、数组

### 题目大意

给定可能含重复数字的 `candidates`，每个数字**只能使用一次**，找出所有和为 target 的组合（不能有重复组合）。

### 思路

- 与题39的区别：(1) 每个数只能用一次 (2) candidates 有重复数字 (3) 组合不能重复。
- **同层去重法**: `if (i > start && nums[i] == nums[i-1]) continue;` 即同一层递归中，相同数字只选第一个，跳过后续重复的。
- 递归时 `dfs(nums, remain - nums[i], i + 1)` — 每个数只能用一次，所以 `i+1`。

### 核心知识点

- 同层去重：`i > start && nums[i] == nums[i-1]`
- 排序的必要性：去重依赖于排序
- 与题39的区别记忆：重复使用 → `i`；仅一次 → `i+1`

### 复杂度

- **时间复杂度**: O(2^N)
- **空间复杂度**: O(N)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
    vector<vector<int>> ans;
    vector<int> path;
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());
        dfs(candidates, target, 0);
        return ans;
    }
    void dfs(vector<int>& nums, int remain, int start) {
        if (remain == 0) {
            ans.push_back(path);
            return;
        }
        for (int i = start; i < nums.size(); i++) {
            if (nums[i] > remain) break;
            // 同层去重：跳过同一层中重复的数字
            if (i > start && nums[i] == nums[i-1]) continue;
            path.push_back(nums[i]);
            dfs(nums, remain - nums[i], i + 1);  // 注意: i+1，每个数只能用一次
            path.pop_back();
        }
    }
};

int main() {
    vector<int> cand = {10,1,2,7,6,1,5};
    Solution s;
    auto res = s.combinationSum2(cand, 8);
    for (auto& v : res) {
        for (int x : v) cout << x << " ";
        cout << endl;
    }
    // [1 1 6] [1 2 5] [1 7] [2 6]
    return 0;
}
```

### 常见错误

- **错误1**: 去重写成 `nums[i] == nums[i-1]` 而没有 `i > start`，导致跨层重复被错误跳过。
- **错误2**: 混淆与题39的去重差异——题40是同层跳过，题39无需去重（因为数字不重复）。
- **错误3**: 以 `used` 数组去重过于复杂——排序 + 同层跳过足矣。

### 面试陷阱

- **陷阱1**: "为什么不用 set<vector<int>> 去重？"——可以但不优雅，且空间大；排序 + 同层跳过是标准做法。
- **陷阱2**: "如果 candidates 有 0？"——0 的情况需要注意：若 target=0，则空集也是一组解，需考虑是否输出。

---

## 92. 组合总和 III (Combination Sum III) — 题号 216

**难度**: 中等
**标签**: 回溯

### 题目大意

找出所有由 k 个数相加和为 n 的组合，只能使用数字 1-9，每个数字最多用一次。

### 思路

- 搜索空间固定为 1-9，深度固定为 k，标准回溯。
- 剪枝策略：
  - 剩余数字不够填满 k 个：`9 - i + 1 < k - path.size()`
  - 和已经超过 n：`remain < 0`
  - 即使全取最小的也不够：`remain < sum_of_smallest`

### 核心知识点

- 深度剪枝：`剩余数字数量 < 还需要的数量`
- 回溯模板

### 复杂度

- **时间复杂度**: O(C(9, k))，k ≤ 9
- **空间复杂度**: O(k)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
    vector<vector<int>> ans;
    vector<int> path;
public:
    vector<vector<int>> combinationSum3(int k, int n) {
        dfs(1, k, n);
        return ans;
    }
    void dfs(int start, int k, int remain) {
        if (k == 0) {
            if (remain == 0) ans.push_back(path);
            return;
        }
        // 9 - i + 1 >= k  →  i <= 10 - k
        for (int i = start; i <= 9 - k + 1; i++) {
            if (i > remain) break;  // 剪枝：i 已经超过剩余和
            path.push_back(i);
            dfs(i + 1, k - 1, remain - i);
            path.pop_back();
        }
    }
};

int main() {
    Solution s;
    auto res = s.combinationSum3(3, 9);
    for (auto& v : res) {
        for (int x : v) cout << x << " ";
        cout << endl;
    }
    return 0;
}
```

### 常见错误

- **错误1**: 剪枝 `i <= 9 - k + 1` 忘记 `+1`，导致少搜一层。
- **错误2**: `if (i > remain) break` 写成 `if (remain < i) break` 逻辑混乱。

### 面试陷阱

- **陷阱1**: "能否用 DP 做？"——如果只是求方案数（不输出方案），可以 DP（0-1背包）；输出具体方案只能回溯。

---

## 93. 子集 II (Subsets II) — 题号 90

**难度**: 中等
**标签**: 回溯、数组

### 题目大意

给定含重复元素的整数数组，返回所有不重复的子集（幂集）。

### 思路

- 标准子集回溯：每个位置选/不选。但因为有重复元素，需去重。
- **同层去重**: 排序后，`if (i > start && nums[i] == nums[i-1]) continue;` 跳过同一层重复的数字。
- 与组合总和 II 的去重方法一模一样。

### 核心知识点

- 子集回溯模板：`path.push; dfs(i+1); path.pop; dfs(i+1)` 或 for 循环法
- 同层去重（与前几题一致）

### 复杂度

- **时间复杂度**: O(N × 2^N)（每个子集的复制开销）
- **空间复杂度**: O(N)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
    vector<vector<int>> ans;
    vector<int> path;
public:
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        dfs(nums, 0);
        return ans;
    }
    void dfs(vector<int>& nums, int start) {
        ans.push_back(path);  // 每个节点都是一个子集
        for (int i = start; i < nums.size(); i++) {
            if (i > start && nums[i] == nums[i-1]) continue;  // 同层去重
            path.push_back(nums[i]);
            dfs(nums, i + 1);
            path.pop_back();
        }
    }
};

int main() {
    vector<int> nums = {1,2,2};
    Solution s;
    auto res = s.subsetsWithDup(nums);
    for (auto& v : res) {
        cout << "[";
        for (int x : v) cout << x << " ";
        cout << "] ";
    }
    cout << endl;
    return 0;
}
```

### 常见错误

- **错误1**: 忘记排序直接使用同层去重，导致去重失效。
- **错误2**: `ans.push_back(path)` 放在 for 循环后面，少收集了当前节点。
- **错误3**: 使用 `used` 数组来去重，但未正确处理恢复逻辑。

### 面试陷阱

- **陷阱1**: "能不能用位运算？"——如果 nums 无重复可用位运算（2^N 个 mask），有重复时位运算无法去重，必须排序回溯。

---

## 94. 全排列 II (Permutations II) — 题号 47

**难度**: 中等
**标签**: 回溯、哈希表

### 题目大意

给定含重复数字的序列，返回所有不重复的全排列。

### 思路

- 全排列与组合/子集的去重逻辑不同！全排列的搜索在"不同层"使用相同数字是合法的（如 `[1,1,2]` 的两个 1 出现在不同位置），需要的是**同层去重 + 已使用标记**。
- **标准做法**: `used[i]` 标记数字是否在当前路径使用过；排序后 `if (i>0 && nums[i]==nums[i-1] && !used[i-1]) continue;`
  - `!used[i-1]` 意味着当前层中，前一个相同数字刚从路径中移除（即刚回溯完），此时跳过当前数字，避免重复排列。

### 核心知识点

- 全排列回溯模板：`for (i=0..N) if (!used[i]) { push; used[i]=true; dfs; pop; used[i]=false; }`
- 去重条件：`nums[i]==nums[i-1] && !used[i-1]`（注意 `!used[i-1]` 是关键）

### 复杂度

- **时间复杂度**: O(N × N!)
- **空间复杂度**: O(N)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
    vector<vector<int>> ans;
    vector<int> path;
    vector<bool> used;
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        used.assign(nums.size(), false);
        dfs(nums);
        return ans;
    }
    void dfs(vector<int>& nums) {
        if (path.size() == nums.size()) {
            ans.push_back(path);
            return;
        }
        for (int i = 0; i < nums.size(); i++) {
            if (used[i]) continue;
            // 去重：同一层中相同数字只选第一个未使用过的
            if (i > 0 && nums[i] == nums[i-1] && !used[i-1]) continue;
            used[i] = true;
            path.push_back(nums[i]);
            dfs(nums);
            path.pop_back();
            used[i] = false;
        }
    }
};

int main() {
    vector<int> nums = {1,1,2};
    Solution s;
    auto res = s.permuteUnique(nums);
    for (auto& v : res) {
        for (int x : v) cout << x << " ";
        cout << endl;
    }
    return 0;
}
```

### 常见错误

- **错误1**: 去重写成 `used[i-1]` 而非 `!used[i-1]`，导致合法的排列也被跳过。
- **错误2**: 忘记对 `nums` 排序，去重逻辑失效。
- **错误3**: `used[i]` 只恢复 path 而忘记恢复 used 标志（回溯不完整）。

### 面试陷阱

- **陷阱1**: "`!used[i-1]` 和 `used[i-1]` 有什么区别？"——`!used[i-1]` 是同层去重（前一个相同元素刚被撤销），`used[i-1]` 是深度方向不同层去重。——但全排列中深度方向也需要用相同数字，所以必须用 `!used[i-1]`。
- **陷阱2**: "能不能用交换法？"——能，但去重逻辑需额外处理（剪枝），不如 used 数组直观。

---

## 95. 复原 IP 地址 (Restore IP Addresses) — 题号 93

**难度**: 中等
**标签**: 回溯、字符串

### 题目大意

给定只含数字的字符串 `s`，插入 3 个点将其分割为 4 段，每段必须是 [0, 255] 的有效整数且不能有前导零。返回所有可能的 IP 地址。

### 思路

- **回溯**：`dfs(start, dots)` 表示从 start 位置开始，还需要插入 dots 个点。
- 每层从 start 开始截取 1-3 位数字，检查合法性（值 ≤ 255，无前导零除非是单字符 "0"）。
- dots == 0 时检查剩余字符串是否合法，合法则加入答案。
- 剪枝：剩余字符太少或太多——`s.size()-start < dots+1` 或 `> (dots+1)*3`。

### 核心知识点

- 字符串截取 + 合法性校验
- 深度剪枝：剩余长度合理范围
- 前导零判断：`s[start] == '0' && len > 1`

### 复杂度

- **时间复杂度**: O(1) —— 最多 C(3, 11) 种方案（在 11 个位置中选 3 个放点）
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
    vector<string> ans;
public:
    vector<string> restoreIpAddresses(string s) {
        dfs(s, 0, 3, "");
        return ans;
    }
    void dfs(string& s, int start, int dots, string cur) {
        int n = s.size();
        // 剪枝：剩余字符数不在合理范围内
        if (n - start < dots + 1 || n - start > (dots + 1) * 3) return;
        if (dots == 0) {
            string last = s.substr(start);
            if (isValid(last)) {
                ans.push_back(cur + last);
            }
            return;
        }
        for (int len = 1; len <= 3 && start + len <= n; len++) {
            string seg = s.substr(start, len);
            if (!isValid(seg)) break;
            dfs(s, start + len, dots - 1, cur + seg + ".");
        }
    }
    bool isValid(string& seg) {
        if (seg.empty() || seg.size() > 3) return false;
        if (seg[0] == '0' && seg.size() > 1) return false;  // 前导零
        int val = stoi(seg);
        return val >= 0 && val <= 255;
    }
};

int main() {
    Solution s;
    auto res = s.restoreIpAddresses("25525511135");
    for (auto& ip : res) cout << ip << endl;
    // 255.255.11.135 和 255.255.111.35
    return 0;
}
```

### 常见错误

- **错误1**: 前导零判断写成 `seg[0] == '0'` 忽略了单字符 `"0"` 是合法的。
- **错误2**: `stoi` 对超过 int 范围的字符串会抛异常，应先用位数检查防止。
- **错误3**: 剪枝条件写错，导致正确 IP 被剪掉。

### 面试陷阱

- **陷阱1**: "IPv6 版本怎么做？"——格式完全不同（冒号分隔、十六进制、压缩表示），需要重新设计。
- **陷阱2**: "能不能不用递归？"——3 重循环枚举 3 个点的位置，`O(N³)`，N ≤ 12 完全可行。

---

## 96. 跳跃游戏 (Jump Game) — 题号 55

**难度**: 中等
**标签**: 贪心、DP

### 题目大意

给定非负整数数组 `nums`，你最初位于位置 0。每个元素代表你在该位置能跳跃的最大长度。判断是否能到达最后一个位置。

### 思路

- **暴力 DP**: `dp[i]` 表示能否到达 i，转移 `dp[i] = dp[j] && j+nums[j] >= i`，O(N²)。
- **贪心优化**: 维护 `maxReach`（当前能到达的最远位置）。遍历时更新 `maxReach = max(maxReach, i + nums[i])`。如果 `i > maxReach` 说明不可达，返回 false。O(N)。

### 核心知识点

- 贪心维护"可达的最远位置"
- 提前退出：`i > maxReach` 直接返回 false
- DP 与贪心的联系：DP 的前缀最优性质可以转化为贪心

### 复杂度

- **时间复杂度**: O(N)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool canJump(vector<int>& nums) {
        int maxReach = 0;
        for (int i = 0; i < nums.size(); i++) {
            if (i > maxReach) return false;  // 达不到当前位置
            maxReach = max(maxReach, i + nums[i]);
            if (maxReach >= nums.size() - 1) return true;  // 提前返回
        }
        return true;
    }
};

int main() {
    vector<int> nums1 = {2,3,1,1,4};
    vector<int> nums2 = {3,2,1,0,4};
    Solution s;
    cout << s.canJump(nums1) << endl;  // 1
    cout << s.canJump(nums2) << endl;  // 0
    return 0;
}
```

### 常见错误

- **错误1**: 把 `i > maxReach` 写成 `i >= maxReach`，导致最后一个位置被错误标记为不可达。
- **错误2**: 忘记更新 `maxReach` 放在判断之前，导致逻辑混乱。

### 面试陷阱

- **陷阱1**: "至少需要多少步？"——变成跳跃游戏 II（题45），贪心或 BFS。
- **陷阱2**: "DP 怎么优化？"——DP 是 O(N²)，可以用线段树优化到 O(N log N)，但此题贪心 O(N) 更优。

---

## 97. 跳跃游戏 II (Jump Game II) — 题号 45

**难度**: 中等
**标签**: 贪心、BFS、DP

### 题目大意

数组每个元素表示从该位置能跳跃的最大长度。从 0 出发，到达最后一个位置的最少跳跃次数（题目保证可达）。

### 思路

- **贪心（最优）**: 维护 `curEnd`（当前跳跃能到达的最远位置）和 `curFarthest`（本次跳跃范围内再跳一步能到达的最远位置）。当 `i == curEnd` 时，必须跳一步，`jumps++`，`curEnd = curFarthest`。
- **BFS 视角**: 每一跳的范围是一个区间，区间内每个位置又可以拓展新区间。
- **DP 视角**: `dp[i] = min(dp[j] + 1)` 其中 `j + nums[j] ≥ i`，O(N²)，不推荐。

### 核心知识点

- 贪心的"区间跳跃"模型
- `curEnd` 和 `curFarthest` 双指针
- BFS 层序与贪心的等价性

### 复杂度

- **时间复杂度**: O(N)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法1: 贪心
class Solution {
public:
    int jump(vector<int>& nums) {
        int n = nums.size();
        if (n == 1) return 0;
        int jumps = 0, curEnd = 0, curFarthest = 0;
        for (int i = 0; i < n - 1; i++) {  // 注意: i < n-1，因为到达最后位置即可
            curFarthest = max(curFarthest, i + nums[i]);
            if (i == curEnd) {  // 到达当前跳跃的边界，必须跳一步
                jumps++;
                curEnd = curFarthest;
            }
        }
        return jumps;
    }
};

// 解法2: BFS 视角（更直观）
class SolutionBFS {
public:
    int jump(vector<int>& nums) {
        int n = nums.size();
        int jumps = 0, l = 0, r = 0;  // 当前层范围 [l, r]
        while (r < n - 1) {
            int farthest = r;
            for (int i = l; i <= r; i++)
                farthest = max(farthest, i + nums[i]);
            l = r + 1;
            r = farthest;
            jumps++;
        }
        return jumps;
    }
};

int main() {
    vector<int> nums = {2,3,1,1,4};
    Solution s;
    cout << s.jump(nums) << endl;  // 2
    return 0;
}
```

### 常见错误

- **错误1**: for 循环写成 `i < n` 而非 `i < n-1`，导致在最后位置还多跳一步。
- **错误2**: 忘记 n==1 的特判，返回 -1 或垃圾值。
- **错误3**: BFS 版本中 l/r 范围写错，导致重复计算。

### 面试陷阱

- **陷阱1**: "为什么贪心一定是最优？"——因为每一步都最大化可达范围，没有理由少走或多走；可以用数学归纳法证明。
- **陷阱2**: "如果某步 nums[i]=0 怎么办？"——题目保证可达，如果不可达则 i 会超过 maxReach。

---

## 98. 买卖股票的最佳时机 II (Best Time to Buy and Sell Stock II) — 题号 122

**难度**: 中等
**标签**: 贪心、DP

### 题目大意

可以买卖多次（但同一天最多持有一股），求最大总利润。

### 思路

- **贪心**: 只要明天的价格比今天高，今天买入明天卖出就有利可图。累加所有 `max(0, prices[i+1] - prices[i])`。
- **DP**: `buy[i]`/`sell[i]` 表示第 i 天持有/不持有股票的最大利润。但贪心足矣。
- 贪心正确性证明：任何多次买卖的利润都可以分解为单日涨跌的和。

### 核心知识点

- "上涨日"累加法
- 状态机 DP 视角（通用股票问题解法）

### 复杂度

- **时间复杂度**: O(N)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int profit = 0;
        for (int i = 1; i < prices.size(); i++) {
            if (prices[i] > prices[i-1])
                profit += prices[i] - prices[i-1];
        }
        return profit;
    }
};

int main() {
    vector<int> prices = {7,1,5,3,6,4};
    Solution s;
    cout << s.maxProfit(prices) << endl;  // 7
    return 0;
}
```

### 常见错误

- **错误1**: 试图找全局最低买入、最高卖出——这是股票 I 的思路，不适用于可多次买卖的版本。
- **错误2**: 用复杂的状态机 DP 而忽略贪心——面试官会追问为什么不能用贪心。

### 面试陷阱

- **陷阱1**: "如果有交易手续费（题714）？"——贪心不够，必须 DP（手续费改变无套利假设）。
- **陷阱2**: "如何输出完整的买卖序列？"——用 DP 记录每个状态的前驱，回溯输出。

---

## 99. 加油站 (Gas Station) — 题号 134

**难度**: 中等
**标签**: 贪心、数组

### 题目大意

环形路线上有 N 个加油站，`gas[i]` 表示可获得的油量，`cost[i]` 表示从 i 到 i+1 消耗的油量。从某个加油站出发绕一圈，判断是否能完成，返回起点编号，否则 -1。

### 思路

- **暴力**: 模拟每个起点，O(N²)。
- **贪心关键观察**: 如果从 A 出发到不了 B，那么 A 和 B 之间的任何一点也到不了 B（因为从 A 出发经过中间点时油箱剩余 ≥ 0，从中间点直接出发只会更少或相等）。
- **O(N) 贪心**: 遍历每个点，维护 `tank`（当前油箱），如果 `tank < 0` 则清空并把起点设为 `i+1`。最后检查 `totalGas >= totalCost` 是否有解。

### 核心知识点

- 贪心跳过不可达区间
- `totalGas >= totalCost` 作为有解的充要条件
- 每次 `tank < 0` 时重置起点

### 复杂度

- **时间复杂度**: O(N)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        int totalGas = 0, totalCost = 0;
        int tank = 0, start = 0;
        for (int i = 0; i < gas.size(); i++) {
            totalGas += gas[i];
            totalCost += cost[i];
            tank += gas[i] - cost[i];
            if (tank < 0) {  // 从start到不了i+1
                start = i + 1;  // 重置起点
                tank = 0;       // 清空油箱
            }
        }
        return totalGas >= totalCost ? start : -1;
    }
};

int main() {
    vector<int> gas  = {1,2,3,4,5};
    vector<int> cost = {3,4,5,1,2};
    Solution s;
    cout << s.canCompleteCircuit(gas, cost) << endl;  // 3
    return 0;
}
```

### 常见错误

- **错误1**: `if (tank < 0)` 后忘记 `tank = 0`，导致后续计算仍受之前负数影响。
- **错误2**: 只检查局部可行但忘记最后判断 `totalGas >= totalCost`。

### 面试陷阱

- **陷阱1**: "证明贪心正确性。"——反证法：假设最优起点在 `[A+1, B]` 之间，但 A 出发时到 B 的剩余油量 > 0，而中间点出发到 B 的油量更少，矛盾。
- **陷阱2**: "如果有多个可行起点？"——题目保证唯一解，否则都会输出。

---

## 100. 分发糖果 (Candy) — 题号 135

**难度**: 困难
**标签**: 贪心、数组

### 题目大意

N 个孩子排一排，每个孩子有一个评分。至少每人发一颗糖，评分高的孩子必须比相邻孩子糖多。求最少糖果数。

### 思路

- **暴力思路**: 不断调整不满足条件的孩子，可能多轮。
- **贪心—两遍扫描**:
  1. 左→右：如果 `ratings[i] > ratings[i-1]`，`candies[i] = candies[i-1] + 1`，否则 = 1。
  2. 右→左：如果 `ratings[i] > ratings[i+1]`，`candies[i] = max(candies[i], candies[i+1] + 1)`。
- **为什么对**: 左→右保证了"对于左邻居"的条件，右→左用 `max` 合并入了"对于右邻居"的条件，且不破坏已满足的左邻居条件（因为 `max` 只增不减）。
- **为什么是"困难"**: 贪心策略不直观，需要理解"只增不减"的正确性。

### 核心知识点

- 两遍扫描（左→右 + 右→左）
- `max` 合并策略：先满足一个方向约束，扫第二个方向时取 max
- 这种"双向约束"题型常见：如题 238 除自身以外数组的乘积

### 复杂度

- **时间复杂度**: O(N)
- **空间复杂度**: O(N)（可优化到 O(1) 用上坡下坡计数）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int candy(vector<int>& ratings) {
        int n = ratings.size();
        vector<int> candies(n, 1);  // 每人至少1颗
        // 左→右：保证相对于左邻居的规则
        for (int i = 1; i < n; i++) {
            if (ratings[i] > ratings[i-1])
                candies[i] = candies[i-1] + 1;
        }
        // 右→左：保证相对于右邻居的规则，取max
        for (int i = n - 2; i >= 0; i--) {
            if (ratings[i] > ratings[i+1])
                candies[i] = max(candies[i], candies[i+1] + 1);
        }
        return accumulate(candies.begin(), candies.end(), 0);
    }
};

int main() {
    vector<int> ratings = {1,0,2};
    Solution s;
    cout << s.candy(ratings) << endl;  // 5
    return 0;
}
```

### 常见错误

- **错误1**: 第二遍扫描从右到左时忘记 `max`，直接覆盖了第一遍的结果。
- **错误2**: 两遍扫描顺序颠倒——先右后左也是对的，但逻辑需要对称调整。
- **错误3**: 初始值写成 0 而非 1（每人至少一颗糖）。

### 面试陷阱

- **陷阱1**: "O(1) 空间怎么做？"——用递增递减序列计数法：记录上坡长度和下坡长度，下坡长度超过上坡时多给糖果。
- **陷阱2**: "如果是环形排列？"——问题变难，需要两次 O(N) 扫描 + 调整。

---

## 101. 无重叠区间 (Non-overlapping Intervals) — 题号 435

**难度**: 中等
**标签**: 贪心、排序

### 题目大意

给定一组区间，移除最少的区间使得剩余区间互不重叠。返回移除的最小数量。

### 思路

- **转化为等价问题**: 移除最少 = 保留最多 = 经典的"活动选择"问题（选择最多不重叠的区间）。
- **贪心策略**: 按**结束时间**升序排序，然后贪心选择：每次选择最早结束的区间，跳过与它重叠的。
- **为什么按结束时间排序**: 给后面的区间留出更多空间。

### 核心知识点

- 逆向转化：移除最少 = 保留最多
- 活动选择贪心：按结束时间排序
- 重叠判断：`intervals[i][0] < lastEnd`

### 复杂度

- **时间复杂度**: O(N log N)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int eraseOverlapIntervals(vector<vector<int>>& intervals) {
        if (intervals.empty()) return 0;
        // 按结束时间升序
        sort(intervals.begin(), intervals.end(),
             [](auto& a, auto& b) { return a[1] < b[1]; });
        int keep = 1;  // 保留第一个
        int lastEnd = intervals[0][1];
        for (int i = 1; i < intervals.size(); i++) {
            if (intervals[i][0] >= lastEnd) {  // 不重叠
                keep++;
                lastEnd = intervals[i][1];
            }
            // 否则重叠 → 移除（不保留）
        }
        return intervals.size() - keep;
    }
};

int main() {
    vector<vector<int>> intervals = {{1,2},{2,3},{3,4},{1,3}};
    Solution s;
    cout << s.eraseOverlapIntervals(intervals) << endl;  // 1
    return 0;
}
```

### 常见错误

- **错误1**: 按开始时间排序而非结束时间排序——贪心策略失效。
- **错误2**: 重叠判断写成 `>` 而非 `>=`——边界相贴不算重叠。

### 面试陷阱

- **陷阱1**: "为什么不能按区间长度排序？"——短的区间未必不重叠，且不一定最优（反例：一个短区间夹在两个长区间中间）。
- **陷阱2**: "如果需要输出被移除的区间编号？"—排序时保留原索引即可。

---

## 102. 用最少数量的箭引爆气球 (Minimum Number of Arrows to Burst Balloons) — 题号 452

**难度**: 中等
**标签**: 贪心、排序

### 题目大意

气球在水平坐标表示为一个区间 `[xstart, xend]`，垂直方向射箭（x 坐标为某值），箭可以穿透所有重叠的气球。求引爆所有气球的最少箭数。

### 思路

- 本质是"合并重叠区间"问题：只要气球重叠，一箭可全爆。
- **贪心**: 按**结束点**升序排序。贪心射在气球簇的最左侧结束点（即当前簇第一个结束的位置）。
- 过程：维护 `arrowPos = intervals[0][1]`，遍历：若 `intervals[i][0] > arrowPos`（不重叠），则需要新箭。

### 核心知识点

- 按结束坐标排序
- 重叠判断：`start > lastEnd`（注意这里 > 而非 >=，因为边界相切仍可一箭穿）
- 和题435的关系：二者互为镜像（本题是求保留数量，435 是求移除数量）

### 复杂度

- **时间复杂度**: O(N log N)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findMinArrowShots(vector<vector<int>>& points) {
        if (points.empty()) return 0;
        sort(points.begin(), points.end(),
             [](auto& a, auto& b) { return a[1] < b[1]; });
        int arrows = 1;
        long long arrowPos = points[0][1];  // 用 long long 防溢出
        for (int i = 1; i < points.size(); i++) {
            if (points[i][0] > arrowPos) {  // 注意: > 而非 >=
                arrows++;
                arrowPos = points[i][1];
            }
        }
        return arrows;
    }
};

int main() {
    vector<vector<int>> points = {{10,16},{2,8},{1,6},{7,12}};
    Solution s;
    cout << s.findMinArrowShots(points) << endl;  // 2
    return 0;
}
```

### 常见错误

- **错误1**: 重叠判断写成 `points[i][0] >= arrowPos`——边界相切（如 [1,2] 和 [2,3]）仍可一箭双雕。
- **错误2**: `arrowPos` 用 int 可能溢出（虽然实际数据范围内安全，但面试官可能追问）。

### 面试陷阱

- **陷阱1**: "为什么不按开始时间排序？"——与题435同理，按结束时间贪心选射箭点已被证明是最优的。
- **陷阱2**: "和合并区间（题56）的联系？"——合并区间是聚合同一组重叠区间，本题是按聚类数量射箭，本质相同。

---

## 103. 合并区间 (Merge Intervals) — 题号 56

**难度**: 中等
**标签**: 数组、排序

### 题目大意

合并所有重叠的区间。区间 `[1,3]` 和 `[2,6]` 重叠 → `[1,6]`。

### 思路

- **排序 + 贪心合并**:
  1. 按开始时间升序排序
  2. 遍历，维护当前合并区间的 `[curL, curR]`，如果新区间 `l > curR`（不重叠），则把当前区间输出，否则扩展 `curR = max(curR, r)`。
- 为什么按开始时间排序：方便顺序合并；按结束时间则不方便。

### 核心知识点

- 排序 + 线性扫描合并
- 重叠判断与扩展

### 复杂度

- **时间复杂度**: O(N log N)
- **空间复杂度**: O(1)（不算输出）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        if (intervals.empty()) return {};
        sort(intervals.begin(), intervals.end());  // 默认按第一个元素排序
        vector<vector<int>> ans;
        for (auto& it : intervals) {
            if (ans.empty() || it[0] > ans.back()[1]) {
                ans.push_back(it);  // 不重叠，新增
            } else {
                ans.back()[1] = max(ans.back()[1], it[1]);  // 重叠，扩展
            }
        }
        return ans;
    }
};

int main() {
    vector<vector<int>> intervals = {{1,3},{2,6},{8,10},{15,18}};
    Solution s;
    auto res = s.merge(intervals);
    for (auto& it : res)
        cout << "[" << it[0] << "," << it[1] << "] ";
    cout << endl;  // [1,6] [8,10] [15,18]
    return 0;
}
```

### 常见错误

- **错误1**: 忘记排序直接合并——结果错误。
- **错误2**: 合并时只更新右边界但用了 `=` 而非 `max`——例如 `[1,5]` 和 `[2,3]` 合并时错误变为 `[1,3]`。

### 面试陷阱

- **陷阱1**: "如果区间太多放不下内存？"——外部排序（归并）然后流式合并。
- **陷阱2**: "如果是二维区间（矩形）合并？"——一维贪心不适用，需要扫描线算法。

---

## 104. 插入区间 (Insert Interval) — 题号 57

**难度**: 中等
**标签**: 数组、排序

### 题目大意

给定无重叠的区间列表（按起始时间排序）和一个新区间，插入并合并所有重叠区间。

### 思路

- 因为原列表已排序且无重叠，可以分三段处理：
  1. 所有在 newInterval 之前且不重叠的区间直接放入结果
  2. 所有与 newInterval 重叠的区间合并成一个
  3. 所有在 newInterval 之后的区间直接放入结果

### 核心知识点

- 三段式处理
- 合并条件：`it[0] <= newInterval[1] && it[1] >= newInterval[0]`

### 复杂度

- **时间复杂度**: O(N)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
        vector<vector<int>> ans;
        int i = 0, n = intervals.size();
        // 1. 左侧不重叠的区间
        while (i < n && intervals[i][1] < newInterval[0]) {
            ans.push_back(intervals[i++]);
        }
        // 2. 重叠区间合并
        while (i < n && intervals[i][0] <= newInterval[1]) {
            newInterval[0] = min(newInterval[0], intervals[i][0]);
            newInterval[1] = max(newInterval[1], intervals[i][1]);
            i++;
        }
        ans.push_back(newInterval);
        // 3. 右侧不重叠的区间
        while (i < n) {
            ans.push_back(intervals[i++]);
        }
        return ans;
    }
};

int main() {
    vector<vector<int>> intervals = {{1,3},{6,9}};
    vector<int> newInterval = {2,5};
    Solution s;
    auto res = s.insert(intervals, newInterval);
    for (auto& it : res)
        cout << "[" << it[0] << "," << it[1] << "] ";
    cout << endl;  // [1,5] [6,9]
    return 0;
}
```

### 常见错误

- **错误1**: 三段处理边界条件写错——第二段 `<= newInterval[1]` 忘记等号。
- **错误2**: 合并时只扩展 newInterval，忘记原区间的 start 可能更小。

### 面试陷阱

- **陷阱1**: "如果不保证原列表无重叠？"——先做 merge（题56），再做 insert。
- **陷阱2**: "时间复杂度能否 O(log N)？"——可以用二分找到重叠的起始和结束位置，但合并仍需 O(k)，k 为重叠区间数。

---

## 105. 划分字母区间 (Partition Labels) — 题号 763

**难度**: 中等
**标签**: 贪心、哈希表、字符串

### 题目大意

将字符串划分为尽可能多的片段，使同一字母最多出现在一个片段中。返回每个片段的长度。

### 思路

- **贪心**: 先遍历一遍记录每个字符的**最后出现位置**。
- 再遍历，维护 `curEnd`（当前片段的最远边界），`i == curEnd` 时切分。
- 形式化：`curEnd = max(curEnd, last[s[i]])`，当 `i == curEnd` 时切分段。

### 核心知识点

- 预处理最后出现位置
- 贪心扩张片段边界

### 复杂度

- **时间复杂度**: O(N)
- **空间复杂度**: O(26) = O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> partitionLabels(string s) {
        int last[26] = {0};
        for (int i = 0; i < s.size(); i++)
            last[s[i] - 'a'] = i;
        vector<int> ans;
        int curEnd = 0, prevEnd = -1;
        for (int i = 0; i < s.size(); i++) {
            curEnd = max(curEnd, last[s[i] - 'a']);
            if (i == curEnd) {
                ans.push_back(i - prevEnd);
                prevEnd = i;
            }
        }
        return ans;
    }
};

int main() {
    Solution s;
    auto res = s.partitionLabels("ababcbacadefegdehijhklij");
    for (int x : res) cout << x << " ";  // 9 7 8
    cout << endl;
    return 0;
}
```

### 常见错误

- **错误1**: `last` 数组未初始化或忘记记录。
- **错误2**: 切分后忘记更新 `prevEnd`。

### 面试陷阱

- **陷阱1**: "和合并区间的关系？"——每个字符的出现范围是一个区间，找到所有不重叠区间的合并结果——等价于题56。

---

## 106. 打家劫舍 (House Robber) — 题号 198

**难度**: 中等
**标签**: DP

### 题目大意

沿街房屋，不能偷相邻两家，求最大盗窃金额。

### 思路

- **DP 定义**: `dp[i]` = 偷到第 i 家时的最大金额。
- **状态转移**: `dp[i] = max(dp[i-1], dp[i-2] + nums[i])`。
  - `dp[i-1]`：不偷第 i 家。
  - `dp[i-2] + nums[i]`：偷第 i 家（则第 i-1 家不能偷）。
- **空间优化**: 只需要 `prev2` 和 `prev1` 两个变量。

### 核心知识点

- 线性 DP（斐波那契变体）
- 状态定义与选择
- 滚动变量优化

### 复杂度

- **时间复杂度**: O(N)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int rob(vector<int>& nums) {
        if (nums.empty()) return 0;
        int prev2 = 0, prev1 = 0;  // prev2=dp[i-2], prev1=dp[i-1]
        for (int x : nums) {
            int cur = max(prev1, prev2 + x);
            prev2 = prev1;
            prev1 = cur;
        }
        return prev1;
    }
};

int main() {
    vector<int> nums = {2,7,9,3,1};
    Solution s;
    cout << s.rob(nums) << endl;  // 12
    return 0;
}
```

### 常见错误

- **错误1**: `dp[0]` 和 `dp[1]` 边界初始化错误——`dp[0] = nums[0]`，`dp[1] = max(nums[0], nums[1])`。
- **错误2**: 滚动变量更新顺序错误（先更新 `prev2` 还是 `prev1`）。

### 面试陷阱

- **陷阱1**: "如果房屋是环形的？"——打家劫舍 II（题213），分开讨论偷/不偷第一家。
- **陷阱2**: "如果是二叉树？"——打家劫舍 III（题337），树形 DP。

---

## 107. 打家劫舍 II (House Robber II) — 题号 213

**难度**: 中等
**标签**: DP、环形

### 题目大意

房屋是环形的（首尾相连），不能偷相邻两家，求最大金额。

### 思路

- 环形的处理技巧：拆成两个线性问题。
  - 不偷第一家 → 问题退化为 `[1, n-1]` 的线性打家劫舍。
  - 不偷最后一家 → 问题退化为 `[0, n-2]` 的线性打家劫舍。
  - 取两者最大值。
- 注意 n == 1 的边界。

### 核心知识点

- 环形 → 两段线性
- 复用题198的 DP

### 复杂度

- **时间复杂度**: O(N)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
    int robLinear(vector<int>& nums, int l, int r) {
        int prev2 = 0, prev1 = 0;
        for (int i = l; i <= r; i++) {
            int cur = max(prev1, prev2 + nums[i]);
            prev2 = prev1;
            prev1 = cur;
        }
        return prev1;
    }
public:
    int rob(vector<int>& nums) {
        int n = nums.size();
        if (n == 1) return nums[0];
        // 不偷第一家 或 不偷最后一家
        return max(robLinear(nums, 0, n - 2), robLinear(nums, 1, n - 1));
    }
};

int main() {
    vector<int> nums = {2,3,2};
    Solution s;
    cout << s.rob(nums) << endl;  // 3
    return 0;
}
```

### 常见错误

- **错误1**: 忘记 n==1 的特判，`robLinear(nums, 0, -1)` 越界。
- **错误2**: 两段都取 `max` 后直接返回，但没考虑首尾都不偷的情况——首尾都不偷的情况已被覆盖（如偷了第二家也偷了倒数第二家）。

### 面试陷阱

- **陷阱1**: "如果是一个图（树）而非环形？"——树形 DP（题337），`dp[node][0/1]` 表示偷/不偷。
- **陷阱2**: "环中能否有零金额？"——有零不影响 DP，但可能影响最优方案的选择。

---

## 108. 打家劫舍 III (House Robber III) — 题号 337

**难度**: 中等
**标签**: DP、二叉树、DFS

### 题目大意

房屋排成二叉树结构，不能偷直接相连的节点（父子关系），求最大金额。

### 思路

- **树形 DP**: 每个节点返回 `{rob, notRob}` 两个值：
  - `rob = root->val + left.notRob + right.notRob`
  - `notRob = max(left.rob, left.notRob) + max(right.rob, right.notRob)`
- 后序遍历（DFS）自底向上计算。

### 核心知识点

- 树形 DP：每个节点维护"偷/不偷"两个状态
- 后序遍历合并子节点结果

### 复杂度

- **时间复杂度**: O(N)
- **空间复杂度**: O(H)，H 为树高

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
    // 返回 {rob, notRob}
    pair<int,int> dfs(TreeNode* root) {
        if (!root) return {0, 0};
        auto [lRob, lNot] = dfs(root->left);
        auto [rRob, rNot] = dfs(root->right);
        int rob = root->val + lNot + rNot;  // 偷当前，子节点不能偷
        int notRob = max(lRob, lNot) + max(rRob, rNot);  // 不偷当前，子节点可选
        return {rob, notRob};
    }
public:
    int rob(TreeNode* root) {
        auto [r, nr] = dfs(root);
        return max(r, nr);
    }
};

int main() {
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->right = new TreeNode(3);
    root->right->right = new TreeNode(1);
    Solution s;
    cout << s.rob(root) << endl;  // 7
    return 0;
}
```

### 常见错误

- **错误1**: 混淆了偷/不偷的含义：偷当前时子节点必须"不偷"。
- **错误2**: 不偷当前时，子节点可以偷也可以不偷——取 `max` 而非直接取 `rob`。

### 面试陷阱

- **陷阱1**: "如果树很大导致递归爆栈？"——用栈模拟后序遍历，或 BFS + 分层 DP（需要先建分层邻接表）。
- **陷阱2**: "如果加限制条件，比如不能同时偷任何两个相距 ≤ 2 的节点？"——扩展 DP 状态数。

---

## 109. 最长递增子序列 (Longest Increasing Subsequence) — 题号 300

**难度**: 中等
**标签**: DP、二分、贪心

### 题目大意

给定整数数组，求最长严格递增子序列（LIS）的长度。子序列不必连续。

### 思路

- **DP O(N²)**: `dp[i] = max(dp[j] + 1)` 对所有 `j < i` 且 `nums[j] < nums[i]`。
- **贪心 + 二分 O(N log N)**: 维护数组 `tails`，`tails[k]` 表示长度为 `k+1` 的递增子序列的最小末尾值。遍历 `nums[i]`，在 `tails` 中二分查找第一个 ≥ `nums[i]` 的位置并替换；如果 `nums[i]` 大于 `tails` 中所有值，则追加。
- **LIS 输出**: 需要额外数组记录每个位置的前驱。

### 核心知识点

- 贪心维护"最小值结尾"
- `lower_bound` 二分
- patience sorting（耐心排序）视角

### 复杂度

- **时间复杂度**: O(N log N)
- **空间复杂度**: O(N)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法1: DP O(N^2)
class SolutionDP {
public:
    int lengthOfLIS(vector<int>& nums) {
        int n = nums.size(), ans = 1;
        vector<int> dp(n, 1);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (nums[j] < nums[i])
                    dp[i] = max(dp[i], dp[j] + 1);
            }
            ans = max(ans, dp[i]);
        }
        return ans;
    }
};

// 解法2: 贪心 + 二分 O(N log N)
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        vector<int> tails;  // tails[k] = 长度为k+1的IS的最小末尾
        for (int x : nums) {
            auto it = lower_bound(tails.begin(), tails.end(), x);
            if (it == tails.end())
                tails.push_back(x);  // 扩展LIS
            else
                *it = x;  // 更新更小的末尾值
        }
        return tails.size();
    }
};

int main() {
    vector<int> nums = {10,9,2,5,3,7,101,18};
    Solution s;
    cout << s.lengthOfLIS(nums) << endl;  // 4
    return 0;
}
```

### 常见错误

- **错误1**: 二分用 `upper_bound` 而非 `lower_bound`——严格递增需要用 `lower_bound` 替换 ≥ 的。
- **错误2**: 认为 `tails` 数组就是 LIS——`tails` 只是维护了每个长度的最小末尾，不一定是实际 LIS。

### 面试陷阱

- **陷阱1**: "如何输出 LIS 本身？"——需要额外 `pre[i]` 记录前驱索引，不能直接从 `tails` 还原。
- **陷阱2**: "非严格递增呢？"——用 `upper_bound` 替换 > 的位置。

---

## 110. 最长连续递增序列 (Longest Continuous Increasing Subsequence) — 题号 674

**难度**: 简单
**标签**: DP、数组

### 题目大意

找最长连续上升的子数组长度。

### 思路

- 连续 → 无需 DP，直接遍历：遇到 `nums[i] > nums[i-1]` 则 `len++` 否则 `len = 1`。

### 核心知识点

- 滑动窗口
- 与题300的区别：连续 vs 不连续

### 复杂度

- **时间复杂度**: O(N)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findLengthOfLCIS(vector<int>& nums) {
        int ans = 1, cur = 1;
        for (int i = 1; i < nums.size(); i++) {
            if (nums[i] > nums[i-1]) cur++;
            else cur = 1;
            ans = max(ans, cur);
        }
        return ans;
    }
};

int main() {
    vector<int> nums = {1,3,5,4,7};
    Solution s;
    cout << s.findLengthOfLCIS(nums) << endl;  // 3
    return 0;
}
```

### 常见错误

- **错误1**: 忘记 `ans = 1` 的初始值（数组非空时至少为 1）。
- **错误2**: 混淆了与题300——本题是子数组（连续），题300是子序列（不连续）。

### 面试陷阱

- **陷阱1**: "如果要求连续的递增子数组且差值恰好为 1？"——`if (nums[i] == nums[i-1] + 1)`。

---

## 111. 最长公共子序列 (Longest Common Subsequence) — 题号 1143

**难度**: 中等
**标签**: DP、字符串

### 题目大意

给定两个字符串 text1 和 text2，求它们的最长公共子序列（LCS）长度。

### 思路

- **经典二维 DP**: `dp[i][j]` = text1 前 i 个字符和 text2 前 j 个字符的 LCS 长度。
  - 若 `text1[i-1] == text2[j-1]`：`dp[i][j] = dp[i-1][j-1] + 1`
  - 否则：`dp[i][j] = max(dp[i-1][j], dp[i][j-1])`
- 索引从 1 开始避免 -1 处理。

### 核心知识点

- 二维 DP 经典转移方程
- LCS 与编辑距离的联系
- 求 LCS 具体序列：从右下角回溯

### 复杂度

- **时间复杂度**: O(m × n)
- **空间复杂度**: O(m × n)，可优化到 O(min(m,n))

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int longestCommonSubsequence(string text1, string text2) {
        int m = text1.size(), n = text2.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (text1[i-1] == text2[j-1])
                    dp[i][j] = dp[i-1][j-1] + 1;
                else
                    dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            }
        }
        return dp[m][n];
    }
};

int main() {
    Solution s;
    cout << s.longestCommonSubsequence("abcde", "ace") << endl;  // 3
    return 0;
}
```

### 常见错误

- **错误1**: 初始化忘了 `dp[0][j] = 0` 和 `dp[i][0] = 0`（但 vector 默认是 0，不用显式写）。
- **错误2**: 把 `dp[i-1][j-1]` 写成 `dp[i][j]`（还没有赋值的值）。
- **错误3**: 求具体序列时从 `dp[m][n]` 回溯方向错误。

### 面试陷阱

- **陷阱1**: "O(min(m,n)) 空间怎么做？"——用两行交替（滚动数组）。
- **陷阱2**: "求最长公共子串（连续）而非子序列？"——转移方程：`text1[i]==text2[j]` 时 `dp[i][j] = dp[i-1][j-1]+1`；否则 `dp[i][j] = 0`。

---

## 112. 最长重复子数组 (Maximum Length of Repeated Subarray) — 题号 718

**难度**: 中等
**标签**: DP、滑动窗口、二分

### 题目大意

给两个整数数组，求最长的公共连续子数组的长度。

### 思路

- **DP O(m×n)**: `dp[i][j]` = 以 `A[i-1]` 和 `B[j-1]` 结尾的最长公共子数组。
  - 若 `A[i-1] == B[j-1]`：`dp[i][j] = dp[i-1][j-1] + 1`
  - 否则：`dp[i][j] = 0`
- **滑动窗口 O(m×n)**: 将两个数组对齐的不同偏移位置逐一比较。
- **二分 + 哈希 O(N log N)**: 二分长度，用滚动哈希（Rabin-Karp）判断是否存在该长度的公共子数组，更高级。

### 核心知识点

- 子数组的 DP：`dp[i][j] = (A[i]==B[j]) ? dp[i-1][j-1]+1 : 0`
- 注意与 LCS（子序列）的区别：子序列不等时取 max，子数组不等时归 0

### 复杂度

- **时间复杂度**: O(m × n)
- **空间复杂度**: O(m × n)，可优化到 O(min(m,n))

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findLength(vector<int>& A, vector<int>& B) {
        int m = A.size(), n = B.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        int ans = 0;
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (A[i-1] == B[j-1]) {
                    dp[i][j] = dp[i-1][j-1] + 1;
                    ans = max(ans, dp[i][j]);
                }
                // 不相等时 dp[i][j] = 0（默认）
            }
        }
        return ans;
    }
};

int main() {
    vector<int> A = {1,2,3,2,1};
    vector<int> B = {3,2,1,4,7};
    Solution s;
    cout << s.findLength(A, B) << endl;  // 3
    return 0;
}
```

### 常见错误

- **错误1**: 加了 `else dp[i][j] = max(dp[i-1][j], dp[i][j-1])`——这是子序列的转移，子数组不加。
- **错误2**: 滚动优化时方向错误导致覆盖未使用的值。

### 面试陷阱

- **陷阱1**: "二分+哈希的细节？"——用 `unordered_set` 存储长度为 mid 的所有子串的哈希值，两数组取交集。需处理哈希碰撞。
- **陷阱2**: "和最长公共子序列的区别？"——LCS 不要求连续，这里要求连续（子数组）。

---

## 113. 最长回文子串 (Longest Palindromic Substring) — 题号 5

**难度**: 中等
**标签**: DP、双指针、中心扩展

### 题目大意

求字符串 s 中的最长回文子串。

### 思路

- **中心扩展法 (O(N²), O(1))**: 回文串有两种中心：单字符（奇数长度）和双字符间隙（偶数长度）。遍历所有 2N-1 个中心，向两边扩展。
- **DP (O(N²), O(N²))**: `dp[i][j]` 表示 `s[i..j]` 是否回文。`dp[i][j] = s[i]==s[j] && (len<=3 || dp[i+1][j-1])`。
- **Manacher 算法 (O(N))**: 线性时间，但机试很少要求。

### 核心知识点

- 中心扩展：奇偶两种中心
- 区间 DP 转移：`s[i]==s[j] && (len<=3 || dp[i+1][j-1])`
- Manacher 算法了解即可

### 复杂度

- **时间复杂度**: O(N²)（中心扩展/DP）
- **空间复杂度**: O(1)（中心扩展）/ O(N²)（DP）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法1: 中心扩展法（推荐）
class Solution {
public:
    string longestPalindrome(string s) {
        int n = s.size(), start = 0, maxLen = 1;
        for (int i = 0; i < n; i++) {
            // 奇数长度回文
            auto [l1, r1] = expand(s, i, i);
            // 偶数长度回文
            auto [l2, r2] = expand(s, i, i + 1);
            if (r1 - l1 + 1 > maxLen) {
                start = l1; maxLen = r1 - l1 + 1;
            }
            if (r2 - l2 + 1 > maxLen) {
                start = l2; maxLen = r2 - l2 + 1;
            }
        }
        return s.substr(start, maxLen);
    }
    pair<int,int> expand(string& s, int l, int r) {
        while (l >= 0 && r < s.size() && s[l] == s[r]) { l--; r++; }
        return {l + 1, r - 1};  // 回退到合法位置
    }
};

// 解法2: DP
class SolutionDP {
public:
    string longestPalindrome(string s) {
        int n = s.size(), start = 0, maxLen = 1;
        vector<vector<bool>> dp(n, vector<bool>(n, false));
        for (int len = 1; len <= n; len++) {
            for (int i = 0; i + len - 1 < n; i++) {
                int j = i + len - 1;
                if (s[i] == s[j] && (len <= 3 || dp[i+1][j-1])) {
                    dp[i][j] = true;
                    if (len > maxLen) { start = i; maxLen = len; }
                }
            }
        }
        return s.substr(start, maxLen);
    }
};

int main() {
    Solution s;
    cout << s.longestPalindrome("babad") << endl;  // bab 或 aba
    return 0;
}
```

### 常见错误

- **错误1**: 中心扩展循环退出条件写反（先加后比还是先比后加）。
- **错误2**: DP 遍历顺序错误——必须按长度从小到大，因为 `dp[i][j]` 依赖 `dp[i+1][j-1]`。

### 面试陷阱

- **陷阱1**: "O(N) 的 Manacher 算法？"——面试中除非专门问，否则中心扩展 O(N²) 足够，Manacher 细节易写错。
- **陷阱2**: "不是求串而是求个数？"——回文子串数量（题647），中心扩展时每扩展一步就 +1。

---

## 114. 回文子串 (Palindromic Substrings) — 题号 647

**难度**: 中等
**标签**: DP、双指针、中心扩展

### 题目大意

统计字符串中回文子串的总数（不同位置但相同内容算不同）。

### 思路

- 与题113完全相同，只是中心扩展时每扩展一步 `count++`。
- 两种中心（奇数/偶数）都遍历。

### 核心知识点

- 中心扩展计数
- DP 也可：`dp[i][j]` 为 true 时 count++

### 复杂度

- **时间复杂度**: O(N²)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int countSubstrings(string s) {
        int n = s.size(), count = 0;
        for (int i = 0; i < n; i++) {
            count += expand(s, i, i);    // 奇数
            count += expand(s, i, i + 1); // 偶数
        }
        return count;
    }
    int expand(string& s, int l, int r) {
        int cnt = 0;
        while (l >= 0 && r < s.size() && s[l] == s[r]) {
            cnt++; l--; r++;
        }
        return cnt;
    }
};

int main() {
    Solution s;
    cout << s.countSubstrings("aaa") << endl;  // 6
    return 0;
}
```

### 常见错误

- **错误1**: 只数了奇数长度回文，漏了偶数。
- **错误2**: DP 做法中遍历顺序同题113，同样要注意。

### 面试陷阱

- **陷阱1**: "如果字符串极长怎么办？"——O(N²) 可能不够，但回文串最坏有 O(N²) 个，输出不可能低于 O(N²)；如果只求最长（题113），Manacher O(N)。

---

## 115. 最长回文子序列 (Longest Palindromic Subsequence) — 题号 516

**难度**: 中等
**标签**: DP、字符串

### 题目大意

求最长回文子序列（不必连续）的长度。

### 思路

- **区间 DP**: `dp[i][j]` = s[i..j] 的最长回文子序列长度。
  - 若 `s[i] == s[j]`：`dp[i][j] = dp[i+1][j-1] + 2`
  - 否则：`dp[i][j] = max(dp[i+1][j], dp[i][j-1])`
- 与题114区别：题114是子串（连续），本题是子序列（不连续）。
- 与 LCS 的关系：`LPS(s) = LCS(s, reverse(s))`。

### 核心知识点

- 区间 DP：按长度从小到大遍历
- 与回文子串的区别：不连续
- LCS 等价转换

### 复杂度

- **时间复杂度**: O(N²)
- **空间复杂度**: O(N²)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int longestPalindromeSubseq(string s) {
        int n = s.size();
        vector<vector<int>> dp(n, vector<int>(n, 0));
        for (int i = n - 1; i >= 0; i--) {
            dp[i][i] = 1;  // 长度为1的子序列
            for (int j = i + 1; j < n; j++) {
                if (s[i] == s[j])
                    dp[i][j] = dp[i+1][j-1] + 2;
                else
                    dp[i][j] = max(dp[i+1][j], dp[i][j-1]);
            }
        }
        return dp[0][n-1];
    }
};

int main() {
    Solution s;
    cout << s.longestPalindromeSubseq("bbbab") << endl;  // 4
    return 0;
}
```

### 常见错误

- **错误1**: 遍历顺序：i 必须从大到小，因为 `dp[i][j]` 依赖 `dp[i+1][j-1]`。
- **错误2**: 初始 `dp[i][i] = 1` 忘记设置。

### 面试陷阱

- **陷阱1**: "LPS 和 LCS 如何转换？"——`LPS(s) = LCS(s, rev(s))`，这是常见面试追问。
- **陷阱2**: "空间能否 O(N)？"——可以，`i` 从大到小遍历，只需上一行（或上两行）。

---

## 116. 编辑距离 (Edit Distance) — 题号 72

**难度**: 困难
**标签**: DP、字符串

### 题目大意

给定两个单词 word1 和 word2，可以进行插入、删除、替换三种操作，每种操作代价为 1。求将 word1 转换为 word2 的最少操作数。

### 思路

- **经典 DP 转移**:
  - `dp[i][j]` = word1 前 i 个字符变成 word2 前 j 个字符的最少操作数。
  - 若 `word1[i-1] == word2[j-1]`：`dp[i][j] = dp[i-1][j-1]`
  - 否则：
    - 替换：`dp[i-1][j-1] + 1`
    - 删除：`dp[i-1][j] + 1`（删除 word1[i-1]）
    - 插入：`dp[i][j-1] + 1`（在 word1 中插入 word2[j-1]）
    - `dp[i][j] = min(替换, 删除, 插入)`
- **为什么是"困难"**: 三个转移方向容易混淆，边界初始化需要注意；且需要理解"插入"和"删除"的对称性。

### 核心知识点

- 三种操作在 DP 中的体现
- 边界初始化：`dp[i][0] = i`（删除 i 次），`dp[0][j] = j`（插入 j 次）
- 插入/删除的对称性：把 word1 变 word2 的插入 = 把 word2 变 word1 的删除

### 复杂度

- **时间复杂度**: O(m × n)
- **空间复杂度**: O(m × n)，可优化到 O(min(m,n))

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minDistance(string word1, string word2) {
        int m = word1.size(), n = word2.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        // 初始化
        for (int i = 0; i <= m; i++) dp[i][0] = i;  // 删除i次
        for (int j = 0; j <= n; j++) dp[0][j] = j;  // 插入j次
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (word1[i-1] == word2[j-1]) {
                    dp[i][j] = dp[i-1][j-1];  // 字符相同，无需操作
                } else {
                    dp[i][j] = min({dp[i-1][j-1],  // 替换
                                    dp[i-1][j],    // 删除word1[i-1]
                                    dp[i][j-1]}) + 1;  // 插入word2[j-1]
                }
            }
        }
        return dp[m][n];
    }
};

int main() {
    Solution s;
    cout << s.minDistance("horse", "ros") << endl;  // 3
    cout << s.minDistance("intention", "execution") << endl;  // 5
    return 0;
}
```

### 常见错误

- **错误1**: `dp[i-1][j]`（删除）和 `dp[i][j-1]`（插入）混淆方向。
  - 删除：把 word1[0..i-1] 变 word2[0..j-1] = 先删除 word1[i-1]，再把 word1[0..i-2] 变 word2[0..j-1]。
  - 插入：word1 已被看了 i 个，word2 被看了 j-1 个，需要在 word1 末尾插入 word2[j-1]。
- **错误2**: 初始化 `dp[i][0] = 0` 而非 `i`。
- **错误3**: 替换时忘记 +1。

### 面试陷阱

- **陷阱1**: "为什么插入和删除的转移方向刚好对称？"——因为 `dp[i][j-1]` 表示 word1[0..i-1] 已经匹配了 word2[0..j-2]，还需要匹配 word2[j-1]，所以在 word1 末尾插入。对称理解：`dp[i][j] = dp[j][i]`。
- **陷阱2**: "如果操作有不同代价？"——把 +1 改成对应的代价即可，转移逻辑不变。
- **陷阱3**: "能否空间 O(min(m,n))？"——可以，用一维数组倒序更新。

---

## 117. 不同路径 (Unique Paths) — 题号 62

**难度**: 中等
**标签**: DP、组合数学

### 题目大意

m x n 网格，从左上角到右下角，每次只能向右或向下走一步。求不同路径数。

### 思路

- **DP**: `dp[i][j] = dp[i-1][j] + dp[i][j-1]`，边界 `dp[0][*] = 1`, `dp[*][0] = 1`。
- **组合数学**: `C(m+n-2, m-1)`，即总共走 `(m-1)+(n-1)` 步，选择哪 `m-1` 步向下。
- 组合数注意可能溢出，用 long long 逐乘逐除。

### 核心知识点

- 经典棋盘 DP
- 组合数 C 与 DP 的等价性

### 复杂度

- **时间复杂度**: O(m × n)（DP）或 O(min(m,n))（组合）
- **空间复杂度**: O(n)（DP 一维优化）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法1: DP
class Solution {
public:
    int uniquePaths(int m, int n) {
        vector<int> dp(n, 1);  // dp[0][j] = 1
        for (int i = 1; i < m; i++)
            for (int j = 1; j < n; j++)
                dp[j] += dp[j-1];  // dp[i][j] = dp[i-1][j] + dp[i][j-1]
        return dp[n-1];
    }
};

// 解法2: 组合数学
class SolutionComb {
public:
    int uniquePaths(int m, int n) {
        long long ans = 1;
        int k = min(m - 1, n - 1);
        int total = m + n - 2;
        // C(total, k) = total! / (k! * (total-k)!)
        for (int i = 1; i <= k; i++) {
            ans = ans * (total - k + i) / i;  // 逐乘逐除，保证整除
        }
        return (int)ans;
    }
};

int main() {
    Solution s;
    cout << s.uniquePaths(3, 7) << endl;  // 28
    return 0;
}
```

### 常见错误

- **错误1**: DP 一维优化时内层 j 从 0 开始覆盖了上一行的值——应该从 1 开始。
- **错误2**: 组合数直接算阶乘导致溢出——必须边乘边除。

### 面试陷阱

- **陷阱1**: "如果有障碍物？"——不同路径 II（题63），障碍物处 `dp=0`。
- **陷阱2**: "如何输出具体路径？"——回溯，每个点记录是从上还是从左来的。

---

## 118. 不同路径 II (Unique Paths II) — 题号 63

**难度**: 中等
**标签**: DP

### 题目大意

网格中有障碍物（`1`），从左上到右下，求不同路径数。

### 思路

- 在题62基础上，如果 `grid[i][j] == 1`，则 `dp[i][j] = 0`。
- 注意起点或终点是障碍物时直接返回 0。

### 核心知识点

- 带障碍的棋盘 DP
- 边界处理

### 复杂度

- **时间复杂度**: O(m × n)
- **空间复杂度**: O(n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        if (grid[0][0] == 1 || grid[m-1][n-1] == 1) return 0;
        vector<int> dp(n, 0);
        dp[0] = 1;  // 起点
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) { dp[j] = 0; continue; }
                if (j > 0) dp[j] += dp[j-1];  // dp[j]已存了上一行的值
            }
        }
        return dp[n-1];
    }
};

int main() {
    vector<vector<int>> grid = {{0,0,0},{0,1,0},{0,0,0}};
    Solution s;
    cout << s.uniquePathsWithObstacles(grid) << endl;  // 2
    return 0;
}
```

### 常见错误

- **错误1**: 障碍物处忘记把 dp[j] 设为 0。
- **错误2**: 没检查起点就是障碍物的情况。

### 面试陷阱

- **陷阱1**: "空间 O(1) 能否做到？"——直接在原数组中修改，但如果不能改输入则不行。

---

## 119. 最小路径和 (Minimum Path Sum) — 题号 64

**难度**: 中等
**标签**: DP、网格

### 题目大意

m x n 网格填充非负数，从左上到右下，每次只能右或下。求路径上数字和最小的路径。

### 思路

- DP: `dp[i][j] = min(dp[i-1][j], dp[i][j-1]) + grid[i][j]`

### 核心知识点

- 棋盘 DP 变体（最小和而非路径数）

### 复杂度

- **时间复杂度**: O(m × n)
- **空间复杂度**: O(n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        vector<int> dp(n, 0);
        dp[0] = grid[0][0];
        for (int j = 1; j < n; j++) dp[j] = dp[j-1] + grid[0][j];  // 第一行
        for (int i = 1; i < m; i++) {
            dp[0] += grid[i][0];  // 第一列
            for (int j = 1; j < n; j++) {
                dp[j] = min(dp[j], dp[j-1]) + grid[i][j];
            }
        }
        return dp[n-1];
    }
};

int main() {
    vector<vector<int>> grid = {{1,3,1},{1,5,1},{4,2,1}};
    Solution s;
    cout << s.minPathSum(grid) << endl;  // 7
    return 0;
}
```

### 常见错误

- **错误1**: 一维 DP 时第一行和第一列的处理遗漏。
- **错误2**: `min(dp[j], dp[j-1])` 中 dp[j] 是上一行的值（上），dp[j-1] 是本行的左值。

### 面试陷阱

- **陷阱1**: "如果能四方向走且都是非负？"——Dijkstra（题中限制了只能右和下）。
- **陷阱2**: "如果求最大路径和？"——max 替代 min，但如果有负数则不能用贪心。

---

## 120. 零钱兑换 (Coin Change) — 题号 322

**难度**: 中等
**标签**: DP、BFS

### 题目大意

给定不同面额的硬币 coins 和金额 amount，求凑出该金额所需的最少硬币数。每种硬币无限使用。若无法凑出返回 -1。

### 思路

- **DP（完全背包）**: `dp[i]` = 凑出金额 i 的最少硬币数。`dp[i] = min(dp[i - coin] + 1)` 对所有 coin。
- **BFS**: 从 0 出发，每次尝试加一个硬币进入新状态，找到 amount 时返回层数。本质上 BFS 与 DP 同构。
- 剪枝：可以事先排序 coins，DP 中 `i` 从 coin 开始遍历。

### 核心知识点

- 完全背包：硬币无限使用 → 正序遍历
- BFS 求最短步数
- `dp` 初始化为 `amount + 1`（无效值）

### 复杂度

- **时间复杂度**: O(amount × k)，k 为硬币种类数
- **空间复杂度**: O(amount)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        vector<int> dp(amount + 1, amount + 1);
        dp[0] = 0;
        for (int i = 1; i <= amount; i++) {
            for (int coin : coins) {
                if (i >= coin)
                    dp[i] = min(dp[i], dp[i - coin] + 1);
            }
        }
        return dp[amount] > amount ? -1 : dp[amount];
    }
};

int main() {
    vector<int> coins = {1, 2, 5};
    Solution s;
    cout << s.coinChange(coins, 11) << endl;  // 3
    return 0;
}
```

### 常见错误

- **错误1**: `dp` 初始化为 `INT_MAX` 导致 `+1` 溢出为负数。
- **错误2**: 把完全背包写成 0-1 背包（倒序遍历），导致每种硬币只能用一次。
- **错误3**: 忘记判断 `i >= coin`。

### 面试陷阱

- **陷阱1**: "如果只求方案数？"——零钱兑换 II（题518），`dp[i] += dp[i-coin]`。
- **陷阱2**: "能用贪心吗？"——只有 coins 满足贪心性质（如人民币 1/2/5/10/50/100）时才能贪心，一般情况下贪心不保证最优（反例：coins=[1,3,4], amount=6，贪心用 [4,1,1]=3 枚，最优是 [3,3]=2 枚）。

---

## 121. 零钱兑换 II (Coin Change 2) — 题号 518

**难度**: 中等
**标签**: DP

### 题目大意

求凑出金额 amount 的硬币组合数。每种硬币无限使用。

### 思路

- **完全背包计数**: `dp[i]` = 凑出金额 i 的组合数。
- 外层遍历硬币，内层遍历金额（正序），避免重复组合。
  - 正确: `for coin: for i = coin..amount: dp[i] += dp[i-coin]`
  - 错误（会重复）: `for i: for coin: dp[i] += dp[i-coin]`

### 核心知识点

- 完全背包计数
- 内外层循环顺序决定是组合还是排列

### 复杂度

- **时间复杂度**: O(amount × k)
- **空间复杂度**: O(amount)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int change(int amount, vector<int>& coins) {
        vector<int> dp(amount + 1, 0);
        dp[0] = 1;  // 凑0元有一种方法（不选任何硬币）
        for (int coin : coins) {  // 外层硬币
            for (int i = coin; i <= amount; i++) {  // 内层金额，正序
                dp[i] += dp[i - coin];
            }
        }
        return dp[amount];
    }
};

int main() {
    vector<int> coins = {1, 2, 5};
    Solution s;
    cout << s.change(5, coins) << endl;  // 4
    return 0;
}
```

### 常见错误

- **错误1**: 内外层循环颠倒，导致计算排列数而非组合数。
- **错误2**: 内层 i 倒序——变成 0-1 背包，每种硬币只能用一次。

### 面试陷阱

- **陷阱1**: "为什么外层硬币保证组合不重复？"——因为每种硬币的处理顺序固定，不会出现 `[1,2]` 和 `[2,1]` 两种情况。
- **陷阱2**: "如果要求排列数（如爬楼梯）？"——内层金额放在外层，硬币放内层。

---

## 122. 单词拆分 (Word Break) — 题号 139

**难度**: 中等
**标签**: DP、哈希表、字符串、Trie

### 题目大意

给定字符串 s 和字典 wordDict，判断 s 是否可以被拆分为一个或多个字典中的单词。

### 思路

- **DP**: `dp[i]` = s[0..i-1] 能否被拆分。`dp[i] = dp[j] && s[j..i-1] in dict`，对于任意 j < i。
- **优化**: (1) j 从 i-1 向前遍历到 `i - maxWordLen`；(2) 用 Trie 替代哈希表查单词。
- BFS 也可行（从 0 出发，每次尝试所有可能的单词匹配）。

### 核心知识点

- 分割型 DP
- 预处理字典到 unordered_set
- 最大词长剪枝

### 复杂度

- **时间复杂度**: O(N²)（取子串是 O(1) 通过索引计算）
- **空间复杂度**: O(N)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> dict(wordDict.begin(), wordDict.end());
        int n = s.size(), maxLen = 0;
        for (auto& w : wordDict) maxLen = max(maxLen, (int)w.size());
        vector<bool> dp(n + 1, false);
        dp[0] = true;
        for (int i = 1; i <= n; i++) {
            // 只检查长度 ≤ maxLen 的前缀
            for (int j = i - 1; j >= max(0, i - maxLen); j--) {
                if (dp[j] && dict.count(s.substr(j, i - j))) {
                    dp[i] = true;
                    break;  // 找到一种拆分即可
                }
            }
        }
        return dp[n];
    }
};

int main() {
    vector<string> dict = {"leet", "code"};
    Solution s;
    cout << s.wordBreak("leetcode", dict) << endl;  // 1
    return 0;
}
```

### 常见错误

- **错误1**: `dp` 下标混淆——`dp[i]` 表示前 i 个字符（即 s[0..i-1]）。
- **错误2**: 没有用 `maxLen` 剪枝，对所有 j 遍历导致超时。
- **错误3**: `substr(j, i-j)` 第二个参数写错。

### 面试陷阱

- **陷阱1**: "如果要求输出所有拆分方案？"——回溯 + DP 记忆化（题140，Word Break II）。
- **陷阱2**: "Trie 如何加速？"——从 j 位置开始在 Trie 上走，每走一步如果遇到 isEnd 就检查 dp[j] 并更新 dp[i]。

---

## 123. 完全平方数 (Perfect Squares) — 题号 279

**难度**: 中等
**标签**: DP、BFS、数学

### 题目大意

给定正整数 n，求最少能用几个完全平方数（1, 4, 9, 16...）之和表示。

### 思路

- **DP 完全背包**: `dp[i] = min(dp[i - j²] + 1)`，j 从 1 到 √i。
- **BFS**: 从 0 出发，每次加上一个完全平方数，求到达 n 的最短步数。
- **数学（拉格朗日四平方定理）**: 每个自然数最多由 4 个完全平方数之和表示。此外若 `n = 4^a(8b+7)` 则返回 4。可 O(log n) 判断。

### 核心知识点

- 完全背包（同零钱兑换）
- BFS 最短步数
- 拉格朗日定理（了解即可）

### 复杂度

- **时间复杂度**: O(n × √n)
- **空间复杂度**: O(n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// DP版本
class Solution {
public:
    int numSquares(int n) {
        vector<int> dp(n + 1, n + 1);
        dp[0] = 0;
        // 预处理完全平方数
        vector<int> squares;
        for (int i = 1; i * i <= n; i++) squares.push_back(i * i);
        for (int sq : squares) {
            for (int i = sq; i <= n; i++) {
                dp[i] = min(dp[i], dp[i - sq] + 1);
            }
        }
        return dp[n];
    }
};

// BFS版本
class SolutionBFS {
public:
    int numSquares(int n) {
        queue<int> q;
        q.push(0);
        vector<bool> visited(n + 1, false);
        visited[0] = true;
        int steps = 0;
        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; i++) {
                int cur = q.front(); q.pop();
                if (cur == n) return steps;
                for (int j = 1; cur + j * j <= n; j++) {
                    int nxt = cur + j * j;
                    if (!visited[nxt]) {
                        visited[nxt] = true;
                        q.push(nxt);
                    }
                }
            }
            steps++;
        }
        return -1;
    }
};

int main() {
    Solution s;
    cout << s.numSquares(12) << endl;  // 3 (4+4+4)
    return 0;
}
```

### 常见错误

- **错误1**: DP 外层遍历金额内层遍历平方数（会重复排列），正确做法是外层平方数内层金额。
- **错误2**: BFS 中没有 visited 标记导致重复入队。

### 面试陷阱

- **陷阱1**: "拉格朗日四平方定理？"——可以做到 O(√n) 甚至 O(1)（通过数学判断），面试中若面试官提示"数学方法"时应能反应。
- **陷阱2**: "和零钱兑换的关系？"——本题是零钱兑换的特例（硬币是平方数集合）。

---

## 124. 乘积最大子数组 (Maximum Product Subarray) — 题号 152

**难度**: 中等
**标签**: DP、数组

### 题目大意

求整数数组中乘积最大的连续子数组，返回乘积值。

### 思路

- **关键洞察**: 负数乘以负数变成正数，所以需要同时维护"以当前位置结尾的最大乘积"和"最小乘积"。
- **DP**: `curMax[i] = max(nums[i], curMax[i-1]×nums[i], curMin[i-1]×nums[i])`
         `curMin[i] = min(nums[i], curMax[i-1]×nums[i], curMin[i-1]×nums[i])`
- 滚动变量优化到 O(1) 空间。

### 核心知识点

- 双状态 DP（最大 + 最小）
- 负数翻转效应
- 与"最大子数组和"（Kadane）的对比

### 复杂度

- **时间复杂度**: O(N)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxProduct(vector<int>& nums) {
        int ans = nums[0], curMax = nums[0], curMin = nums[0];
        for (int i = 1; i < nums.size(); i++) {
            // 如果当前负数，交换 curMax 和 curMin（因为负负得正）
            if (nums[i] < 0) swap(curMax, curMin);
            curMax = max(nums[i], curMax * nums[i]);
            curMin = min(nums[i], curMin * nums[i]);
            ans = max(ans, curMax);
        }
        return ans;
    }
};

int main() {
    vector<int> nums = {2,3,-2,4};
    Solution s;
    cout << s.maxProduct(nums) << endl;  // 6
    return 0;
}
```

### 常见错误

- **错误1**: 忘记维护 `curMin`（只维护 max），导致负数翻转后无法获得正乘积。
- **错误2**: 更新顺序错误（用新的 curMax 算了 curMin）。

### 面试陷阱

- **陷阱1**: "为什么最大子数组和不需要维护 min 状态？"——因为加法单调，负数不会翻转符号；乘积会出现负数乘负数变正的情况。
- **陷阱2**: "如果数组中只有 0？"——返回 0，特殊判断。

---

## 125. 分割等和子集 (Partition Equal Subset Sum) — 题号 416

**难度**: 中等
**标签**: DP、0-1背包

### 题目大意

判断是否可以将数组分割成两个子集，使它们的和相等。

### 思路

- **转化**: 能否选一些数，使它们的和为 `sum/2`（0-1背包问题）。
- **DP**: `dp[j]` 表示能否凑出和为 j 的子集。
  - `dp[j] = dp[j] || dp[j - nums[i]]`（从大到小倒序遍历，0-1 背包）。
- 剪枝：若 sum 为奇数直接 false；从大到小提前退出。

### 核心知识点

- 问题转化为 0-1 背包
- 0-1 背包倒序遍历
- 位集优化：`bitset<10001>`

### 复杂度

- **时间复杂度**: O(N × sum/2)
- **空间复杂度**: O(sum/2)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool canPartition(vector<int>& nums) {
        int sum = accumulate(nums.begin(), nums.end(), 0);
        if (sum % 2) return false;
        int target = sum / 2;
        vector<bool> dp(target + 1, false);
        dp[0] = true;
        for (int x : nums) {
            for (int j = target; j >= x; j--) {  // 倒序：0-1背包
                dp[j] = dp[j] || dp[j - x];
            }
            if (dp[target]) return true;  // 提前退出
        }
        return dp[target];
    }
};

int main() {
    vector<int> nums = {1,5,11,5};
    Solution s;
    cout << s.canPartition(nums) << endl;  // 1
    return 0;
}
```

### 常见错误

- **错误1**: 内层正序遍历（完全背包），导致同一数字被重复使用。
- **错误2**: 忘记 `sum % 2` 的剪枝。

### 面试陷阱

- **陷阱1**: "bitset 如何优化？"——`bitset<10001> dp; dp[0]=1; for(int x:nums) dp |= dp<<x;` O(N×sum/64)。
- **陷阱2**: "如果要求输出两个子集？"——dp 需要记录选了哪些元素（记录前驱/路径回溯）。

---

## 126. 目标和 (Target Sum) — 题号 494

**难度**: 中等
**标签**: DP、回溯、0-1背包

### 题目大意

在每个数前面添加 `+` 或 `-`，使表达式结果等于 target。求方案数。

### 思路

- **转化 0-1 背包**: 设正号数之和为 P，负号数绝对值之和为 N。
  - `P - N = target`
  - `P + N = sum`
  - 推出 `P = (target + sum) / 2`
  - 问题变为：从 nums 中选若干个数，使它们的和等于 P（0-1 背包计数）。
- 注意：`target + sum` 必须是非负偶数，否则无解。
- 回溯 O(2^N) 会超时，必须 DP。

### 核心知识点

- 问题转化技巧：`P = (target + sum) / 2`
- 0-1 背包计数
- 边界判断：`target + sum` 奇偶性

### 复杂度

- **时间复杂度**: O(N × P)
- **空间复杂度**: O(P)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        int sum = accumulate(nums.begin(), nums.end(), 0);
        // P = (target + sum) / 2 必须是整数且 ≥ 0
        if (target + sum < 0 || (target + sum) % 2) return 0;
        int P = (target + sum) / 2;
        if (P > sum) return 0;
        vector<int> dp(P + 1, 0);
        dp[0] = 1;  // 和为0有一种：什么都不选
        for (int x : nums) {
            for (int j = P; j >= x; j--) {  // 0-1背包倒序
                dp[j] += dp[j - x];
            }
        }
        return dp[P];
    }
};

int main() {
    vector<int> nums = {1,1,1,1,1};
    Solution s;
    cout << s.findTargetSumWays(nums, 3) << endl;  // 5
    return 0;
}
```

### 常见错误

- **错误1**: 忘记检查 `(target + sum) % 2` 和 `target + sum < 0`。
- **错误2**: 内层正序遍历变成完全背包，每个数字被无限使用。
- **错误3**: dp 求和时写成 `dp[j] = dp[j - x]` 而非 `dp[j] += dp[j - x]`。

### 面试陷阱

- **陷阱1**: "如果不转化为背包，直接 DP 怎么做？"——`dp[i][j+sum]` 表示前 i 个数凑成偏移后值 j 的方案数，状态空间大一倍。
- **陷阱2**: "如果 nums 包含 0？"——0 对两种符号等价，dp 计数时会翻倍（选不选 0 都是两种）。

---

## 127. 最大正方形 (Maximal Square) — 题号 221

**难度**: 中等
**标签**: DP、单调栈

### 题目大意

在由 `'0'` 和 `'1'` 组成的二维矩阵中，找到只包含 `'1'` 的最大正方形，返回其面积。

### 思路

- **DP**: `dp[i][j]` = 以 `(i,j)` 为右下角的最大正方形边长。
  - 若 `matrix[i][j] == '1'`：`dp[i][j] = min(dp[i-1][j], dp[i][j-1], dp[i-1][j-1]) + 1`
  - 直观理解：左、上、左上三个位置的瓶颈 + 1。
- **单调栈**: 可以转化为"柱状图中最大矩形"的变体，但 DP 更简洁。

### 核心知识点

- 正方形 DP 转移方程
- `min` 三方向的含义

### 复杂度

- **时间复杂度**: O(m × n)
- **空间复杂度**: O(n)（一维优化）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maximalSquare(vector<vector<char>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        vector<int> dp(n + 1, 0);  // dp[j] = 以(i, j-1)为右下角的最大边长
        int maxSide = 0, prev = 0;  // prev = dp[i-1][j-1]
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                int tmp = dp[j+1];  // 保存旧的 dp[j+1] (即 dp[i-1][j])
                if (matrix[i][j] == '1') {
                    dp[j+1] = min({dp[j], dp[j+1], prev}) + 1;
                    maxSide = max(maxSide, dp[j+1]);
                } else {
                    dp[j+1] = 0;
                }
                prev = tmp;
            }
        }
        return maxSide * maxSide;
    }
};

int main() {
    vector<vector<char>> matrix = {
        {'1','0','1','0','0'},
        {'1','0','1','1','1'},
        {'1','1','1','1','1'},
        {'1','0','0','1','0'}
    };
    Solution s;
    cout << s.maximalSquare(matrix) << endl;  // 4
    return 0;
}
```

### 常见错误

- **错误1**: 一维优化时忘记保存 `prev`（左上角的值），导致 dp 计算错误。
- **错误2**: `dp[j+1]` 下标未对齐（公式中是 `j+1` 对应旧 `j`）。

### 面试陷阱

- **陷阱1**: "如果是最大矩形（不正方形）？"——最大矩形（题85），需要单调栈 + 柱状图。
- **陷阱2**: "能否用二分？"——二分边长 + 前缀和验证 O(mn log min(m,n))，不如 DP。

---

## 128. 最大矩形 (Maximal Rectangle) — 题号 85

**难度**: 困难
**标签**: 单调栈、DP、数组

### 题目大意

给定只含 `'0'` 和 `'1'` 的矩阵，找出只包含 `'1'` 的最大矩形，返回其面积。

### 思路

- **暴力 O(m²n²)**: 枚举所有可能的矩形的左上和右下角。
- **优化——转化为柱状图**: 逐行扫描，对于每一行，计算每个位置向上连续 `'1'` 的个数（高度），然后用"柱状图中最大矩形"（题84）的方法求该行的最大矩形。总体 O(m × n)。
- **为什么是"困难"**: 需要将二维问题转化为一维 + 单调栈，且需要理解题84作为子问题。

### 核心知识点

- 逐行构建柱状图：`heights[j] = (matrix[i][j]=='1') ? heights[j]+1 : 0`
- 单调栈求柱状图最大矩形（题84）
- 问题分解：二维 → 一维

### 复杂度

- **时间复杂度**: O(m × n)（每行 O(n) 单调栈）
- **空间复杂度**: O(n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        if (matrix.empty()) return 0;
        int m = matrix.size(), n = matrix[0].size();
        vector<int> heights(n, 0);
        int ans = 0;
        for (int i = 0; i < m; i++) {
            // 更新柱状图
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] == '1') heights[j]++;
                else heights[j] = 0;
            }
            // 求当前柱状图的最大矩形（题84解法）
            ans = max(ans, largestRectangleInHistogram(heights));
        }
        return ans;
    }
    // 题84: 柱状图中最大的矩形
    int largestRectangleInHistogram(vector<int>& heights) {
        heights.push_back(0);  // 哨兵
        int n = heights.size(), ans = 0;
        stack<int> st;
        for (int i = 0; i < n; i++) {
            while (!st.empty() && heights[i] < heights[st.top()]) {
                int h = heights[st.top()]; st.pop();
                int w = st.empty() ? i : i - st.top() - 1;
                ans = max(ans, h * w);
            }
            st.push(i);
        }
        heights.pop_back();  // 恢复原数组
        return ans;
    }
};

int main() {
    vector<vector<char>> matrix = {
        {'1','0','1','0','0'},
        {'1','0','1','1','1'},
        {'1','1','1','1','1'},
        {'1','0','0','1','0'}
    };
    Solution s;
    cout << s.maximalRectangle(matrix) << endl;  // 6
    return 0;
}
```

### 常见错误

- **错误1**: heights 更新时忘记把 `'0'` 的位置清零。
- **错误2**: 单调栈中 `w = i - st.top() - 1` 忘记 `-1`（因为 st.top() 是左边第一个比当前矮的位置，宽度不包括它）。
- **错误3**: 忘记哨兵或恢复数组。

### 面试陷阱

- **陷阱1**: "为什么要用哨兵？"——确保栈中所有元素最终都能出栈被处理，避免最后再写一遍清理栈的循环。
- **陷阱2**: "DP 能做吗？"——可以，但较复杂：对每个位置维护 left/right/height 三个数组，`area = height * (right - left)`。

---

## 129. 柱状图中最大的矩形 (Largest Rectangle in Histogram) — 题号 84

**难度**: 困难
**标签**: 单调栈、数组

### 题目大意

给定非负整数数组表示柱状图的高度，宽度均为 1。求能勾勒出的最大矩形面积。

### 思路

- **暴力**: 枚举每个柱子作为矩形的最低高度，向两边扩展到第一个比它矮的位置。O(N²)。
- **单调栈（递增栈）**: 
  - 栈中存下标，保证对应高度递增。
  - 遇到高度小于栈顶时，弹出栈顶，以弹出元素为"最矮高度"计算矩形面积。
  - 宽度 = 当前下标 - 新栈顶下标 - 1（即向左延伸到新栈顶的右边一位）。
  - O(N)，每个元素入栈出栈各一次。
- **为什么是"困难"**: 单调栈的状态含义需要理解："栈顶元素的右边界是当前 i，左边界是新栈顶"；哨兵技巧也需注意。

### 核心知识点

- 单调递增栈
- 宽度计算公式：`w = st.empty() ? i : i - st.top() - 1`
- 哨兵：末尾加 0 确保全部出栈
- 前缀哨兵：头部加 0 避免空栈判断

### 复杂度

- **时间复杂度**: O(N)
- **空间复杂度**: O(N)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法1: 单调栈 + 后置哨兵
class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        heights.push_back(0);  // 末尾哨兵
        int n = heights.size(), ans = 0;
        stack<int> st;
        for (int i = 0; i < n; i++) {
            // 维护递增栈：当前高度小于栈顶时，栈顶的右边界确定
            while (!st.empty() && heights[i] < heights[st.top()]) {
                int h = heights[st.top()]; st.pop();
                int w = st.empty() ? i : i - st.top() - 1;
                // 左边界是栈中下一个元素（或 -1），右边界是 i
                ans = max(ans, h * w);
            }
            st.push(i);
        }
        heights.pop_back();  // 恢复
        return ans;
    }
};

// 解法2: 左右哨兵（无需特殊判断空栈）
class SolutionV2 {
public:
    int largestRectangleArea(vector<int>& heights) {
        heights.insert(heights.begin(), 0);  // 前置哨兵
        heights.push_back(0);                // 后置哨兵
        stack<int> st;
        int ans = 0;
        for (int i = 0; i < heights.size(); i++) {
            while (!st.empty() && heights[i] < heights[st.top()]) {
                int h = heights[st.top()]; st.pop();
                int w = i - st.top() - 1;  // st一定非空（因为有前置哨兵）
                ans = max(ans, h * w);
            }
            st.push(i);
        }
        return ans;
    }
};

int main() {
    vector<int> heights = {2,1,5,6,2,3};
    Solution s;
    cout << s.largestRectangleArea(heights) << endl;  // 10
    return 0;
}
```

### 常见错误

- **错误1**: 宽度公式写成 `i - st.top()` 而非 `i - st.top() - 1`——st.top() 是左边第一个比当前矮的位置，不能包含。
- **错误2**: 弹出后 `st` 可能为空，忘记处理——要么用 `st.empty() ? i : ...`，要么加前置哨兵。
- **错误3**: 忘记恢复原数组（加哨兵后 pop_back / erase）。

### 面试陷阱

- **陷阱1**: "为什么用单调栈而非分治？"——分治 O(N log N) 也可过，但单调栈 O(N) 且代码更短。分治：找最小高度，结果 = max(左半边, 右半边, 最小高度×当前宽度)。
- **陷阱2**: "如果柱状图是动态的（高度不断变化）？"——单调栈每次都需要 O(N) 重建，动态版本需要线段树或更复杂的数据结构。

---

## 130. 数组中的第K个最大元素 (Kth Largest Element in an Array) — 题号 215

**难度**: 中等
**标签**: 堆、快速选择、分治

### 题目大意

给定整数数组 nums 和整数 k，返回第 k 个最大的元素（第 k 大即排序后的倒数第 k 个，1-indexed）。

### 思路

- **排序法 O(N log N)**: 排序后取倒数第 k 个。最简单但不最优。
- **堆 O(N log k)**: 维护大小为 k 的小顶堆。遍历数组，堆满时若新元素大于堆顶则替换。最终堆顶即为第 k 大。
- **快速选择 O(N) 期望 / O(N²) 最坏**: 基于快速排序的 partition。每次选 pivot 划分，根据 pivot 最终位置决定递归左还是右。加入随机化可期望 O(N)。
- **STL**: `nth_element(nums.begin(), nums.begin()+k-1, nums.end(), greater<>())`。

### 核心知识点

- 小顶堆求第 k 大
- 快速选择算法（QuickSelect）
- Partition 技巧

### 复杂度

- 堆: 时间 O(N log k)，空间 O(k)
- 快速选择: 时间 O(N) 期望，空间 O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法1: 小顶堆 O(N log k)
class SolutionHeap {
public:
    int findKthLargest(vector<int>& nums, int k) {
        priority_queue<int, vector<int>, greater<int>> pq;  // 小顶堆
        for (int x : nums) {
            pq.push(x);
            if (pq.size() > k) pq.pop();  // 保持堆大小为k
        }
        return pq.top();
    }
};

// 解法2: 快速选择 O(N) 期望
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        int n = nums.size();
        return quickSelect(nums, 0, n - 1, n - k);  // 第k大 = 第n-k小(0-indexed)
    }
    int quickSelect(vector<int>& nums, int l, int r, int target) {
        if (l == r) return nums[l];
        // 随机选 pivot 防退化
        int pivotIdx = l + rand() % (r - l + 1);
        swap(nums[pivotIdx], nums[r]);
        int pos = partition(nums, l, r);
        if (pos == target) return nums[pos];
        else if (pos > target) return quickSelect(nums, l, pos - 1, target);
        else return quickSelect(nums, pos + 1, r, target);
    }
    int partition(vector<int>& nums, int l, int r) {
        int pivot = nums[r], i = l;
        for (int j = l; j < r; j++) {
            if (nums[j] <= pivot)  // 从小到大分区
                swap(nums[i++], nums[j]);
        }
        swap(nums[i], nums[r]);
        return i;
    }
};

// 解法3: STL (面试中可提但不建议实战写)
class SolutionSTL {
public:
    int findKthLargest(vector<int>& nums, int k) {
        nth_element(nums.begin(), nums.begin() + k - 1, nums.end(), greater<int>());
        return nums[k - 1];
    }
};

int main() {
    vector<int> nums = {3,2,1,5,6,4};
    Solution s;
    cout << s.findKthLargest(nums, 2) << endl;  // 5
    SolutionHeap sh;
    vector<int> nums2 = {3,2,1,5,6,4};
    cout << sh.findKthLargest(nums2, 2) << endl;
    return 0;
}
```

### 常见错误

- **错误1**: 堆用成大顶堆（默认 `priority_queue`）——求第 k 大需要小顶堆。
- **错误2**: 快速选择的 `target` 索引混淆——第 k 大（1-indexed）对应第 n-k 小（0-indexed）。
- **错误3**: partition 中忘记随机选 pivot，遇到有序数组退化为 O(N²)。
- **错误4**: 快速选择用迭代而非递归时边界更新写反。

### 面试陷阱

- **陷阱1**: "堆 vs 快速选择 各什么场景用？"——堆适合 k 很小或数据是流式的（随时有新数据来）；快速选择适合 O(1) 空间、数据量不大的静态数组。
- **陷阱2**: "最坏 O(N) 的算法？"——BFPRT（Median of Medians），保证 O(N) 但常数大，面试中几乎不要求手写，提到即可。
- **陷阱3**: "如果求前 k 大？"——用小顶堆维护 k 个即可；或排序后取后 k 个。

# LeetCode Hot200 刷题文档 — 阶段四：强化（50题）

> **阶段定位**：堆、Trie、图论（拓扑排序、最短路、并查集、欧拉路径、Tarjan）——机试高频难点集中营。
> **使用建议**：每道题先自己想5分钟，再看思路；代码默写三遍；常见错误必须背诵。

---

## 131. 前 K 个高频元素 (Top K Frequent Elements) — 题号 347

**难度**: 中等
**标签**: 堆、哈希表、桶排序

### 题目大意

给定一个非空整数数组，返回出现频率前 k 高的元素。

### 思路

**暴力**：统计频率后按频率排序，取前 k 个 —— O(n log n)，面试官会要求更优。

**优化（小顶堆）**：为什么用小顶堆而不是大顶堆？大顶堆需要把所有 n 个元素都入堆（O(n log n)），而小顶堆只需维护 k 个元素：当堆大小 > k 时弹出堆顶（最小值），最终堆中留下的就是 k 个最大频率的元素。每次操作 O(log k)，总时间 O(n log k)。

**最优（桶排序）**：频率最大不超过 n。建立 n+1 个桶，桶 i 存放频率为 i 的所有元素。从高频到低频遍历桶，收集 k 个即可 —— O(n)。

### 核心知识点

- 小顶堆维护 Top K：`priority_queue` 默认大顶堆，用 `greater` 或自定义比较器
- 桶排序（计数排序变体）：空间换时间，频率有上限时最优
- `unordered_map` 统计频率

### 复杂度

- **时间复杂度**: O(n log k)（堆）/ O(n)（桶排序）
- **空间复杂度**: O(n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法一：小顶堆 O(n log k)
vector<int> topKFrequent_heap(vector<int>& nums, int k) {
    unordered_map<int, int> freq;
    for (int x : nums) freq[x]++;

    // pair<频率, 元素>，小顶堆按频率排序
    auto cmp = [](pair<int,int>& a, pair<int,int>& b) { return a.first > b.first; };
    priority_queue<pair<int,int>, vector<pair<int,int>>, decltype(cmp)> pq(cmp);

    for (auto& [num, f] : freq) {
        pq.push({f, num});
        if (pq.size() > k) pq.pop();  // 弹出频率最小的
    }

    vector<int> res;
    while (!pq.empty()) { res.push_back(pq.top().second); pq.pop(); }
    return res;
}

// 解法二：桶排序 O(n)
vector<int> topKFrequent_bucket(vector<int>& nums, int k) {
    unordered_map<int, int> freq;
    for (int x : nums) freq[x]++;
    int n = nums.size();
    vector<vector<int>> buckets(n + 1);
    for (auto& [num, f] : freq) buckets[f].push_back(num);

    vector<int> res;
    for (int i = n; i >= 0 && res.size() < k; i--) {
        for (int num : buckets[i]) {
            res.push_back(num);
            if (res.size() == k) break;
        }
    }
    return res;
}

int main() {
    vector<int> nums = {1,1,1,2,2,3};
    int k = 2;
    auto r1 = topKFrequent_heap(nums, k);
    auto r2 = topKFrequent_bucket(nums, k);
    for (int x : r1) cout << x << " "; cout << endl;
    for (int x : r2) cout << x << " "; cout << endl;
    return 0;
}
```

### 常见错误

- 错误1：用大顶堆存储所有元素，然后 pop k 次 —— O(n log n)，没利用好小顶堆的 O(n log k) 优势
- 错误2：桶排序时忘记频率可能为 0，桶数组大小应是 n+1 而非 n
- 错误3：`priority_queue` 自定义比较器语法写错，`decltype` 漏写导致编译错误

### 面试陷阱

- 陷阱1：面试官追问「如果元素个数远大于 k，哪种方法更好？」—— 小顶堆更好，桶排序需要 O(n) 空间存储所有桶
- 陷阱2：追问「数据流场景怎么做？」—— 先用哈希表维护频率，再用小顶堆；可引出下一题 703

---

## 132. 合并 K 个升序链表 (Merge k Sorted Lists) — 题号 23

**难度**: 困难
**标签**: 堆、链表、分治

### 题目大意

给定 k 个升序链表，将它们合并为一个升序链表。

### 思路

**暴力1**：依次合并 —— 第 i 次合并链表长度为 i*n/k，总时间 O(k²n)，太慢。

**暴力2**：把所有节点值取出来排序再建链表 —— O(N log N)，N=kn，可以但不是最优，且没有利用"每个链表已有序"的性质。

**优化（小顶堆）**：类似合并2个链表，每次取 k 个链表头中的最小值。维护一个小顶堆存 k 个链表当前的头部节点，每次弹出最小值接到结果链表，然后把该节点的 next 入堆。每个节点入堆出堆各一次，O(N log k)。

**最优（分治法）**：两两合并，第一轮 k/2 次合并，第二轮 k/4 次……总时间也是 O(N log k)，但常数更小（堆的插入删除开销较大）。

### 核心知识点

- 小顶堆维护 k 路归并
- `priority_queue` 存储自定义类型需要重载 `operator>` 或自定义比较器
- 分治两两合并：递归 `merge(l, r)` 分割再合并

### 复杂度

- **时间复杂度**: O(N log k)，N 为所有节点总数
- **空间复杂度**: O(k)（堆）/ O(log k)（分治递归栈）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
};

// 解法一：小顶堆 O(N log k)
ListNode* mergeKLists_heap(vector<ListNode*>& lists) {
    // 小顶堆：按节点值排序
    auto cmp = [](ListNode* a, ListNode* b) { return a->val > b->val; };
    priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);

    // 所有链表头入堆
    for (auto head : lists)
        if (head) pq.push(head);

    ListNode dummy(0), *tail = &dummy;
    while (!pq.empty()) {
        ListNode* cur = pq.top(); pq.pop();
        tail->next = cur;
        tail = cur;
        if (cur->next) pq.push(cur->next);  // 当前节点的后继入堆
    }
    return dummy.next;
}

// 解法二：分治 O(N log k)
ListNode* mergeTwo(ListNode* a, ListNode* b) {
    if (!a) return b; if (!b) return a;
    if (a->val < b->val) { a->next = mergeTwo(a->next, b); return a; }
    else { b->next = mergeTwo(a, b->next); return b; }
}

ListNode* mergeKLists_divide(vector<ListNode*>& lists, int l, int r) {
    if (l == r) return lists[l];
    if (l > r) return nullptr;
    int mid = l + (r - l) / 2;
    return mergeTwo(mergeKLists_divide(lists, l, mid),
                    mergeKLists_divide(lists, mid + 1, r));
}

ListNode* mergeKLists(vector<ListNode*>& lists) {
    if (lists.empty()) return nullptr;
    return mergeKLists_divide(lists, 0, lists.size() - 1);
}

int main() {
    // 构建测试链表省略
    return 0;
}
```

### 常见错误

- 错误1：堆中存储链表节点时，修改了节点的 next 后没及时处理，导致链表断开
- 错误2：分治法忘了处理 `l > r` 的 base case（空 lists 的情况）
- 错误3：堆解法中节点入堆前忘记判空（`if (head)` 检查必不可少）

### 面试陷阱

- 陷阱1：面试官追问「堆解法和分治法哪个常数小？」—— 分治法常数小，因为没有堆的 push/pop 开销；但堆解法代码更简洁
- 陷阱2：追问「k 很大但链表很短，哪种好？」—— 堆解法 O(N log k) 的 k 因子影响更大，分治法更好

---

## 133. 查找和最小的 K 对数字 (Find K Pairs with Smallest Sums) — 题号 373

**难度**: 中等
**标签**: 堆、数组

### 题目大意

给定两个升序数组 nums1 和 nums2，以及整数 k。定义一对数字 (u,v)，u 来自 nums1，v 来自 nums2。找出和最小的 k 对数字。

### 思路

**暴力**：枚举所有 m×n 对，排序取前 k —— O(mn log(mn))，不可接受。

**优化（大顶堆维护Top K最小）**：为什么这里用大顶堆？因为我们要维护"最小的 k 个"，所以堆中存当前找到的最小 k 对，堆顶是"k 对中最大的那个"。新来的对如果比堆顶小，就替换堆顶。这里大顶堆用的原因是"堆顶是淘汰标准"。

**更优（小顶堆多路归并）**：类似合并 k 个链表。先把 nums1[0] 和所有 nums2[j] 配对入堆，每次弹出最小值 (i,j)，然后将 (i+1,j) 入堆。这样每次 O(log m)，总 O(k log m)。

### 核心知识点

- 大顶堆维护 Top K 最小（淘汰最大值）
- 多路归并思想：把矩阵的行看作 m 条有序链表
- `pair<int,int>` 入堆，避免重复需要记录索引

### 复杂度

- **时间复杂度**: O(k log m)（多路归并）/ O(mn log k)（大顶堆暴力枚举）
- **空间复杂度**: O(m)（多路归并）/ O(k)（大顶堆）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
    int m = nums1.size(), n = nums2.size();
    vector<vector<int>> res;
    if (m == 0 || n == 0) return res;

    // 小顶堆存 {sum, i, j}
    auto cmp = [](auto& a, auto& b) { return a[0] > b[0]; };
    priority_queue<vector<int>, vector<vector<int>>, decltype(cmp)> pq(cmp);

    // 初始：nums1[0] 配所有 nums2[j]
    for (int j = 0; j < n; j++)
        pq.push({nums1[0] + nums2[j], 0, j});

    while (k-- && !pq.empty()) {
        auto cur = pq.top(); pq.pop();
        int i = cur[1], j = cur[2];
        res.push_back({nums1[i], nums2[j]});
        // 下一个：nums1[i+1] 配 nums2[j]
        if (i + 1 < m)
            pq.push({nums1[i+1] + nums2[j], i + 1, j});
    }
    return res;
}

int main() {
    vector<int> nums1 = {1,7,11}, nums2 = {2,4,6};
    int k = 3;
    auto res = kSmallestPairs(nums1, nums2, k);
    for (auto& p : res) cout << p[0] << "," << p[1] << endl;
    return 0;
}
```

### 常见错误

- 错误1：用大顶堆实现时，比较函数方向写反，把"淘汰最大值"写成了"淘汰最小值"
- 错误2：初始入堆时把 nums1[0] 和所有 nums2[j] 入堆，忘了处理重复问题 —— (i+1,j) 可能和已有的 (i+1,j-1) 重复？不会，因为 j 不同
- 错误3：忘记 k 可能大于 m×n 的情况

### 面试陷阱

- 陷阱1：追问「两个数组都很大怎么办？」—— 此题 m,n 可达 10^4，O(k log m) 的多路归并是可以的
- 陷阱2：追问「如果 k 很大接近 m×n 怎么办？」—— 直接两两枚举 + 快速选择 O(mn) 可能更优

---

## 134. 数据流的中位数 (Find Median from Data Stream) — 题号 295

**难度**: 困难
**标签**: 堆、设计、双堆

### 题目大意

设计一个数据结构，支持向数据流中添加整数，并能随时返回当前所有数的中位数。

### 思路

**暴力**：每次插入后排序 —— O(n log n)，不可接受。

**暴力2**：用有序数组，插入时二分找位置 O(log n)，但数组插入 O(n)。

**优化（双堆法）**：维护两个堆 —— 大顶堆 `left`（存较小的一半）、小顶堆 `right`（存较大的一半）。保持 `left.size() == right.size()` 或 `left.size() == right.size() + 1`。中位数要么是 left 堆顶，要么是两堆顶的平均值。

每次插入：先判断放左还是放右 → 放进去 → 调整平衡。插入 O(log n)，查询中位数 O(1)。

为什么不用平衡树？平衡树也能做（如 multiset + 迭代器），但双堆实现简洁且 O(log n) 足够。

### 核心知识点

- 双堆维护中位数：大顶堆存较小半部分，小顶堆存较大半部分
- `priority_queue<int>` 默认大顶堆，`priority_queue<int, vector<int>, greater<int>>` 小顶堆
- 平衡策略：保持 `left.size() >= right.size()` 且差值 ≤ 1

### 复杂度

- **时间复杂度**: 插入 O(log n)，查询 O(1)
- **空间复杂度**: O(n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class MedianFinder {
    // left: 大顶堆存较小的一半；right: 小顶堆存较大的一半
    priority_queue<int> left;                          // 大顶堆
    priority_queue<int, vector<int>, greater<int>> right; // 小顶堆

public:
    MedianFinder() {}

    void addNum(int num) {
        // 先放入 left（较小半）
        left.push(num);
        // 把 left 的最大值移到 right
        right.push(left.top()); left.pop();
        // 保持 left 大小 >= right 大小
        if (left.size() < right.size()) {
            left.push(right.top()); right.pop();
        }
    }

    double findMedian() {
        if (left.size() > right.size())
            return left.top();
        else
            return (left.top() + right.top()) / 2.0;
    }
};

int main() {
    MedianFinder mf;
    mf.addNum(1); mf.addNum(2);
    cout << mf.findMedian() << endl; // 1.5
    mf.addNum(3);
    cout << mf.findMedian() << endl; // 2
    return 0;
}
```

### 常见错误

- 错误1：平衡逻辑写反 —— `left.size() < right.size()` 时应该从 right 移到 left，反之亦然
- 错误2：忘记整数除法问题 —— 中位数的平均值要用 `/ 2.0` 而非 `/ 2`
- 错误3：初始把元素放入 right 而非 left，导致 left 可能为空时查询出错

### 面试陷阱

- 陷阱1：追问「如果数据量极大且大部分集中在某区间，如何优化？」—— 可以考虑分桶 + 双堆
- 陷阱2：追问「双堆法能否处理删除操作？」—— 不能直接删除；需要懒删除（延迟删除），维护一个删除计数器

---

## 135. 数据流中的第 K 大元素 (Kth Largest Element in a Stream) — 题号 703

**难度**: 简单
**标签**: 堆、设计

### 题目大意

设计一个类，构造函数接收 k 和一个数组。每次调用 add(val) 返回当前数据流中第 k 大的元素。

### 思路

**核心**：维护一个大小为 k 的小顶堆，堆顶就是第 k 大的元素。

为什么是小顶堆？因为我们要找"第 k 大"，堆中维护 k 个最大的元素，堆顶是这 k 个中最小的——即第 k 大。

新元素 val 来时：如果堆大小 < k，直接入堆；否则比较 val 与堆顶，如果 val > 堆顶，则弹出堆顶并压入 val。

**暴力**：每次插入后排序取第 k 大 —— O(n log n)，不可接受。

### 核心知识点

- 小顶堆维护第 K 大：堆大小恒为 K
- `priority_queue<int, vector<int>, greater<int>>` 即小顶堆
- 初始化优化：数组元素可以逐个 `add`

### 复杂度

- **时间复杂度**: 初始化 O(n log k)，每次 add O(log k)
- **空间复杂度**: O(k)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class KthLargest {
    priority_queue<int, vector<int>, greater<int>> pq; // 小顶堆
    int K;
public:
    KthLargest(int k, vector<int>& nums) : K(k) {
        for (int x : nums) {
            pq.push(x);
            if (pq.size() > K) pq.pop();
        }
    }

    int add(int val) {
        pq.push(val);
        if (pq.size() > K) pq.pop();
        return pq.top();
    }
};

int main() {
    vector<int> nums = {4,5,8,2};
    KthLargest kl(3, nums);
    cout << kl.add(3) << endl;  // 4
    cout << kl.add(5) << endl;  // 5
    cout << kl.add(10) << endl; // 5
    return 0;
}
```

### 常见错误

- 错误1：混淆大小顶堆方向 —— 找第 K 大用小顶堆，找第 K 小用大顶堆
- 错误2：add 时总是入堆，忘记检查堆大小是否超过 K
- 错误3：构造函数中忘记用 K 初始化成员变量

### 面试陷阱

- 陷阱1：追问「如果 k 可能动态变化怎么办？」—— 需要重新调整堆，或改用平衡树
- 陷阱2：追问「找第 K 大和第 K 小的区别？」—— 第 K 大 = 升序排序后倒数第 K 个 = 第 n-K+1 小

---

## 136. 滑动窗口中位数 (Sliding Window Median) — 题号 480

**难度**: 困难
**标签**: 堆、滑动窗口

### 题目大意

给定数组 nums 和窗口大小 k，返回每个窗口的中位数组成的数组。

### 思路

**暴力**：每个窗口排序取中位数 —— O(n × k log k)，太慢。

**优化**：复用 295 题的双堆思想，但要支持删除（窗口滑动时移除最左元素）。直接用 `priority_queue` 不支持删除，因此需要"懒删除"技巧：

- 维护两个 `multiset`（或 `priority_queue` + 哈希表记录延迟删除的元素）
- 窗口滑动时：删除左边元素（标记为待删除），添加右边元素，然后平衡双堆

**核心难题**：堆不支持删除指定元素。解决方案是用 `multiset` 替代堆（O(log n) 删除），或者用双 `multiset`。

### 核心知识点

- 双堆/双平衡树维护滑动窗口中位数
- `multiset` 的删除：`erase(find(x))` 删除一个元素，`erase(x)` 删除所有等于 x 的元素（坑！）
- 懒删除策略：用 map 记录"应该删除但还没删除"的元素

### 复杂度

- **时间复杂度**: O(n log k)（multiset）/ O(n log k)（懒删除堆）
- **空间复杂度**: O(k)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法：双 multiset
vector<double> medianSlidingWindow(vector<int>& nums, int k) {
    vector<double> res;
    multiset<int> window(nums.begin(), nums.begin() + k);
    auto mid = next(window.begin(), k / 2);

    for (int i = k; ; i++) {
        // 计算中位数
        double median = (*mid + *prev(mid, 1 - k % 2)) / 2.0;
        res.push_back(median);
        if (i == nums.size()) break;

        // 插入新元素
        window.insert(nums[i]);
        if (nums[i] < *mid) mid--;

        // 删除旧元素
        if (nums[i - k] <= *mid) mid++;
        window.erase(window.lower_bound(nums[i - k]));
    }
    return res;
}

int main() {
    vector<int> nums = {1,3,-1,-3,5,3,6,7};
    auto res = medianSlidingWindow(nums, 3);
    for (double x : res) cout << x << " ";
    return 0;
}
```

### 常见错误

- 错误1：`multiset::erase(x)` 会删除所有等于 x 的元素，应该用 `erase(find(x))` 或 `erase(lower_bound(x))`
- 错误2：中位数迭代器 `mid` 的调整方向写反 —— 插入小于 `*mid` 的元素时 `mid--`，删除小于等于 `*mid` 的元素时 `mid++`
- 错误3：k 为奇数和偶数时中位数计算公式混淆

### 面试陷阱

- 陷阱1：追问「multiset 的 erase(x) 和 erase(iterator) 有什么区别？」—— 前者删除所有等于 x 的元素 O(log n + count)，后者只删除一个 O(log n)
- 陷阱2：追问「如果不用 multiset，用堆怎么做？」—— 需要懒删除：在哈希表记录被删除的元素，取堆顶时检查是否已被删除

---

## 137. 找到 K 个最接近的元素 (Find K Closest Elements) — 题号 658

**难度**: 中等
**标签**: 堆、双指针、二分

### 题目大意

给定排序数组 arr，找到 k 个最接近 x 的元素（按差值的绝对值排序）。结果升序排列。

### 思路

**暴力**：计算每个元素与 x 的差值，排序取前 k —— O(n log n)。

**优化1（堆）**：大顶堆维护差值最大的 k 个（即要淘汰的），保留差值最小的 k 个 —— O(n log k)。

**优化2（双指针）**：因为数组有序，最接近 x 的 k 个元素一定是连续子数组。用双指针从两端收缩，直到窗口大小为 k。每次比较 `x - arr[L]` 和 `arr[R] - x`，去掉差值较大的一端 —— O(n)。

**优化3（二分+双指针）**：先用二分找到 >= x 的第一个位置，然后左右扩展。二分 O(log n) + 扩展 O(k)。

### 核心知识点

- 排序数组中"最接近"的 k 个元素一定是连续的
- 用 `lower_bound` 找插入位置
- 双指针从中间向两边扩展

### 复杂度

- **时间复杂度**: O(log n + k)（二分+双指针）/ O(n)（纯双指针收缩）
- **空间复杂度**: O(1)（不计返回结果）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法一：双指针收缩 O(n)
vector<int> findClosestElements(vector<int>& arr, int k, int x) {
    int L = 0, R = arr.size() - 1;
    while (R - L + 1 > k) {
        if (x - arr[L] <= arr[R] - x)
            R--;          // 右端更远，收缩右边
        else
            L++;          // 左端更远，收缩左边
    }
    return vector<int>(arr.begin() + L, arr.begin() + R + 1);
}

// 解法二：二分+双指针 O(log n + k)
vector<int> findClosestElements2(vector<int>& arr, int k, int x) {
    // 找插入位置
    int R = lower_bound(arr.begin(), arr.end(), x) - arr.begin();
    int L = R - 1;
    while (k--) {
        if (L < 0) R++;
        else if (R >= arr.size()) L--;
        else if (x - arr[L] <= arr[R] - x) L--;
        else R++;
    }
    return vector<int>(arr.begin() + L + 1, arr.begin() + R);
}

int main() {
    vector<int> arr = {1,2,3,4,5};
    auto res = findClosestElements(arr, 4, 3);
    for (int x : res) cout << x << " "; // 1 2 3 4
    return 0;
}
```

### 常见错误

- 错误1：双指针收缩时，当 `x - arr[L] == arr[R] - x` 时收缩哪边？题目要求差值相同时选较小元素，所以应收缩 R
- 错误2：二分+双指针时忘记处理边界（L < 0 或 R >= n）
- 错误3：大顶堆做法中比较函数写反

### 面试陷阱

- 陷阱1：追问「数组未排序怎么做？」—— 必须用堆 O(n log k) 或快速选择 O(n)
- 陷阱2：追问「当差值相等时为什么选较小元素？」—— 题目要求，这也是双指针收缩右端的原因

---

## 138. 前 K 个高频单词 (Top K Frequent Words) — 题号 692

**难度**: 中等
**标签**: 堆、哈希表、Trie

### 题目大意

给定单词列表，返回出现频率前 k 高的单词。频率相同时按字典序升序排列。

### 思路

**核心**：此题与 347 的区别在于"频率相同按字典序排序"。这意味着堆的比较器需要两步：
1. 先按频率：频率低的优先弹出（小顶堆按频率）
2. 频率相同：字典序大的优先弹出（因为最终结果要字典序升序）

所以堆中比较器：`(freq_a < freq_b) || (freq_a == freq_b && word_a > word_b)`，小顶堆的逻辑是"我们希望留在堆里的是频率高、字典序小的"，所以堆顶应该弹出频率低、字典序大的。

### 核心知识点

- 复合比较器：先频率后字典序
- 小顶堆 vs 大顶堆的选择逻辑
- `priority_queue` 自定义排序：`return a.freq > b.freq || (a.freq == b.freq && a.word < b.word)` 表示 a 的优先级低于 b 时 a 在堆顶

### 复杂度

- **时间复杂度**: O(n log k)
- **空间复杂度**: O(n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<string> topKFrequent(vector<string>& words, int k) {
    unordered_map<string, int> freq;
    for (auto& w : words) freq[w]++;

    // 小顶堆：频率低且在字典序大的优先弹出
    auto cmp = [](pair<int,string>& a, pair<int,string>& b) {
        if (a.first != b.first) return a.first > b.first; // 频率低的在堆顶
        return a.second < b.second; // 频率相同，字典序大的在堆顶（先弹出）
    };
    priority_queue<pair<int,string>, vector<pair<int,string>>, decltype(cmp)> pq(cmp);

    for (auto& [w, f] : freq) {
        pq.push({f, w});
        if (pq.size() > k) pq.pop();
    }

    vector<string> res;
    while (!pq.empty()) { res.push_back(pq.top().second); pq.pop(); }
    reverse(res.begin(), res.end()); // 堆顶是第k大，需要反转
    return res;
}

int main() {
    vector<string> words = {"i","love","leetcode","i","love","coding"};
    auto res = topKFrequent(words, 2);
    for (auto& s : res) cout << s << " "; // i love
    return 0;
}
```

### 常见错误

- 错误1：复合比较器写反 —— 频率相同应该优先弹出字典序大的，这样堆里剩的是字典序小的
- 错误2：最后忘记 `reverse`，直接返回堆顶顺序（从大到小）
- 错误3：用大顶堆存所有元素然后取前 k —— O(n log n)，不够优

### 面试陷阱

- 陷阱1：追问「如果单词来自一个流，如何实时维护？」—— 哈希表 + 堆，每次查询 Top K 时重建堆 O(n log k)
- 陷阱2：追问「能否用 Trie 优化？」—— 可以用 Trie 存储单词 + 频率，但本题核心在于排序逻辑，Trie 优势不明显

---

## 139. 添加与搜索单词 (Design Add and Search Words Data Structure) — 题号 211

**难度**: 中等
**标签**: Trie、回溯、DFS

### 题目大意

设计数据结构，支持添加单词和搜索单词。搜索单词中 `.` 可以匹配任意字母。

### 思路

**暴力（哈希表+遍历）**：add 时存入哈希表；search 时如果包含 `.`，遍历哈希表中所有同长度单词逐一比较 —— O(n×m)，n 为单词数。

**优化（Trie + DFS）**：add 时插入 Trie（O(len)）；search 时用 DFS 遍历 Trie，遇到 `.` 时遍历所有 26 个孩子 —— 理论上最坏 O(26^len)，但实际上 Trie 剪枝效果很好。

### 核心知识点

- Trie 节点结构：`TrieNode* children[26]` + `bool isEnd`
- DFS 回溯搜索带通配符的单词
- `.` 的处理：遍历所有非空子节点

### 复杂度

- **时间复杂度**: add O(L)；search 最坏 O(26^L)，但实际远小于此
- **空间复杂度**: O(N×L)，N 为单词数，L 为平均长度

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class WordDictionary {
    struct TrieNode {
        TrieNode* children[26] = {};
        bool isEnd = false;
    };
    TrieNode* root;

    bool dfs(TrieNode* node, string& word, int pos) {
        if (pos == word.size()) return node->isEnd;
        char c = word[pos];
        if (c != '.') {
            int idx = c - 'a';
            if (!node->children[idx]) return false;
            return dfs(node->children[idx], word, pos + 1);
        } else {
            // '.' 匹配任意字符
            for (int i = 0; i < 26; i++) {
                if (node->children[i] && dfs(node->children[i], word, pos + 1))
                    return true;
            }
            return false;
        }
    }

public:
    WordDictionary() { root = new TrieNode(); }

    void addWord(string word) {
        TrieNode* cur = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!cur->children[idx])
                cur->children[idx] = new TrieNode();
            cur = cur->children[idx];
        }
        cur->isEnd = true;
    }

    bool search(string word) {
        return dfs(root, word, 0);
    }
};

int main() {
    WordDictionary wd;
    wd.addWord("bad");
    wd.addWord("dad");
    wd.addWord("mad");
    cout << wd.search("pad") << endl; // 0 (false)
    cout << wd.search("bad") << endl; // 1 (true)
    cout << wd.search(".ad") << endl; // 1 (true)
    return 0;
}
```

### 常见错误

- 错误1：DFS 中 `.` 的处理忘记返回 `false`（所有26个孩子都失败时）
- 错误2：Trie 中忘了 `isEnd` 标记，搜索"a"时可能匹配到"abc"的前缀
- 错误3：递归终止条件写错：`pos == word.size()` 而非 `pos == word.size() - 1`

### 面试陷阱

- 陷阱1：追问「如果单词中包含 `*`（匹配任意长度）怎么办？」—— DFS 需要尝试跳过和不跳过两种可能，更复杂
- 陷阱2：追问「Trie vs 哈希集合的 tradeoff？」—— Trie 适合前缀/通配符搜索；哈希集合适合精确搜索

---

## 140. 单词替换 (Replace Words) — 题号 648

**难度**: 中等
**标签**: Trie、哈希表、字符串

### 题目大意

给定词根字典和句子，将句子中每个单词替换为能匹配到的最短词根。

### 思路

**暴力**：对每个单词，遍历字典中所有词根，检查是否是前缀 —— O(W×D×L)。

**优化（哈希集合）**：将词根存入哈希集合。对每个单词，从长度1到word.length逐步检查前缀是否在集合中 —— O(W×L²)，L 为单词长度。

**优化（Trie）**：将词根插入 Trie。对每个单词，沿 Trie 走，遇到 `isEnd` 即停止 —— O(W×L)。

### 核心知识点

- Trie 的前缀匹配：沿路径走，第一个 `isEnd` 节点对应最短词根
- `istringstream` 分割句子
- 字符串拼接效率

### 复杂度

- **时间复杂度**: O(N×L)，N 为句子单词数，L 为平均长度
- **空间复杂度**: O(D×L)，D 为词根数

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TrieNode {
    TrieNode* children[26] = {};
    bool isEnd = false;
};

class Solution {
    TrieNode* root = new TrieNode();

    void insert(string& word) {
        TrieNode* cur = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!cur->children[idx]) cur->children[idx] = new TrieNode();
            cur = cur->children[idx];
        }
        cur->isEnd = true;
    }

    string searchRoot(string& word) {
        TrieNode* cur = root;
        string prefix;
        for (char c : word) {
            int idx = c - 'a';
            if (!cur->children[idx]) break;
            cur = cur->children[idx];
            prefix += c;
            if (cur->isEnd) return prefix;  // 找到最短词根
        }
        return word;  // 没找到，回原单词
    }

public:
    string replaceWords(vector<string>& dictionary, string sentence) {
        for (auto& root : dictionary) insert(root);

        istringstream iss(sentence);
        string word, res;
        while (iss >> word) {
            if (!res.empty()) res += " ";
            res += searchRoot(word);
        }
        return res;
    }
};

int main() {
    Solution sol;
    vector<string> dict = {"cat","bat","rat"};
    cout << sol.replaceWords(dict, "the cattle was rattled by the battery") << endl;
    return 0;
}
```

### 常见错误

- 错误1：Trie 搜索时遇到 `isEnd` 没立即返回，继续走了更长的词根
- 错误2：句子分割时忘记处理首单词前不应有空格
- 错误3：空句子或空字典的边界情况未处理

### 面试陷阱

- 陷阱1：追问「如果词根很多但句子很短，Trie 和哈希集合哪个好？」—— 哈希集合更好，建 Trie 开销大
- 陷阱2：追问「词根之间有包含关系（如 a / ab）怎么办？」—— 遇到第一个 isEnd 就返回，保证了取最短词根

---

## 141. 键值映射 (Map Sum Pairs) — 题号 677

**难度**: 中等
**标签**: Trie、哈希表、设计

### 题目大意

实现两个操作：`insert(key, val)` 插入键值对（如果 key 已存在则覆盖）；`sum(prefix)` 返回以 prefix 为前缀的所有键的值的总和。

### 思路

**暴力**：哈希表存 key-val，sum 时遍历所有 key 检查前缀 —— O(N×L)。

**优化（Trie + 节点存储和值）**：每个 Trie 节点存储"以该节点为前缀的所有键的值之和"。插入时沿路更新每个节点的 sum；查询前缀时直接返回对应节点的 sum —— O(L)。

覆盖更新时：先查出 key 的旧值，然后沿路增加 delta = new_val - old_val。

### 核心知识点

- Trie 节点存储累加和
- 覆盖更新的 delta 技巧
- 边上存储 sum 而非只在叶子节点

### 复杂度

- **时间复杂度**: insert O(L)，sum O(L)
- **空间复杂度**: O(N×L)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class MapSum {
    struct TrieNode {
        TrieNode* children[26] = {};
        int val = 0;  // 以当前节点为前缀的所有 key 的 val 之和
    };
    TrieNode* root;
    unordered_map<string, int> mp;  // 存储原始 key-val，用于覆盖更新

public:
    MapSum() { root = new TrieNode(); }

    void insert(string key, int val) {
        int delta = val - mp[key];  // 计算增量
        mp[key] = val;
        TrieNode* cur = root;
        for (char c : key) {
            int idx = c - 'a';
            if (!cur->children[idx]) cur->children[idx] = new TrieNode();
            cur = cur->children[idx];
            cur->val += delta;  // 沿路更新
        }
    }

    int sum(string prefix) {
        TrieNode* cur = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (!cur->children[idx]) return 0;
            cur = cur->children[idx];
        }
        return cur->val;
    }
};

int main() {
    MapSum ms;
    ms.insert("apple", 3);
    cout << ms.sum("ap") << endl; // 3
    ms.insert("app", 2);
    cout << ms.sum("ap") << endl; // 5
    ms.insert("apple", 1);        // 覆盖
    cout << ms.sum("ap") << endl; // 3
    return 0;
}
```

### 常见错误

- 错误1：覆盖更新时直接用新值覆盖，而不是计算 delta 沿路更新
- 错误2：忘记在哈希表中同步更新，下次覆盖时 delta 计算错误
- 错误3：sum 查询时前缀不存在却返回了父节点的总和

### 面试陷阱

- 陷阱1：追问「如果 key 很多，如何减少 Trie 内存？」—— 用 `unordered_map<int, TrieNode*>` 替代固定 26 大小的数组
- 陷阱2：追问「如果要支持删除操作？」—— 需要沿路减去值，但要注意删除后节点可能无孩子，可惰性不清除

---

## 142. 数组中两个数的最大异或值 (Maximum XOR of Two Numbers in an Array) — 题号 421

**难度**: 中等
**标签**: Trie、位运算

### 题目大意

给定整数数组，求 nums[i] XOR nums[j] 的最大值。

### 思路

**暴力**：双重循环 O(n²)，不可接受。

**优化（Trie + 贪心）**：将每个数的 31 位二进制插入 Trie。对每个数 x，在 Trie 中尽可能走与 x 当前位相反的方向（因为异或：不同得 1）。每次沿 Trie 贪心走 31 位，得到与 x 异或的最大值，O(31n)。

**更优（哈希集合 + 逐位确定）**：从高位到低位逐位确定答案的每一位。假设前几位已经确定是 `ans`，检查 `ans | (1 << i)` 是否可达：检查数组中是否存在两数异或的前缀等于目标值。O(31n)。

### 核心知识点

- 二分 Trie 求最大异或：对每个数贪心走相反方向的位
- 位运算逐位构建答案
- `x >> i & 1` 取第 i 位

### 复杂度

- **时间复杂度**: O(31n)，31 是位宽
- **空间复杂度**: O(31n)（Trie 节点数）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法一：Trie 贪心
struct TrieNode {
    TrieNode* children[2] = {};  // 0 和 1
};

class Solution {
    TrieNode* root = new TrieNode();

    void insert(int num) {
        TrieNode* cur = root;
        for (int i = 31; i >= 0; i--) {
            int bit = (num >> i) & 1;
            if (!cur->children[bit]) cur->children[bit] = new TrieNode();
            cur = cur->children[bit];
        }
    }

    int findMaxXor(int num) {
        TrieNode* cur = root;
        int res = 0;
        for (int i = 31; i >= 0; i--) {
            int bit = (num >> i) & 1;
            int want = 1 - bit;  // 希望走相反位
            if (cur->children[want]) {
                res |= (1 << i);    // 这一位异或结果为 1
                cur = cur->children[want];
            } else {
                cur = cur->children[bit];
            }
        }
        return res;
    }

public:
    int findMaximumXOR(vector<int>& nums) {
        for (int x : nums) insert(x);
        int ans = 0;
        for (int x : nums) ans = max(ans, findMaxXor(x));
        return ans;
    }
};

int main() {
    Solution sol;
    vector<int> nums = {3,10,5,25,2,8};
    cout << sol.findMaximumXOR(nums) << endl; // 28
    return 0;
}
```

### 常见错误

- 错误1：位遍历方向从低位到高位（应该从高位到低位，高位对结果影响更大）
- 错误2：Trie 中只存了 0 分支，忘记存 1 分支
- 错误3：`want` 分支不存在时忘记走回原分支

### 面试陷阱

- 陷阱1：追问「为什么从高位向低位贪心是正确的？」—— 因为高位对数值大小的贡献远远超过所有低位之和（2^i > sum(2^j for j < i)）
- 陷阱2：追问「能否不建 Trie？」—— 可以用哈希集合逐位确定法，但 Trie 更直观

---

## 143. 课程表 (Course Schedule) — 题号 207

**难度**: 中等
**标签**: DFS、BFS、拓扑排序、图

### 题目大意

给定 n 门课程和先修关系 `prerequisites`（如 [1,0] 表示要先学 0 再学 1），判断是否可能完成所有课程（有向图是否有环）。

### 思路

**问题转化**：判断有向图是否有环。若有环，无法拓扑排序，返回 false。

**BFS（Kahn 算法）**：入度表 + 队列。所有入度为 0 的节点入队，每次出队时将邻接节点入度减 1，如果减到 0 则入队。最后检查是否所有节点都出队过。出队计数 == n 则无环。

**DFS（三色标记法）**：0=未访问，1=正在访问（当前 DFS 路径上），2=已完成。如果 DFS 时遇到状态 1 的节点，说明有环。

### 图存储方式

邻接表 `vector<vector<int>>` —— 稀疏图，入度数组辅助 BFS。

### 核心知识点

- Kahn 算法（BFS 拓扑排序）
- 三色 DFS 判环
- 有向图环检测

### 复杂度

- **时间复杂度**: O(V + E)
- **空间复杂度**: O(V + E)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法一：BFS Kahn 算法
bool canFinish(int n, vector<vector<int>>& prerequisites) {
    vector<vector<int>> graph(n);
    vector<int> indegree(n, 0);
    for (auto& p : prerequisites) {
        graph[p[1]].push_back(p[0]);  // p[1] -> p[0]
        indegree[p[0]]++;
    }

    queue<int> q;
    for (int i = 0; i < n; i++)
        if (indegree[i] == 0) q.push(i);

    int cnt = 0;
    while (!q.empty()) {
        int cur = q.front(); q.pop();
        cnt++;
        for (int nxt : graph[cur]) {
            if (--indegree[nxt] == 0) q.push(nxt);
        }
    }
    return cnt == n;
}

// 解法二：DFS 三色标记
bool dfs(int u, vector<vector<int>>& graph, vector<int>& state) {
    state[u] = 1;  // 正在访问
    for (int v : graph[u]) {
        if (state[v] == 1) return true;   // 遇到环
        if (state[v] == 0 && dfs(v, graph, state)) return true;
    }
    state[u] = 2;  // 访问完成
    return false;
}

bool canFinish_dfs(int n, vector<vector<int>>& prerequisites) {
    vector<vector<int>> graph(n);
    for (auto& p : prerequisites) graph[p[1]].push_back(p[0]);
    vector<int> state(n, 0);
    for (int i = 0; i < n; i++)
        if (state[i] == 0 && dfs(i, graph, state)) return false;
    return true;
}

int main() {
    vector<vector<int>> pre = {{1,0},{0,1}};
    cout << canFinish(2, pre) << endl; // 0 (false)
    return 0;
}
```

### 常见错误

- 错误1：边的方向建反 —— `[1,0]` 表示 0->1，建图时 `graph[1].push_back(0)` 是错的
- 错误2：Kahn 算法忘记检查初始入度为 0 的节点（可能有多个起点）
- 错误3：DFS 三色标记中 `state[v] == 1` 判断环，但漏判了 `state[v] == 0` 时递归的情况

### 面试陷阱

- 陷阱1：追问「为什么用 BFS 而不是 DFS？」—— BFS 可以同时得到拓扑序；DFS 更方便输出环的路径
- 陷阱2：追问「如果图不连通？」—— 两种算法都能处理，BFS 初始化时遍历所有节点，DFS 在主函数中遍历所有未访问节点

---

## 144. 课程表 II (Course Schedule II) — 题号 210

**难度**: 中等
**标签**: BFS、拓扑排序、图

### 题目大意

在 207 基础上，返回任意一个可行的学习顺序（拓扑序）。如果不可能，返回空数组。

### 思路

Kahn 算法的出队顺序就是拓扑序。在每次节点出队时记录到结果数组即可。

### 核心知识点

- 拓扑排序输出序列
- BFS 出队顺序即拓扑序

### 复杂度

- **时间复杂度**: O(V + E)
- **空间复杂度**: O(V + E)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<int> findOrder(int n, vector<vector<int>>& prerequisites) {
    vector<vector<int>> graph(n);
    vector<int> indegree(n, 0);
    for (auto& p : prerequisites) {
        graph[p[1]].push_back(p[0]);  // 先修 p[1] 再修 p[0]
        indegree[p[0]]++;
    }

    queue<int> q;
    for (int i = 0; i < n; i++)
        if (indegree[i] == 0) q.push(i);

    vector<int> order;
    while (!q.empty()) {
        int cur = q.front(); q.pop();
        order.push_back(cur);  // 记录拓扑序
        for (int nxt : graph[cur]) {
            if (--indegree[nxt] == 0) q.push(nxt);
        }
    }
    return order.size() == n ? order : vector<int>();
}

int main() {
    vector<vector<int>> pre = {{1,0},{2,0},{3,1},{3,2}};
    auto order = findOrder(4, pre);
    for (int x : order) cout << x << " "; // 0 1 2 3 (或其他合法拓扑序)
    return 0;
}
```

### 常见错误

- 错误1：结果数组中元素数量判断错误 —— 判断 `order.size() == n` 而非 `!order.empty()`
- 错误2：建图方向与 207 一致，但题意理解反了
- 错误3：DFS 解法中结果数组需要反转（DFS 后序遍历得到的是逆拓扑序）

### 面试陷阱

- 陷阱1：追问「DFS 得到的顺序为什么需要反转？」—— DFS 后序（节点访问完成时加入）得到的是逆拓扑序，因为最后完成的是起点
- 陷阱2：追问「如果要求字典序最小的拓扑序？」—— 用 `priority_queue<int, vector<int>, greater<int>>` 替代 `queue`

---

## 145. 最小高度树 (Minimum Height Trees) — 题号 310

**难度**: 中等
**标签**: BFS、拓扑排序、图

### 题目大意

给定无向无环图（树），选择某个节点作为根，使得树的高度最小。返回所有这样的根节点。

### 思路

**暴力**：对每个节点 BFS 求高度 —— O(n²)，超时。

**关键洞察**：最小高度树的根一定是图的"中心"。对于树来说，中心最多有 2 个。可以从叶子节点（度为 1）开始"剥洋葱"式 BFS：每轮删除所有叶子节点，最后剩下的 1 或 2 个节点就是答案。

**为什么正确？** 类比最长路径的中点：树的直径的中点即是最小高度树的根。不断剥叶子等价于不断缩小直径，最终剩下的就是中心。

### 核心知识点

- 拓扑排序思想在无向树上的应用
- "剥洋葱"法找树的中心
- 叶子节点的度 = 1（而不是入度）

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<int> findMinHeightTrees(int n, vector<vector<int>>& edges) {
    if (n == 1) return {0};

    vector<vector<int>> graph(n);
    vector<int> degree(n, 0);
    for (auto& e : edges) {
        graph[e[0]].push_back(e[1]);
        graph[e[1]].push_back(e[0]);
        degree[e[0]]++;
        degree[e[1]]++;
    }

    queue<int> q;
    for (int i = 0; i < n; i++)
        if (degree[i] == 1) q.push(i);  // 叶子节点

    int remaining = n;
    while (remaining > 2) {
        int sz = q.size();
        remaining -= sz;
        while (sz--) {
            int leaf = q.front(); q.pop();
            for (int nxt : graph[leaf]) {
                if (--degree[nxt] == 1)  // 变为新叶子
                    q.push(nxt);
            }
        }
    }

    vector<int> res;
    while (!q.empty()) { res.push_back(q.front()); q.pop(); }
    return res;
}

int main() {
    vector<vector<int>> edges = {{1,0},{1,2},{1,3}};
    auto res = findMinHeightTrees(4, edges);
    for (int x : res) cout << x << " "; // 1
    return 0;
}
```

### 常见错误

- 错误1：把叶子节点定义为 `degree[i] == 0` 而非 `degree[i] == 1`
- 错误2：忘记 n == 1 的特殊情况
- 错误3：每轮 while 循环中没控制 `sz`，而是随加随处理，导致同一轮中新增的叶子被提前处理

### 面试陷阱

- 陷阱1：追问「为什么最多 2 个答案？」—— 树的最长路径（直径）的中点最多 2 个
- 陷阱2：追问「如果不是树（有环）怎么办？」—— 不能保证最多 2 个答案，此方法不适用

---

## 146. 火星词典 (Alien Dictionary) — 题号 269

**难度**: 困难
**标签**: 拓扑排序、图、字符串

### 题目大意

给定一个已按外星字母序排列的单词列表，推导出字母的排列顺序。如果不存在合法顺序返回空字符串，有多种可能返回任意一种。

### 思路

**核心**：相邻单词比较，找到第一对不同字符，建立有向边 `a -> b`（a 在 b 前面）。然后对建好的图做拓扑排序。

**细节难点**：
1. 无效情况检测：如果 `word1` 是 `word2` 的前缀且 `word1` 更长（如 "abc" 在 "ab" 前），这是无效的
2. 字符集提取：并非所有 26 个字母都出现，只需考虑出现过的字母
3. 多种有效拓扑序：任意一个即可

### 图存储方式

邻接表 + 入度数组。因为字母最多 26 个，也可以用 `vector<unordered_set<int>>` 去重（相邻单词可能多次产生同一条边）。

### 核心知识点

- 从相邻字符串推导偏序关系
- 拓扑排序检测环
- 前缀关系导致的无效输入处理

### 复杂度

- **时间复杂度**: O(N×L)，N 个单词，L 平均长度
- **空间复杂度**: O(1)（字母集大小固定 26）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

string alienOrder(vector<string>& words) {
    // 1. 收集所有出现过的字符
    unordered_set<char> chars;
    for (auto& w : words)
        for (char c : w) chars.insert(c);

    // 2. 建图
    unordered_map<char, unordered_set<char>> graph;
    unordered_map<char, int> indegree;
    for (char c : chars) indegree[c] = 0;

    for (int i = 0; i + 1 < words.size(); i++) {
        string& a = words[i], & b = words[i + 1];
        int len = min(a.size(), b.size());
        int j = 0;
        for (; j < len; j++) {
            if (a[j] != b[j]) {
                if (!graph[a[j]].count(b[j])) {  // 避免重复边
                    graph[a[j]].insert(b[j]);
                    indegree[b[j]]++;
                }
                break;
            }
        }
        // 无效情况：a 比 b 长但 b 是 a 的前缀
        if (j == len && a.size() > b.size()) return "";
    }

    // 3. Kahn 拓扑排序
    queue<char> q;
    for (auto& [c, deg] : indegree)
        if (deg == 0) q.push(c);

    string res;
    while (!q.empty()) {
        char c = q.front(); q.pop();
        res += c;
        for (char nxt : graph[c]) {
            if (--indegree[nxt] == 0) q.push(nxt);
        }
    }

    return res.size() == chars.size() ? res : "";
}

int main() {
    vector<string> words = {"wrt","wrf","er","ett","rftt"};
    cout << alienOrder(words) << endl; // wertf
    return 0;
}
```

### 常见错误

- 错误1：没有检测无效输入 —— "abc" 排在 "ab" 前面是矛盾的
- 错误2：重复边导致入度重复累加 —— 需要去重或使用 `set` 邻接表
- 错误3：忘记只对出现过的字符建图（不存在的字符入度不会变成 0）

### 面试陷阱

- 陷阱1：追问「如果有多解，返回的是哪种？」—— Kahn 算法队列顺序决定，用 `queue` 返回任意一种，用 `priority_queue` 返回字典序最小
- 陷阱2：追问「输入只有一个单词怎么办？」—— 无法推导顺序，返回该单词中所有唯一字符的任意排列即可

---

## 147. 矩阵中的最长递增路径 (Longest Increasing Path in a Matrix) — 题号 329

**难度**: 困难
**标签**: DFS、拓扑排序、记忆化搜索

### 题目大意

给定 m×n 整数矩阵，找出最长递增路径的长度。路径只能向上下左右四个方向移动，且不能走出矩阵。

### 思路

**暴力（DFS 无记忆化）**：对每个起点 DFS 探索所有递增路径 —— O(mn × 4^(mn))，指数级，不可行。

**优化（DFS + 记忆化）**：`memo[i][j]` 表示从 (i,j) 出发的最长递增路径长度。DFS 时先查 memo，避免重复计算。每个格子的值依赖于比它大的邻居，天然形成 DAG（有向无环图），记忆化搜索本质是 DAG 上的 DP。

**视角（拓扑排序 + DP）**：矩阵可以看作 DAG，边从小的格子指向大的格子。按值从小到大排序，对每个格子取邻居中比它小的格子的最大 dp 值 +1。

### 核心知识点

- 矩阵上的记忆化 DFS
- DAG 上的最长路径 DP
- 四个方向遍历技巧：`int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}}`

### 复杂度

- **时间复杂度**: O(mn)，每个格子计算一次
- **空间复杂度**: O(mn)（memo + 递归栈）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
    int m, n;
    int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    vector<vector<int>> memo;

    int dfs(vector<vector<int>>& matrix, int i, int j) {
        if (memo[i][j] != 0) return memo[i][j];
        int maxLen = 1;
        for (auto& d : dirs) {
            int ni = i + d[0], nj = j + d[1];
            if (ni >= 0 && ni < m && nj >= 0 && nj < n
                && matrix[ni][nj] > matrix[i][j]) {
                maxLen = max(maxLen, 1 + dfs(matrix, ni, nj));
            }
        }
        return memo[i][j] = maxLen;
    }

public:
    int longestIncreasingPath(vector<vector<int>>& matrix) {
        m = matrix.size();
        n = matrix[0].size();
        memo.assign(m, vector<int>(n, 0));
        int ans = 0;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                ans = max(ans, dfs(matrix, i, j));
        return ans;
    }
};

int main() {
    Solution sol;
    vector<vector<int>> matrix = {{9,9,4},{6,6,8},{2,1,1}};
    cout << sol.longestIncreasingPath(matrix) << endl; // 4 (1->2->6->9)
    return 0;
}
```

### 常见错误

- 错误1：DFS 方向只检查递增，但忘记了边界检查
- 错误2：memo 初始化为 0，但可能和真正的最长路径 1 混淆 —— 用 -1 初始化更安全（此题 0 可以是因为最小路径是 1）
- 错误3：DFS 中比较方向写反 —— 是 `matrix[ni][nj] > matrix[i][j]` 而非 `<`

### 面试陷阱

- 陷阱1：追问「为什么记忆化搜索不会有环？」—— 因为必须是严格递增的，所以边一定有方向性（从小指向大），不会回到起点
- 陷阱2：追问「拓扑排序做法和记忆化搜索哪个好？」—— 记忆化搜索更直观；拓扑排序需要先排序，但可以避免递归栈溢出

---

## 148. 克隆图 (Clone Graph) — 题号 133

**难度**: 中等
**标签**: DFS、BFS、图、哈希表

### 题目大意

给定无向连通图的一个节点引用，返回该图的深拷贝。

### 思路

**核心**：遍历原图（BFS/DFS），用哈希表 `unordered_map<Node*, Node*> visited` 记录已克隆的节点，避免重复克隆和死循环。

- BFS：队列遍历，每次处理当前节点的所有邻居
- DFS：递归遍历，`visited` 同时充当 visited 集合和旧→新映射

### 图存储方式

题目给出的结构：`Node` 有 `val` 和 `vector<Node*> neighbors`。即邻接表表示。

### 核心知识点

- 图的深拷贝 = 遍历 + 哈希表映射
- BFS 和 DFS 两种遍历方式
- `visited` 映射同时解决"已访问"和"获取克隆节点"两个问题

### 复杂度

- **时间复杂度**: O(V + E)
- **空间复杂度**: O(V)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node() : val(0) {}
    Node(int _val) : val(_val) {}
};

// 解法一：DFS
class Solution {
    unordered_map<Node*, Node*> visited;
public:
    Node* cloneGraph(Node* node) {
        if (!node) return nullptr;
        if (visited.count(node)) return visited[node];

        Node* clone = new Node(node->val);
        visited[node] = clone;

        for (auto& nxt : node->neighbors) {
            clone->neighbors.push_back(cloneGraph(nxt));
        }
        return clone;
    }
};

// 解法二：BFS
Node* cloneGraph_bfs(Node* node) {
    if (!node) return nullptr;
    unordered_map<Node*, Node*> visited;
    queue<Node*> q;
    visited[node] = new Node(node->val);
    q.push(node);

    while (!q.empty()) {
        Node* cur = q.front(); q.pop();
        for (auto& nxt : cur->neighbors) {
            if (!visited.count(nxt)) {
                visited[nxt] = new Node(nxt->val);
                q.push(nxt);
            }
            visited[cur]->neighbors.push_back(visited[nxt]);
        }
    }
    return visited[node];
}
```

### 常见错误

- 错误1：忘记处理 `node == nullptr` 的情况
- 错误2：BFS 中把邻居加入队列后，又把当前节点的邻居链表追加操作放在了 visited 检查之后但队列 push 之前导致遗漏
- 错误3：DFS 中先创建克隆节点但忘记存入 visited，导致重复创建

### 面试陷阱

- 陷阱1：追问「如果图有自环或重边？」—— 自环：neighbors 中包含自身，visited 映射会正确处理；重边无所谓
- 陷阱2：追问「BFS 和 DFS 哪种更好？」—— 空间上 DFS 最坏 O(V)（递归栈），BFS 也是 O(V)（队列），实际差距不大

---

## 149. 省份数量 (Number of Provinces) — 题号 547

**难度**: 中等
**标签**: DFS、BFS、并查集、图

### 题目大意

给定 n 个城市的邻接矩阵 `isConnected`，求省份（连通分量）数量。

### 思路

**DFS/BFS**：对每个未访问过的城市，DFS/BFS 标记所有连通的城市，计数+1。

**并查集**：初始化 n 个独立集合，遍历矩阵上半三角（i < j），如果 `isConnected[i][j] == 1` 则合并 i 和 j。最后统计不同根的数量。

### 核心知识点

- 无向图连通分量计数
- 并查集的路径压缩和按秩合并
- 邻接矩阵到并查集的转换

### 图存储方式

题目给的是邻接矩阵，可以直接用于 DFS 或转化为并查集操作。

### 复杂度

- **时间复杂度**: O(n²)（需要遍历整个矩阵）
- **空间复杂度**: O(n)（并查集）/ O(n)（DFS 递归栈）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法一：并查集
class UnionFind {
    vector<int> parent, rank;
public:
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int find(int x) {
        return parent[x] == x ? x : (parent[x] = find(parent[x])); // 路径压缩
    }
    void unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return;
        if (rank[ra] < rank[rb]) swap(ra, rb);  // 按秩合并
        parent[rb] = ra;
        if (rank[ra] == rank[rb]) rank[ra]++;
    }
    int countRoots() {
        int cnt = 0;
        for (int i = 0; i < parent.size(); i++)
            if (parent[i] == i) cnt++;
        return cnt;
    }
};

int findCircleNum(vector<vector<int>>& isConnected) {
    int n = isConnected.size();
    UnionFind uf(n);
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (isConnected[i][j]) uf.unite(i, j);
    return uf.countRoots();
}

// 解法二：DFS
void dfs(vector<vector<int>>& g, vector<bool>& vis, int u) {
    vis[u] = true;
    for (int v = 0; v < g.size(); v++)
        if (g[u][v] && !vis[v]) dfs(g, vis, v);
}

int findCircleNum_dfs(vector<vector<int>>& isConnected) {
    int n = isConnected.size(), cnt = 0;
    vector<bool> vis(n, false);
    for (int i = 0; i < n; i++) {
        if (!vis[i]) {
            dfs(isConnected, vis, i);
            cnt++;
        }
    }
    return cnt;
}

int main() {
    vector<vector<int>> g = {{1,1,0},{1,1,0},{0,0,1}};
    cout << findCircleNum(g) << endl; // 2
    return 0;
}
```

### 常见错误

- 错误1：并查集 find 没做路径压缩，可能导致退化 O(n) 查找
- 错误2：统计根时判断 `parent[i] == i`，但之前可能有未完成压缩的节点（不过有 find 了之后最终 parent 都会指向根）
- 错误3：DFS 解法中，邻接矩阵是对称的但没有利用（遍历了全部 n 个元素而非只遍历上半部分）

### 面试陷阱

- 陷阱1：追问「并查集能处理有向图吗？」—— 不能！并查集用于无向图的连通性，有向图需要 Tarjan 或 Kosaraju 求强连通分量
- 陷阱2：追问「DFS 和并查集哪种更好？」—— 时间都是 O(n²)，并查集代码短且可以增量更新（动态加边）

---

## 150. 冗余连接 (Redundant Connection) — 题号 684

**难度**: 中等
**标签**: 并查集、图

### 题目大意

给定一个含有 n 个节点的树加上一条额外的边（共 n 条边），找出这条多余的边（返回最后出现的）。

### 思路

**并查集**：遍历每条边 (u,v)，如果 u 和 v 已经在同一集合中，说明这条边是多余的（形成了环）；否则合并 u 和 v。因为要返回"最后出现"的多余边，所以遍历到的最后一个形成环的边就是答案。

**为什么恰好一条多余边？** 树有 n-1 条边，加一条变成 n 条边，一定有且仅有一个环。

### 核心知识点

- 并查集检测无向图中的环
- 第一道"形成环"的边 vs 最后一道：本题要求最后出现，但只需要不断更新答案即可

### 复杂度

- **时间复杂度**: O(n × α(n))，α 是反阿克曼函数
- **空间复杂度**: O(n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<int> findRedundantConnection(vector<vector<int>>& edges) {
    int n = edges.size();
    vector<int> parent(n + 1), rank(n + 1, 0);
    for (int i = 1; i <= n; i++) parent[i] = i;

    function<int(int)> find = [&](int x) {
        return parent[x] == x ? x : (parent[x] = find(parent[x]));
    };

    auto unite = [&](int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return false;  // 已经在同一集合
        if (rank[ra] < rank[rb]) swap(ra, rb);
        parent[rb] = ra;
        if (rank[ra] == rank[rb]) rank[ra]++;
        return true;
    };

    for (auto& e : edges) {
        if (!unite(e[0], e[1]))
            return e;  // 形成环的边
    }
    return {};
}

int main() {
    vector<vector<int>> edges = {{1,2},{1,3},{2,3}};
    auto res = findRedundantConnection(edges);
    cout << res[0] << "," << res[1] << endl; // 2,3
    return 0;
}
```

### 常见错误

- 错误1：节点编号从 1 开始，parent 数组大小为 n（应该是 n+1）
- 错误2：遍历到成环边直接返回，但题目可能要求最后出现的 —— 此题恰好指向第一个成环边就是答案，因为前面不构成环
- 错误3：并查集的 unite 忘记先 find

### 面试陷阱

- 陷阱1：追问「为什么恰好只有一个答案？」—— n 个节点 n 条边构成恰好一个环
- 陷阱2：追问「如果多条冗余边怎么办？」—— 这是下一题 685 的情况（有向图冗余连接）

---

## 151. 冗余连接 II (Redundant Connection II) — 题号 685

**难度**: 困难
**标签**: 并查集、图

### 题目大意

给定 n 个节点的有向图（n 条边），原本是一棵有根树（每个节点父节点唯一），多加了一条有向边。找出这条多余的边。

### 思路

**关键分析**：有向树中每个节点入度 ≤ 1。加了边后可能出现：
1. **存在入度为 2 的节点**：该节点有两条入边，其中一条是多余的。分别尝试删除其中一条，检查剩下的图是否构成树（连通且无环）。
2. **所有节点入度 ≤ 1 但存在有向环**：类似 684，用并查集找到形成环的最后一条边。

**算法**：
- 先统计所有节点的入度
- 如果存在入度为 2 的节点：将指向它的两条边分别尝试删除，用并查集检查剩下的边是否构成树
- 如果所有人入度 ≤ 1：用并查集找形成有向环的边（同 684）

### 核心知识点

- 有向树的定义：根入度=0，其他节点入度=1，无环
- 入度为 2 的情况需要分情况讨论
- 并查集在"尝试删除"边时的预处理

### 复杂度

- **时间复杂度**: O(n × α(n))
- **空间复杂度**: O(n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class UnionFind {
    vector<int> parent, rank;
public:
    UnionFind(int n) {
        parent.resize(n); rank.resize(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int find(int x) {
        return parent[x] == x ? x : (parent[x] = find(parent[x]));
    }
    bool unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return false;
        if (rank[ra] < rank[rb]) swap(ra, rb);
        parent[rb] = ra;
        if (rank[ra] == rank[rb]) rank[ra]++;
        return true;
    }
};

vector<int> findRedundantDirectedConnection(vector<vector<int>>& edges) {
    int n = edges.size();
    vector<int> indegree(n + 1, 0), candA, candB;

    // 1. 统计入度，记录可能的候选边（指向同一节点的两条边）
    for (auto& e : edges) indegree[e[1]]++;
    for (auto& e : edges) {
        if (indegree[e[1]] == 2) {
            if (candA.empty()) candA = e;
            else candB = e;
        }
    }

    // 2. 如果存在入度为 2 的节点
    if (!candA.empty()) {
        // 先尝试删除 candB（后出现的那条）
        UnionFind uf(n + 1);
        bool valid = true;
        for (auto& e : edges) {
            if (e == candB) continue;  // 跳过 candB
            if (!uf.unite(e[0], e[1])) { valid = false; break; }
        }
        if (valid) return candB;  // 删 candB 后是树
        else return candA;         // 否则 candA 是多余的
    }

    // 3. 所有人入度 ≤ 1，用并查集找环
    UnionFind uf(n + 1);
    for (auto& e : edges) {
        if (!uf.unite(e[0], e[1]))
            return e;
    }
    return {};
}

int main() {
    vector<vector<int>> edges = {{1,2},{2,3},{3,4},{4,1},{1,5}};
    auto res = findRedundantDirectedConnection(edges);
    cout << res[0] << "," << res[1] << endl;
    return 0;
}
```

### 常见错误

- 错误1：入度为 2 时，先尝试删除 candA 而不是 candB —— 题目要求返回最后出现的，但逻辑应该先删 candB（后出现）检验
- 错误2：并查集的节点编号从 1 开始，初始化大小应该是 n+1
- 错误3：只有一个入度为 2 的节点时，两条候选边指向同一个节点，需要分情况判断

### 面试陷阱

- 陷阱1：追问「为什么入度为 2 时要分两种情况？」—— 因为不知道哪条边是多余的：可能删 candA 后形成环，也可能删 candB 后不连通
- 陷阱2：追问「684 和 685 的本质区别？」—— 684 是无向图（树加一条边），685 是有向图（有根树加一条边），有向图的约束更强（入度 ≤ 1）

---

## 152. 账户合并 (Accounts Merge) — 题号 721

**难度**: 中等
**标签**: 并查集、哈希表、字符串

### 题目大意

给定账户列表，每个账户包含姓名和若干邮箱。合并属于同一人的账户（有相同邮箱即视为同一人）。结果中账户的邮箱按字典序排列。

### 思路

**核心**：将每个邮箱看作节点，同一账户下的邮箱之间连边。最终每个连通分量对应一个人。

**并查集实现**：
1. 遍历所有账户，将每个账户的第一个邮箱（或其他任意一个）作为代表，与其他邮箱合并
2. 同时记录每个邮箱对应的姓名（`email -> name`）
3. 遍历所有账户中的所有邮箱，通过 find 分组
4. 对每组邮箱排序后输出

### 核心知识点

- 并查集处理集合合并问题
- 邮箱到姓名的映射
- 分组后排序

### 复杂度

- **时间复杂度**: O(N×M×α(N))，N 账户数，M 平均邮箱数
- **空间复杂度**: O(N×M)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class UnionFind {
    vector<int> parent, rank;
public:
    UnionFind(int n) {
        parent.resize(n); rank.resize(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int find(int x) {
        return parent[x] == x ? x : (parent[x] = find(parent[x]));
    }
    void unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return;
        if (rank[ra] < rank[rb]) swap(ra, rb);
        parent[rb] = ra;
        if (rank[ra] == rank[rb]) rank[ra]++;
    }
};

vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
    int n = accounts.size();
    UnionFind uf(n);
    unordered_map<string, int> emailToIdx;  // 邮箱 → 账户索引

    // 合并：同一账户下的邮箱合并 + 跨账户相同邮箱合并
    for (int i = 0; i < n; i++) {
        for (int j = 1; j < accounts[i].size(); j++) {
            string& email = accounts[i][j];
            if (emailToIdx.count(email)) {
                uf.unite(i, emailToIdx[email]);  // 跨账户合并
            } else {
                emailToIdx[email] = i;
            }
        }
    }

    // 分组
    unordered_map<int, vector<string>> groups;
    for (auto& [email, idx] : emailToIdx) {
        int root = uf.find(idx);
        groups[root].push_back(email);
    }

    // 输出
    vector<vector<string>> res;
    for (auto& [root, emails] : groups) {
        sort(emails.begin(), emails.end());
        vector<string> row = {accounts[root][0]};  // 姓名
        row.insert(row.end(), emails.begin(), emails.end());
        res.push_back(row);
    }
    return res;
}

int main() {
    vector<vector<string>> accounts = {
        {"John","john@mail.com","john_newyork@mail.com"},
        {"John","johnsmith@mail.com","john@mail.com"},
        {"Mary","mary@mail.com"}
    };
    auto res = accountsMerge(accounts);
    for (auto& r : res) {
        for (auto& s : r) cout << s << " ";
        cout << endl;
    }
    return 0;
}
```

### 常见错误

- 错误1：只合并同一账户下的邮箱，没有跨账户合并（通过 emailToIdx 处理）
- 错误2：emailToIdx 中先出现的邮箱没有被后续相同的邮箱合并
- 错误3：排序时忘记账户的姓名也应该输出

### 面试陷阱

- 陷阱1：追问「时间复杂度分析？」—— 主要是并查集操作和排序，排序的总复杂度 O(K log K)，K 是所有邮箱总数
- 陷阱2：追问「如果账户非常多，如何优化？」—— 可以用按邮箱字典序的方式避免最后的大排序

---

## 153. 除法求值 (Evaluate Division) — 题号 399

**难度**: 中等
**标签**: DFS、BFS、并查集、图

### 题目大意

给定方程式 A / B = k 的列表，以及查询 C / D，返回查询结果。如果无法确定，返回 -1。

### 思路

**图论建模**：将每个变量看作节点，A / B = k 表示两条有向边：A -> B（权重 k），B -> A（权重 1/k）。查询 C / D 等价于找 C 到 D 的路径权重之积。

**DFS/BFS**：对每个查询，从 C 出发 DFS 找 D，沿途累乘权重。

**并查集（带权）**：`weight[x] = x / parent[x]`。合并时更新权重关系。查询时如果 C 和 D 在同一集合，答案 = weight[C] / weight[D]。

### 图存储方式

邻接表 `unordered_map<string, vector<pair<string, double>>>` —— 节点是字符串，稀疏图。

### 核心知识点

- 图的最短路变种（路径权重之积）
- 带权并查集（ratio 关系）
- 建图方式：双向边

### 复杂度

- **时间复杂度**: O(Q×(V+E))（DFS）/ O((V+E+Q)×α(V))（带权并查集）
- **空间复杂度**: O(V+E)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法一：DFS
class Solution {
    unordered_map<string, vector<pair<string, double>>> graph;

    double dfs(string& cur, string& target, unordered_set<string>& vis) {
        if (!graph.count(cur) || !graph.count(target)) return -1;
        if (cur == target) return 1;
        vis.insert(cur);
        for (auto& [nxt, w] : graph[cur]) {
            if (vis.count(nxt)) continue;
            double sub = dfs(nxt, target, vis);
            if (sub != -1) return w * sub;
        }
        return -1;
    }

public:
    vector<double> calcEquation(vector<vector<string>>& equations,
                                vector<double>& values,
                                vector<vector<string>>& queries) {
        // 建图
        for (int i = 0; i < equations.size(); i++) {
            graph[equations[i][0]].push_back({equations[i][1], values[i]});
            graph[equations[i][1]].push_back({equations[i][0], 1.0 / values[i]});
        }
        vector<double> res;
        unordered_set<string> vis;
        for (auto& q : queries) {
            vis.clear();
            res.push_back(dfs(q[0], q[1], vis));
        }
        return res;
    }
};

// 解法二：带权并查集
class UnionFind {
    unordered_map<string, string> parent;
    unordered_map<string, double> weight;  // weight[x] = x / parent[x]

public:
    string find(string x) {
        if (!parent.count(x)) {
            parent[x] = x;
            weight[x] = 1.0;
        }
        if (parent[x] != x) {
            string root = find(parent[x]);
            weight[x] *= weight[parent[x]];  // 路径压缩时更新权值
            parent[x] = root;
        }
        return parent[x];
    }

    void unite(string a, string b, double val) {  // a / b = val
        string ra = find(a), rb = find(b);
        if (ra == rb) return;
        parent[ra] = rb;
        weight[ra] = val * weight[b] / weight[a];  // 推导见思路
    }

    double query(string a, string b) {
        if (!parent.count(a) || !parent.count(b)) return -1;
        if (find(a) != find(b)) return -1;
        return weight[a] / weight[b];
    }
};

int main() {
    Solution sol;
    vector<vector<string>> eq = {{"a","b"},{"b","c"}};
    vector<double> vals = {2.0, 3.0};
    vector<vector<string>> q = {{"a","c"},{"b","a"},{"a","e"}};
    auto res = sol.calcEquation(eq, vals, q);
    for (double x : res) cout << x << " ";  // 6.0 0.5 -1.0
    return 0;
}
```

### 常见错误

- 错误1：建图时只建了一条有向边，忘记反向边
- 错误2：DFS 搜索时忘记处理 vis 集合，导致死循环
- 错误3：带权并查集中 weight 的更新公式推导错误

### 面试陷阱

- 陷阱1：追问「带权并查集的 weight 更新公式如何推导？」—— a/parent[a] = weight[a]，合并时 ra 连到 rb：weight[ra] = a/ra = (a/b) * (b/rb) / (a/ra) 的整理
- 陷阱2：追问「DFS 和带权并查集分别适合什么场景？」—— 查询少用 DFS，查询多用并查集（每次查询 O(1)）

---

## 154. 等式方程的可满足性 (Satisfiability of Equality Equations) — 题号 990

**难度**: 中等
**标签**: 并查集、图、字符串

### 题目大意

给定形如 "a==b" 或 "a!=b" 的方程数组，判断是否存在满足所有方程的字母赋值。

### 思路

**核心**：先处理所有 "==" 等式，用并查集将相等的变量合并。再处理 "!=" 不等式，检查不等式两端的变量是否在同一集合中。如果在，说明矛盾。

**为什么先处理 "=="？** 因为相等关系是传递的（a==b && b==c => a==c），不等关系不是传递的。先建立所有等价类，再检查冲突。

### 核心知识点

- 并查集处理等价关系
- 先合并后检查冲突
- 字母映射到 0-25

### 复杂度

- **时间复杂度**: O(N × α(26))，N 方程数
- **空间复杂度**: O(26) = O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

bool equationsPossible(vector<string>& equations) {
    vector<int> parent(26);
    for (int i = 0; i < 26; i++) parent[i] = i;

    function<int(int)> find = [&](int x) {
        return parent[x] == x ? x : (parent[x] = find(parent[x]));
    };

    auto unite = [&](int a, int b) {
        parent[find(a)] = find(b);
    };

    // 第一遍：处理 ==
    for (auto& eq : equations) {
        if (eq[1] == '=') {
            unite(eq[0] - 'a', eq[3] - 'a');
        }
    }

    // 第二遍：处理 !=
    for (auto& eq : equations) {
        if (eq[1] == '!') {
            if (find(eq[0] - 'a') == find(eq[3] - 'a'))
                return false;  // 矛盾
        }
    }
    return true;
}

int main() {
    vector<string> eq = {"a==b","b!=a"};
    cout << equationsPossible(eq) << endl; // 0 (false)
    return 0;
}
```

### 常见错误

- 错误1：不区分 == 和 != 的处理顺序，可能导致错误判断
- 错误2：忘记 != 中的字符也可能是位置 1（检查 eq[1] 而非总是 eq[1]=='=')
- 错误3：字符到索引转换错误（eq[0] vs eq[3]）

### 面试陷阱

- 陷阱1：追问「为什么不需要处理不等关系的传递性？」—— 不等关系不是传递的（a!=b, b!=c 不推出 a!=c），只需要检查与等价类是否冲突
- 陷阱2：追问「如果变量不只是单个字母？」—— 用 `unordered_map<string, int>` 动态分配索引

---

## 155. 连通网络的操作次数 (Number of Operations to Make Network Connected) — 题号 1319

**难度**: 中等
**标签**: DFS、BFS、并查集、图

### 题目大意

给定 n 台计算机和已有的网线连接，求最少需要移动多少根网线使所有计算机连通。如果不可能返回 -1。

### 思路

**核心**：n 台计算机需要至少 n-1 根网线才能全部连通。如果 `connections.size() < n - 1`，直接返回 -1。

否则，最少操作次数 = 连通分量数 - 1。因为每个连通分量内部多余的网线可以被移到分量之间建立连接。

**并查集**：统计连通分量数，返回 `components - 1`。

### 核心知识点

- 最小边数条件：n 个节点需要至少 n-1 条边才能连通
- 连通分量数与所需操作数的关系
- 并查集统计连通分量

### 复杂度

- **时间复杂度**: O(E × α(n))
- **空间复杂度**: O(n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

int makeConnected(int n, vector<vector<int>>& connections) {
    if (connections.size() < n - 1) return -1;  // 网线不够

    vector<int> parent(n);
    for (int i = 0; i < n; i++) parent[i] = i;

    function<int(int)> find = [&](int x) {
        return parent[x] == x ? x : (parent[x] = find(parent[x]));
    };

    for (auto& c : connections) {
        parent[find(c[0])] = find(c[1]);
    }

    int components = 0;
    for (int i = 0; i < n; i++)
        if (parent[i] == i) components++;

    return components - 1;
}

int main() {
    vector<vector<int>> conn = {{0,1},{0,2},{1,2}};
    cout << makeConnected(4, conn) << endl; // 1
    return 0;
}
```

### 常见错误

- 错误1：忘记检查 `connections.size() < n - 1` 的边界情况
- 错误2：统计连通分量时没做 find 压缩就直接判断 `parent[i] == i`
- 错误3：返回 components - 1 而不是 components（连接 k 个连通分量需要 k-1 条边）

### 面试陷阱

- 陷阱1：追问「如何找到具体移动哪些网线？」—— 需要额外记录连通分量内的冗余边
- 陷阱2：追问「如果 n=1，需要多少操作？」—— 0 次，已经是连通的

---

## 156. 无向图中连通分量的数目 (Number of Connected Components in Undirected Graph) — 题号 323

**难度**: 中等
**标签**: DFS、BFS、并查集、图

### 题目大意

给定 n 个节点和边列表，求无向图中连通分量的数量。

### 思路

三种解法都可以：

1. **DFS/BFS**：对每个未访问节点进行 DFS/BFS，计数 +1
2. **并查集**：合并所有边后，统计根的数量

### 图存储方式

邻接表（DFS/BFS）或直接用边列表（并查集）。

### 复杂度

- **时间复杂度**: O(V + E)
- **空间复杂度**: O(V + E)（邻接表）/ O(V)（并查集）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法一：并查集
int countComponents(int n, vector<vector<int>>& edges) {
    vector<int> parent(n);
    for (int i = 0; i < n; i++) parent[i] = i;

    function<int(int)> find = [&](int x) {
        return parent[x] == x ? x : (parent[x] = find(parent[x]));
    };

    for (auto& e : edges) {
        parent[find(e[0])] = find(e[1]);
    }

    int cnt = 0;
    for (int i = 0; i < n; i++)
        if (parent[i] == i) cnt++;
    return cnt;
}

// 解法二：DFS
int countComponents_dfs(int n, vector<vector<int>>& edges) {
    vector<vector<int>> graph(n);
    for (auto& e : edges) {
        graph[e[0]].push_back(e[1]);
        graph[e[1]].push_back(e[0]);
    }
    vector<bool> vis(n, false);
    function<void(int)> dfs = [&](int u) {
        vis[u] = true;
        for (int v : graph[u]) if (!vis[v]) dfs(v);
    };
    int cnt = 0;
    for (int i = 0; i < n; i++)
        if (!vis[i]) { dfs(i); cnt++; }
    return cnt;
}

int main() {
    vector<vector<int>> edges = {{0,1},{1,2},{3,4}};
    cout << countComponents(5, edges) << endl; // 2
    return 0;
}
```

### 常见错误

- 错误1：并查集统计时忘记做 find 压缩
- 错误2：DFS 时图建成了有向图（只加了一条边）
- 错误3：节点从 0 开始还是 1 开始混淆

### 面试陷阱

- 陷阱1：追问「动态加边/删边场景用什么？」—— 加边用并查集（O(α(n))），删边用线段树/动态树
- 陷阱2：追问「如何输出每个连通分量的所有节点？」—— 用哈希表按根分组

---

## 157. 网络延迟时间 (Network Delay Time) — 题号 743

**难度**: 中等
**标签**: 图、最短路、Dijkstra、堆

### 题目大意

给定 n 个节点和带权有向边，从节点 k 出发发送信号，求所有节点都收到信号的最短时间（即 k 到所有节点的最短路中的最大值）。如果无法到达所有节点返回 -1。

### 思路

**朴素 Dijkstra**：O(n²)，适合稠密图。

**堆优化 Dijkstra**：O(E log V)，适合稀疏图。

**为何不用 Bellman-Ford？** 所有权重非负，Dijkstra 更优。

### 图存储方式

邻接表 `vector<vector<pair<int,int>>> graph[u] = {v, w}` —— 稀疏图典型存储。

### 核心知识点

- Dijkstra 算法：贪心 + 松弛
- 堆优化：`priority_queue<pair<int,int>, vector<...>, greater<>>` —— 小顶堆存 {距离, 节点}
- vis 标记的意义：节点出堆时标记，表示已确定最短距离

### 复杂度

- **时间复杂度**: O(E log V)（堆优化）/ O(V²)（朴素）
- **空间复杂度**: O(V + E)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

int networkDelayTime(vector<vector<int>>& times, int n, int k) {
    // 邻接表
    vector<vector<pair<int,int>>> graph(n + 1);
    for (auto& t : times)
        graph[t[0]].push_back({t[1], t[2]});  // u -> v, weight w

    const int INF = 1e9;
    vector<int> dist(n + 1, INF);
    dist[k] = 0;

    // 小顶堆：{距离, 节点}
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    pq.push({0, k});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;  // 懒惰删除：已找到更短路径
        for (auto& [v, w] : graph[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    int ans = 0;
    for (int i = 1; i <= n; i++) {
        if (dist[i] == INF) return -1;
        ans = max(ans, dist[i]);
    }
    return ans;
}

int main() {
    vector<vector<int>> times = {{2,1,1},{2,3,1},{3,4,1}};
    cout << networkDelayTime(times, 4, 2) << endl; // 2
    return 0;
}
```

### 常见错误

- 错误1：`d > dist[u]` 的懒惰删除忘记写，导致处理已过时的堆元素
- 错误2：vis 标记放在入堆时而非出堆时 —— Dijkstra 的 vis 应该在出堆时标记（确定最短距离后）
- 错误3：距离数组初始化太小，应该用 `1e9` 或 `INT_MAX/2`

### 面试陷阱

- 陷阱1：追问「Dijkstra 能处理负权边吗？」—— 不能！负权边会导致已出堆的节点距离被更新，破坏贪心性质。需要用 Bellman-Ford 或 SPFA
- 陷阱2：追问「为什么用 `priority_queue` 而不是 `set`？」—— `priority_queue` 不能更新内部元素，但可以用惰性删除；`set` 可以删除再插入但常数大

---

## 158. 概率最大的路径 (Path with Maximum Probability) — 题号 1514

**难度**: 中等
**标签**: 图、最短路、Dijkstra、堆

### 题目大意

给定无向带权边（每条边有一个成功概率），求从 start 到 end 的最大概率路径。

### 思路

**核心**：把 Dijkstra 的"最短"改成"最大"即可。由于概率都是 [0,1] 且路径概率是乘积（非负），Dijkstra 贪心仍然有效。

**关键调整**：
1. 距离初始化为 0（而非 INF）
2. 堆改为大顶堆（取最大概率）
3. 松弛条件：`prob[u] * w > prob[v]`

### 核心知识点

- Dijkstra 的"最长路"变种（适用于边权 ≤ 1 的乘积场景）
- 大顶堆取最大值
- log 变换可转化为最短路，但不需要

### 复杂度

- **时间复杂度**: O(E log V)
- **空间复杂度**: O(V + E)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

double maxProbability(int n, vector<vector<int>>& edges,
                      vector<double>& succProb, int start, int end) {
    vector<vector<pair<int,double>>> graph(n);
    for (int i = 0; i < edges.size(); i++) {
        graph[edges[i][0]].push_back({edges[i][1], succProb[i]});
        graph[edges[i][1]].push_back({edges[i][0], succProb[i]});
    }

    vector<double> prob(n, 0.0);
    prob[start] = 1.0;

    // 大顶堆：{概率, 节点}
    priority_queue<pair<double,int>> pq;
    pq.push({1.0, start});

    while (!pq.empty()) {
        auto [p, u] = pq.top(); pq.pop();
        if (p < prob[u]) continue;
        if (u == end) return p;  // 提前返回
        for (auto& [v, w] : graph[u]) {
            if (prob[u] * w > prob[v]) {
                prob[v] = prob[u] * w;
                pq.push({prob[v], v});
            }
        }
    }
    return 0.0;
}

int main() {
    vector<vector<int>> edges = {{0,1},{1,2},{0,2}};
    vector<double> prob = {0.5, 0.5, 0.2};
    cout << maxProbability(3, edges, prob, 0, 2) << endl; // 0.25
    return 0;
}
```

### 常见错误

- 错误1：继续用小顶堆而不是大顶堆
- 错误2：概率初始化为 1 而非 0（起点应该是 1）
- 错误3：松弛方向写反：`prob[u] * w > prob[v]` 而非 `<`

### 面试陷阱

- 陷阱1：追问「概率乘积为什么可以用 Dijkstra？」—— 因为是单调递减的（乘 ≤1 的数不会变大），满足 Dijkstra 的贪心性质
- 陷阱2：追问「如何避免浮点精度问题？」—— 可以取 log 转化为加法（最短路），但通常不需要

---

## 159. K 站中转内最便宜的航班 (Cheapest Flights Within K Stops) — 题号 787

**难度**: 中等
**标签**: 图、最短路、Bellman-Ford、DP

### 题目大意

给定 n 个城市和航班（带权有向边），求从 src 到 dst 最多经过 k 次中转（k+1 条边）的最便宜价格。

### 思路

**Dijkstra（加中转限制）**：堆中存储 {价格, 节点, 剩余中转次数}。但存在一个问题：价格低但消耗中转多的路径可能堵死价格高但中转少的好路径。

**Bellman-Ford（标准解法）**：进行 k+1 轮松弛。每轮松弛所有边，但防止同一轮内多次中转（需要用上一轮的距离数组做备份）。

**为什么 Bellman-Ford？** 此题本质是"经过最多 k+1 条边的最短路"，这正是 Bellman-Ford 多轮松弛的处理范围。

### 核心知识点

- Bellman-Ford 算法：进行 V-1 轮松弛，第 i 轮后求出经过最多 i 条边的最短路
- 备份数组防止"串联"更新
- Bellman-Ford 可检测负环，但此题无负权

### 复杂度

- **时间复杂度**: O(k × E)（Bellman-Ford）/ O(E log V)（BFS + 堆，但不如 BF 精准）
- **空间复杂度**: O(V)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
    const int INF = 1e9;
    vector<int> dist(n, INF);
    dist[src] = 0;

    // Bellman-Ford：进行 k+1 轮松弛（最多 k+1 条边）
    for (int i = 0; i <= k; i++) {
        vector<int> backup = dist;  // 备份防止串联更新
        for (auto& f : flights) {
            int u = f[0], v = f[1], w = f[2];
            if (dist[u] != INF) {
                backup[v] = min(backup[v], dist[u] + w);
            }
        }
        dist = move(backup);
    }
    return dist[dst] == INF ? -1 : dist[dst];
}

int main() {
    vector<vector<int>> flights = {{0,1,100},{1,2,100},{0,2,500}};
    cout << findCheapestPrice(3, flights, 0, 2, 1) << endl; // 200
    return 0;
}
```

### 常见错误

- 错误1：Bellman-Ford 忘记备份 dist 数组，导致在同一轮内飞机可以串联（中转多次），绕过了 k 的限制
- 错误2：松弛轮数写成 k 而非 k+1（k 次中转 = k+1 条边）
- 错误3：用 `dist[u] + w < backup[v]` 的比较方式，但更新的是 backup 而非 dist

### 面试陷阱

- 陷阱1：追问「为什么不能用 Dijkstra？」—— Dijkstra 是按距离贪心选择下一个节点，但这里还受中转次数限制，可能距离近但中转多的节点提前出堆，堵死距离远但中转少的好路径
- 陷阱2：追问「什么时候用 Bellman-Ford 而不是 Dijkstra？」—— 有负权边，或有步数/边数限制

---

## 160. 最低成本联通所有城市 (Connecting Cities With Minimum Cost) — 题号 1135

**难度**: 中等
**标签**: 图、并查集、最小生成树、Kruskal

### 题目大意

给定 n 个城市和带权无向边，求连接所有城市的最小成本。如果不能连通返回 -1。

### 思路

**Kruskal 算法**：按边权排序，用并查集贪心加边。如果边的两个端点不在同一集合，加入这条边并合并集合。加满 n-1 条边即得最小生成树。

**Prim 算法**：从任意节点开始，每次选择连接已选集合和未选集合的最小边。适合稠密图。

### 核心知识点

- Kruskal：排序 + 并查集
- Prim：堆 + 邻接表（类似 Dijkstra）
- 最小生成树的 n-1 条边条件

### 复杂度

- **时间复杂度**: O(E log E)（Kruskal 排序）/ O(E log V)（Prim 堆优化）
- **空间复杂度**: O(V + E)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

int minimumCost(int n, vector<vector<int>>& connections) {
    // Kruskal
    sort(connections.begin(), connections.end(),
         [](auto& a, auto& b) { return a[2] < b[2]; });

    vector<int> parent(n + 1), rank(n + 1, 0);
    for (int i = 1; i <= n; i++) parent[i] = i;

    function<int(int)> find = [&](int x) {
        return parent[x] == x ? x : (parent[x] = find(parent[x]));
    };

    int cost = 0, edges = 0;
    for (auto& c : connections) {
        int u = c[0], v = c[1], w = c[2];
        int ru = find(u), rv = find(v);
        if (ru != rv) {
            parent[ru] = rv;
            cost += w;
            edges++;
            if (edges == n - 1) break;  // 最小生成树完成
        }
    }
    return edges == n - 1 ? cost : -1;
}

int main() {
    vector<vector<int>> conn = {{1,2,5},{1,3,6},{2,3,1}};
    cout << minimumCost(3, conn) << endl; // 6
    return 0;
}
```

### 常见错误

- 错误1：排序时忘记自定义比较器（默认按第一个元素排序）
- 错误2：最终判断 `edges != n - 1` 返回 -1，但忘记处理
- 错误3：节点编号从 1 开始，parent 数组大小应为 n+1

### 面试陷阱

- 陷阱1：追问「Kruskal 和 Prim 的选择依据？」—— 稀疏图用 Kruskal（O(E log E)），稠密图用 Prim（O(V²) 朴素版）
- 陷阱2：追问「如果要求次小生成树？」—— 枚举每条不在 MST 上的边，替换 MST 中环上最大的边

---

## 161. 到达目的地的方案数 (Number of Ways to Arrive at Destination) — 题号 1976

**难度**: 中等
**标签**: 图、最短路、Dijkstra、DP

### 题目大意

给定无向带权图，求从 0 到 n-1 的最短路径数量（mod 1e9+7）。

### 思路

**Dijkstra + DP**：在 Dijkstra 松弛过程中同时维护 `ways[v]`（到达 v 的最短路径数量）。

- 如果 `dist[u] + w < dist[v]`：更新 `dist[v]`，`ways[v] = ways[u]`
- 如果 `dist[u] + w == dist[v]`：`ways[v] = (ways[v] + ways[u]) % MOD`

**关键**：必须在 Dijkstra 确定节点最短距离时（出堆时）才固定 ways 值。

### 核心知识点

- Dijkstra 过程中 DP 计数
- 等距离路径的累加
- 大数取模

### 复杂度

- **时间复杂度**: O(E log V)
- **空间复杂度**: O(V + E)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

int countPaths(int n, vector<vector<int>>& roads) {
    const int MOD = 1e9 + 7;
    vector<vector<pair<int,long long>>> graph(n);
    for (auto& r : roads) {
        graph[r[0]].push_back({r[1], r[2]});
        graph[r[1]].push_back({r[0], r[2]});
    }

    vector<long long> dist(n, LLONG_MAX);
    vector<long long> ways(n, 0);
    dist[0] = 0; ways[0] = 1;

    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
    pq.push({0, 0});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto& [v, w] : graph[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                ways[v] = ways[u];
                pq.push({dist[v], v});
            } else if (dist[u] + w == dist[v]) {
                ways[v] = (ways[v] + ways[u]) % MOD;
            }
        }
    }
    return ways[n - 1] % MOD;
}

int main() {
    vector<vector<int>> roads = {{1,0,10}};
    cout << countPaths(2, roads) << endl; // 1
    return 0;
}
```

### 常见错误

- 错误1：`ways[v]` 在距离相等时累加，但忘记取模
- 错误2：`dist` 用 int 会溢出（边权可达 1e9）
- 错误3：距离更新时忘记同时更新 ways[v] = ways[u]

### 面试陷阱

- 陷阱1：追问「如果图不连通？」—— ways[n-1] 会保持 0，答案自然为 0
- 陷阱2：追问「为什么要在出堆时判断 d > dist[u]？」—— 同 Dijkstra，可能有旧的堆元素

---

## 162. 从第一个节点出发到最后一个节点的受限路径数 (Number of Restricted Paths) — 题号 1786

**难度**: 中等
**标签**: 图、最短路、Dijkstra、DP

### 题目大意

定义受限路径：从 1 到 n 的路径，满足路径上每个节点到 n 的距离严格递减。求受限路径的数量。

### 思路

**两步走**：
1. **反向 Dijkstra**：从 n 出发，计算每个节点到 n 的最短距离 `dist[i]`
2. **DP on DAG**：按 `dist` 递增的顺序（从 n 往 1），DP 计算受限路径数。对于每条边 (u,v)，如果 `dist[u] > dist[v]`，则 u 可以走 v：`dp[u] += dp[v]`

排序方式：用一个 `vector<int>` 按 dist 升序排列所有节点。

### 核心知识点

- 反向最短路预处理距离
- 在隐含的 DAG 上 DP（dist 严格递减保证无环）
- 按 dist 排序确定 DP 顺序

### 复杂度

- **时间复杂度**: O(E log V)
- **空间复杂度**: O(V + E)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

int countRestrictedPaths(int n, vector<vector<int>>& edges) {
    const int MOD = 1e9 + 7;
    vector<vector<pair<int,int>>> graph(n + 1);
    for (auto& e : edges) {
        graph[e[0]].push_back({e[1], e[2]});
        graph[e[1]].push_back({e[0], e[2]});
    }

    // 1. Dijkstra 从 n 出发
    vector<long long> dist(n + 1, LLONG_MAX);
    dist[n] = 0;
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
    pq.push({0, n});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto& [v, w] : graph[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    // 2. 按 dist 升序排序节点
    vector<int> nodes(n);
    for (int i = 1; i <= n; i++) nodes[i-1] = i;
    sort(nodes.begin(), nodes.end(), [&](int a, int b) {
        return dist[a] < dist[b];
    });

    // 3. DP
    vector<int> dp(n + 1, 0);
    dp[n] = 1;
    for (int u : nodes) {
        for (auto& [v, w] : graph[u]) {
            if (dist[u] > dist[v]) {  // u -> v 使距离递减
                dp[u] = (dp[u] + dp[v]) % MOD;
            }
        }
    }
    return dp[1];
}
```

### 常见错误

- 错误1：Dijkstra 从 1 出发而不是从 n（受限路径的定义基于到 n 的距离）
- 错误2：DP 方向搞反 —— 应该是从 dist 小的（靠近 n）往 dist 大的（靠近 1）计算
- 错误3：边的条件写成 `dist[u] < dist[v]` 而非 `dist[u] > dist[v]`

### 面试陷阱

- 陷阱1：追问「为什么按 dist 排序后 DP 是正确的？」—— 因为受限路径要求距离严格递减，隐含形成 DAG，排序后就是拓扑序
- 陷阱2：追问「如果有重复 dist 怎么办？」—— 严格递减意味着 dist 相等时不能互相走，所以不会形成环

---

## 163. 细分图中的可到达节点 (Reachable Nodes In Subdivided Graph) — 题号 882

**难度**: 困难
**标签**: 图、最短路、Dijkstra

### 题目大意

给定无向图，每条边被细分为 cnt 个小边。从节点 0 出发，在 maxMoves 步内能到达多少节点（包括中间细分出的节点）？

### 思路

**核心**：先跑 Dijkstra 求 0 到所有原始节点的最短距离。对于每条原始边 (u,v,cnt)：

- 从 u 出发沿边可走的额外节点数 = min(cnt, maxMoves - dist[u])
- 从 v 出发沿边可走的额外节点数 = min(cnt, maxMoves - dist[v])
- 两者之和不能超过 cnt（因为边上的节点是共享的）

**结果** = 可到达的原始节点数 + 所有边上可到达的额外节点数。

### 核心知识点

- Dijkstra 预处理最短距离
- 边上的节点数计算（注意去重，两个端点不能重复走同一边上的节点）
- 题目建模：把细分节点看成边上的"中间点"

### 复杂度

- **时间复杂度**: O(E log V)
- **空间复杂度**: O(V + E)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

int reachableNodes(vector<vector<int>>& edges, int maxMoves, int n) {
    vector<vector<pair<int,int>>> graph(n);
    for (auto& e : edges) {
        graph[e[0]].push_back({e[1], e[2] + 1});  // 边权 = 细分节点数 + 1
        graph[e[1]].push_back({e[0], e[2] + 1});
    }

    // Dijkstra
    vector<int> dist(n, INT_MAX);
    dist[0] = 0;
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    pq.push({0, 0});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto& [v, w] : graph[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    // 统计
    int ans = 0;
    for (int i = 0; i < n; i++)
        if (dist[i] <= maxMoves) ans++;  // 可到达的原始节点

    for (auto& e : edges) {
        int u = e[0], v = e[1], cnt = e[2];
        int fromU = max(0, maxMoves - dist[u]);
        int fromV = max(0, maxMoves - dist[v]);
        ans += min(cnt, fromU + fromV);  // 边上的额外节点
    }
    return ans;
}
```

### 常见错误

- 错误1：图中边权设置为 cnt（细分节点数），应该是 cnt + 1（走到邻接节点本身也要 1 步）
- 错误2：边上额外节点数没有 min(cnt, ...) 限定
- 错误3：fromU 和 fromV 的和可能超过 cnt，但题目要求不重复计算

### 面试陷阱

- 陷阱1：追问「为什么边权是 cnt+1 而不是 cnt？」—— 从 u 走到 v 需要经过 cnt 个细分点再加 1 步到 v 本身
- 陷阱2：追问「如果 maxMoves 非常大怎么办？」—— Dijkstra 仍然 O(E log V)，统计公式不变

---

## 164. 最小体力消耗路径 (Path With Minimum Effort) — 题号 1631

**难度**: 中等
**标签**: 图、Dijkstra、并查集、二分

### 题目大意

给定 m×n 高度矩阵，从 (0,0) 到 (m-1,n-1)，路径的体力消耗定义为相邻格子高度差的最大绝对值。求最小体力消耗。

### 思路

**解法一（Dijkstra 变种）**：定义 `effort[i][j]` 为从起点到 (i,j) 的最小体力消耗。对于邻居 (ni,nj)，路径的体力消耗 = `max(effort[i][j], abs(h[i][j] - h[ni][nj]))`。用堆维护当前最小 effort，类似 Dijkstra。

**解法二（二分 + BFS/并查集）**：二分答案 effort，检查是否存在一条路径使得每步高度差 ≤ effort。用 BFS/DFS/并查集检查连通性。

### 核心知识点

- Dijkstra 的"瓶颈路径"变种（从"和最小"变为"最大值最小"）
- 二分 + 连通性验证
- 矩阵上的最短路

### 复杂度

- **时间复杂度**: O(mn log(mn))（Dijkstra）/ O(mn log H)（二分+BFS）
- **空间复杂度**: O(mn)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法一：Dijkstra 变种
int minimumEffortPath(vector<vector<int>>& heights) {
    int m = heights.size(), n = heights[0].size();
    vector<vector<int>> effort(m, vector<int>(n, INT_MAX));
    effort[0][0] = 0;

    int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    // {effort, i, j}
    priority_queue<array<int,3>, vector<array<int,3>>, greater<>> pq;
    pq.push({0, 0, 0});

    while (!pq.empty()) {
        auto [e, i, j] = pq.top(); pq.pop();
        if (e > effort[i][j]) continue;
        if (i == m - 1 && j == n - 1) return e;  // 提前返回
        for (auto& d : dirs) {
            int ni = i + d[0], nj = j + d[1];
            if (ni < 0 || ni >= m || nj < 0 || nj >= n) continue;
            int newEffort = max(e, abs(heights[i][j] - heights[ni][nj]));
            if (newEffort < effort[ni][nj]) {
                effort[ni][nj] = newEffort;
                pq.push({newEffort, ni, nj});
            }
        }
    }
    return 0;
}

int main() {
    vector<vector<int>> h = {{1,2,2},{3,8,2},{5,3,5}};
    cout << minimumEffortPath(h) << endl; // 2
    return 0;
}
```

### 常见错误

- 错误1：松弛条件写成了加法而非 max
- 错误2：二分+BFS 中 BFS 写法应该用队列而非递归
- 错误3：忘记在终点时可以提前返回（Dijkstra 的贪心性质保证第一个弹出来的终点就是答案）

### 面试陷阱

- 陷阱1：追问「Dijkstra 和二分哪个更好？」—— Dijkstra 一次即可，二分需要多次 BFS，通常 Dijkstra 更好
- 陷阱2：追问「为什么 Dijkstra 适用于'最大值最小'问题？」—— 因为 effort=max(...) 是单调的，且 Dijkstra 贪心性质适用于非负单调路径代价

---

## 165. 水位上升的泳池中游泳 (Swim in Rising Water) — 题号 778

**难度**: 困难
**标签**: 图、Dijkstra、并查集、二分

### 题目大意

给定 n×n 网格，每个格子有高度。在时刻 t，水位上升到 t。你可以在水位不低于格子高度时游进该格子。求从 (0,0) 到 (n-1,n-1) 的最少时间。

### 思路

**解法一（Dijkstra 变种）**：`time[i][j]` = 到达 (i,j) 所需的最小水位高度。对于邻居：`newTime = max(time[i][j], grid[ni][nj])`（因为需要水位同时满足当前格子和目标格子）。

**解法二（二分 + BFS）**：二分答案 T，检查水位为 T 时能否从起点游到终点（BFS/DFS 只走高度 ≤ T 的格子）。

### 核心知识点

- "最大值最小"类型的 Dijkstra 变种
- 松弛条件：`max(当前值, 目标格子值)`
- 二分+BFS 验证的通用套路

### 复杂度

- **时间复杂度**: O(n² log n)（Dijkstra）/ O(n² log H)（二分+BFS）
- **空间复杂度**: O(n²)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 解法一：Dijkstra
int swimInWater(vector<vector<int>>& grid) {
    int n = grid.size();
    vector<vector<int>> time(n, vector<int>(n, INT_MAX));
    time[0][0] = grid[0][0];

    int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    priority_queue<array<int,3>, vector<array<int,3>>, greater<>> pq;
    pq.push({grid[0][0], 0, 0});

    while (!pq.empty()) {
        auto [t, i, j] = pq.top(); pq.pop();
        if (t > time[i][j]) continue;
        if (i == n - 1 && j == n - 1) return t;
        for (auto& d : dirs) {
            int ni = i + d[0], nj = j + d[1];
            if (ni < 0 || ni >= n || nj < 0 || nj >= n) continue;
            int newTime = max(t, grid[ni][nj]);
            if (newTime < time[ni][nj]) {
                time[ni][nj] = newTime;
                pq.push({newTime, ni, nj});
            }
        }
    }
    return -1;
}

// 解法二：二分 + BFS
bool canReach(vector<vector<int>>& grid, int T) {
    int n = grid.size();
    if (grid[0][0] > T) return false;
    vector<vector<bool>> vis(n, vector<bool>(n, false));
    queue<pair<int,int>> q;
    q.push({0,0}); vis[0][0] = true;
    int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    while (!q.empty()) {
        auto [i, j] = q.front(); q.pop();
        if (i == n - 1 && j == n - 1) return true;
        for (auto& d : dirs) {
            int ni = i + d[0], nj = j + d[1];
            if (ni >= 0 && ni < n && nj >= 0 && nj < n
                && !vis[ni][nj] && grid[ni][nj] <= T) {
                vis[ni][nj] = true;
                q.push({ni, nj});
            }
        }
    }
    return false;
}

int swimInWater_bs(vector<vector<int>>& grid) {
    int n = grid.size();
    int L = grid[0][0], R = n * n - 1;
    while (L < R) {
        int mid = L + (R - L) / 2;
        if (canReach(grid, mid)) R = mid;
        else L = mid + 1;
    }
    return L;
}

int main() {
    vector<vector<int>> grid = {{0,2},{1,3}};
    cout << swimInWater(grid) << endl; // 3
    return 0;
}
```

### 常见错误

- 错误1：松弛条件写成 `newTime = t + grid[ni][nj]` 而非 `max(t, grid[ni][nj])`
- 错误2：二分边界：左边界应该是 `grid[0][0]`（至少需要起点的高度），右边界 `n*n-1`
- 错误3：BFS 验证时忘记检查起点是否 ≤ T

### 面试陷阱

- 陷阱1：追问「这题和 1631 最小体力消耗的异同？」—— 都是"最大值最小"问题；1631 代价是差值，778 代价是格子值本身
- 陷阱2：追问「为什么 Dijkstra 能用在这类问题上？」—— 因为新代价 = max(旧代价, 新增量)，具有单调性，且取 max 满足"路径代价非递减"

---

## 166. 太平洋大西洋水流问题 (Pacific Atlantic Water Flow) — 题号 417

**难度**: 中等
**标签**: DFS、BFS、图、矩阵

### 题目大意

给定 m×n 矩阵表示陆地高度。水从格子流向高度 ≤ 它的相邻格子。太平洋是左和上边界，大西洋是右和下边界。求所有能同时流向太平洋和大西洋的格子。

### 思路

**逆向思维（关键！）**：不从每个格子 DFS 看能否到达边界，而是从边界出发，逆流而上（从低往高）。如果太平洋的逆流和大西洋的逆流都能到达同一个格子，该格子就是答案。

**具体做法**：
1. 从太平洋边界（左列 + 上行）开始DFS/BFS，标记可到达的格子（`pac[i][j] = true`）
2. 从大西洋边界（右列 + 下行）开始DFS/BFS，标记可到达的格子（`atl[i][j] = true`）
3. 遍历所有格子，如果 `pac[i][j] && atl[i][j]`，加入答案

### 核心知识点

- 逆向 BFS/DFS：从目标出发反推
- 二维矩阵的边界遍历
- `matrix[ni][nj] >= matrix[i][j]`（逆流：从低往高走）

### 复杂度

- **时间复杂度**: O(mn)
- **空间复杂度**: O(mn)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
    int m = heights.size(), n = heights[0].size();
    vector<vector<bool>> pac(m, vector<bool>(n, false));
    vector<vector<bool>> atl(m, vector<bool>(n, false));
    int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};

    function<void(int,int,vector<vector<bool>>&)> dfs =
        [&](int i, int j, vector<vector<bool>>& ocean) {
        ocean[i][j] = true;
        for (auto& d : dirs) {
            int ni = i + d[0], nj = j + d[1];
            if (ni >= 0 && ni < m && nj >= 0 && nj < n
                && !ocean[ni][nj]
                && heights[ni][nj] >= heights[i][j]) {  // 逆向：从低向高
                dfs(ni, nj, ocean);
            }
        }
    };

    // 太平洋：上边界 + 左边界
    for (int i = 0; i < m; i++) dfs(i, 0, pac);
    for (int j = 0; j < n; j++) dfs(0, j, pac);
    // 大西洋：下边界 + 右边界
    for (int i = 0; i < m; i++) dfs(i, n - 1, atl);
    for (int j = 0; j < n; j++) dfs(m - 1, j, atl);

    vector<vector<int>> res;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            if (pac[i][j] && atl[i][j]) res.push_back({i, j});
    return res;
}

int main() {
    vector<vector<int>> h = {{1,2,2,3,5},{3,2,3,4,4},{2,4,5,3,1},{6,7,1,4,5},{5,1,1,2,4}};
    auto res = pacificAtlantic(h);
    for (auto& p : res) cout << "[" << p[0] << "," << p[1] << "] ";
    return 0;
}
```

### 常见错误

- 错误1：从每个格子出发正向 DFS（会超时），忘记逆向思维
- 错误2：逆向时水流方向搞反 —— 应该是 `heights[ni][nj] >= heights[i][j]`（从低往高）
- 错误3：四个角被重复 DFS 初始化（左列+上行、右列+下行），但没关系

### 面试陷阱

- 陷阱1：追问「为什么逆向DFS是 O(mn) 而正向是 O((mn)²)？」—— 正向每个格子都需要一次 DFS；逆向只需 2 次全图 DFS
- 陷阱2：追问「如果边界条件变了（如太平洋变成三面）怎么办？」—— 只需修改 DFS 起始的边界范围

---

## 167. 以图判树 (Graph Valid Tree) — 题号 261

**难度**: 中等
**标签**: DFS、BFS、并查集、图

### 题目大意

给定 n 个节点和边列表，判断这个无向图是否是一棵合法的树。

### 思路

**树的定义**：
1. 连通（所有节点在一个连通分量中）
2. 无环（边数 = n - 1 且连通）

**并查集**：遍历边，如果两端点在同一集合说明有环。最后检查连通分量数是否为 1。

**DFS**：从节点 0 出发 DFS，检查是否访问了所有节点（连通）且无环（`vis[neighbor]` 但 neighbor 不是父节点）。

### 核心知识点

- 树的两个必要条件：边数 = n-1，无环且连通
- 并查集判环
- DFS 判环（记录父节点）

### 复杂度

- **时间复杂度**: O(n × α(n))（并查集）/ O(V + E)（DFS）
- **空间复杂度**: O(n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

bool validTree(int n, vector<vector<int>>& edges) {
    if (edges.size() != n - 1) return false;  // 必要条件

    vector<int> parent(n);
    for (int i = 0; i < n; i++) parent[i] = i;

    function<int(int)> find = [&](int x) {
        return parent[x] == x ? x : (parent[x] = find(parent[x]));
    };

    for (auto& e : edges) {
        int ru = find(e[0]), rv = find(e[1]);
        if (ru == rv) return false;  // 有环
        parent[ru] = rv;
    }
    return true;  // 边数满足且无环 => 连通
}

int main() {
    vector<vector<int>> edges = {{0,1},{0,2},{0,3},{1,4}};
    cout << validTree(5, edges) << endl; // 1 (true)
    return 0;
}
```

### 常见错误

- 错误1：忘记检查 `edges.size() != n - 1`，直接并查集会漏判"边不够导致不连通"
- 错误2：DFS 判环时忘记传父节点参数，导致回溯时误判为环
- 错误3：n=0 或 n=1 的边界情况：空图是树吗？（n=0 通常不算，n=1 满足条件）

### 面试陷阱

- 陷阱1：追问「为什么 edges.size() == n-1 且无环就一定是树？」—— 因为 n 个节点的无环图最多 n-1 条边，恰好 n-1 且无环 => 是树（森林中只有一棵树）
- 陷阱2：追问「DFS 和并查集哪个好？」—— 都可以；并查集代码更短，DFS 可以顺便输出连通分量

---

## 168. 引爆最多的炸弹 (Detonate the Maximum Bombs) — 题号 2101

**难度**: 中等
**标签**: DFS、BFS、图、几何

### 题目大意

给定 n 个炸弹的位置 (x,y) 和爆炸半径 r。如果炸弹 A 在炸弹 B 的爆炸范围内，引爆 B 会连锁引爆 A。求引爆一个炸弹最多能引爆多少个。

### 思路

**建图**：对每对炸弹 (i,j)，如果 i 的爆炸范围覆盖 j 的中心（即 dist(i,j) ≤ r_i），则建立有向边 i → j。

**DFS/BFS**：对每个炸弹作为起点，DFS/BFS 计算能到达的节点数（连锁引爆）。取最大值。

**注意**：是有向图！A 能引爆 B 不代表 B 能引爆 A（半径不同）。

### 图存储方式

邻接表（有向图）。n ≤ 100，O(n³) 的 Floyd/DFS 均可。

### 复杂度

- **时间复杂度**: O(n³)（n 次 DFS，每次 O(n²)）
- **空间复杂度**: O(n²)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

int maximumDetonation(vector<vector<int>>& bombs) {
    int n = bombs.size();
    vector<vector<int>> graph(n);
    // 建图
    for (int i = 0; i < n; i++) {
        long long xi = bombs[i][0], yi = bombs[i][1], ri = bombs[i][2];
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            long long dx = xi - bombs[j][0];
            long long dy = yi - bombs[j][1];
            if (dx * dx + dy * dy <= ri * ri) // 距离 <= 半径
                graph[i].push_back(j);
        }
    }

    int ans = 0;
    for (int i = 0; i < n; i++) {
        vector<bool> vis(n, false);
        // BFS
        queue<int> q;
        q.push(i); vis[i] = true;
        int cnt = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop(); cnt++;
            for (int v : graph[u]) {
                if (!vis[v]) { vis[v] = true; q.push(v); }
            }
        }
        ans = max(ans, cnt);
    }
    return ans;
}

int main() {
    vector<vector<int>> bombs = {{2,1,3},{6,1,4}};
    cout << maximumDetonation(bombs) << endl; // 2
    return 0;
}
```

### 常见错误

- 错误1：距离比较忘记用 long long（坐标可达 10^5，平方后爆 int）
- 错误2：图建成无向图（炸弹 1 能引爆 2 但 2 可能引爆不了 1）
- 错误3：`dx*dx + dy*dy <= ri*ri` 写成 `<` 遗漏等于情况

### 面试陷阱

- 陷阱1：追问「为什么是有向边？」—— 因为每个炸弹的爆炸半径不同，半径大的能覆盖小的但反过来不一定
- 陷阱2：追问「n=100 时 n³ 复杂度是否可行？」—— 100³ = 1e6，完全可行

---

## 169. 重新安排行程 (Reconstruct Itinerary) — 题号 332

**难度**: 困难
**标签**: DFS、图、欧拉路径、回溯

### 题目大意

给定机票列表 [from, to]，从 "JFK" 出发，用完所有机票，返回字典序最小的行程。

### 思路

**问题本质**：求有向图的欧拉路径（使用所有边恰好一次）。

**Hierholzer 算法**：从起点出发 DFS，每次走最小字典序的边并删除。当某个节点没有出边时，将其加入结果栈。最后反转结果栈即可得到欧拉路径。

**为什么最后要反转？** Hierholzer 算法是"后进先出"的——先完成遍历的节点其实是路径末尾。反转后就得到从起点出发的正确顺序。

**为什么删除已走的边？** 避免重复走同一条边。用 `multiset` 或 `priority_queue` 存储邻居，保证每次选字典序最小的。

### 核心知识点

- 有向图欧拉路径的 Hierholzer 算法
- 后序遍历 + 反转
- `multiset` 的删除和使用

### 图存储方式

邻接表：`unordered_map<string, multiset<string>>` —— multiset 自动排序 + 支持 O(log n) 删除。

### 复杂度

- **时间复杂度**: O(E log E)
- **空间复杂度**: O(E)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<string> findItinerary(vector<vector<string>>& tickets) {
    // graph: from -> multiset of to（自动字典序排序）
    unordered_map<string, multiset<string>> graph;
    for (auto& t : tickets)
        graph[t[0]].insert(t[1]);

    vector<string> route;
    function<void(string)> dfs = [&](string airport) {
        while (!graph[airport].empty()) {
            string next = *graph[airport].begin();  // 取字典序最小的
            graph[airport].erase(graph[airport].begin());  // 删除该边
            dfs(next);
        }
        route.push_back(airport);  // 后序加入
    };

    dfs("JFK");
    reverse(route.begin(), route.end());
    return route;
}

int main() {
    vector<vector<string>> tickets = {{"MUC","LHR"},{"JFK","MUC"},{"SFO","SJC"},{"LHR","SFO"}};
    auto res = findItinerary(tickets);
    for (auto& s : res) cout << s << " "; // JFK MUC LHR SFO SJC
    return 0;
}
```

### 常见错误

- 错误1：忘记反转 route 数组（Hierholzer 算法输出的是逆序）
- 错误2：用 `unordered_set` 存储邻居，丢失了字典序排序
- 错误3：没有正确处理"死胡同"——当机票用完但还有未访问的机场时，后序入栈的设计天然处理了这个问题

### 面试陷阱

- 陷阱1：追问「为什么 Hierholzer 算法后序入栈 + 反转能处理"死胡同"？」—— 因为 DFS 后序保证了先走完的子路径才是正确的后半段，死胡同节点最早入栈（反转后在末尾）
- 陷阱2：追问「如果存在多解，如何保证最小字典序？」—— multiset 每次取 begin()（最小元素），贪心保证了字典序最小

---

## 170. 破解保险箱 (Cracking the Safe) — 题号 753

**难度**: 困难
**标签**: DFS、图、欧拉回路

### 题目大意

一个密码由 k 进制 n 位数组成。你需要构造一个最短的字符串，使得任意一个可能的 n 位密码都是它的子串。

### 思路

**建模**：将问题建模为求有向图的欧拉回路（或欧拉路径）。

- 节点：所有 n-1 位的 k 进制数（共 k^(n-1) 个）
- 边：对于每个节点 u（n-1 位），添加一位数字 d，形成新节点 v（取 u 的后 n-2 位 + d 位）。边 (u,v) 代表数字 d。

这样走完所有 k^n 条边恰好需要 k^n + n - 1 位输出（初始 n-1 位 + 每条边贡献 1 位）。

**Hierholzer 算法**：保证每个节点的入度 = 出度 (= k)，从任意节点出发求欧拉回路。

### 核心知识点

- De Bruijn 序列
- 有向图欧拉回路
- 节点编码技巧（n-1 位前缀做节点）

### 复杂度

- **时间复杂度**: O(k^n)
- **空间复杂度**: O(k^n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

string crackSafe(int n, int k) {
    int total = pow(k, n);  // 所有可能的 n 位密码数
    unordered_set<string> visited;  // 已走过的边

    string ans(n - 1, '0');  // 初始 n-1 个 0
    // 从 "00..0" 开始 DFS
    function<void(string)> dfs = [&](string node) {
        for (int i = 0; i < k; i++) {
            string edge = node + to_string(i);
            if (!visited.count(edge)) {
                visited.insert(edge);
                string next = edge.substr(1);  // 取后 n-1 位
                dfs(next);
                ans += to_string(i);  // 后序添加
            }
        }
    };

    dfs(string(n - 1, '0'));
    return ans;
}

int main() {
    cout << crackSafe(2, 2) << endl; // "00110" 或 "01100" 等
    return 0;
}
```

### 常见错误

- 错误1：节点表示搞错 —— 节点应该是 n-1 位，边代表新增的一位数字
- 错误2：初始节点写成了 n 个 '0' 而非 n-1 个
- 错误3：visited 集合存储边的方式错误，应该存储 "n位完整密码" 作为边

### 面试陷阱

- 陷阱1：追问「答案长度为什么是 k^n + n - 1？」—— 初始 n-1 位，之后每条新边（对应一个新密码）贡献 1 位，共 k^n 个密码
- 陷阱2：追问「为什么 Hierholzer 算法一定能找到欧拉回路？」—— 每个节点入度 = 出度 = k（因为可以从任意数字进入和离开），满足欧拉回路充要条件

---

## 171. 课程表 IV (Course Schedule IV) — 题号 1462

**难度**: 中等
**标签**: 图、拓扑排序、Floyd、BFS

### 题目大意

给定 n 门课程和先修关系，以及若干查询 [u,v]，判断 u 是否是 v 的先修课程（直接或间接）。

### 思路

**问题本质**：求有向图的传递闭包（transitive closure）。

**Floyd-Warshall**：O(n³)，适合 n ≤ 100。`reach[i][j]` 表示 i 是否可达 j。三重循环即可。

**BFS/DFS 拓扑排序**：对每个节点 BFS/DFS 收集可达集合，O(n×(V+E))。

### 图存储方式

邻接表 + `vector<bitset<N>>` 优化传递闭包（bitset 可以将 Floyd 的 O(n³/64)）。

### 核心知识点

- 传递闭包 = 预处理所有可达性
- Floyd-Warshall 变种（使用布尔值）
- bitset 优化传递闭包

### 复杂度

- **时间复杂度**: O(n³)（Floyd）/ O(n³/64)（bitset 优化）
- **空间复杂度**: O(n²)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<bool> checkIfPrerequisite(int n, vector<vector<int>>& prerequisites,
                                  vector<vector<int>>& queries) {
    // Floyd-Warshall 传递闭包
    vector<vector<bool>> reach(n, vector<bool>(n, false));

    for (auto& p : prerequisites)
        reach[p[0]][p[1]] = true;

    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (reach[i][k] && reach[k][j])
                    reach[i][j] = true;

    vector<bool> res;
    for (auto& q : queries)
        res.push_back(reach[q[0]][q[1]]);
    return res;
}

int main() {
    vector<vector<int>> pre = {{1,0}};
    vector<vector<int>> q = {{0,1},{1,0}};
    auto res = checkIfPrerequisite(2, pre, q);
    for (bool b : res) cout << b << " "; // 0 1
    return 0;
}
```

### 常见错误

- 错误1：Floyd 三重循环的 k 层放在最外层（标准 Floyd 写法要求 k 是最外层）
- 错误2：`reach[i][j] = reach[i][j] || (reach[i][k] && reach[k][j])` 中忘记 `reach[i][j]` 本身
- 错误3：n > 100 时 Floyd O(n³) 可能超时，考虑 bitset 或 BFS

### 面试陷阱

- 陷阱1：追问「Floyd 为什么 k 放在最外层？」—— Floyd 的 DP 含义是 `dp[k][i][j]` 表示只用前 k 个节点作为中间点时 i 到 j 是否可达，k 是阶段，必须最外层
- 陷阱2：追问「n=1000 怎么做？」—— 用拓扑排序 + BFS（对每个节点 BFS 一次），O(n×(V+E))

---

## 172. 公交路线 (Bus Routes) — 题号 815

**难度**: 困难
**标签**: BFS、图、哈希表

### 题目大意

给定若干公交车线路（每个线路有若干站），求从 S 到 T 的最少乘车次数。

### 思路

**建模**：把公交线路看作节点！如果两条线路有共同车站，则它们之间有边。从包含 S 的所有线路出发，BFS 扩展到包含 T 的线路。

**算法**：
1. 建立车站到线路的映射：`stationToRoutes[station] = {线路索引集合}`
2. BFS：队列存储线路索引，dist 数组记录到每条线路的最少乘车次数
3. 起点：所有包含 S 的线路 dist = 1
4. 终点：检查所有包含 T 的线路

**为什么把线路当节点？** 车站数量可达 10^6，线路数 ≤ 500，线路作节点更高效。

### 核心知识点

- 图建模技巧：将"线路"抽象为节点
- BFS 求无权图最短路径
- 双向映射技巧

### 复杂度

- **时间复杂度**: O(N²)，N 为线路数（≤ 500）
- **空间复杂度**: O(N² + M)，M 为车站数

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

int numBusesToDestination(vector<vector<int>>& routes, int S, int T) {
    if (S == T) return 0;

    int n = routes.size();
    // 车站 -> 线路列表
    unordered_map<int, vector<int>> st2rt;
    for (int i = 0; i < n; i++)
        for (int st : routes[i]) st2rt[st].push_back(i);

    // 线路间是否共享车站（边）
    vector<vector<int>> graph(n);
    for (auto& [st, rts] : st2rt) {
        for (int i : rts)
            for (int j : rts)
                if (i != j) graph[i].push_back(j);  // 同车站的线路互连
    }

    // BFS
    vector<int> dist(n, -1);
    queue<int> q;
    for (int rt : st2rt[S]) {  // 所有包含 S 的线路
        dist[rt] = 1;
        q.push(rt);
    }

    while (!q.empty()) {
        int rt = q.front(); q.pop();
        for (int nxt : graph[rt]) {
            if (dist[nxt] == -1) {
                dist[nxt] = dist[rt] + 1;
                q.push(nxt);
            }
        }
    }

    int ans = INT_MAX;
    for (int rt : st2rt[T])  // 所有包含 T 的线路
        if (dist[rt] != -1) ans = min(ans, dist[rt]);

    return ans == INT_MAX ? -1 : ans;
}

int main() {
    vector<vector<int>> routes = {{1,2,7},{3,6,7}};
    cout << numBusesToDestination(routes, 1, 6) << endl; // 2
    return 0;
}
```

### 常见错误

- 错误1：忘记 S == T 的特判（不需要乘车）
- 错误2：线路间建边时，同一个车站的线路两两建边 O(k²)，k 是同车站的线路数，但总边数不超过 500²
- 错误3：BFS 距离从 0 开始还是从 1 开始混淆（从 1 开始表示已经上了一辆车）

### 面试陷阱

- 陷阱1：追问「为什么把线路当节点而不是车站？」—— 车站最多 10^6，线路最多 500，建图效率天差地别
- 陷阱2：追问「如何优化同车站多条线路的两两连边？」—— 可以用虚拟节点（车站本身作为中间节点），BFS 时交替在车站和线路间跳

---

## 173. 蛇梯棋 (Snakes and Ladders) — 题号 909

**难度**: 中等
**标签**: BFS、图、矩阵

### 题目大意

N×N 棋盘按 Boustrophedon（之字形）编号 1 到 N²。某些格子有蛇（向下）或梯子（向上）。从 1 出发，每次掷骰子走 1-6 步，遇到蛇/梯子立即传送。求到 N² 的最少步数。

### 思路

**BFS**：将棋盘转化为一维数组 `board_1d[pos]`，表示从 pos 出发实际到达的位置（考虑了蛇和梯子）。从 1 开始 BFS，每次尝试 1-6 步。

**坐标转换**：需要将一维编号转换为二维坐标（处理之字形）。

**`getPos` 函数**：将编号 id 转换为 (r, c)，注意行号从底部开始且交替反向。

### 核心知识点

- BFS 求最短路径
- 一维编号与二维矩阵的之字形转换
- 蛇/梯子传送的处理

### 复杂度

- **时间复杂度**: O(N²)
- **空间复杂度**: O(N²)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

int snakesAndLadders(vector<vector<int>>& board) {
    int n = board.size(), target = n * n;

    // 一维位置 -> 实际到达位置（考虑蛇梯）
    vector<int> jump(target + 1);
    for (int i = 1; i <= target; i++) {
        // 计算 (r, c)
        int r = n - 1 - (i - 1) / n;
        int c = ((i - 1) / n) % 2 == 0
                ? (i - 1) % n           // 从左到右
                : n - 1 - (i - 1) % n;  // 从右到左
        jump[i] = board[r][c] == -1 ? i : board[r][c];
    }

    // BFS
    vector<int> dist(target + 1, -1);
    queue<int> q;
    q.push(1); dist[1] = 0;

    while (!q.empty()) {
        int cur = q.front(); q.pop();
        if (cur == target) return dist[cur];
        for (int dice = 1; dice <= 6; dice++) {
            int nxt = cur + dice;
            if (nxt > target) break;
            nxt = jump[nxt];  // 蛇或梯子传送
            if (dist[nxt] == -1) {
                dist[nxt] = dist[cur] + 1;
                q.push(nxt);
            }
        }
    }
    return -1;
}

int main() {
    vector<vector<int>> board = {
        {-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1},
        {-1,35,-1,-1,13,-1},
        {-1,-1,-1,-1,-1,-1},
        {-1,15,-1,-1,-1,-1}
    };
    cout << snakesAndLadders(board) << endl; // 4
    return 0;
}
```

### 常见错误

- 错误1：之字形坐标转换公式写错（行号和列号的反向）
- 错误2：被蛇传送后 BFS 应该从传送目标继续，但误从传送前的位置继续
- 错误3：`jump` 数组没有处理 `board[r][c] == -1` 的情况（没有传送则保持不变）

### 面试陷阱

- 陷阱1：追问「坐标转换如何推导？」—— 行从下往上：`r = n-1 - floor((id-1)/n)`；列交替：偶数行从左到右，奇数行从右到左
- 陷阱2：追问「如果蛇/梯子形成死循环怎么办？」—— 题目保证不会，但可以加 visited 防无限循环

---

## 174. 查找集群内的关键连接 (Critical Connections in a Network) — 题号 1192

**难度**: 困难
**标签**: 图、Tarjan、DFS

### 题目大意

给定无向连通图，找所有"桥"（删除后图不再连通的边）。

### 思路

**Tarjan 算法**（求无向图的桥）：

- `dfn[u]`：节点 u 首次被访问的时间戳
- `low[u]`：从 u 出发通过 DFS 树边或至多一条回边能到达的最小 dfn

**桥的判定**：对于 DFS 树边 (u,v)（u 是 v 的父节点），如果 `low[v] > dfn[u]`，说明 v 及其子树无法通过回边到达 u 及其祖先，那么 (u,v) 是桥。

**注意无向图**：DFS 时记录父节点，避免沿树边回走（即忽略 `v == parent`）。

### 核心知识点

- Tarjan 算法求桥（或割点）
- `dfn` 和 `low` 数组的定义和维护
- DFS 树与回边的概念

### 复杂度

- **时间复杂度**: O(V + E)
- **空间复杂度**: O(V)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections) {
    vector<vector<int>> graph(n);
    for (auto& c : connections) {
        graph[c[0]].push_back(c[1]);
        graph[c[1]].push_back(c[0]);
    }

    vector<int> dfn(n, -1), low(n, -1);
    vector<vector<int>> bridges;
    int time = 0;

    function<void(int,int)> dfs = [&](int u, int parent) {
        dfn[u] = low[u] = time++;
        for (int v : graph[u]) {
            if (v == parent) continue;  // 忽略父节点（树边反向）
            if (dfn[v] == -1) {          // v 未访问
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u])    // 桥的判定条件
                    bridges.push_back({u, v});
            } else {                     // 回边
                low[u] = min(low[u], dfn[v]);
            }
        }
    };

    dfs(0, -1);
    return bridges;
}

int main() {
    vector<vector<int>> conn = {{0,1},{1,2},{2,0},{1,3}};
    auto res = criticalConnections(4, conn);
    for (auto& b : res) cout << b[0] << "-" << b[1] << endl; // 1-3
    return 0;
}
```

### 常见错误

- 错误1：回边更新 `low[u]` 时用 `low[v]` 而非 `dfn[v]`（回边应该用 dfn[v] 因为只能沿一条回边）
- 错误2：忘记 `if (v == parent) continue`，导致把树边当作回边
- 错误3：桥的条件写成 `low[v] >= dfn[u]`（这是割点的条件，桥应该是 `>`）

### 面试陷阱

- 陷阱1：追问「low[v] > dfn[u] 为什么是桥的条件？」—— 因为 low[v] 是 v 及其子树能到达的最小编号，如果它大于 dfn[u]，说明 v 无回边能到达 u 或以上
- 陷阱2：追问「Tarjan 求桥和求割点的区别？」—— 桥：`low[v] > dfn[u]`；割点：`low[v] >= dfn[u]`（对非根节点），根节点需要至少两个子节点

---

## 175. 可以到达所有点的最少点数目 (Minimum Number of Vertices to Reach All Nodes) — 题号 1557

**难度**: 中等
**标签**: 图、入度

### 题目大意

给定有向无环图，找出最小的点集，使得从这些点出发可以到达所有节点。

### 思路

**关键洞察**：入度为 0 的节点必须全部被选中（因为没有任何其他节点能到达它们）。而且，选中所有入度为 0 的节点后，所有节点都可以到达（因为 DAG 可以从入度为 0 的节点出发到达任何节点）。

**证明**：在 DAG 中，任何节点都可以从其某个祖先出发到达，追溯祖先直到入度为 0 的节点。

### 核心知识点

- 有向无环图中入度 = 0 的节点是最小可达集
- DAG 的性质：不存在环，所有节点都可以追溯到入度为 0 的源

### 复杂度

- **时间复杂度**: O(V + E)
- **空间复杂度**: O(V)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<int> findSmallestSetOfVertices(int n, vector<vector<int>>& edges) {
    vector<int> indegree(n, 0);
    for (auto& e : edges) indegree[e[1]]++;

    vector<int> res;
    for (int i = 0; i < n; i++)
        if (indegree[i] == 0) res.push_back(i);
    return res;
}

int main() {
    vector<vector<int>> edges = {{0,1},{0,2},{2,5},{3,4},{4,2}};
    auto res = findSmallestSetOfVertices(6, edges);
    for (int x : res) cout << x << " "; // 0 3
    return 0;
}
```

### 常见错误

- 错误1：复杂化问题，想用 DFS/BFS/强连通分量
- 错误2：忘记题目保证是 DAG，用 Tarjan/SCC 过于复杂
- 错误3：入度数组忘记初始化

### 面试陷阱

- 陷阱1：追问「如果不是 DAG 而是普通有向图？」—— 需要缩点（Tarjan/Kosaraju 找 SCC），将 SCC 缩成一个 DAG，再找 DAG 中入度为 0 的 SCC
- 陷阱2：追问「为什么入度为 0 的节点集就是最小的？」—— 因为这些节点互不可达，必须全部选中；且选中后可达所有节点

---

## 176. 找到最终的安全状态 (Find Eventual Safe States) — 题号 802

**难度**: 中等
**标签**: DFS、BFS、拓扑排序、图

### 题目大意

给定有向图，安全节点定义为所有可能路径最终都能到达终端节点（无出边节点）。求所有安全节点（升序）。

### 思路

**三色 DFS**：0=未访问，1=正在访问或在不安全循环中，2=安全。

**反向拓扑排序（更优雅）**：将所有边反向。原图中安全节点 = 从终端节点出发在反向图中可以到达的节点（反向拓扑排序）。

**反向图 Kahn**：
1. 建反向图，统计原图的出度（在反向图中就是入度）
2. 将所有出度为 0 的节点入队（安全节点）
3. Kahn 算法处理反向图
4. 所有出队的节点就是安全节点

### 核心知识点

- 三色 DFS 判定安全节点
- 反向图 + 拓扑排序
- 安全节点的定义：不出现在任何环中，也不通向环

### 复杂度

- **时间复杂度**: O(V + E)
- **空间复杂度**: O(V + E)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 反向图 + 拓扑排序
vector<int> eventualSafeNodes(vector<vector<int>>& graph) {
    int n = graph.size();
    // 反向图
    vector<vector<int>> rev(n);
    vector<int> outdegree(n, 0);
    for (int i = 0; i < n; i++) {
        for (int v : graph[i]) {
            rev[v].push_back(i);  // 反向边: v -> i
        }
        outdegree[i] = graph[i].size();
    }

    queue<int> q;
    for (int i = 0; i < n; i++)
        if (outdegree[i] == 0) q.push(i);  // 终端节点是安全的

    vector<int> res;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        res.push_back(u);  // 安全的节点
        for (int v : rev[u]) {  // v -> u (反向)
            if (--outdegree[v] == 0) q.push(v);
        }
    }
    sort(res.begin(), res.end());  // 升序输出
    return res;
}

int main() {
    vector<vector<int>> graph = {{1,2},{2,3},{5},{0},{5},{},{}};
    auto res = eventualSafeNodes(graph);
    for (int x : res) cout << x << " "; // 2 4 5 6
    return 0;
}
```

### 常见错误

- 错误1：正着做拓扑排序（入度为 0 的节点入队），但安全节点应该从出度为 0 的开始
- 错误2：反向图中"入度"实际对应原图的"出度"
- 错误3：三色 DFS 中，从"状态 1"节点返回时误标记为安全

### 面试陷阱

- 陷阱1：追问「为什么反向拓扑排序能找安全节点？」—— 在反向图中，安全的定义变为"从终端出发能反向到达的节点"，拓扑排序正好从出度=0 的节点开始扩散
- 陷阱2：追问「三色 DFS vs 反向拓扑排序？」—— 三色 DFS 更直观；反向拓扑排序代码更简洁，且天然得到拓扑序

---

## 177. 找到小镇的法官 (Find the Town Judge) — 题号 997

**难度**: 简单
**标签**: 图、入度/出度

### 题目大意

有 n 个人，法官满足：1. 不信任任何人（出度=0）；2. 被所有其他人信任（入度=n-1）；3. 恰好一个法官。

### 思路

**数组计数**：用两个数组 `trusts[i]`（i 信任的人数，即出度）和 `trusted[i]`（信任 i 的人数，即入度）。遍历 trust 关系更新两数组。最后找 `trusts[i] == 0 && trusted[i] == n - 1` 的 i。

### 核心知识点

- 图论问题简化：不需要建图，只需要统计出入度
- 法官的两个条件的数学表达

### 复杂度

- **时间复杂度**: O(N)，N 是 trust 数组长度
- **空间复杂度**: O(n)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

int findJudge(int n, vector<vector<int>>& trust) {
    vector<int> trusts(n + 1, 0);   // 出度：信任别人
    vector<int> trusted(n + 1, 0);  // 入度：被信任

    for (auto& t : trust) {
        trusts[t[0]]++;
        trusted[t[1]]++;
    }

    for (int i = 1; i <= n; i++) {
        if (trusts[i] == 0 && trusted[i] == n - 1)
            return i;
    }
    return -1;
}

int main() {
    vector<vector<int>> trust = {{1,3},{2,3}};
    cout << findJudge(3, trust) << endl; // 3
    return 0;
}
```

### 常见错误

- 错误1：只检查 `trusted[i] == n-1`，忘记检查 `trusts[i] == 0`
- 错误2：忘记 n=1 且 trust 为空的情况（法官就是 1）
- 错误3：节点编号从 1 开始数组大小是 n+1

### 面试陷阱

- 陷阱1：追问「如果有多个法官候选怎么办？」—— 题目保证恰好一个或不存在的法官
- 陷阱2：追问「能否只用一个数组？」—— 可以：`degree[i] = trusted[i] - trusts[i]`，法官的 degree = n-1

---

## 178. 所有可能的路径 (All Paths From Source to Target) — 题号 797

**难度**: 中等
**标签**: DFS、BFS、图、回溯

### 题目大意

给定有向无环图（DAG），求所有从节点 0 到节点 n-1 的路径。

### 思路

**DFS 回溯**：从节点 0 出发 DFS，维护当前路径。到达 n-1 时记录路径。由于是 DAG，无需 visited 数组（不会走回头路）。

**为什么不需要 visited？** DAG 中不可能有环，从任意节点出发不会回到已访问节点。

### 图存储方式

题目给定了邻接表（`graph[i]` 是节点 i 可达的邻居列表）。

### 核心知识点

- DFS 回溯找所有路径
- DAG 上无环，无需 visited
- 路径保存和回溯（push/pop）

### 复杂度

- **时间复杂度**: O(2^V × V) 最坏（路径数可达 2^V）
- **空间复杂度**: O(V)（递归深度）

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
    vector<vector<int>> res;
    vector<int> path;
    int n = graph.size();

    function<void(int)> dfs = [&](int u) {
        path.push_back(u);
        if (u == n - 1) {
            res.push_back(path);
        } else {
            for (int v : graph[u]) {
                dfs(v);
            }
        }
        path.pop_back();  // 回溯
    };

    dfs(0);
    return res;
}

int main() {
    vector<vector<int>> graph = {{1,2},{3},{3},{}};
    auto res = allPathsSourceTarget(graph);
    for (auto& p : res) {
        for (int x : p) cout << x << " ";
        cout << endl;
    }
    return 0;
}
```

### 常见错误

- 错误1：忘记回溯 pop_back，导致路径中残留其他节点
- 错误2：加了 visited 数组导致路径无法经过同一节点（但 DAG 中路径确实不会重复访问同一节点，所以加了也没错但没必要）
- 错误3：到达 n-1 后忘记 continue 或 return，继续 DFS

### 面试陷阱

- 陷阱1：追问「如果不是 DAG 而是普通图怎么办？」—— 需要 visited 数组，且到达目标后回溯时需清除 visited 标记
- 陷阱2：追问「路径数量可能很大，如何优化内存？」—— 可以按需生成（用生成器模式），但 C++ 中通常直接存

---

## 179. 可能的二分法 (Possible Bipartition) — 题号 886

**难度**: 中等
**标签**: DFS、BFS、并查集、二分图

### 题目大意

将 n 个人分成两组，给定不喜欢关系（无向边），不喜欢的两人不能在同一组。判断是否可能。

### 思路

**二分图判定**：将"不喜欢"视为边，问题等价于判断图是否是二分图。

**DFS 染色**：任选一个未染色节点染 0，将其邻居染 1，邻居的邻居染 0...如果发现冲突则不可二分。

**并查集**：对于每个人 i，将他不喜欢的那些人合并到同一个集合中（敌人的敌人是朋友）。如果发现 i 和讨厌的人在同一个集合中，矛盾。

### 核心知识点

- 二分图判定：DFS/BFS 二染色
- 并查集处理"敌人的敌人是朋友"
- 图的染色冲突检测

### 复杂度

- **时间复杂度**: O(V + E)
- **空间复杂度**: O(V + E)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// DFS 染色
bool possibleBipartition(int n, vector<vector<int>>& dislikes) {
    vector<vector<int>> graph(n + 1);
    for (auto& d : dislikes) {
        graph[d[0]].push_back(d[1]);
        graph[d[1]].push_back(d[0]);
    }

    vector<int> color(n + 1, -1);  // -1 未染色, 0 和 1 两组

    function<bool(int,int)> dfs = [&](int u, int c) {
        color[u] = c;
        for (int v : graph[u]) {
            if (color[v] == c) return false;      // 同色冲突
            if (color[v] == -1 && !dfs(v, 1 - c)) return false;
        }
        return true;
    };

    for (int i = 1; i <= n; i++)
        if (color[i] == -1 && !dfs(i, 0)) return false;
    return true;
}

// 并查集（种类并查集）
bool possibleBipartition_uf(int n, vector<vector<int>>& dislikes) {
    vector<int> parent(n + 1);
    for (int i = 1; i <= n; i++) parent[i] = i;

    function<int(int)> find = [&](int x) {
        return parent[x] == x ? x : (parent[x] = find(parent[x]));
    };

    vector<vector<int>> graph(n + 1);
    for (auto& d : dislikes) {
        graph[d[0]].push_back(d[1]);
        graph[d[1]].push_back(d[0]);
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j < graph[i].size(); j++) {
            // 把 i 的所有不喜欢的对象合并到一起
            parent[find(graph[i][0])] = find(graph[i][j]);
        }
    }

    for (int i = 1; i <= n; i++) {
        for (int j : graph[i]) {
            if (find(i) == find(j)) return false;  // 矛盾
        }
    }
    return true;
}

int main() {
    vector<vector<int>> dislikes = {{1,2},{1,3},{2,4}};
    cout << possibleBipartition(4, dislikes) << endl; // 1
    return 0;
}
```

### 常见错误

- 错误1：DFS 染色时忘记处理非连通图（主函数中需要遍历所有节点）
- 错误2：并查集方法中只合并了前两个不喜欢的对象，但需要合并所有
- 错误3：节点编号从 1 开始，数组大小是 n+1

### 面试陷阱

- 陷阱1：追问「二分图和二分法的关系？」—— 二分法就是判断图能否二染色，即判断是否为二分图
- 陷阱2：追问「DFS 染色和并查集哪种更好？」—— DFS 染色 O(V+E) 且代码直观；并查集 O(E×α(n))，可以做增量查询

---

## 180. 判断二分图 (Is Graph Bipartite?) — 题号 785

**难度**: 中等
**标签**: DFS、BFS、并查集、图

### 题目大意

判断给定的无向图是否是二分图。

### 思路

同 886，本质是二分图判定。用 BFS/DFS 对图染色即可。

**BFS 染色**：从每个未染色节点出发 BFS，交替染色。如果邻居已染色且颜色相同，则冲突。

**并查集**：对每个节点的邻居集合进行合并（敌人合并到同一个集合）。

### 核心知识点

- 二分图的充要条件：图中不含奇环
- BFS/DFS 二染色
- 图的遍历 + 颜色标记

### 复杂度

- **时间复杂度**: O(V + E)
- **空间复杂度**: O(V)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// BFS 染色
bool isBipartite(vector<vector<int>>& graph) {
    int n = graph.size();
    vector<int> color(n, -1);  // -1 未染色, 0 和 1

    for (int i = 0; i < n; i++) {
        if (color[i] != -1) continue;

        queue<int> q;
        q.push(i);
        color[i] = 0;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : graph[u]) {
                if (color[v] == color[u]) return false;  // 同色冲突
                if (color[v] == -1) {
                    color[v] = 1 - color[u];  // 交替染色
                    q.push(v);
                }
            }
        }
    }
    return true;
}

// DFS 染色
bool isBipartite_dfs(vector<vector<int>>& graph) {
    int n = graph.size();
    vector<int> color(n, -1);

    function<bool(int,int)> dfs = [&](int u, int c) {
        color[u] = c;
        for (int v : graph[u]) {
            if (color[v] == c) return false;
            if (color[v] == -1 && !dfs(v, 1 - c)) return false;
        }
        return true;
    };

    for (int i = 0; i < n; i++)
        if (color[i] == -1 && !dfs(i, 0)) return false;
    return true;
}

int main() {
    vector<vector<int>> graph = {{1,3},{0,2},{1,3},{0,2}};
    cout << isBipartite(graph) << endl; // 1 (true)
    graph = {{1,2,3},{0,2},{0,1,3},{0,2}};
    cout << isBipartite(graph) << endl; // 0 (false)
    return 0;
}
```

### 常见错误

- 错误1：只从一个节点出发 BFS/DFS，忘记图可能不连通
- 错误2：染色逻辑写错——应该 `color[v] = 1 - color[u]` 而非固定的 0 或 1
- 错误3：BFS 中入队时染色和出队时染色的区别（应该入队时就染色，避免重复入队）

### 面试陷阱

- 陷阱1：追问「二分图判定的时间复杂度为什么是 O(V+E)？」—— 每个节点和每条边都只处理一次
- 陷阱2：追问「如何找出一个奇环来证明不是二分图？」—— 可以在 DFS 中记录父节点，发现冲突时回溯构造环

---

> **阶段四总结**：本阶段覆盖堆(8)、Trie(4)、拓扑排序(5)、并查集(8)、最短路径(12)、图论杂项(13)共50道题。核心难点：Dijkstra的变种（最大值最小路径、方案计数）、Bellman-Ford的边数限制、Tarjan求桥、Hierholzer欧拉路径、双堆维护中位数。反复练习，做到"只见题目，代码自动从指尖流出"。





# LeetCode Hot200 刷题文档 — 阶段五：冲刺（25题）

---

## 181. 下一个排列 (Next Permutation) — 题号 31

**难度**: 中等
**标签**: 数组、双指针

### 题目大意

给定一个整数数组 `nums`，将其重新排列为字典序中下一个更大的排列。如果不存在下一个更大的排列（数组已是降序），则将数组重新排列为最小的排列（升序）。必须原地修改，只允许使用常数级额外空间。

### 思路

**暴力思路**：生成所有排列，排序后找下一个。时间复杂度 O(n!)，不可接受。

**优化推导**：观察字典序规律。以 `[1, 2, 3]` 为例，全排列顺序为 `123 → 132 → 213 → 231 → 312 → 321`。核心规律：

1. 下一个排列总是希望**尽量靠右**的位置变大，使得增幅最小。
2. 从右往左找**第一个相邻升序对** `(i, i+1)` 满足 `nums[i] < nums[i+1]`。这个 `i` 就是需要换掉的"转折点"——因为 `i` 右侧已经是降序，无法仅通过重排右侧得到更大的排列。
3. 在 `i` 右侧的降序区间中，从右往左找**第一个大于 `nums[i]` 的数** `nums[j]`（恰好比 `nums[i]` 大的最小数），交换二者。
4. 交换后 `i` 右侧仍然是降序，将其**反转**为升序，即得到最小的后缀排列。

**STL 原理**：`std::next_permutation` 正是以上算法的实现。它返回 `bool` 表示是否存在下一个排列。

### 核心知识点

- 字典序下一个排列算法（两遍扫描）
- 降序区间反转 = 最小化后缀

### 复杂度

- **时间复杂度**: O(n) — 三次遍历（找 i、找 j、反转）均为 O(n)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

// 手动实现 next_permutation
class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        int n = nums.size();
        // 1. 从右往左找第一个 nums[i] < nums[i+1] 的位置
        int i = n - 2;
        while (i >= 0 && nums[i] >= nums[i+1]) --i;

        if (i >= 0) {
            // 2. 从右往左找第一个大于 nums[i] 的数
            int j = n - 1;
            while (j > i && nums[j] <= nums[i]) --j;
            swap(nums[i], nums[j]);
        }
        // 3. 反转 i+1 到末尾（无论 i 是否 >= 0 都要反转）
        reverse(nums.begin() + i + 1, nums.end());
    }
};

// STL 写法（面试中可提）
// void nextPermutation(vector<int>& nums) {
//     if (!next_permutation(nums.begin(), nums.end()))
//         sort(nums.begin(), nums.end());  // 不存在下一个时回到最小
// }

int main() {
    Solution s;
    vector<int> v1 = {1, 2, 3};
    s.nextPermutation(v1);  // [1,3,2]
    for (int x : v1) cout << x << " "; cout << endl;

    vector<int> v2 = {3, 2, 1};
    s.nextPermutation(v2);  // [1,2,3]
    for (int x : v2) cout << x << " "; cout << endl;

    vector<int> v3 = {1, 1, 5};
    s.nextPermutation(v3);  // [1,5,1]
    for (int x : v3) cout << x << " "; cout << endl;
    return 0;
}
```

### 常见错误

- **错误1**：找 `i` 的条件写成 `nums[i] > nums[i+1]`，应找**第一个** `nums[i] < nums[i+1]`。
- **错误2**：当 `i < 0`（整个数组降序）时忘记反转，应该 `reverse` 整个数组回到最小排列。
- **错误3**：找 `j` 时条件是 `nums[j] <= nums[i]`（有等于号），因为要严格大于 `nums[i]`。
- **错误4**：交换后不反转，直接返回。交换后 `i` 右侧仍然是降序，必须反转为升序才得到最小后缀。

### 面试陷阱

- **陷阱1**：面试官会问"重复元素怎么处理？"——使用 `<=` 和 `<` 的条件天然处理重复元素，`[1,1,5] → [1,5,1]`。
- **陷阱2**：面试官追问"如何证明算法正确性？"——核心是：寻找增幅最小的下一个排列等同于寻找最靠右的可变大位置 + 最小化后缀。
- **陷阱3**："时间复杂度能更优吗？"——不能，最坏情况需要扫描整个数组。

---

## 182. 旋转图像 (Rotate Image) — 题号 48

**难度**: 中等
**标签**: 数组、矩阵、数学

### 题目大意

给定一个 n x n 的二维矩阵，将其顺时针旋转 90 度。必须原地旋转，不能使用额外矩阵。

### 思路

**暴力思路**：新建一个 n x n 矩阵，`new[j][n-1-i] = old[i][j]`。需要 O(n²) 额外空间，不满足原地要求。

**优化推导**：顺时针旋转 90° 可分解为两个对称操作：
1. **先转置**（沿主对角线翻转）：`matrix[i][j] ↔ matrix[j][i]`
2. **再水平翻转**（每行反转）：`matrix[i][j] ↔ matrix[i][n-1-j]`

两步操作的组合效果就是顺时针旋转 90°。这是因为：
- 转置：`(i, j) → (j, i)`
- 水平翻转：`(j, i) → (j, n-1-i)`
- 组合：`(i, j) → (j, n-1-i)`，恰好是顺时针旋转 90° 的坐标变换。

也可以**上下翻转 + 转置**得到逆时针旋转 90°，或直接做**四元交换**（一圈一圈旋转）。

### 核心知识点

- 矩阵旋转 = 转置 + 翻转的组合
- 原地操作只需交换，无需额外空间

### 复杂度

- **时间复杂度**: O(n²) — 每个元素访问常数次
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        int n = matrix.size();
        // 第一步：沿主对角线转置（只交换上三角，避免重复交换）
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                swap(matrix[i][j], matrix[j][i]);
            }
        }
        // 第二步：每行水平翻转（左右镜像）
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n / 2; ++j) {
                swap(matrix[i][j], matrix[i][n - 1 - j]);
            }
        }
    }
};

int main() {
    Solution s;
    vector<vector<int>> m = {{1,2,3},{4,5,6},{7,8,9}};
    s.rotate(m);
    for (auto& row : m) {
        for (int x : row) cout << x << " ";
        cout << endl;
    }
    // 输出：7 4 1 / 8 5 2 / 9 6 3
    return 0;
}
```

### 常见错误

- **错误1**：转置时用 `j = 0; j < n` 遍历整个矩阵，导致已经交换过的元素被再换一次（等于白做）。
- **错误2**：忘记 n x n 是方阵，矩形矩阵的旋转方法完全不同（需要新矩阵）。
- **错误3**：旋转方向搞反——逆时针 90° = 水平翻转 + 转置。

### 面试陷阱

- **陷阱1**："怎么做逆时针 90°？"——先上下翻转再转置，或先转置再垂直翻转。面试官考察对翻转顺序与旋转方向对应关系的理解。
- **陷阱2**："如果不是方阵怎么做？"——如果是旋转 m x n 矩阵 90°，目标矩阵是 n x m，大小变了，必须用额外空间。
- **陷阱3**："能否一次遍历完成？"——可以，使用四个元素的循环交换方案，循环遍历外圈到内圈。但代码更复杂，转置+翻转方案足够清晰。

---

## 183. 螺旋矩阵 (Spiral Matrix) — 题号 54

**难度**: 中等
**标签**: 数组、矩阵、模拟

### 题目大意

给定一个 m x n 矩阵，按螺旋顺序返回所有元素。即从左上角开始，顺时针逐圈向内遍历。

### 思路

**暴力思路**：用 visited 数组标记已访问位置。每次走到边界或已访问位置就右转。空间 O(mn)。

**优化推导**：不需要 visited 数组，维护四个边界变量 `top, bottom, left, right`。每走完一个方向就收缩对应边界：
1. 从左到右遍历 `top` 行，`top++`
2. 从上到下遍历 `right` 列，`right--`
3. 如果 `top <= bottom`，从右到左遍历 `bottom` 行，`bottom--`
4. 如果 `left <= right`，从下到上遍历 `left` 列，`left++`

每一步后检查是否越界即可终止。这是 O(1) 额外空间的模拟方案。

### 核心知识点

- 边界收缩法模拟螺旋遍历
- 方向数组 + visited 也是可行方案

### 复杂度

- **时间复杂度**: O(mn) — 每个元素恰好访问一次
- **空间复杂度**: O(1) — 不计返回数组

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        vector<int> res;
        if (matrix.empty()) return res;
        int top = 0, bottom = matrix.size() - 1;
        int left = 0, right = matrix[0].size() - 1;

        while (top <= bottom && left <= right) {
            // 向右走
            for (int j = left; j <= right; ++j)
                res.push_back(matrix[top][j]);
            top++;

            // 向下走
            for (int i = top; i <= bottom; ++i)
                res.push_back(matrix[i][right]);
            right--;

            // 向左走（需要检查 top <= bottom，防止单行重复）
            if (top <= bottom) {
                for (int j = right; j >= left; --j)
                    res.push_back(matrix[bottom][j]);
                bottom--;
            }

            // 向上走（需要检查 left <= right，防止单列重复）
            if (left <= right) {
                for (int i = bottom; i >= top; --i)
                    res.push_back(matrix[i][left]);
                left++;
            }
        }
        return res;
    }
};

int main() {
    Solution s;
    vector<vector<int>> m1 = {{1,2,3},{4,5,6},{7,8,9}};
    auto r = s.spiralOrder(m1);  // [1,2,3,6,9,8,7,4,5]
    for (int x : r) cout << x << " ";
    return 0;
}
```

### 常见错误

- **错误1**：向左和向上遍历前不检查边界，导致单行或单列矩阵重复输出。
- **错误2**：边界更新顺序错误——如先更新 `bottom` 再用它遍历，或忘记遍历完后更新边界。
- **错误3**：用 `while (top < bottom && left < right)` 作为循环条件，会漏掉最后一个（或一行一列时）的元素。

### 面试陷阱

- **陷阱1**："不规则的螺旋怎么遍历？"——如果螺旋是从中心向外、或逆时针，只需调整方向和边界收缩顺序。
- **陷阱2**："方向数组方案 vs 边界收缩方案哪个更好？"——边界收缩 O(1) 空间更优。方向数组方案需要 visited 矩阵 O(mn)，但更容易扩展到复杂路径。

---

## 184. 螺旋矩阵 II (Spiral Matrix II) — 题号 59

**难度**: 中等
**标签**: 数组、矩阵、模拟

### 题目大意

给定正整数 n，生成一个 n x n 的矩阵，按 1 到 n² 的螺旋顺序填充。

### 思路

与螺旋矩阵 I 完全一样的边界收缩法，只是从"读"变成"写"。维护 `top, bottom, left, right` 四个边界，`num` 从 1 递增到 n²，按顺序填入即可。

也可以不手动模拟，使用**方向数组**：`dr = {0, 1, 0, -1}`（右、下、左、上），每次走到越界或已填入位置就转向。

### 核心知识点

- 边界收缩法填充螺旋矩阵
- 方向数组 + 原地转向

### 复杂度

- **时间复杂度**: O(n²)
- **空间复杂度**: O(1) — 不计返回矩阵

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<int>> generateMatrix(int n) {
        vector<vector<int>> res(n, vector<int>(n, 0));
        int top = 0, bottom = n - 1, left = 0, right = n - 1;
        int num = 1;

        while (top <= bottom && left <= right) {
            for (int j = left; j <= right; ++j) res[top][j] = num++;
            top++;
            for (int i = top; i <= bottom; ++i) res[i][right] = num++;
            right--;
            if (top <= bottom) {
                for (int j = right; j >= left; --j) res[bottom][j] = num++;
                bottom--;
            }
            if (left <= right) {
                for (int i = bottom; i >= top; --i) res[i][left] = num++;
                left++;
            }
        }
        return res;
    }
};

int main() {
    Solution s;
    auto m = s.generateMatrix(3);
    for (auto& row : m) {
        for (int x : row) cout << x << "\t";
        cout << endl;
    }
    return 0;
}
```

### 常见错误

- **错误1**：`res` 忘记初始化为 0，导致方向数组方案无法判断位置是否已填充。
- **错误2**：边界收缩时的判断条件漏写，单行/单列时重复填充。
- **错误3**：n=0 的边界情况未处理。

### 面试陷阱

- **陷阱1**："如何做逆时针螺旋填充？"——调整遍历顺序：上、左、下、右，或调整方向数组顺序。
- **陷阱2**："能否用数学公式直接计算每个位置的数字而不用模拟？"——可以推导公式，但非常复杂且不通用。面试更看中模拟能力。

---

## 185. 搜索二维矩阵 II (Search a 2D Matrix II) — 题号 240

**难度**: 中等
**标签**: 数组、二分查找、分治

### 题目大意

在一个 m x n 矩阵中搜索目标值。矩阵特性：每行从左到右升序，每列从上到下升序。返回 true/false。

### 思路

**暴力思路**：遍历所有元素，O(mn)。

**优化推导一（每行二分）**：对每一行执行二分查找，O(m log n)。

**优化推导二（Z 字形搜索 / 从左下角出发）**：
观察矩阵特性：从**左下角**开始：
- 如果当前值 > target，说明当前行右侧都比 target 大，向上移动一行（消去当前行）
- 如果当前值 < target，说明当前列上方都比 target 小，向右移动一列（消去当前列）
- 等于则返回 true

每一步排除一行或一列，最多 m + n 步结束。时间复杂度 O(m + n)，空间 O(1)。

为什么从左下角？因为左下角是一行中的最小值、一列中的最大值，比较一次可以确定消去方向。从右上角出发同理。

**分治方案**：取中心元素，将矩阵分为四个子矩阵，其中两个可直接排除（全小于或全大于 target），递归搜索另外两个。但实现复杂，O(m + n) 的 Z 字形搜索已最优。

### 核心知识点

- Z 字形搜索（从左下/右上出发逐步排除行/列）
- 利用矩阵行列有序的双重性质

### 复杂度

- **时间复杂度**: O(m + n) — Z 字形搜索
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        if (matrix.empty() || matrix[0].empty()) return false;
        int m = matrix.size(), n = matrix[0].size();
        // 从左下角出发
        int i = m - 1, j = 0;
        while (i >= 0 && j < n) {
            if (matrix[i][j] == target) return true;
            else if (matrix[i][j] > target) --i;  // 当前值太大，向上走
            else ++j;                              // 当前值太小，向右走
        }
        return false;
    }
};

int main() {
    Solution s;
    vector<vector<int>> m = {
        {1,  4,  7,  11, 15},
        {2,  5,  8,  12, 19},
        {3,  6,  9,  16, 22},
        {10, 13, 14, 17, 24},
        {18, 21, 23, 26, 30}
    };
    cout << s.searchMatrix(m, 5) << endl;   // 1 (true)
    cout << s.searchMatrix(m, 20) << endl;  // 0 (false)
    return 0;
}
```

### 常见错误

- **错误1**：从左上角或右下角出发——左上角是行列最小值，右下角是行列最大值，比较后无法唯一确定消去方向（两个方向都可能）。
- **错误2**：越界检查遗漏，while 条件写成 `i > 0 && j < n`，应该是 `i >= 0 && j < n`。
- **错误3**：混淆本题（行列分别有序）和 74 题（整个矩阵按严格顺序排列），74 题可以把矩阵当成一维有序数组做二分，本题不行。

### 面试陷阱

- **陷阱1**："为什么 O(m+n) 是最优的？"——可以证明，从信息论角度，每次比较最多排除一行或一列，需要至少 min(m, n) 次比较。O(m+n) 在渐进意义上是最优的。
- **陷阱2**："如果用二进制搜索每行 O(m log n)，什么时候更好？"——当 m << n（非常扁的矩阵）时 O(m log n) 可能比 O(m+n) 更好。
- **陷阱3**："分治方案是什么？"——取矩阵中心 (mid_row, mid_col)，将矩阵分成 4 块，若 target 小于中心值，排除右下块；若大于，排除左上块。递归搜索剩余块，复杂度 O(n^log4(3)) 约 O(n^1.79)。这是 follow-up，了解即可。

---

## 186. 搜索二维矩阵 (Search a 2D Matrix) — 题号 74

**难度**: 中等
**标签**: 数组、二分查找

### 题目大意

在一个 m x n 矩阵中搜索目标值。矩阵特性：每行升序，且**下一行的第一个元素大于上一行的最后一个元素**（矩阵按行展开是严格升序的一维数组）。

### 思路

**暴力思路**：遍历所有元素，O(mn)。

**优化推导**：由于矩阵按行展开是完全有序的一维数组，可以直接将二维坐标映射为一维下标，使用二分查找：
- 一维下标 `mid` → 二维坐标 `(mid / n, mid % n)`
- 在 `[0, m*n - 1]` 范围内二分搜索 target

**两步法**：也可以先用二分确定 target 可能所在的行（比较每行第一个和最后一个元素），再在该行内二分。但一维映射法更简洁。

### 核心知识点

- 二维坐标与一维下标的转换：`row = idx / n, col = idx % n`
- 严格有序 → 标准二分查找

### 复杂度

- **时间复杂度**: O(log(mn))
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        if (matrix.empty() || matrix[0].empty()) return false;
        int m = matrix.size(), n = matrix[0].size();
        // 左闭右闭区间 [0, m*n - 1]
        int left = 0, right = m * n - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;  // 防溢出
            int val = matrix[mid / n][mid % n];
            if (val == target) return true;
            else if (val < target) left = mid + 1;
            else right = mid - 1;
        }
        return false;
    }
};

int main() {
    Solution s;
    vector<vector<int>> m = {{1,3,5,7},{10,11,16,20},{23,30,34,60}};
    cout << s.searchMatrix(m, 3) << endl;   // 1
    cout << s.searchMatrix(m, 13) << endl;  // 0
    return 0;
}
```

### 常见错误

- **错误1**：mid 计算用 `(left + right) / 2` 可能溢出，应使用 `left + (right - left) / 2` 或 `left + (right - left) >> 1`。
- **错误2**：二维坐标映射错误——`mid / m` 和 `mid % n` 写反。列数是 n，每行 n 个元素，所以是 `mid / n` 和 `mid % n`。
- **错误3**：混淆本题与 240 搜索二维矩阵 II，使用 Z 字形搜索。本题的行列条件不同（下一行首元素 > 上一行尾元素），Z 字形搜索虽然碰巧也能用，但最优解是二分。

### 面试陷阱

- **陷阱1**："如果矩阵按列展开也是有序的，怎么做？"——同样映射为一维数组，只是坐标变换变成 `(mid % m, mid / m)`。
- **陷阱2**："为什么用左闭右闭而不是左闭右开？"——本题用任意一种都可以。左闭右闭的 while 条件是 `left <= right`，最后 `left = right + 1`。面试官通常让手写一种并解释边界处理。

---

## 187. 搜索旋转排序数组 (Search in Rotated Sorted Array) — 题号 33

**难度**: 中等
**标签**: 数组、二分查找

### 题目大意

给定一个旋转后的升序数组（无重复元素），搜索 target 的下标，不存在返回 -1。要求 O(log n)。

例如：`[4,5,6,7,0,1,2]` 是 `[0,1,2,4,5,6,7]` 旋转后的结果。

### 思路

**暴力思路**：遍历数组，O(n)。

**优化推导**：旋转数组有一个关键性质——虽然整体不是有序的，但**二分后，mid 的左右两侧至少有一侧是完全有序的**。利用这一点：
1. 计算 mid，判断 `nums[left] <= nums[mid]`（左侧有序）还是 `nums[mid] <= nums[right]`（右侧有序）。
2. 如果左侧有序：判断 target 是否在 `[nums[left], nums[mid])` 范围内。若在，搜左侧；否则搜右侧。
3. 如果右侧有序：判断 target 是否在 `(nums[mid], nums[right]]` 范围内。若在，搜右侧；否则搜左侧。

**边界细节**：判断左侧有序用 `nums[left] <= nums[mid]`，加上等号是因为当 `left == mid` 时（区间长度为 1 或 2），左侧视为"有序"。

### 核心知识点

- 旋转数组二分的关键：每次二分至少有一半是有序的
- 判断哪一半有序 → 判断 target 是否在有序区间 → 决定搜索方向

### 复杂度

- **时间复杂度**: O(log n)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = 0, right = nums.size() - 1;
        while (left <= right) {                         // 左闭右闭
            int mid = left + (right - left) / 2;
            if (nums[mid] == target) return mid;

            // 判断哪一侧有序
            if (nums[left] <= nums[mid]) {              // 左侧有序
                if (nums[left] <= target && target < nums[mid])
                    right = mid - 1;                    // 在左侧有序区间内
                else
                    left = mid + 1;                     // 去右侧
            } else {                                    // 右侧有序
                if (nums[mid] < target && target <= nums[right])
                    left = mid + 1;                     // 在右侧有序区间内
                else
                    right = mid - 1;                    // 去左侧
            }
        }
        return -1;
    }
};

int main() {
    Solution s;
    vector<int> v = {4,5,6,7,0,1,2};
    cout << s.search(v, 0) << endl;   // 4
    cout << s.search(v, 3) << endl;   // -1
    cout << s.search(v, 1) << endl;   // 5
    return 0;
}
```

### 常见错误

- **错误1**：判断左侧有序时忘记等号——`nums[left] <= nums[mid]` 不是 `<`，当 `left == mid` 时也必须正确判断。
- **错误2**：判断 target 是否在区间内时开闭区间错误——`target < nums[mid]` vs `target <= nums[mid]`。因为 `nums[mid]` 已经被检查过不等于 target，所以用 `<` 和 `<=` 都可以，但要一致。
- **错误3**：`nums[left] <= target` 写成 `nums[0] <= target`——当数组只有两个元素且旋转后可能走错分支。
- **错误4**：左侧有序时 target 的范围写为 `nums[left] <= target <= nums[mid]`，而 target == mid 已在前面返回，所以只需 `< nums[mid]`。

### 面试陷阱

- **陷阱1**："数组有重复元素怎么办？"——这是下一题 81。重复元素会导致 `nums[left] == nums[mid]` 时无法判断哪边有序，最坏退化到 O(n)。
- **陷阱2**："如果旋转点在位置 0（即数组未旋转），算法还正确吗？"——正确。此时 `nums[left] <= nums[mid]` 恒成立，退化回标准二分。
- **陷阱3**："能否一次二分同时找旋转点和 target？"——可以。先二分找旋转点（最小值位置），再根据 target 与 `nums[0]` 的关系决定在哪段有序区间中二分。这也是经典解法之一，但需要两次二分。

---

## 188. 搜索旋转排序数组 II (Search in Rotated Sorted Array II) — 题号 81

**难度**: 中等
**标签**: 数组、二分查找

### 题目大意

与 33 题相同，但数组中**存在重复元素**。返回 true/false 而非下标。

### 思路

重复元素带来的问题：当 `nums[left] == nums[mid] == nums[right]` 时，无法判断哪一侧有序。例如 `[1,0,1,1,1]` 中搜索 0，`nums[left] = nums[mid] = nums[right] = 1`。

**解决方案**：当 `nums[left] == nums[mid] == nums[right]` 时，无法判断，只能 `left++` 或 `right--` 缩小范围。这使得最坏情况（全相同元素，搜索不存在元素）退化为 O(n)。但当重复元素有限时，平均仍接近 O(log n)。

### 核心知识点

- 重复元素破坏"一半有序"的可判定性
- 无法判定时退化为线性收缩

### 复杂度

- **时间复杂度**: 平均 O(log n)，最坏 O(n)（全相同元素）
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool search(vector<int>& nums, int target) {
        int left = 0, right = nums.size() - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] == target) return true;

            // 核心：当左右和中间都相等时，无法判断哪边有序，缩小范围
            if (nums[left] == nums[mid] && nums[mid] == nums[right]) {
                ++left;
                --right;
                continue;
            }

            if (nums[left] <= nums[mid]) {  // 左侧有序或左=中但左≠右
                if (nums[left] <= target && target < nums[mid])
                    right = mid - 1;
                else
                    left = mid + 1;
            } else {                        // 右侧有序
                if (nums[mid] < target && target <= nums[right])
                    left = mid + 1;
                else
                    right = mid - 1;
            }
        }
        return false;
    }
};

int main() {
    Solution s;
    vector<int> v1 = {2,5,6,0,0,1,2};
    cout << s.search(v1, 0) << endl;  // 1 (true)
    cout << s.search(v1, 3) << endl;  // 0 (false)

    // 边界测试：全相同元素
    vector<int> v2 = {1,1,1,1,1};
    cout << s.search(v2, 2) << endl;  // 0
    cout << s.search(v2, 1) << endl;  // 1
    return 0;
}
```

### 常见错误

- **错误1**：只判断 `nums[left] == nums[mid]` 就 `left++`，忽略了 `nums[left] != nums[right]` 时仍然可以判断的情况。
- **错误2**：`left++` 和 `right--` 同时进行但忘记 continue，导致继续进入下方逻辑用错误的区间判断。
- **错误3**：认为结果还是 O(log n)——面试时必须说明最坏退化到 O(n)。

### 面试陷阱

- **陷阱1**："能否在 O(log n) 内解决？"——不能。可以举反例：全 1 数组插入一个 0，必须扫描几乎所有元素才能找到。
- **陷阱2**："为什么不用找旋转点的方法？"——因为重复元素存在时，旋转点（最小值）也无法在 O(log n) 内找到。
- **陷阱3**："有没有办法优化？"——当 `nums[left] == nums[mid]` 但 `nums[mid] != nums[right]` 时，可以从 left 到 mid 相等，说明这段都是重复值，可以跳过。这比简单 `left++` 效率更高。

---

## 189. 寻找旋转排序数组中的最小值 (Find Minimum in Rotated Sorted Array) — 题号 153

**难度**: 中等
**标签**: 数组、二分查找

### 题目大意

给定一个无重复元素的旋转升序数组，找出最小值。要求 O(log n)。

### 思路

**暴力思路**：遍历找最小值，O(n)。

**优化推导**：旋转数组的最小值就是"旋转点"位置的值。二分时比较 `nums[mid]` 与 `nums[right]`：
- 如果 `nums[mid] > nums[right]`，说明最小值在右半部分（mid 在左半段大值区），`left = mid + 1`
- 如果 `nums[mid] <= nums[right]`，说明最小值在左半部分（mid 在右半段小值区），`right = mid`（不能 `mid - 1`，因为 mid 可能就是最小值）

为什么要和 `nums[right]` 比而不是 `nums[left]`？因为与 `nums[right]` 比较能唯一确定最小值在哪一侧；与 `nums[left]` 比较在未旋转的情况下会失效。

**循环终止**：`left` 就是最小值的位置（因为 `left == right` 时退出）。使用 `while (left < right)` 防止死循环。

### 核心知识点

- 旋转数组最小值 = 二分找旋转点
- 与 `nums[right]` 比较确定最小值区间
- `right = mid` 不能减一，避免跳过最小值

### 复杂度

- **时间复杂度**: O(log n)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findMin(vector<int>& nums) {
        int left = 0, right = nums.size() - 1;
        // 左闭右闭，但用 left < right 防止死循环
        while (left < right) {
            int mid = left + (right - left) / 2;

            // 关键：比较 nums[mid] 和 nums[right]
            if (nums[mid] > nums[right]) {
                // mid 在左半段（大值区），最小值在 mid 右侧
                left = mid + 1;
            } else {
                // mid 在右半段（小值区），最小值可能是 mid 或其左侧
                right = mid;
            }
        }
        return nums[left];  // 或 nums[right]，两者相等
    }
};

int main() {
    Solution s;
    vector<int> v1 = {3,4,5,1,2};
    cout << s.findMin(v1) << endl;  // 1

    vector<int> v2 = {4,5,6,7,0,1,2};
    cout << s.findMin(v2) << endl;  // 0

    vector<int> v3 = {11,13,15,17};
    cout << s.findMin(v3) << endl;  // 11 (未旋转)
    return 0;
}
```

### 常见错误

- **错误1**：与 `nums[left]` 比较而非 `nums[right]`。在数组未旋转（完全有序）时 `nums[mid] > nums[left]` 恒成立，算法失效。
- **错误2**：`right = mid - 1` 跳过 mid。mid 可能就是最小值，减一会导致错过。
- **错误3**：while 条件使用 `left <= right`，与 `right = mid` 配合会导致死循环（当 `left == right` 时 `mid == left == right`，`right = mid` 不改变区间）。

### 面试陷阱

- **陷阱1**："为什么和 right 比较而不是和 left？"——见错误1。和 right 比较可以处理未旋转的情况。
- **陷阱2**："如果比较 `nums[mid]` 和 `nums[mid+1]` 呢？"——那是另一种思路：找第一个 `nums[i] > nums[i+1]` 的位置，`nums[i+1]` 就是最小值。
- **陷阱3**："怎么求旋转数组的最大值？"——对称：最小值索引的前一个位置（模 n）就是最大值。或把比较条件反转。

---

## 190. 寻找旋转排序数组中的最小值 II (Find Minimum in Rotated Sorted Array II) — 题号 154

**难度**: 困难
**标签**: 数组、二分查找

### 题目大意

与 153 相同，但数组中**存在重复元素**。找出最小值，要求尽可能高效。

### 思路

**问题分析**：重复元素破坏了"与 `nums[right]` 比较即可确定最小值区间"的性质。考虑 `[10, 1, 10, 10, 10]`：nums[mid] = 10, nums[right] = 10，相等时无法判断最小值在左侧还是右侧。

**三种情况的判定**（比较 nums[mid] 与 nums[right]）：

1. **`nums[mid] > nums[right]`**：最小值一定在右侧，`left = mid + 1`
2. **`nums[mid] < nums[right]`**：最小值在左侧（包括 mid），`right = mid`
3. **`nums[mid] == nums[right]`**：无法确定，但可以安全地 `right--`（因为即使 nums[right] 是最小值，nums[mid] 是它的副本，不会丢失最小值）

**复杂度分析**：
- 平均 O(log n)
- 最坏 O(n)：全相等数组 `[2,2,2,2,2]`，每次 `right--`，需要遍历所有元素

**其他解法**：
- 暴力遍历 O(n)：最简洁，在 n 不大时实际最快
- 与 `nums[left]` 比较 + 线性收缩：另一种写法，本质相同

### 核心知识点

- 重复元素下二分退化的处理策略
- `nums[mid] == nums[right]` 时 `right--` 是安全的收缩操作

### 复杂度

- **时间复杂度**: 平均 O(log n)，最坏 O(n)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findMin(vector<int>& nums) {
        int left = 0, right = nums.size() - 1;
        while (left < right) {
            int mid = left + (right - left) / 2;

            if (nums[mid] > nums[right]) {
                // 最小值在右侧
                left = mid + 1;
            } else if (nums[mid] < nums[right]) {
                // 最小值在左侧（包括 mid）
                right = mid;
            } else {
                // nums[mid] == nums[right]，安全收缩右边界
                // 因为 nums[mid] 是 nums[right] 的"备份"，不会丢失最小值
                --right;
            }
        }
        return nums[left];
    }
};

// 暴力解法（当 n 不大时同样适用，面试中可以先给出作为 baseline）
int findMinBrute(vector<int>& nums) {
    return *min_element(nums.begin(), nums.end());
}

int main() {
    Solution s;
    vector<int> v1 = {1,3,5};
    cout << s.findMin(v1) << endl;  // 1

    vector<int> v2 = {2,2,2,0,1};
    cout << s.findMin(v2) << endl;  // 0

    vector<int> v3 = {10,1,10,10,10};
    cout << s.findMin(v3) << endl;  // 1

    vector<int> v4 = {2,2,2,2,2};
    cout << s.findMin(v4) << endl;  // 2 (最坏情况)
    return 0;
}
```

### 常见错误

- **错误1**：`nums[mid] == nums[right]` 时直接 `right = mid`（像 153 一样），在 `[10, 1, 10, 10, 10]` 中会错过最小值 1。
- **错误2**：相等时 `right = mid - 1`，可能把最小值跳过去。
- **错误3**：认为 O(log n) 总能达到——面试时必须说明最坏情况退化。

### 面试陷阱

- **陷阱1**："能不能不退化到 O(n)？"——不能。最坏情况（全相等）下，你需要排除 n-1 个元素才能确定唯一的最小值（如果最小值是唯一的那个不同值），必须扫描几乎所有元素。
- **陷阱2**："如果用 `left` 和 `mid` 比较呢？"——对称地处理：`nums[left] < nums[mid]` → 左侧有序；`nums[left] > nums[mid]` → 右侧有序；`nums[left] == nums[mid]` → `left++`。
- **陷阱3**："这道题是 Hard，面试中常问吗？"——常作为 follow-up：先做 153（无重复），再问"有重复元素怎么处理"。核心考察对二分退化场景的理解。

---

## 191. 寻找两个正序数组的中位数 (Median of Two Sorted Arrays) — 题号 4

**难度**: 困难
**标签**: 数组、二分查找、分治

### 题目大意

给定两个升序数组 `nums1` 和 `nums2`，大小分别为 m 和 n，找出两个数组所有元素的中位数。要求 O(log(m+n)) 时间。

### 思路

**暴力思路**：合并两个有序数组，取中位数。O(m+n)，不满足对数要求。

**优化推导（二分+划分法）**：

中位数的本质是将数组分为**左右两部分**，使得左半部分所有元素 <= 右半部分所有元素，且左右部分大小相等（或左比右多 1）。

设 `nums1` 划分为 `A[0..i-1] | A[i..m-1]`，`nums2` 划分为 `B[0..j-1] | B[j..n-1]`。
合并后的左半部分为 `A_left + B_left`，右半部分为 `A_right + B_right`。

两个条件：
1. `i + j = (m + n + 1) / 2`（左半部分元素个数，确保左多一/相等）
2. `A[i-1] <= B[j]` 且 `B[j-1] <= A[i]`（左半部分所有元素 <= 右半部分所有元素）

在较短的数组上二分 `i`，`j` 由条件 1 自动计算。调整 `i` 直到满足条件 2。

**核心细节**：
- 总是在较短的数组上二分，确保 j 不会出现负数（因为 `j = half - i`，如果 i 过大 j 会变成负数）。
- 当 `i` 确定后，左半部分最大值为 `max(A[i-1], B[j-1])`，右半部分最小值为 `min(A[i], B[j])`。
- 处理边界：`i = 0` 时 `A[i-1]` 不存在（用 `INT_MIN`），`i = m` 时 `A[i]` 不存在（用 `INT_MAX`）。

**另一种解法（找第 k 小数）**：每次排除 `k/2` 个元素。比较 `A[k/2-1]` 和 `B[k/2-1]`，较小的那部分一定都在第 k 小的左半边，可以跳过。

### 核心知识点

- 二分 + 划分法找中位数
- 边界处理（INT_MIN / INT_MAX）
- 在较短数组上二分防止越界

### 复杂度

- **时间复杂度**: O(log(min(m, n)))
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格) — 划分法

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        // 确保在较短的数组上二分，降低复杂度并防止 j 出界
        if (nums1.size() > nums2.size())
            return findMedianSortedArrays(nums2, nums1);

        int m = nums1.size(), n = nums2.size();
        int totalLeft = (m + n + 1) / 2;  // 左半部分应包含的元素数

        int left = 0, right = m;
        while (left < right) {  // 左闭右开，二分 nums1 的划分位置 i
            int i = left + (right - left) / 2;      // nums1 左半部分元素数
            int j = totalLeft - i;                   // nums2 左半部分元素数

            if (nums1[i] < nums2[j - 1]) {
                // A_right 的第一个 < B_left 的最后一个 → i 太小，右移
                left = i + 1;
            } else {
                // 满足 A[i] >= B[j-1]，继续尝试更小的 i
                right = i;
            }
        }

        int i = left, j = totalLeft - i;

        // 边界处理：i/j 为 0 表示左半部分不包含该数组元素
        int leftMaxA = (i == 0) ? INT_MIN : nums1[i - 1];
        int leftMaxB = (j == 0) ? INT_MIN : nums2[j - 1];
        int rightMinA = (i == m) ? INT_MAX : nums1[i];
        int rightMinB = (j == n) ? INT_MAX : nums2[j];

        if ((m + n) % 2 == 1) {
            // 奇数：左半部分的最大值就是中位数
            return max(leftMaxA, leftMaxB);
        } else {
            // 偶数：左最大和右最小取平均
            return (max(leftMaxA, leftMaxB) + min(rightMinA, rightMinB)) / 2.0;
        }
    }
};

int main() {
    Solution s;
    vector<int> a1 = {1, 3}, b1 = {2};
    cout << s.findMedianSortedArrays(a1, b1) << endl;  // 2.0

    vector<int> a2 = {1, 2}, b2 = {3, 4};
    cout << s.findMedianSortedArrays(a2, b2) << endl;  // 2.5

    vector<int> a3 = {}, b3 = {1};
    cout << s.findMedianSortedArrays(a3, b3) << endl;  // 1.0
    return 0;
}
```

### C++ 代码 (ACM风格) — 第 k 小数法（递归）

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution2 {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int m = nums1.size(), n = nums2.size();
        int leftK = (m + n + 1) / 2;
        int rightK = (m + n + 2) / 2;
        // 中位数 = (第 leftK 小 + 第 rightK 小) / 2
        // 奇偶统一：奇数时 leftK == rightK
        return (getKth(nums1, 0, m - 1, nums2, 0, n - 1, leftK) +
                getKth(nums1, 0, m - 1, nums2, 0, n - 1, rightK)) * 0.5;
    }

    // 在 A[start1..end1] 和 B[start2..end2] 中找第 k 小的数
    int getKth(vector<int>& A, int start1, int end1,
               vector<int>& B, int start2, int end2, int k) {
        int len1 = end1 - start1 + 1;
        int len2 = end2 - start2 + 1;

        // 保证 len1 <= len2，简化后续判断
        if (len1 > len2) return getKth(B, start2, end2, A, start1, end1, k);
        // 如果较短数组为空，直接从较长数组取第 k 小
        if (len1 == 0) return B[start2 + k - 1];
        if (k == 1) return min(A[start1], B[start2]);

        // 比较 A[k/2-1] 和 B[k/2-1]，排除较小的一侧
        int i = start1 + min(len1, k / 2) - 1;
        int j = start2 + min(len2, k / 2) - 1;

        if (A[i] < B[j]) {
            // A 的前 k/2 个元素一定都在第 k 小之前，排除
            return getKth(A, i + 1, end1, B, start2, end2, k - (i - start1 + 1));
        } else {
            // B 的前 k/2 个元素排除
            return getKth(A, start1, end1, B, j + 1, end2, k - (j - start2 + 1));
        }
    }
};
```

### 常见错误

- **错误1**：不在较短数组上二分，导致 `j = totalLeft - i` 可能为负数（当 i > totalLeft 时）。
- **错误2**：边界值用 `0` 而不是 `INT_MIN`/`INT_MAX`，导致比较出错（数组中可能有正负值）。
- **错误3**：奇偶判断混淆——奇数时中位数是左半部分最大值，偶数时是（左最大 + 右最小）/ 2。
- **错误4**：第 k 小数解法中，`k/2` 没有与数组长度取 min，导致越界。

### 面试陷阱

- **陷阱1**："为什么是 O(log(min(m,n))) 而不是 O(log(m+n))？"——因为只在较短数组上二分，二分范围是 min(m,n)，所以复杂度是 O(log(min(m,n)))。这也比 O(log(m+n)) 更精确。
- **陷阱2**："如果两个数组都很大，内存放不下怎么办？"——用第 k 小数法，每次读一小部分数据。属于大数据场景的 follow-up。
- **陷阱3**："能否推广到 k 个有序数组的中位数？"——可以，使用值域二分（猜答案）：猜一个数 x，数有多少个元素 <= x，二分到中位数位置的元素值。O(n log(value_range))。

---

## 192. 寻找峰值 (Find Peak Element) — 题号 162

**难度**: 中等
**标签**: 数组、二分查找

### 题目大意

给定一个整数数组，峰值为大于左右相邻元素的值。`nums[-1]` 和 `nums[n]` 视为负无穷。返回任意一个峰值下标。要求 O(log n)。

### 思路

**暴力思路**：遍历数组，找到第一个满足 `nums[i] > nums[i+1]` 的位置，或扫描所有元素。O(n)。

**优化推导**（利用单调性进行二分）：
关键洞察：如果 `nums[mid] < nums[mid+1]`，说明 mid 正处于**上坡**，右侧一定存在峰值（最坏情况也是 `nums[n-1]`，因为 `nums[n]` 是 -∞）。反之，如果 `nums[mid] > nums[mid+1]`，说明 mid 正处于**下坡**，左侧一定存在峰值。

二分策略：比较 `nums[mid]` 与 `nums[mid+1]`，选择上坡方向继续二分搜索。

**为什么对数正确？** 因为二分选择了"保证有峰值"的一侧，类似数学中"极值存在定理"的构造性证明。

### 核心知识点

- 利用"上坡必有峰值"的性质进行二分
- 比较 `nums[mid]` 与 `nums[mid+1]` 判断坡度方向

### 复杂度

- **时间复杂度**: O(log n)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findPeakElement(vector<int>& nums) {
        int left = 0, right = nums.size() - 1;
        while (left < right) {  // 注意用 < 而非 <=，防止 mid == n-1 时 mid+1 越界
            int mid = left + (right - left) / 2;
            if (nums[mid] < nums[mid + 1]) {
                // 上坡：峰值在右侧
                left = mid + 1;
            } else {
                // 下坡（或平顶）：峰值在左侧（mid 本身可能是峰值）
                right = mid;
            }
        }
        return left;  // left == right 时退出，即峰值位置
    }
};

int main() {
    Solution s;
    vector<int> v1 = {1,2,3,1};
    cout << s.findPeakElement(v1) << endl;  // 2 (nums[2]=3)

    vector<int> v2 = {1,2,1,3,5,6,4};
    cout << s.findPeakElement(v2) << endl;  // 1 或 5

    vector<int> v3 = {1};
    cout << s.findPeakElement(v3) << endl;  // 0
    return 0;
}
```

### 常见错误

- **错误1**：用 `left <= right` 作为 while 条件，mid 可能为 n-1，`mid+1` 越界。
- **错误2**：`right = mid - 1` 跳过 mid——mid 可能就是峰值，不应跳过。
- **错误3**：试图比较 `nums[mid-1]`、`nums[mid]`、`nums[mid+1]` 三者，导致边界处理复杂。与 `mid+1` 比较就够了。

### 面试陷阱

- **陷阱1**："如果存在连续的相等元素（高原），算法还能用吗？"——LeetCode 中定义峰值为严格大于邻居，如果 `nums[mid] == nums[mid+1]`，两边都可能存在峰值，算法仍能找到（因为 `else` 分支包括相等情况并搜索左侧），但需要调整逻辑。
- **陷阱2**："如何找全局最大值（而非任意峰值）？"——不能用二分，必须 O(n) 扫描或使用三分搜索（只能找到局部极值）。
- **陷阱3**："如果二维矩阵找峰值呢？"——这是 LeetCode 1901，可以用类似思路：先找中间行的最大值，比较其上下方向，向较大方向递归。O(m log n) 或 O(n log m)。

---

## 193. 在排序数组中查找元素的第一个和最后一个位置 — 题号 34

**难度**: 中等
**标签**: 数组、二分查找

### 题目大意

给定一个升序数组，找到 target 的起始位置和结束位置。如果不存在返回 `[-1, -1]`。要求 O(log n)。

### 思路

**暴力思路**：从左到右扫描找到第一个和最后一个位置，O(n)。

**优化推导**：两次二分查找：
1. **找左边界**：在 `[0, n-1]` 二分，当 `nums[mid] >= target` 时 `right = mid`（收缩右边界），最后 left 指向第一个 `>= target` 的位置。
2. **找右边界**：在 `[0, n-1]` 二分，当 `nums[mid] <= target` 时 `left = mid + 1`（收缩左边界），最后 left 指向第一个 `> target` 的位置，右边界 = left - 1。

**二分模板选择**：本题展示"二分左边界"和"二分右边界"两个模板。这两个模板是二分查找中最常用的变体。

**左边界模板（左闭右开）**：
```cpp
while (left < right) {
    mid = left + (right - left) / 2;
    if (nums[mid] >= target) right = mid;
    else left = mid + 1;
}
// left 是第一个 >= target 的位置
```

**右边界模板**：
```cpp
while (left < right) {
    mid = left + (right - left + 1) / 2;  // 注意 +1 防止死循环
    if (nums[mid] <= target) left = mid;
    else right = mid - 1;
}
// left 是最后一个 <= target 的位置
```

### 核心知识点

- 二分左边界（第一个等于）和右边界（最后一个等于）
- 左闭右闭 vs 左闭右开的选择
- 防止死循环：找右边界时 mid 计算要 +1

### 复杂度

- **时间复杂度**: O(log n)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        if (nums.empty()) return {-1, -1};

        // 找左边界：第一个 >= target 的位置
        int left = lower_bound(nums, target);
        if (left == nums.size() || nums[left] != target)
            return {-1, -1};  // 没找到

        // 找右边界：第一个 > target 的位置 - 1
        int right = lower_bound(nums, target + 1) - 1;
        return {left, right};
    }

private:
    // 自定义 lower_bound：返回第一个 >= target 的位置
    int lower_bound(vector<int>& nums, int target) {
        int left = 0, right = nums.size();  // 左闭右开
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] >= target)
                right = mid;
            else
                left = mid + 1;
        }
        return left;
    }
};

// 清晰版：分别实现左边界和右边界二分
class Solution2 {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int n = nums.size();
        if (n == 0) return {-1, -1};

        int first = -1, last = -1;

        // 找左边界（第一个 >= target 的位置）
        int l = 0, r = n - 1;
        while (l <= r) {  // 左闭右闭
            int mid = l + (r - l) / 2;
            if (nums[mid] >= target) {
                r = mid - 1;        // 收缩右边界
                if (nums[mid] == target) first = mid;
            } else {
                l = mid + 1;
            }
        }

        // 找右边界（最后一个 <= target 的位置）
        l = 0, r = n - 1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] <= target) {
                l = mid + 1;        // 收缩左边界
                if (nums[mid] == target) last = mid;
            } else {
                r = mid - 1;
            }
        }

        return {first, last};
    }
};

int main() {
    Solution s;
    vector<int> v1 = {5,7,7,8,8,10};
    auto r1 = s.searchRange(v1, 8);    // [3, 4]
    cout << r1[0] << " " << r1[1] << endl;

    auto r2 = s.searchRange(v1, 6);    // [-1, -1]
    cout << r2[0] << " " << r2[1] << endl;

    vector<int> v2 = {};
    auto r3 = s.searchRange(v2, 0);    // [-1, -1]
    cout << r3[0] << " " << r3[1] << endl;
    return 0;
}
```

### 常见错误

- **错误1**：左右边界用同一套二分逻辑，未区分边界类型。找左边界和右边界 mid 的计算方法不同。
- **错误2**：右边界二分中 `mid = (l + r) / 2`，当 `l = r - 1` 时 mid = l，如果 `nums[mid] <= target` 则 `l = mid`，区间不变，死循环。必须用 `(l + r + 1) / 2`。
- **错误3**：找到左边界后直接线性扫描找右边界，导致最坏 O(n)。

### 面试陷阱

- **陷阱1**："为什么找右边界时 mid 要 +1？"——防止死循环。当区间只剩两个元素且 `nums[mid] <= target` 时，`left = mid` 可能让 left 停在原地。
- **陷阱2**："STL 的 lower_bound 和 upper_bound 有什么区别？"——`lower_bound` 返回第一个 `>= target`，`upper_bound` 返回第一个 `> target`。range = `[lower_bound, upper_bound - 1]`。
- **陷阱3**："左闭右开和左闭右闭哪个更好？"——左闭右开在区间为空时自然表达（left == right），且 mid 不需要 +1 防死循环。左闭右闭更容易直观理解但不能用于"找峰值"等需要保留 mid 的场景。根据问题选择合适的模板。

---

## 194. x 的平方根 (Sqrt(x)) — 题号 69

**难度**: 简单
**标签**: 数学、二分查找

### 题目大意

给定非负整数 x，返回 x 的算术平方根的整数部分（向下取整）。不允许使用内置指数函数和运算符。

### 思路

**暴力思路**：从 0 遍历到 x，找到 `i*i <= x` 的最大 i。O(sqrt(x))。

**优化推导（二分查找）**：
在 `[0, x]`（x = 0 或 1 时直接返回 x）范围内二分。对于每个 mid：
- 如果 `mid * mid <= x`，mid 可能是答案，记录并搜索右侧
- 如果 `mid * mid > x`，搜索左侧

**注意溢出**：`mid * mid` 可能超过 int 范围，用 `long long` 或者用除法替代（`mid <= x / mid`）。

**牛顿迭代法**（面试加分项）：
牛顿法求 `f(y) = y^2 - x = 0` 的根。迭代公式：`y_{k+1} = (y_k + x / y_k) / 2`。收敛速度极快（二次收敛）。

### 核心知识点

- 二分查找求整数平方根
- 防止溢出：用除法或 long long
- 牛顿迭代法（数值方法）

### 复杂度

- **时间复杂度**: O(log x) — 二分；牛顿法更快，约 O(log log x)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // 法一：二分查找
    int mySqrt(int x) {
        if (x <= 1) return x;
        int left = 0, right = x;
        int ans = 0;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            // 用除法避免溢出：mid <= x / mid 等价于 mid * mid <= x
            if (mid <= x / mid) {
                ans = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return ans;
    }

    // 法二：牛顿迭代法
    int mySqrtNewton(int x) {
        if (x <= 1) return x;
        long long r = x;  // 初始猜测
        while (r * r > x) {
            r = (r + x / r) / 2;
        }
        return (int)r;
    }
};

int main() {
    Solution s;
    cout << s.mySqrt(4) << endl;      // 2
    cout << s.mySqrt(8) << endl;      // 2
    cout << s.mySqrt(0) << endl;      // 0
    cout << s.mySqrt(2147395599) << endl;  // 46339
    return 0;
}
```

### 常见错误

- **错误1**：`mid * mid` 直接计算导致溢出（int 最大值约 2.1e9，平方 > 4e18）。
- **错误2**：对 x = 0 或 x = 1 不做特殊处理，二分范围从 0 开始但 right = x = 0，while 循环不执行。
- **错误3**：牛顿法不设收敛条件，`r * r <= x` 作为终止条件时需要注意 r 可能抖到略小于真实值的值。

### 面试陷阱

- **陷阱1**："为什么用 `mid <= x / mid` 判断？"——考察对整数溢出的防控意识。面试官希望你主动提及。
- **陷阱2**："如果要精确到小数点后 10 位呢？"——用浮点数二分或牛顿法，控制精度（如 `abs(r*r - x) < 1e-10`）。牛顿法在浮点版本中迭代约 10 次即可达到双精度。
- **陷阱3**："归并排序里面也有二分，二分法的实质是什么？"——在**单调函数**上二分查找解。本题是 `f(y) = y^2` 在 `[0, x]` 上单调递增。

---

## 195. Pow(x, n) — 题号 50

**难度**: 中等
**标签**: 数学、递归、快速幂

### 题目大意

实现 `pow(x, n)`，即计算 x 的 n 次方。n 可正可负。x 是 double，n 是 int。

### 思路

**暴力思路**：循环乘 n 次，O(n)。n 可达 2^31-1，不可接受。

**优化推导（快速幂 / 二进制幂）**：
利用指数的二进制表示：`x^n = x^{b_k·2^k + ... + b_0·2^0} = Π x^{b_i·2^i}`。

每次将 n 折半：
- 如果 n 是偶数：`x^n = (x^2)^{n/2}`
- 如果 n 是奇数：`x^n = x * (x^2)^{n/2}`

**递归版本**：`myPow(x, n) = myPow(x*x, n/2) * (n%2 ? x : 1)`

**迭代版本**：
```
ans = 1, cur = x
while n > 0:
    if n & 1: ans *= cur
    cur *= cur
    n >>= 1
```

**负数处理**：当 n < 0 时，`x^n = 1 / (x^{-n})`。注意 n = INT_MIN 时 `-n` 会溢出（INT_MIN = -2147483648 > INT_MAX），需要用 `long long` 转换。

### 核心知识点

- 快速幂（二进制分解指数）
- INT_MIN 取反溢出防护
- 递归与迭代两种写法

### 复杂度

- **时间复杂度**: O(log n) — 指数二进制位数
- **空间复杂度**: O(log n) 递归，O(1) 迭代

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // 递归版本
    double myPow(double x, int n) {
        long long N = n;  // 防止 INT_MIN 取反溢出
        if (N < 0) {
            x = 1.0 / x;
            N = -N;
        }
        return fastPow(x, N);
    }

    double fastPow(double x, long long n) {
        if (n == 0) return 1.0;
        double half = fastPow(x, n / 2);
        if (n % 2 == 0)
            return half * half;
        else
            return half * half * x;
    }

    // 迭代版本
    double myPowIter(double x, int n) {
        long long N = n;
        if (N < 0) { x = 1.0 / x; N = -N; }
        double ans = 1.0;
        double cur = x;
        while (N > 0) {
            if (N & 1) ans *= cur;  // 当前位为 1，乘上当前的 cur^(2^k)
            cur *= cur;              // cur 平方：cur^(2^k) → cur^(2^{k+1})
            N >>= 1;
        }
        return ans;
    }
};

int main() {
    Solution s;
    cout << s.myPow(2.0, 10) << endl;      // 1024
    cout << s.myPow(2.1, 3) << endl;       // 9.261
    cout << s.myPow(2.0, -2) << endl;      // 0.25
    cout << s.myPow(1.0, -2147483648) << endl;  // 1.0 (INT_MIN 边界)
    return 0;
}
```

### 常见错误

- **错误1**：`n = -n` 直接取反，当 n = INT_MIN 时溢出为自身（INT_MIN 的绝对值超出正数范围）。
- **错误2**：递归版本中 `half * half * (n % 2 ? x : 1)` 中 n 的类型仍然是 int，n 为奇数时 `n % 2 == 1`（正）或 `n % 2 == -1`（负奇数），需要先转为 `long long`。
- **错误3**：迭代版本中忘记 `N >>= 1` 造成死循环。

### 面试陷阱

- **陷阱1**："递归和迭代哪个好？"——迭代版本 O(1) 空间，避免栈溢出（虽然 log n 层递归栈也很小）。面试官可能追问递归展开过程。
- **陷阱2**："快速幂还可以用在哪些场景？"——矩阵快速幂（求斐波那契第 n 项 O(log n)）、快速乘法（处理大数取模）、RSA 加密等。这展示你的知识广度。
- **陷阱3**："如果是模运算 `(x^n) % mod` 呢？"——在每次乘法和 cur 平方时对 mod 取模即可，公式完全相同。

---

## 196. 分数到小数 (Fraction to Recurring Decimal) — 题号 166

**难度**: 中等
**标签**: 哈希表、数学、字符串

### 题目大意

给定分子 numerator 和分母 denominator，以字符串形式返回小数结果。如果有循环小数，用括号包围循环部分。

### 思路

**核心问题**：如何检测循环？——当某个余数重复出现时，就开始循环。

**算法步骤**：
1. 处理符号：结果是否为负（异号得负）。
2. 整数部分：`abs(num) / abs(den)`。
3. 小数部分：
   - 取余数 `rem = abs(num) % abs(den)`
   - 每次 `rem *= 10`，商 `rem / den` 加入小数部分，更新 `rem = rem % den`
   - 用哈希表 `map<rem, pos>` 记录每个余数对应的位置（小数字符串中的位置）
   - 当余数重复出现时，从该位置插入 '('，末尾加 ')'
4. 余数为 0 时结束（有限小数）。

### 核心知识点

- 模拟长除法
- 哈希表检测循环——余数重复是循环的充要条件
- int 溢出防护：转 long long

### 复杂度

- **时间复杂度**: O(分母) — 余数最多出现 den 次
- **空间复杂度**: O(den) — 哈希表存储余数

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string fractionToDecimal(int numerator, int denominator) {
        if (numerator == 0) return "0";

        string res;
        // 处理符号
        if ((numerator < 0) ^ (denominator < 0)) res += '-';

        // 转 long long 防溢出
        long long num = abs((long long)numerator);
        long long den = abs((long long)denominator);

        // 整数部分
        res += to_string(num / den);
        long long rem = num % den;
        if (rem == 0) return res;  // 整除，直接返回

        // 小数部分
        res += '.';
        unordered_map<long long, int> pos_map;  // 余数 -> 位置
        while (rem != 0) {
            // 如果余数已出现过，说明开始循环
            if (pos_map.count(rem)) {
                res.insert(pos_map[rem], "(");
                res += ')';
                break;
            }
            // 记录当前余数的位置（即当前小数部分的长度）
            pos_map[rem] = res.size();

            rem *= 10;
            res += to_string(rem / den);
            rem %= den;
        }
        return res;
    }
};

int main() {
    Solution s;
    cout << s.fractionToDecimal(1, 2) << endl;    // "0.5"
    cout << s.fractionToDecimal(2, 1) << endl;    // "2"
    cout << s.fractionToDecimal(4, 333) << endl;  // "0.(012)"
    cout << s.fractionToDecimal(1, 6) << endl;    // "0.1(6)"
    cout << s.fractionToDecimal(-50, 8) << endl;  // "-6.25"
    return 0;
}
```

### 常见错误

- **错误1**：取绝对值时用 `abs(numerator)`，numerator 可能为 INT_MIN，`abs(INT_MIN)` 溢出（仍然是 INT_MIN）。必须转 `long long` 再 `abs`。
- **错误2**：循环起点不是第一个出现的位置。示例 `1/6 = 0.1(6)`，循环从第 2 位开始不是第 1 位。
- **错误3**：忘记处理 `numerator == 0` 的特殊情况。
- **错误4**：异号判断用 `(a < 0 && b > 0) || (a > 0 && b < 0)` 写成 `a * b < 0`，乘法溢出。

### 面试陷阱

- **陷阱1**："为什么余数重复就意味着循环？"——长除法的每一步完全由余数决定。相同的余数 → 相同的商 → 相同的下一步余数 → 无限循环。这是数学中的鸽巢原理（余数最多 den 种，所以最差 den 步后必重复）。
- **陷阱2**："如果是无限不循环小数呢？"——不存在。任何有理数的小数表示要么有限，要么无限循环。只有无理数（如 sqrt(2)）才是无限不循环的。
- **陷阱3**："循环节的最大长度是多少？"——欧拉定理告诉我们，对于真分数 n/d (n < d 且 d 与 10 互质)，循环节长度是满足 10^k ≡ 1 (mod d) 的最小正整数 k，最大是 φ(d)。最坏情况下循环节长度为 d-1。

---

## 197. 阶乘后的零 (Factorial Trailing Zeroes) — 题号 172

**难度**: 中等
**标签**: 数学

### 题目大意

给定整数 n，返回 n! 的末尾零的个数。要求在 O(log n) 时间内解决（日志级）。

### 思路

**暴力思路**：计算 n! 然后数末尾 0——n 很大时阶乘远超 long long 范围，不可行。

**优化推导**：
末尾 0 来自 10 = 2 × 5。在 n! 中，因子 2 的数量远多于 5（偶数比 5 的倍数多），因此 **5 的个数是瓶颈**。

问题转化为：计算 n! 中因子 5 的个数。

1~n 中有多少个数至少包含 1 个因子 5？`n / 5` 个。
1~n 中有多少个数至少包含 2 个因子 5？`n / 25` 个。
1~n 中有多少个数至少包含 3 个因子 5？`n / 125` 个。
...

总数 = `n/5 + n/25 + n/125 + ...` 直到 0。

### 核心知识点

- 末尾 0 的本质是 10 的幂次 = min(因子2的个数, 因子5的个数)
- 因子 5 的数量公式：Σ n/(5^k)

### 复杂度

- **时间复杂度**: O(log_5 n)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int trailingZeroes(int n) {
        int count = 0;
        while (n >= 5) {
            n /= 5;      // n/5 是一轮，下一轮是 n/5/5 = n/25...
            count += n;  // 累加当前轮的 5 的幂次数
        }
        return count;
    }
};

int main() {
    Solution s;
    cout << s.trailingZeroes(3) << endl;    // 0 (3! = 6)
    cout << s.trailingZeroes(5) << endl;    // 1 (5! = 120)
    cout << s.trailingZeroes(10) << endl;   // 2 (10! = 3628800)
    cout << s.trailingZeroes(25) << endl;   // 6 (25/5 + 25/25 = 5+1)
    return 0;
}
```

### 常见错误

- **错误1**：只计算 `n / 5`，忽略 `n / 25`、`n / 125` 等的额外因子 5。如 25! 中 25 贡献 2 个 5。
- **错误2**：循环写成 `while (n > 0)` 每次 `n /= 5`，但用 `n` 的初始值直接累加——`n/5 + n/25` 中 n 应该每次更新。
- **错误3**：试图用除法计算因子 2 的个数——虽然也会得到结果，但 2 的个数永远 >= 5 的个数，取 min 后答案由 5 决定。

### 面试陷阱

- **陷阱1**："如果二进制末尾 0 呢？"——那就是因子 2 的个数：`n/2 + n/4 + n/8 + ...`。同理可推广到任何进制。
- **陷阱2**："n! 本身有多少位？"——斯特林公式近似：log10(n!) ≈ n·log10(n/e) + 0.5·log10(2πn)。面试中提斯特林公式是大加分项。
- **陷阱3**："复杂度是多少？"——O(log_5 n)，这个分析比单纯说 O(log n) 更准确。

---

## 198. 计数质数 (Count Primes) — 题号 204

**难度**: 中等
**标签**: 数组、数学

### 题目大意

统计所有小于非负整数 n 的质数的数量。

### 思路

**暴力思路**：对每个数 i 判断是否为质数（检查到 sqrt(i)）。O(n √n)，超时。

**优化推导（埃拉托色尼筛法）**：
- 创建一个 bool 数组 `isPrime[0..n-1]`，初始全为 true（0 和 1 除外）
- 从 2 开始，如果 `isPrime[i]` 为 true，则 i 是质数，将 i 的所有倍数（从 i*i 开始，因为 2i, 3i, ..., (i-1)i 已被更小的质数筛过）标记为 false
- 优化：
  - 只需筛到 sqrt(n)，因为比 sqrt(n) 大的合数一定有小于 sqrt(n) 的因子
  - 从 `i * i` 开始标记，因为 `i * (2..i-1)` 已经在之前的轮次中筛过
- 时间：O(n log log n)，接近线性

**线性筛（欧拉筛）**（面试加分）：
每个合数只被其**最小质因子**筛掉一次，严格 O(n)。维护一个质数列表，对于每个 i：
1. 如果 i 是质数，加入列表
2. 对于列表中的每个质数 p，将 `i * p` 标记为合数
3. 当 `i % p == 0` 时 break（确保每个数只被最小质因子筛）

### 核心知识点

- 埃拉托色尼筛法 O(n log log n)
- 欧拉筛（线性筛）O(n)
- 筛到 sqrt(n) 的优化

### 复杂度

- **时间复杂度**: 埃氏筛 O(n log log n)；欧拉筛 O(n)
- **空间复杂度**: O(n)

### C++ 代码 (ACM风格) — 埃氏筛 + 欧拉筛

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // 埃拉托色尼筛法
    int countPrimesSieve(int n) {
        if (n <= 2) return 0;
        vector<bool> isPrime(n, true);
        isPrime[0] = isPrime[1] = false;

        // 只需筛到 sqrt(n)
        for (int i = 2; (long long)i * i < n; ++i) {
            if (isPrime[i]) {
                // 从 i*i 开始标记，因为 2i, 3i... (i-1)i 已被标记
                for (int j = i * i; j < n; j += i) {
                    isPrime[j] = false;
                }
            }
        }

        int count = 0;
        for (int i = 2; i < n; ++i)
            if (isPrime[i]) ++count;
        return count;
    }

    // 欧拉筛（线性筛）—— 每个合数只被最小质因子筛掉
    int countPrimesLinear(int n) {
        if (n <= 2) return 0;
        vector<bool> isPrime(n, true);
        vector<int> primes;  // 存储已找到的质数
        isPrime[0] = isPrime[1] = false;

        for (int i = 2; i < n; ++i) {
            if (isPrime[i])
                primes.push_back(i);

            // 用 i 和已知质数筛合数
            for (int p : primes) {
                if ((long long)i * p >= n) break;
                isPrime[i * p] = false;
                // 核心：如果 i % p == 0，说明 p 是 i 的最小质因子
                // 那么 i*next_prime 的最小质因子是 p 而不是 next_prime
                // 停止筛选，确保每个合数只被最小质因子筛掉
                if (i % p == 0) break;
            }
        }

        return primes.size();
    }
};

int main() {
    Solution s;
    cout << s.countPrimesSieve(10) << endl;    // 4 (2,3,5,7)
    cout << s.countPrimesSieve(0) << endl;     // 0
    cout << s.countPrimesSieve(1) << endl;     // 0
    cout << s.countPrimesSieve(100) << endl;   // 25

    cout << s.countPrimesLinear(100) << endl;  // 25
    return 0;
}
```

### 常见错误

- **错误1**：内层循环从 `2 * i` 开始标记，导致重复标记并降低效率。应从 `i * i` 开始。
- **错误2**：`i * i` 可能溢出 int（当 i > 46340 时），需用 `long long` 或在条件中判断。
- **错误3**：判断质数时循环到 sqrt(n)，但每次重新计算 `sqrt(n)`。应直接用 `i * i <= n` 做条件（注意溢出）。
- **错误4**：欧拉筛中忘记 `i % p == 0` 时 break，退化为 O(n log log n)。

### 面试陷阱

- **陷阱1**："埃氏筛和欧拉筛的根本区别？"——埃氏筛中一个合数可能被多个质因子筛选多次（如 30 被 2, 3, 5 各筛一次）。欧拉筛保证每个合数只被其最小质因子筛一次，达到线性复杂度。
- **陷阱2**："如何筛选区间 [L, R] 内的质数？"——先用筛法筛出 [1, sqrt(R)] 的质数，然后用这些质数在 [L, R] 上做区间筛。
- **陷阱3**："欧拉筛中为什么 `i % p == 0` 就 break？"——因为如果 p 整除 i，那么对于更大的质数 q，`i * q` 的最小质因子是 p（p 能整除 i*q），不应由 q 来筛。这保证了每个合数只被最小质因子筛掉。

---

## 199. 每日温度 (Daily Temperatures) — 题号 739

**难度**: 中等
**标签**: 栈、单调栈、数组

### 题目大意

给定一个温度数组，对于每一天，你需要等待多少天才能等到更暖和的温度。如果没有，填 0。

### 思路

**暴力思路**：对第 i 天，往后扫描找到第一个更高的温度。O(n²)，最坏 n = 10^5 超时。

**优化推导（单调栈）**：
维护一个**递减栈**（栈中存储温度的下标，对应温度从栈底到栈顶递减）。
遍历每一天：
1. 当天的温度如果比栈顶对应的温度高，说明栈顶元素终于等到了更暖和的温度。弹出栈顶，记录结果 `res[top_idx] = i - top_idx`。
2. 重复直到栈为空或当天的温度不高于栈顶。
3. 将当天入栈。

**为什么用下标而不是值？** 因为答案需要计算天数差 `i - prev_i`，需要知道位置信息。用值只能判断是否更高，无法算出天数。

### 核心知识点

- 单调栈：维护一个单调递减（或递增）的序列
- 典型应用：找下一个更大元素 / 上一个更大元素
- 栈存下标：保留位置信息用于计算距离

### 复杂度

- **时间复杂度**: O(n) — 每个元素最多入栈出栈各一次
- **空间复杂度**: O(n) — 栈的最坏大小

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& T) {
        int n = T.size();
        vector<int> res(n, 0);
        stack<int> stk;  // 单调递减栈，存下标

        for (int i = 0; i < n; ++i) {
            // 当前温度比栈顶高 → 栈顶元素找到答案
            while (!stk.empty() && T[i] > T[stk.top()]) {
                int prev = stk.top();
                stk.pop();
                res[prev] = i - prev;  // 等待天数 = 当前日期 - 之前日期
            }
            stk.push(i);
        }
        // 栈中剩余元素保持默认值 0（没有更暖和的天气）
        return res;
    }
};

int main() {
    Solution s;
    vector<int> t1 = {73,74,75,71,69,72,76,73};
    auto r = s.dailyTemperatures(t1);
    // [1, 1, 4, 2, 1, 1, 0, 0]
    for (int x : r) cout << x << " "; cout << endl;

    vector<int> t2 = {30,40,50,60};
    r = s.dailyTemperatures(t2);  // [1,1,1,0]
    for (int x : r) cout << x << " "; cout << endl;
    return 0;
}
```

### 常见错误

- **错误1**：栈中存温度值而不是下标，无法计算天数差。
- **错误2**：比较条件写成 `>=` 而非 `>`，导致相等温度也触发弹出——题目要求"更暖和"（严格大于）。
- **错误3**：忘记给栈中剩余元素赋 0 作为默认值。在初始化时 `res(n, 0)` 已处理。
- **错误4**：栈的单调方向搞反（维护递增栈而非递减栈）。

### 面试陷阱

- **陷阱1**："为什么用单调栈而不是 DP？"——本质上是在找"下一个更大元素"（Next Greater Element, NGE），单调栈是标准解法。DP 从后往前也可以做：`res[i] = (T[i+1] > T[i]) ? 1 : res[i+1] > 0 ? res[i+res[i+1]] + res[i+1] : 0`，但理解起来不如单调栈直观。
- **陷阱2**："如果是循环数组（即最后一天可以等到第一天的温度）呢？"——遍历两遍数组（`for i in [0..2n-1]`，下标取模），同样是单调栈。这是 LeetCode 503。
- **陷阱3**："单调栈还可以用来解决什么问题？"——柱状图中最大的矩形 (84)、接雨水 (42)、下一个更大元素系列 (496, 503, 556)。这些是面试高频题。

---

## 200. 正则表达式匹配 (Regular Expression Matching) — 题号 10

**难度**: 困难
**标签**: DP、字符串、回溯

### 题目大意

给定字符串 s 和模式 p，实现 `.` 和 `*` 的正则表达式匹配。`.` 匹配任意单个字符，`*` 匹配零个或多个前面的那个元素。匹配应覆盖整个字符串。

### 思路

**回溯（递归）思路**：
核心是处理 `*`：`*` 总是与其前面的字符绑定（如 `a*` 表示 0 个或多个 a）。
- 如果下一个字符是 `*`：
  - 选择 1：匹配 0 次（跳过 `a*`）
  - 选择 2：匹配 1 次（如果当前字符匹配，s 前进 1，p 不动，因为 `*` 还可以继续匹配）
- 如果没有 `*`：直接匹配当前字符

**DP 优化思路**：
`dp[i][j]` = s 的前 i 个字符与 p 的前 j 个字符是否匹配。

状态转移：
- 如果 `p[j-1] == '.'` 或 `p[j-1] == s[i-1]`：`dp[i][j] = dp[i-1][j-1]`
- 如果 `p[j-1] == '*'`：
  - 匹配 0 次：`dp[i][j] = dp[i][j-2]`（忽略 `x*`）
  - 匹配 1+ 次：需要 `s[i-1] == p[j-2]` 或 `p[j-2] == '.'`，然后 `dp[i][j] = dp[i-1][j]`（s 去掉最后一个字符，p 保持不变继续匹配）

**为什么 DP 是二维的？** 因为每个状态依赖于之前的状态（回溯中有大量重复计算），DP 避免了指数级爆炸。

### 核心知识点

- DP 状态定义 `dp[i][j]`：前 i 和 前 j 个字符是否匹配
- `*` 匹配 0 次的前向依赖 `dp[i][j-2]`
- `*` 匹配 1+ 次需要当前字符匹配 + 依赖 `dp[i-1][j]`
- 回溯 → DP 的转化思想

### 复杂度

- **时间复杂度**: O(mn) — DP 表格
- **空间复杂度**: O(mn)，可优化到 O(n)（滚动数组）

### C++ 代码 (ACM风格) — DP 解法

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool isMatch(string s, string p) {
        int m = s.size(), n = p.size();
        // dp[i][j] = s[0..i-1] 与 p[0..j-1] 是否匹配
        vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));

        dp[0][0] = true;  // 空串匹配空模式

        // 初始化：空串 s 匹配 p（只有 a* b* c* 这种模式能匹配空串）
        for (int j = 2; j <= n; j += 1) {
            if (p[j - 1] == '*')
                dp[0][j] = dp[0][j - 2];  // a* 选 0 次
        }

        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (p[j - 1] == '.' || p[j - 1] == s[i - 1]) {
                    // 普通字符或 '.' 直接匹配
                    dp[i][j] = dp[i - 1][j - 1];
                } else if (p[j - 1] == '*') {
                    // 情况 1：x* 匹配 0 次 → 忽略 p[j-2] 和 p[j-1]
                    dp[i][j] = dp[i][j - 2];

                    // 情况 2：x* 匹配 1+ 次
                    // 前提：当前字符能匹配 x（即 p[j-2] == s[i-1] 或 p[j-2] == '.'）
                    if (p[j - 2] == '.' || p[j - 2] == s[i - 1]) {
                        dp[i][j] = dp[i][j] || dp[i - 1][j];
                        // dp[i-1][j] 的含义：s 少一个字符，p 不变（x* 继续生效）
                    }
                }
                // else: 普通字符不匹配，dp[i][j] 保持 false
            }
        }
        return dp[m][n];
    }
};

// 递归回溯写法（面试中可先写这个再优化到 DP）
class SolutionRecursive {
public:
    bool isMatch(string s, string p) {
        return match(s, p, 0, 0);
    }

    bool match(const string& s, const string& p, int i, int j) {
        if (j == p.size()) return i == s.size();  // 模式用完，字符串也必须用完

        // 检查当前字符是否匹配
        bool firstMatch = (i < s.size()) && (p[j] == s[i] || p[j] == '.');

        // 下一个字符是 '*'（需要确保 j+1 存在）
        if (j + 1 < p.size() && p[j + 1] == '*') {
            // 选择 1：x* 匹配 0 次（跳过 x*）
            // 选择 2：x* 匹配 1 次（当前字符必须匹配，s 前进 1，p 不动）
            return match(s, p, i, j + 2) ||          // 匹配 0 次
                   (firstMatch && match(s, p, i + 1, j));  // 匹配 1 次
        } else {
            // 没有 '*'：当前字符必须匹配，然后各自前进 1
            return firstMatch && match(s, p, i + 1, j + 1);
        }
    }
};

int main() {
    Solution s;
    cout << s.isMatch("aa", "a") << endl;        // 0 (false)
    cout << s.isMatch("aa", "a*") << endl;       // 1 (true)
    cout << s.isMatch("ab", ".*") << endl;       // 1 (true)
    cout << s.isMatch("aab", "c*a*b") << endl;   // 1 (true)
    cout << s.isMatch("mississippi", "mis*is*p*.") << endl; // 0 (false)
    return 0;
}
```

### 常见错误

- **错误1**：DP 初始化时忘记处理 `dp[0][j]`——空串 s 可以被 `a*`、`.*` 等模式匹配（选 0 次）。
- **错误2**：`*` 总是企图匹配 1 次以上，忽略了匹配 0 次的可能性（`dp[i][j-2]`）。
- **错误3**：`dp[i-1][j]` 的含义不理解——这表示 s 去掉最后 1 个字符后，仍然能被当前模式 p[0..j-1] 匹配。这意味着 `*` 至少匹配了那个去掉的字符。
- **错误4**：数组越界——访问 `p[j-2]` 时必须确保 `j >= 2`（即模式中 `*` 前面有有效字符）。

### 面试陷阱

- **陷阱1**："为什么 `dp[i][j] = dp[i-1][j]` 而不是 `dp[i-1][j-1]`？"——因为 `*` 可以匹配多个。`dp[i-1][j]` 意味着 s 少一个字符时模式 p 不变，即 `*` 继续生效。
- **陷阱2**："递归回溯和 DP 各有什么优缺点？"——回溯简洁易懂，但最坏 O(2^n)（大量分支）。DP O(mn) 稳定，但状态转移不直观。面试官希望你先说回溯思路，再优化到 DP。
- **陷阱3**："如果模式中有嵌套的 `*`（如 `a**`）呢？"——标准的正则表达式中 `**` 是非法的（`*` 必须跟在字符后面）。但可以讨论：`a**` 等价于 `a*`。面试官考察边界意识。
- **陷阱4**："能不能用 NFA（非确定有限自动机）来做？"——可以！这是正则表达式匹配的通用方法（Thompson NFA）。但面试中 DP 足够。

---

## 201. 字符串转换整数 (atoi) — 题号 8

**难度**: 中等
**标签**: 字符串、模拟、状态机

### 题目大意

实现 `atoi` 函数，将字符串转为 32 位有符号整数。规则：
1. 丢弃前置空格
2. 可选 `+` 或 `-` 符号
3. 读取数字直到非数字字符或字符串结尾
4. 若溢出，返回 INT_MAX 或 INT_MIN
5. 若无有效数字，返回 0

### 思路

这道题考的是**细节处理**和**边界条件**，而非算法技巧。建议使用**有限状态机（DFA）**思路，将处理过程分为 4 个状态：
- START：初始，可接受空格、符号、数字
- SIGNED：已读符号，接受数字
- NUMBER：正在读数字，接受数字
- END：终止（遇到非法字符或结束）

**溢出处理**是核心难点：
- 在累加数字前预判：如果 `res > INT_MAX / 10` 或 `res == INT_MAX / 10 && digit > INT_MAX % 10`，则溢出
- 正数溢出返回 INT_MAX，负数溢出返回 INT_MIN

### 核心知识点

- 状态机（DFA）处理字符串解析
- 整数溢出预判（提前检查而非等溢出后判断）
- 边界条件处理

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格) — 状态机

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int myAtoi(string s) {
        int i = 0, n = s.size();
        // 1. 跳过前置空格
        while (i < n && s[i] == ' ') ++i;
        if (i == n) return 0;

        // 2. 处理符号
        int sign = 1;
        if (s[i] == '+' || s[i] == '-') {
            sign = (s[i] == '-') ? -1 : 1;
            ++i;
        }

        // 3. 读取数字
        long long res = 0;  // 用 long long 方便溢出判断（另一种方式是用 int + 预判）
        while (i < n && isdigit(s[i])) {
            res = res * 10 + (s[i] - '0');

            // 溢出检查
            if (sign == 1 && res > INT_MAX) return INT_MAX;
            if (sign == -1 && -res < INT_MIN) return INT_MIN;

            ++i;
        }
        return (int)(sign * res);
    }

    // 法二：用 int + 预判溢出（不用 long long）
    int myAtoiInt(string s) {
        int i = 0, n = s.size();
        while (i < n && s[i] == ' ') ++i;
        if (i == n) return 0;

        int sign = 1;
        if (s[i] == '+' || s[i] == '-') {
            sign = (s[i] == '-') ? -1 : 1;
            ++i;
        }

        int res = 0;
        while (i < n && isdigit(s[i])) {
            int digit = s[i] - '0';

            // 溢出预判（乘法/加法之前检查）
            if (res > INT_MAX / 10 ||
                (res == INT_MAX / 10 && digit > INT_MAX % 10)) {
                return (sign == 1) ? INT_MAX : INT_MIN;
            }

            res = res * 10 + digit;
            ++i;
        }
        return sign * res;
    }
};

int main() {
    Solution s;
    cout << s.myAtoi("42") << endl;              // 42
    cout << s.myAtoi("   -42") << endl;          // -42
    cout << s.myAtoi("4193 with words") << endl; // 4193
    cout << s.myAtoi("words and 987") << endl;   // 0
    cout << s.myAtoi("-91283472332") << endl;    // -2147483648
    cout << s.myAtoi("21474836460") << endl;     // 2147483647
    return 0;
}
```

### 常见错误

- **错误1**：溢出判断写在 `res = res * 10 + digit` 之后——此时已经溢出（或 UB），无法正确判断。必须预判。
- **错误2**：符号后的空格未跳过——如 `"   +  413"`，`+` 后面有空格的应返回 0（空格只在数字之前有效，读符号后空格视为非法）。
- **错误3**：`INT_MAX` 和 `INT_MIN` 的绝对值不对称：`abs(INT_MIN) = INT_MAX + 1`，直接用正数存储会溢出。用 `long long` 或分开处理。
- **错误4**：返回时忘记乘符号 `sign * res`。

### 面试陷阱

- **陷阱1**："为什么不用 long long 也能判断溢出？"——在乘法和加法前预判，见代码中的 `myAtoiInt`。这是面试官考察的重点。
- **陷阱2**："状态机方案适用于什么场景？"——任何有明确规则的字符串解析（如解析 IP 地址 468、有效数字 65）。状态机让代码结构清晰、便于扩展。
- **陷阱3**："`res == INT_MAX / 10 && digit > 7` 中的 7 是什么？"——`INT_MAX = 2147483647`，末位是 7；`INT_MIN = -2147483648`，末位是 8。正溢出时 digit > 7，负溢出时 digit > 8。可以用 `INT_MAX % 10` 代替硬编码。

---

## 202. 最长有效括号 (Longest Valid Parentheses) — 题号 32

**难度**: 困难
**标签**: 栈、DP、字符串

### 题目大意

给定一个只包含 `(` 和 `)` 的字符串，找出最长有效（格式正确且连续）括号子串的长度。

### 思路

**法一：栈解法（推荐）**

关键：要计算子串长度，需要知道"断点"位置——即无效字符的位置。

1. 栈中存储**下标**。初始压入 `-1` 作为虚拟的无效位置。
2. 遍历字符串：
   - 遇到 `(`：将其下标压入栈中
   - 遇到 `)`：
     - 弹出栈顶（匹配最近的 `(`）
     - 如果栈为空（没有匹配的 `(` 了），将当前 `)` 的下标压入栈中，作为新的"无效位置"（断点）
     - 如果栈非空，用 `i - stk.top()` 更新最长长度（当前下标 - 栈中最后一个未被匹配的位置）

**为什么初始压入 -1？** 这样在第一个有效括号序列结束时，`i - (-1)` 能正确计算长度。

**法二：DP 解法**

`dp[i]` = 以 `s[i]` 结尾的最长有效括号子串长度。
- 如果 `s[i] == '('`：`dp[i] = 0`（`(` 不能作为有效子串的结尾）
- 如果 `s[i] == ')'`：
  - 如果 `s[i-1] == '('`（形成 `...()`）：`dp[i] = dp[i-2] + 2`
  - 如果 `s[i-1] == ')'` 且 `s[i-dp[i-1]-1] == '('`：`dp[i] = dp[i-1] + 2 + dp[i-dp[i-1]-2]`

**法三：双向扫描（O(1) 空间）**

用 left 和 right 分别计数。从左到右扫描：left 计数 `(`，right 计数 `)`。当 left == right 时记录长度；当 right > left 时归零。然后再从右到左扫描交换角色（因为从左到右可能漏掉如 `(()` 的情况）。

### 核心知识点

- 栈存下标："断点"思想
- DP 状态转移：`...() ` 和 `...))` 两种情况
- 双向扫描：O(1) 空间的贪心解法

### 复杂度

- **时间复杂度**: O(n) — 所有解法
- **空间复杂度**: 栈 O(n)；DP O(n)；双向扫描 O(1)

### C++ 代码 (ACM风格) — 三种解法

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // 法一：栈（推荐）
    int longestValidParentheses(string s) {
        int maxLen = 0;
        stack<int> stk;
        stk.push(-1);  // 虚拟的无效位置，作为"断点"的初始值

        for (int i = 0; i < s.size(); ++i) {
            if (s[i] == '(') {
                stk.push(i);
            } else {  // s[i] == ')'
                stk.pop();
                if (stk.empty()) {
                    // 当前 ')' 没有匹配的 '('，它成为新的断点
                    stk.push(i);
                } else {
                    // 栈顶是最后一个无效位置的前一个位置
                    maxLen = max(maxLen, i - stk.top());
                }
            }
        }
        return maxLen;
    }

    // 法二：DP
    int longestValidParenthesesDP(string s) {
        int n = s.size(), maxLen = 0;
        vector<int> dp(n, 0);  // dp[i] = 以 s[i] 结尾的最长有效括号长度

        for (int i = 1; i < n; ++i) {
            if (s[i] == ')') {
                if (s[i - 1] == '(') {
                    // 情况 1: ...()
                    dp[i] = (i >= 2 ? dp[i - 2] : 0) + 2;
                } else if (i - dp[i - 1] - 1 >= 0 && s[i - dp[i - 1] - 1] == '(') {
                    // 情况 2: ...))
                    // s[i-dp[i-1]-1] 是与当前 ')' 匹配的 '('
                    dp[i] = dp[i - 1] + 2 +
                            (i - dp[i - 1] - 2 >= 0 ? dp[i - dp[i - 1] - 2] : 0);
                }
                maxLen = max(maxLen, dp[i]);
            }
        }
        return maxLen;
    }

    // 法三：双向扫描（O(1) 空间）
    int longestValidParenthesesTwoPass(string s) {
        int maxLen = 0;
        int left = 0, right = 0;

        // 从左到右扫描
        for (char c : s) {
            if (c == '(') ++left;
            else ++right;

            if (left == right)
                maxLen = max(maxLen, 2 * right);
            else if (right > left)
                left = right = 0;  // 无效，重置
        }

        // 从右到左扫描
        left = right = 0;
        for (int i = s.size() - 1; i >= 0; --i) {
            if (s[i] == '(') ++left;
            else ++right;

            if (left == right)
                maxLen = max(maxLen, 2 * left);
            else if (left > right)
                left = right = 0;
        }

        return maxLen;
    }
};

int main() {
    Solution s;
    cout << s.longestValidParentheses("(()") << endl;        // 2
    cout << s.longestValidParentheses(")()())") << endl;     // 4
    cout << s.longestValidParentheses("") << endl;           // 0
    cout << s.longestValidParentheses("()(()") << endl;      // 2
    cout << s.longestValidParentheses("()(())") << endl;     // 6

    cout << "--- DP ---" << endl;
    cout << s.longestValidParenthesesDP("(()") << endl;
    cout << s.longestValidParenthesesDP(")()())") << endl;

    cout << "--- Two Pass ---" << endl;
    cout << s.longestValidParenthesesTwoPass("(()") << endl;
    cout << s.longestValidParenthesesTwoPass(")()())") << endl;
    return 0;
}
```

### 常见错误

- **错误1**：栈解法中初始不压入 -1，导致第一个有效子串的长度计算错误（如 `"()"`，栈为空时无法计算长度）。
- **错误2**：DP 解法中忘记处理 `dp[i-2]` 或 `dp[i-dp[i-1]-2]` 的前向依赖，索引可能为负。
- **错误3**：双向扫描中只做从左到右，漏掉如 `(()` 的情况（从左到右 left 始终 >= right，未归零，但最长是 2）。必须再反向扫描。
- **错误4**：栈中存字符而不是下标。需要下标来计算长度。

### 面试陷阱

- **陷阱1**："三种解法的适用场景分别是什么？"——栈最容易想到和理解；DP 是字符串类问题通用方法，面试中可展示 DP 功底；双向扫描在需要 O(1) 空间时有优势（如嵌入式系统）。
- **陷阱2**："如果有多种括号 `()[]{}` 呢？"——栈解法失去"断点"的优势（因为不同括号不能混合匹配）。需要分别处理每种括号的最长匹配，或用 DP。
- **陷阱3**："如何输出最长有效子串本身（而非长度）？"——在更新 maxLen 时记录起始位置 `start = stk.top() + 1`，长度 `len = i - stk.top()`，最后用 `substr(start, len)` 即可。

---

## 203. 字符串解码 (Decode String) — 题号 394

**难度**: 中等
**标签**: 栈、递归、字符串

### 题目大意

给定一个编码字符串，返回解码后的字符串。编码规则：`k[encoded_string]` 表示 encoded_string 重复 k 次。保证括号是有效的。可以嵌套。

例如：`"3[a2[c]]"` → `"accaccacc"`。

### 思路

**法一：栈解法（推荐）**

遍历字符串，用两个栈分别存**数字**和**字符串**：
- 遇到数字：解析完整数字，压入数字栈
- 遇到 `[`：将当前累积的字符串压入字符串栈，并将其重置为空
- 遇到 `]`：弹出数字栈的乘数 k 和字符串栈的前缀，将当前字符串重复 k 次加到前缀后，作为新的当前字符串
- 遇到字母：直接追加到当前字符串

**法二：递归解法**

本质就是解析 `数字[递归子结构]`。每次遇到数字时，解析数字，跳过 `[`，递归解析 `[...]` 内的内容，重复 k 次，然后继续解析之后的内容。

递归比栈更直观，但有函数调用栈开销。

### 核心知识点

- 双栈处理嵌套结构
- 栈/递归处理括号嵌套的通用模式

### 复杂度

- **时间复杂度**: O(n) — 每个字符处理一次
- **空间复杂度**: O(n) — 栈或递归深度

### C++ 代码 (ACM风格) — 栈 + 递归

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // 法一：双栈法
    string decodeString(string s) {
        stack<int> numStack;       // 存储重复次数
        stack<string> strStack;    // 存储 `[` 之前的字符串前缀
        string cur;                // 当前累积的字符串
        int num = 0;

        for (char c : s) {
            if (isdigit(c)) {
                num = num * 10 + (c - '0');  // 处理多位数（如 123）
            } else if (c == '[') {
                // 将当前数字和前缀字符串压入栈，重置
                numStack.push(num);
                strStack.push(cur);
                num = 0;
                cur = "";
            } else if (c == ']') {
                // 弹出重复次数和前缀
                int repeat = numStack.top(); numStack.pop();
                string prefix = strStack.top(); strStack.pop();

                // 重复 cur，加到前缀后面
                string temp;
                while (repeat--) temp += cur;
                cur = prefix + temp;
            } else {
                cur += c;  // 普通字母
            }
        }
        return cur;
    }

    // 法二：递归法（使用索引引用）
    string decodeStringRecursive(const string& s, int& i) {
        string res;
        while (i < s.size() && s[i] != ']') {
            if (!isdigit(s[i])) {
                res += s[i++];  // 普通字母直接加入
            } else {
                // 解析数字
                int num = 0;
                while (i < s.size() && isdigit(s[i])) {
                    num = num * 10 + (s[i++] - '0');
                }
                // 跳过 '['
                ++i;  // s[i] == '['
                // 递归解析括号内的内容
                string inner = decodeStringRecursive(s, i);
                // 跳过 ']'
                ++i;  // s[i] == ']'
                // 重复 inner num 次
                while (num--) res += inner;
            }
        }
        return res;
    }

    string decodeStringRec(string s) {
        int i = 0;
        return decodeStringRecursive(s, i);
    }
};

int main() {
    Solution s;
    cout << s.decodeString("3[a]2[bc]") << endl;       // "aaabcbc"
    cout << s.decodeString("3[a2[c]]") << endl;        // "accaccacc"
    cout << s.decodeString("2[abc]3[cd]ef") << endl;   // "abcabccdcdcdef"
    cout << s.decodeString("abc3[cd]xyz") << endl;     // "abccdcdcdxyz"

    cout << "--- Recursive ---" << endl;
    cout << s.decodeStringRec("3[a2[c]]") << endl;
    return 0;
}
```

### 常见错误

- **错误1**：解析数字时忘记处理多位数（如 `10[abc]`），只读了一位数字。
- **错误2**：遇到 `]` 时栈操作顺序错误——应该先弹出数字栈和字符串栈，构建重复字符串，再赋值给 cur。
- **错误3**：重复字符串时直接 `cur = prefix + repeat(cur)` 覆盖了 cur，但如果后面还有嵌套内容会丢失。
- **错误4**：递归法中索引 `i` 通过值传递而非引用传递，递归返回后外层索引未更新（必须传引用 `int& i`）。

### 面试陷阱

- **陷阱1**："嵌套层级很深（如 1000 层）时栈会溢出吗？"——栈解法使用 `std::stack`（堆内存），不会溢出；递归法则可能栈溢出（取决于系统栈大小）。面试官跟进优化方案。
- **陷阱2**："如果有不匹配的括号呢？"——根据题意一定有匹配的括号。但可以让代码更鲁棒：检查栈为空的情况并处理。
- **陷阱3**："不用栈只用一个计数器怎么做？"——如果只有一层括号（非嵌套），可以用简单方法。嵌套结构必须用栈或递归来跟踪上下文。

---

## 204. 重排链表 (Reorder List) — 题号 143

**难度**: 中等
**标签**: 链表、双指针、栈、递归

### 题目大意

给定单链表 `L0 → L1 → ... → Ln-1 → Ln`，重新排列为 `L0 → Ln → L1 → Ln-1 → L2 → Ln-2 → ...`。不能修改节点的值，只能改动节点指向。

### 思路

**暴力思路**：用数组/列表存储所有节点，然后用双指针重组。O(n) 时间 + O(n) 空间。

**优化推导（三步法）**：
1. **找到中点**：快慢指针找到链表中点（或中间偏左的位置）
2. **反转后半部分**：将链表后半部分反转
3. **交叉合并**：将前半部分和反转后的后半部分交替合并

这样 O(n) 时间 + O(1) 空间。

**详细步骤**：
- 快慢指针：快指针一次两步，慢指针一次一步。快指针到尾时，慢指针到中间位置（或偏左的位置，取决于快指针起点的细节）。
  - 对于 `[1,2,3,4]`，慢指针停在 2（前半部分比后半多 1 个节点）
  - 取 `mid = slow->next` 作为后半部分的起点，将 `slow->next = nullptr` 断开链表
- 反转后半部分：标准的迭代反转链表
- 合并：`l1` 在前半部分，`l2` 在后半部分（已反转）。交替取 `l1` 和 `l2` 的下一个节点

### 核心知识点

- 快慢指针找中点
- 反转链表（核心基础操作）
- 链表合并

### 复杂度

- **时间复杂度**: O(n)
- **空间复杂度**: O(1)

### C++ 代码 (ACM风格)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    void reorderList(ListNode* head) {
        if (!head || !head->next) return;

        // 1. 快慢指针找中点（前半部分比后半部分多 1 或相等）
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        // slow 现在是前半部分的最后一个节点（或正中偏左）

        // 2. 反转后半部分
        ListNode* mid = slow->next;
        slow->next = nullptr;  // 断开链表
        ListNode* l2 = reverseList(mid);

        // 3. 交替合并 l1 (head) 和 l2
        ListNode* l1 = head;
        while (l2) {
            ListNode* next1 = l1->next;
            ListNode* next2 = l2->next;

            l1->next = l2;
            l2->next = next1;

            l1 = next1;
            l2 = next2;
        }
    }

private:
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* cur = head;
        while (cur) {
            ListNode* next = cur->next;
            cur->next = prev;
            prev = cur;
            cur = next;
        }
        return prev;
    }
};

// 辅助函数
void printList(ListNode* head) {
    while (head) {
        cout << head->val;
        if (head->next) cout << " -> ";
        head = head->next;
    }
    cout << endl;
}

ListNode* buildList(vector<int> v) {
    ListNode dummy(0);
    ListNode* cur = &dummy;
    for (int x : v) {
        cur->next = new ListNode(x);
        cur = cur->next;
    }
    return dummy.next;
}

int main() {
    Solution s;

    ListNode* h1 = buildList({1, 2, 3, 4});
    s.reorderList(h1);
    printList(h1);  // 1 -> 4 -> 2 -> 3

    ListNode* h2 = buildList({1, 2, 3, 4, 5});
    s.reorderList(h2);
    printList(h2);  // 1 -> 5 -> 2 -> 4 -> 3

    return 0;
}
```

### 常见错误

- **错误1**：断开链表时忘记 `slow->next = nullptr`，导致合并时形成环。
- **错误2**：快慢指针找中点时，奇数节点 `[1,2,3,4,5]` 的后半部分长度应该比前半少（或相等）——慢指针停在 2（前半 3 个节点 123，后半 2 个节点 45 反转后 54），合并正确。
- **错误3**：合并循环条件只用 `while (l1 && l2)`，但前半部分可能比后半长（奇数节点时），应只判断 `while (l2)`，因为 l2 更短或相等。
- **错误4**：交替合并时忘记保存 `next1` 和 `next2`，导致链表断开。

### 面试陷阱

- **陷阱1**："三步法 O(1) 空间是最优的吗？"——是的。可以用递归（回溯 + 全局指针）实现，但递归使用 O(n) 栈空间。
- **陷阱2**："快慢指针找中点时，fast 从 head 还是 head->next 开始？"——这里从 head 开始，确保前半部分 >= 后半部分（奇数时前半多 1 个）。如果从 head->next 开始，奇数时前后相等，但合并逻辑需要调整。
- **陷阱3**："如果要求用递归做呢？"——回溯法：用全局指针 cur 指向头部，递归到链表尾部，返回时将 cur 的 next 指向当前节点，cur 前进。这种方法本质上是交叉合并的递归实现。

---

## 205. 排序数组 (Sort an Array) — 题号 912

**难度**: 中等
**标签**: 排序、分治、堆、快速排序、归并排序

### 题目大意

给定一个整数数组，将其升序排列并返回。直接考察排序算法的实现。

### 思路

这道题的核心是**手写排序算法**，通常要求 O(n log n) 的算法。在面试中推荐实现**归并排序**（稳定、最坏 O(n log n)）和**快速排序**（平均快、但最坏 O(n²)）。

**归并排序**：
- 分治：将数组不断二分直到长度为 1
- 合并：将两个有序子数组合并

**快速排序**：
- 选 pivot，分区（partition），递归排序左右区间
- 优化 pivot 选择：随机选 pivot 或三数取中，避免在已排序数组上退化
- 对于大量重复元素可以用三路快排

**堆排序**（第三种选择）：
- 建堆 O(n)，不断取堆顶 O(n log n)
- 实际速度通常慢于快排（缓存不友好），但作为比较可以提

**面试策略**：先写归并排序（稳定 + 最坏保证），再写快排（优化 pivot），最后提堆排序。

### 核心知识点

- 归并排序：分治 O(n log n)，稳定排序
- 快速排序：pivot 选择、分区、三路快排
- 随机化 pivot 避免退化

### 复杂度

- **时间复杂度**: 归并 O(n log n)；快排平均 O(n log n)，最坏 O(n²)；堆排 O(n log n)
- **空间复杂度**: 归并 O(n)；快排 O(log n)（递归栈）；堆排 O(1)

### C++ 代码 (ACM风格) — 归并排序 + 快速排序

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // ==================== 归并排序 ====================
    vector<int> sortArray(vector<int>& nums) {
        mergeSort(nums, 0, nums.size() - 1);
        return nums;
    }

    void mergeSort(vector<int>& nums, int left, int right) {
        if (left >= right) return;
        int mid = left + (right - left) / 2;

        // 分治
        mergeSort(nums, left, mid);
        mergeSort(nums, mid + 1, right);

        // 合并
        merge(nums, left, mid, right);
    }

    void merge(vector<int>& nums, int left, int mid, int right) {
        // 临时数组存储合并结果
        vector<int> temp(right - left + 1);
        int i = left, j = mid + 1, k = 0;

        while (i <= mid && j <= right) {
            if (nums[i] <= nums[j])
                temp[k++] = nums[i++];
            else
                temp[k++] = nums[j++];
        }
        while (i <= mid) temp[k++] = nums[i++];
        while (j <= right) temp[k++] = nums[j++];

        // 写回原数组
        for (int idx = 0; idx < temp.size(); ++idx)
            nums[left + idx] = temp[idx];
    }

    // ==================== 快速排序 ====================
    vector<int> sortArrayQuick(vector<int>& nums) {
        srand(time(0));  // 随机种子
        quickSort(nums, 0, nums.size() - 1);
        return nums;
    }

    void quickSort(vector<int>& nums, int left, int right) {
        if (left >= right) return;

        // 随机选择 pivot，避免在有序数组上退化
        int pivotIdx = left + rand() % (right - left + 1);
        swap(nums[pivotIdx], nums[right]);  // 将 pivot 移到最右

        int pivot = nums[right];
        int i = left;  // i 指向第一个 > pivot 的位置

        // Lomuto 分区方案
        for (int j = left; j < right; ++j) {
            if (nums[j] <= pivot) {
                swap(nums[i], nums[j]);
                ++i;
            }
        }
        swap(nums[i], nums[right]);  // 将 pivot 放到正确位置

        quickSort(nums, left, i - 1);
        quickSort(nums, i + 1, right);
    }
};

// 堆排序（作为第三种实现）
class HeapSort {
public:
    vector<int> sortArray(vector<int>& nums) {
        int n = nums.size();
        // 1. 建堆（Floyd 算法，O(n)）
        for (int i = n / 2 - 1; i >= 0; --i)
            heapify(nums, n, i);

        // 2. 不断取堆顶放末尾
        for (int i = n - 1; i > 0; --i) {
            swap(nums[0], nums[i]);   // 堆顶（max）移到末尾
            heapify(nums, i, 0);      // 缩小堆并调整
        }
        return nums;
    }

private:
    void heapify(vector<int>& nums, int heapSize, int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < heapSize && nums[left] > nums[largest])
            largest = left;
        if (right < heapSize && nums[right] > nums[largest])
            largest = right;

        if (largest != i) {
            swap(nums[i], nums[largest]);
            heapify(nums, heapSize, largest);
        }
    }
};

int main() {
    Solution s;
    vector<int> v1 = {5, 2, 3, 1};
    auto r1 = s.sortArray(v1);  // 归并排序
    for (int x : r1) cout << x << " "; cout << endl;  // 1 2 3 5

    vector<int> v2 = {5, 1, 1, 2, 0, 0};
    auto r2 = s.sortArrayQuick(v2);  // 快速排序
    for (int x : r2) cout << x << " "; cout << endl;  // 0 0 1 1 2 5

    // 边界测试：逆序数组（快排的最坏情况被随机 pivot 避免）
    vector<int> v3 = {9,8,7,6,5,4,3,2,1};
    auto r3 = s.sortArrayQuick(v3);
    for (int x : r3) cout << x << " "; cout << endl;

    return 0;
}
```

### 常见错误

- **错误1**：快排 pivot 始终选最左边或最右边，在已排序数组上退化成 O(n²)——如 `[1,2,3,4,5]` 选最右 pivot=5，partition 后右边为空左边全 4 个，递归深度 n。
- **错误2**：归并排序的 `merge` 函数中，temp 数组每次都分配，频繁内存分配影响性能。可以预分配一个全局临时数组复用。
- **错误3**：快排的 partition 函数中，`swap(nums[i], nums[j])` 的 i 初始值应为 `left` 而非 `left + 1`。如果只有两个元素且有序，会出错。
- **错误4**：堆排序中建堆时使用 `heapify`（从上往下），复杂度 O(n log n)；使用 Floyd 的从后往前建堆才是 O(n)。
- **错误5**：堆排序的索引计算——对于 0 为基础下标的数组，左孩子是 `2i+1`，右孩子 `2i+2`，父节点 `(i-1)/2`。

### 面试陷阱

- **陷阱1**："快排最坏情况怎么避免？"——随机化 pivot、三数取中法（取 left, mid, right 三者的中位数做 pivot）、在递归深度过大时转而用堆排序（Introsort = sort() 的实现）。
- **陷阱2**："大量重复元素时快排怎么优化？"——三路快排（Dutch national flag problem）：将数组分为 `< pivot | == pivot | > pivot` 三部分，只递归 `<` 和 `>` 部分。对于全相同元素，三路快排退化到 O(n)。
- **陷阱3**："归并排序的空间复杂度能优化到 O(1) 吗？"——可以，原地归并但实现复杂且通常影响时间到 O(n log² n)。实际场景中 O(n) 辅助空间更常用。
- **陷阱4**："C++ STL 的 `std::sort` 用的是什么？"——Introsort（内省排序）：快排 + 堆排序混合。快排深度超过 log n 时自动切换到堆排序，保证最坏 O(n log n)。是快排和堆排的优点的结合。
- **陷阱5**："为什么工程中常用快排而不是归并？"——快排原地排序（O(log n) 递归栈空间），缓存局部性好，常数因子小。归并排序需要 O(n) 辅助空间，在内存受限时不如快排。但外部排序（数据在磁盘）时归并排序是首选。

---

> **阶段五总结**：本阶段涵盖了二分查找进阶（7 道旋转数组+搜索）、数学与快速幂（4 道）、单调栈与栈模拟（3 道）、分治与排序（2 道）、矩阵遍历（4 道）、字符串 DP（2 道）及链表操作。这 25 道题是 LeetCode Hot200 中难度最高的部分，重点关注二分模板的灵活运用、DP 状态转移的推导、单调栈的维护逻辑、以及排序算法的底层实现。完成本阶段后，你已经覆盖了机试中 90% 以上的算法题型。
