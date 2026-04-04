#include <bits/stdc++.h>
#include <chrono>
using namespace std;


struct Node {
    string key;
    string value;
    long long expiry;
    Node* prev;
    Node* next;

    Node(string k, string v, long long e)
        : key(k), value(v), expiry(e), prev(nullptr), next(nullptr) {}
};

class LRUCacheTTL {

private:

    int capacity;
    unordered_map<string, Node*> cache;

    Node* head;
    Node* tail;

    long long now() {
        return chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch()
        ).count();
    }

    // Remove node from list
    void remove(Node* node) {
        if (!node) return;

        if (node->prev) node->prev->next = node->next;
        else head = node->next;

        if (node->next) node->next->prev = node->prev;
        else tail = node->prev;
    }

    // Insert at front
    void insertFront(Node* node) {
        node->next = head;
        node->prev = nullptr;

        if (head) head->prev = node;
        head = node;

        if (!tail) tail = node;
    }

    bool isExpired(Node* node) {
        return now() > node->expiry;
    }

public:

    LRUCacheTTL(int cap) : capacity(cap), head(nullptr), tail(nullptr) {}

    string get(string key) {
        if (cache.find(key) == cache.end()) return "null";

        Node* node = cache[key];

        // TTL check
        if (isExpired(node)) {
            remove(node);
            cache.erase(key);
            delete node;
            return "null";
        }

        // Move to front
        remove(node);
        insertFront(node);

        return node->value;
    }

    void put(string key, string value, long long ttl_ms) {
        long long expiry = now() + ttl_ms;

        if (cache.find(key) != cache.end()) {
            Node* node = cache[key];
            node->value = value;
            node->expiry = expiry;

            remove(node);
            insertFront(node);
            return;
        }

        // Evict if needed
        if ((int)cache.size() >= capacity) {
            Node* lru = tail;

            remove(lru);
            cache.erase(lru->key);
            delete lru;
        }

        Node* node = new Node(key, value, expiry);
        insertFront(node);
        cache[key] = node;
    }
};

int main() {
    LRUCacheTTL cache(2);

    cache.put("1", "A", 3000); // 3 sec
    cache.put("2", "B", 3000);

    cout << cache.get("1") << endl; // A

    cache.put("3", "C", 3000); // evicts "2"

    cout << cache.get("2") << endl; // null

    this_thread::sleep_for(chrono::seconds(4));

    cout << cache.get("1") << endl; // null (expired)
    cout << cache.get("3") << endl; // null (expired)

    return 0;
}