#include <bits/stdc++.h>
using namespace std;

class URLShortener {
private:
    struct URLData {
        string longURL;
        long long expiryTime; // epoch time (seconds), -1 means no expiry
    };

    unordered_map<string, URLData> shortToData;
    unordered_map<string, string> longToShort;

    long long counter = 1;
    const string base62 = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // number → Base62 string
    string encode(long long num) {
        string s = "";
        while (num > 0) {
            s += base62[num % 62];
            num /= 62;
        }
        reverse(s.begin(), s.end());
        return s;
    }

    long long getCurrentTime() {
        return chrono::duration_cast<chrono::seconds>(
                   chrono::system_clock::now().time_since_epoch())
            .count();
    }

    bool isExpired(const string &shortURL) {
        if (shortToData.find(shortURL) == shortToData.end()) return true;

        long long expiry = shortToData[shortURL].expiryTime;
        if (expiry == -1) return false;

        return getCurrentTime() > expiry;
    }

public:
    string createURL(string longURL) {
        return createURL(longURL, -1);
    }

    string createURL(string longURL, long long ttlSeconds) {
        if (longToShort.find(longURL) != longToShort.end()) {
            return longToShort[longURL];
        }

        string shortURL = encode(counter++);

        long long expiry = -1;
        if (ttlSeconds != -1) {
            expiry = getCurrentTime() + ttlSeconds;
        }

        shortToData[shortURL] = {longURL, expiry};
        longToShort[longURL] = shortURL;

        return shortURL;
    }

    string redirect(string shortURL) {
        if (shortToData.find(shortURL) == shortToData.end()) {
            return "NOT FOUND";
        }

        if (isExpired(shortURL)) {
            string longURL = shortToData[shortURL].longURL;
            shortToData.erase(shortURL);
            longToShort.erase(longURL);
            return "NOT FOUND";
        }

        return shortToData[shortURL].longURL;
    }

    bool removeURL(string shortURL) {
        if (shortToData.find(shortURL) == shortToData.end()) {
            return false;
        }

        string longURL = shortToData[shortURL].longURL;

        shortToData.erase(shortURL);
        longToShort.erase(longURL);

        return true;
    }

    bool update(string shortURL, string newLongURL) {
        if (shortToData.find(shortURL) == shortToData.end()) {
            return false;
        }

        if (isExpired(shortURL)) {
            removeURL(shortURL);
            return false;
        }

        string oldLongURL = shortToData[shortURL].longURL;
        long long expiry = shortToData[shortURL].expiryTime;

        longToShort.erase(oldLongURL);

        shortToData[shortURL] = {newLongURL, expiry};
        longToShort[newLongURL] = shortURL;

        return true;
    }
};


int main() {
    URLShortener obj;

    // Without TTL 
    string s1 = obj.createURL("https://google.com");
    cout << obj.redirect(s1) << endl;

    // With TTL
    string s2 = obj.createURL("https://openai.com", 5);
    cout << obj.redirect(s2) << endl;

    this_thread::sleep_for(chrono::seconds(6));

    cout << obj.redirect(s2) << endl; // NOT FOUND
}