#include <bits/stdc++.h>
using namespace std;

class URLShortener {
private:
    unordered_map<string, string> shortToLong;
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

public:
    string createURL(string longURL) {
        if (longToShort.find(longURL) != longToShort.end()) {
            return longToShort[longURL];
        }

        string shortURL = encode(counter++);
        
        shortToLong[shortURL] = longURL;
        longToShort[longURL] = shortURL;

        return shortURL;
    }

    string redirect(string shortURL) {
        if (shortToLong.find(shortURL) != shortToLong.end()) {
            return shortToLong[shortURL];
        }
        return "NOT FOUND";
    }

    bool removeURL(string shortURL) {
        if (shortToLong.find(shortURL) == shortToLong.end()) {
            return false;
        }

        string longURL = shortToLong[shortURL];

        shortToLong.erase(shortURL);
        longToShort.erase(longURL);

        return true;
    }

    bool update(string shortURL, string newLongURL) {
        if (shortToLong.find(shortURL) == shortToLong.end()) {
            return false;
        }

        string oldLongURL = shortToLong[shortURL];

        longToShort.erase(oldLongURL);
        shortToLong[shortURL] = newLongURL;
        longToShort[newLongURL] = shortURL;

        return true;
    }
};


int main() {
    URLShortener obj;

    string shorturl = obj.createURL("https://google.com");
    cout << "Short URL: " << shorturl << endl;

    cout << "Redirect: " << obj.redirect(shorturl) << endl;

    obj.update(shorturl, "https://openai.com");
    cout << "After Update: " << obj.redirect(shorturl) << endl;

    obj.removeURL(shorturl);
    cout << "After Delete: " << obj.redirect(shorturl) << endl;
}