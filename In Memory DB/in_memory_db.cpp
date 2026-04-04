#include <bits/stdc++.h>
#include <chrono>
using namespace std;

class Db {

private:
    unordered_map<string, unordered_map<string, string>> database;
    unordered_map<string, unordered_map<string, pair<long long, long long>>> ttl;

    long long getCurrentTime() {
        auto now = chrono::system_clock::now();
        return chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
    }

    bool isPresent(const string& table, const string& key) {
        auto it1 = database.find(table);
        if (it1 == database.end()) return false;

        auto it2 = it1->second.find(key);
        return it2 != it1->second.end();
    }

    bool isStillAlive(const string& table, const string& key) {
        auto it1 = ttl.find(table);
        if (it1 == ttl.end()) return true; // no TTL → alive

        auto it2 = it1->second.find(key);
        if (it2 == it1->second.end()) return true;

        auto [timestamp, duration] = it2->second;

        long long expiry = timestamp + duration;
        long long current = getCurrentTime();

        return current <= expiry;
    }

    void removeKey(const string& table, const string& key) {
        database[table].erase(key);
        ttl[table].erase(key);

        if (database[table].empty()) {
            database.erase(table);
            ttl.erase(table);
        }
    }

public:

    // SET
    void set(const string& table, const string& key, const string& value) {
        database[table][key] = value;
        ttl[table][key] = {getCurrentTime(), LLONG_MAX}; // no expiry
    }

    // GET
    string get(const string& table, const string& key) {
        if (!isPresent(table, key)) return "null";

        if (!isStillAlive(table, key)) {
            removeKey(table, key);
            return "null";
        }

        return database[table][key];
    }

    // DELETE
    bool deleteKey(const string& table, const string& key) {
        if (!isPresent(table, key)) return false;

        if (!isStillAlive(table, key)) {
            removeKey(table, key);
            return false;
        }

        removeKey(table, key);
        return true;
    }

    // SET TTL
    void setTTL(const string& table, const string& key, long long duration) {
        if (!isPresent(table, key)) return;

        ttl[table][key] = {getCurrentTime(), duration};
    }
};




int main() {
    Db obj;

    cout << "===== Basic Test =====\n";
    obj.set("users", "1", "Alice");
    cout << obj.get("users", "1") << endl; // Alice

    cout << obj.deleteKey("users", "1") << endl; // 1
    cout << obj.get("users", "1") << endl; // null

    cout << "\n===== TTL Test =====\n";
    obj.set("users", "2", "Bob");

    cout << obj.get("users", "2") << endl; // Bob

    obj.setTTL("users", "2", 2000); // 2 seconds

    cout << "Sleeping 1 sec...\n";
    this_thread::sleep_for(chrono::seconds(1));

    cout << obj.get("users", "2") << endl; // Bob (still alive)

    cout << "Sleeping 2 sec...\n";
    this_thread::sleep_for(chrono::seconds(2));

    cout << obj.get("users", "2") << endl; // null (expired)

    cout << "\n===== Edge Case =====\n";
    obj.set("users", "3", "Charlie");
    obj.setTTL("users", "3", 0); // immediate expiry

    cout << obj.get("users", "3") << endl; // null

    return 0;
}