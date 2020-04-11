//
// Created by ludwig on 2020/4/9.
//

#ifndef SIGMOD_CONTEST_MERGE_SAME_H
#define SIGMOD_CONTEST_MERGE_SAME_H

#include <string>
#include <unordered_set>
#include <list>
#include <unordered_map>
using namespace std;

extern unordered_map<string, unordered_set<string> >prefix_dict;
extern unordered_map<string, string> meaningful_postfix_dict;
extern unordered_map<string, list<list<string> > > auxiliary_same_dict;

string refine_model(const string& brand_name, const string& model_raw);

void merge_same();
#endif //SIGMOD_CONTEST_MERGE_SAME_H
