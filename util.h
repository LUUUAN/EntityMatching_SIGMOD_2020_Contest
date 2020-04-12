#ifndef SIGMOD_CONTEST_UTIL_H
#define SIGMOD_CONTEST_UTIL_H

#include <string>
#include <unordered_set>
#include <vector>
#include <set>
using namespace std;

string str_lower(const string& str);
string str_upper(const string& str);
string str_remove_space(string model_str);
void split(const string& s, vector<string>& tokens, const string& delimiters = " ");
int str_edit_distance(string a, string b);

bool set_share_more_than(const set<string>& a, const set<string>& b, const int& threshold = 2);
void set_difference(const set<string>& a, const set<string>& b, set<string>& ret);
bool is_alphabet(char c);
bool is_digit(char c);

#endif //SIGMOD_CONTEST_UTIL_H