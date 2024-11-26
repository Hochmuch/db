#include "utils.h"

void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

std::string toUpper(std::string s) {
    std::string res;
    for (char c : s) {
        if (std::isupper(c)) {
            res += c;
        } else {
            res += std::toupper(c);
        }
    }
    return res;
}