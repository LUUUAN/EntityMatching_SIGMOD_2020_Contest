#ifndef SIGMOD_CONTEST_MERGE_SAME_H
#define SIGMOD_CONTEST_MERGE_SAME_H

#include <string>
#include <unordered_set>
#include <list>
#include <unordered_map>
#include <map>
#include <set>
using namespace std;

extern map<string, list<string>>prefix_dict;
extern map<string, string> meaningful_postfix_dict;
extern map<string, list<list<string>>> auxiliary_same_dict;

string refine_model(const string& brand_name, const string& model_raw);

void merge_same();
#endif //SIGMOD_CONTEST_MERGE_SAME_H