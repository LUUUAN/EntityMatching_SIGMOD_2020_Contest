#include <string>
#include <algorithm>
#include <vector>
#include "util.h"
#include <unordered_set>

using namespace std;

string str_lower(const string& str) {
    string res = str;
    for(int i=0;i<str.length();i++){
        if(str[i]>=65 &&str[i]<=90) res[i] = str[i] + 32;
    }
    return res;
}


string str_upper(const string& str) {
    string res = str;
    for(int i=0;i<str.length();i++){
        if(str[i]>=97 &&str[i]<=122) res[i] = str[i] - 32;
    }
    return res;
}

string str_remove_space(string model_str) {
    str_lower(model_str);
    string ret;
    for (char i : model_str) {
        if (i == ' ' || i == '-') {
            continue;
        }
        ret += i;
    }
    return ret;
}

// Haotian: 传引用少一次函数对战更快，用find_first_(not)_of可以减少字符串的生成，字符串没加一次重新生成一个。并且原方法有一处没法替换。
//vector<string> split(string str){
//    vector<string> res;
//    string buf;
//    for(int i=0;i<str.length();i++){
//        if(str[i]==' '){
//            if(buf.length()!=0){
//                res.push_back(buf);
//                buf="";
//            }
//            continue;
//        }
//        buf += str[i];
//    }
//    return res;
//}

void split(const string& s, vector<string>& tokens, const string& delimiters) {
    string::size_type last_pos = s.find_first_not_of(delimiters, 0);
    string::size_type pos = s.find_first_not_of(delimiters, last_pos);
    while (string::npos != pos || string::npos != last_pos) {
        tokens.push_back(s.substr(last_pos, pos - last_pos));
        last_pos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, last_pos);
    }
}



bool is_alphabet(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}



int str_edit_distance(string a, string b) {
    a = str_lower(a);
    b = str_lower(b);
    int n = a.length();
    int m = b.length();
    int dp[n + 1][m + 1];
    for (int i = 0; i < n + 1; i++) {
        dp[i][0] = i;
    }
    for (int j = 0; j < m + 1; j++) {
        dp[0][j] = j;
    }
    for (int i = 1; i < n + 1; i++) {
        for (int j = 1; j < m + 1; j++) {
            int left = dp[i - 1][j] + 1;
            int down = dp[i][j - 1] + 1;
            int left_down = dp[i - 1][j - 1];
            if (a[i - 1] != b[j - 1]) left_down += 1;
            dp[i][j] = min(left, min(down, left_down));
        }
    }
    return dp[n][m];
}


bool set_share_more_than(const set<string>& a, const set<string>& b, const int& threshold) {
    int count = 0;
    for (const string& str_a: a) {
        if (b.find(str_a) != b.end()) {
            count ++;
            if (count > threshold) {
                return true;
            }
        }
    }
    return false;

}


void set_difference(const set<string>& a, const set<string>& b, set<string>& ret) {
    for (const string& str_a: a) {
        if (b.find(str_a) == b.end()) {
            ret.insert(str_a);
        }
    }
}