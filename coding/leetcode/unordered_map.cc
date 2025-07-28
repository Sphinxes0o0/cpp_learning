#include <iostream>
#include <vector>

// 自定义哈希集合实现
class MyHashSet {
private:
    // 链表节点结构
    struct ListNode {
        int key;
        ListNode* next;
        ListNode(int k) : key(k), next(nullptr) {}
    };
    
    static const int BUCKET_SIZE = 1000;  // 哈希表大小
    std::vector<ListNode*> buckets;       // 存储链表头节点的数组
    
    // 哈希函数
    int hash(int key) {
        return key % BUCKET_SIZE;
    }
    
    // 在指定桶中查找节点
    ListNode* findNode(int bucketIdx, int key) {
        ListNode* curr = buckets[bucketIdx];
        while (curr != nullptr) {
            if (curr->key == key) {
                return curr;
            }
            curr = curr->next;
        }
        return nullptr;
    }

public:
    // 构造函数
    MyHashSet() : buckets(BUCKET_SIZE, nullptr) {}
    
    // 析构函数 - 释放内存
    ~MyHashSet() {
        for (int i = 0; i < BUCKET_SIZE; i++) {
            ListNode* curr = buckets[i];
            while (curr != nullptr) {
                ListNode* next = curr->next;
                delete curr;
                curr = next;
            }
        }
    }
    
    // 向哈希集合中插入值 key
    void add(int key) {
        int bucketIdx = hash(key);
        
        // 如果已经存在，直接返回
        if (findNode(bucketIdx, key) != nullptr) {
            return;
        }
        
        // 在链表头部插入新节点
        ListNode* newNode = new ListNode(key);
        newNode->next = buckets[bucketIdx];
        buckets[bucketIdx] = newNode;
    }
    
    // 返回哈希集合中是否存在这个值 key
    bool contains(int key) {
        int bucketIdx = hash(key);
        return findNode(bucketIdx, key) != nullptr;
    }
    
    // 将给定值 key 从哈希集合中删除
    void remove(int key) {
        int bucketIdx = hash(key);
        ListNode* curr = buckets[bucketIdx];
        
        // 如果要删除的是头节点
        if (curr != nullptr && curr->key == key) {
            buckets[bucketIdx] = curr->next;
            delete curr;
            return;
        }
        
        // 查找要删除的节点
        while (curr != nullptr && curr->next != nullptr) {
            if (curr->next->key == key) {
                ListNode* nodeToDelete = curr->next;
                curr->next = nodeToDelete->next;
                delete nodeToDelete;
                return;
            }
            curr = curr->next;
        }
    }
    
    // 辅助函数：打印哈希集合内容（用于调试）
    void printSet() {
        std::cout << "HashSet contents: ";
        for (int i = 0; i < BUCKET_SIZE; i++) {
            ListNode* curr = buckets[i];
            while (curr != nullptr) {
                std::cout << curr->key << " ";
                curr = curr->next;
            }
        }
        std::cout << std::endl;
    }
};

int main() {
    // 测试示例
    MyHashSet myHashSet;
    
    myHashSet.add(1);      // set = [1]
    myHashSet.add(2);      // set = [1, 2]
    std::cout << "contains(1): " << myHashSet.contains(1) << std::endl; // 返回 true
    std::cout << "contains(3): " << myHashSet.contains(3) << std::endl; // 返回 false
    myHashSet.add(2);      // set = [1, 2] (2已存在，不会重复添加)
    std::cout << "contains(2): " << myHashSet.contains(2) << std::endl; // 返回 true
    myHashSet.remove(2);   // set = [1]
    std::cout << "contains(2): " << myHashSet.contains(2) << std::endl; // 返回 false
    
    // 打印当前集合内容
    myHashSet.printSet();
    
    return 0;
}