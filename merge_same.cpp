#include "merge_same.h"
#include "util.h"
#include "dataset.h"
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <list>
#include <regex>
#include <iostream>
#include <map>

using namespace std;


map<string, list<string>>prefix_dict = {
        {"Canon", {"IXUS", "ELPH", "IXY", "Powershot", "SX", "EOS", "SD", "S", "A", "G"}},
        {"Casio", {"EX", "QV"}},
        {"Fujifilm", {"Finepix", "X", "mini"}},
        {"Olympus", {"SP", "E", "TG", "SZ", "Stylus", "x", "XZ", "TG", "SH"}},
        {"Panasonic", {"DMC"}},
        {"Samsung", {"EC"}},
        {"Sony", {"NEX", "DSC", "DMC", "DSLR", "SLT", "ILCE"}},
        {"Nikon", {"Nikon"}}
};

map<string, string> meaningful_postfix_dict = {
        {"Fujifilm", "(s|t|w)"},
        {"Sony", "(r|s)"},
        {"Nikon", "(s|x|e|h)"},
        {"Canon", "i"}
};

map<string, list<list<string>>> auxiliary_same_dict = {
        {"Panasonic", {{"FT25", "TZ25", "ZS15"}, {"FT4", "TS4"}, {"TZ18", "ZS8"}, {"TZ20", "ZS10"}, {"TZ30", "ZS20"},
                       {"TZ40", "ZS30"}, {"TZ55", "ZS25"}, {"TZ7", "ZS3"}, {"TZ10", "ZS7"}, {"TZ60", "ZS40"}}},
        {"Canon", {{"IXUS 155", "ELPH 150"}, {"IXUS 510", "ELPH 530"}, {"IXUS 140", "ELPH 130"},
                   {"IXUS 145", "ELPH 135"}, {"IXUS 150", "ELPH 140"}, {"IXUS 240", "ELPH 320"},
                   {"IXUS 40", "SD300"}, {"IXUS 85", "SD770"}, {"IXUS 90", "SD790"},
                   {"IXUS 110", "SD960"}, {"IXUS 210", "SD3500"}, {"IXUS 700", "SD500"},
                   {"IXUS 800", "SD700"}, {"IXUS 850", "SD800"}, {"IXUS 900", "SD900"},
                   {"IXUS 950", "SD850"}, {"IXUS 960", "SD960"}, {"IXUS 970", "SD890"},
                   {"IXUS 980", "SD990"}, {"IXUS 1000", "SD4500"}, {"DS6041", "300D"}, {"DS126191", "1000D"},
                   {"X50", "1100D"}, {"DS126311", "600D"}, {"A3400", "A34000"}, {"IXUS i5", "SD20"},
                   {"IXUS i7", "SD40"}, {"1D", "EOS1"}, {"5D", "EOS5"}, {"7D", "EOS7"}}},
        {"Fujifilm", {{"JX520", "JX500"}, {"HS30", "HS33"}, {"HS25", "HS28"}}},
{"Olympus", {{"EM5", "M5"}}},
        {"Sony", {{"RX100", "RX100M3", "RX100M2", "RX100M3B"}}}
};



string refine_model(const string& brand_name, const string& model_raw) {
    string model_clean = model_raw;
    const list<string>& prefixes = prefix_dict[brand_name];
    string meaningful_postfix_reg;
    if (meaningful_postfix_dict.find(brand_name) != meaningful_postfix_dict.end()) {
        meaningful_postfix_reg = meaningful_postfix_dict[brand_name];
    }
    for (const string& reg: prefixes) { // 前缀标准化
        string reg_ext_str;
        if (reg.size() == 1) {
            reg_ext_str = "^" + reg + "[- ](.*)";
        }
        else {
            reg_ext_str = "^" + reg + "[- ]?(.*)";
        }
        regex pattern_1(reg_ext_str, regex_constants::icase);
        smatch results_1;
        bool success = regex_match(model_clean, results_1, pattern_1);
        if (success) {
            string tmp_str = *(results_1.begin()+1);
            if (!regex_match(tmp_str, regex("[0-9]+[a-zA-Z]*$", regex_constants::icase))) {
                model_clean = tmp_str;
            }
            else {
                model_clean = reg + tmp_str;
            }
        }
    }

    regex pattern_2("([a-zA-Z]+)[ -]([0-9]+)$", regex_constants::icase);
    smatch results_2;
    bool success = regex_match(model_clean, results_2, pattern_2);
    if (success) {
        string tmp_1 = *(results_2.begin()+1);
        string tmp_2 = *(results_2.begin()+2);
        model_clean = tmp_1 + tmp_2;
    }

    string post_reg_str = ".*" + meaningful_postfix_reg + "$";
    if (regex_match(model_clean, regex("[a-zA-Z]+[0-9]+[a-zA-Z]+", regex_constants::icase))) {
        if (!(!meaningful_postfix_reg.empty() && regex_match(model_raw, regex(post_reg_str, regex_constants::icase)))) {
            regex pattern_3 ("([a-zA-Z]+[0-9]+)[-a-zA-Z]+");
            smatch result;
            regex_search(model_clean, result, pattern_3);
            model_clean = *(result.begin()+1);
        }
    }
    return str_upper(model_clean);

}



inline void merge(const string& to_model, const string& from_model, const string& brand) {
    if (from_model == to_model) return;
    model_index[brand][to_model].insert(model_index[brand][from_model].begin(),
                                        model_index[brand][from_model].end());
    model_index[brand].erase(from_model);
}



void merge_same() {
    for (const auto& brand_item: model_index) {
        string brand_name = brand_item.first;
        if (prefix_dict.find(brand_name) == prefix_dict.end()) {
            continue;
        }
        cout << "Merge same: " << brand_name << endl;
        map<string, list<string>> model_dict;
        for (const auto& model_item: brand_item.second) {
            string model_name = model_item.first;
            string refined_model = refine_model(brand_name, model_name);
            //cout << "REFINE: " << model_name << "->" << refined_model << endl;
            model_dict[refined_model].push_back(model_name);
        }


//        for (auto item: model_dict) {
//            if (item.second.size() >= 2) {
//                for (string i: item.second) {
//                    cout << i << ",";
//                }
//                cout << endl;
//            }
//        }


        //unordered_map<string, unordered_set<string>>
        if (auxiliary_same_dict.find(brand_name) != auxiliary_same_dict.end()) {
            list<list<string>> same_models = auxiliary_same_dict[brand_name];
            for (const list<string>& same_list: same_models) {
                vector<string> merge_list;
                for (const string& item: same_list) {
                    string tmp = refine_model(brand_name, item);
                    if (model_dict.find(tmp) != model_dict.end()) {
                        merge_list.push_back(tmp);
                    }
                }

                if (merge_list.size() > 1) {

                    cout << "merge list: ";
                    for (string s: merge_list) {
                        cout << s << " ";
                    }
                    cout << endl;



                    string dest_model = merge_list[0];
                    for (int i = 1; i < merge_list.size(); i++) {
			if(merge_list[i]==dest_model) continue;
                        for (string s: model_dict[merge_list[i]]) {
                            model_dict[dest_model].push_back(s);
                        }
//                        model_dict[dest_model].insert(model_dict[merge_list[i]].begin(),
//                                                      model_dict[merge_list[i]].end());
                        cout << "Model dict: " << merge_list[i] << " to " << dest_model << endl;
                        model_dict.erase(merge_list[i]);
                    }
                }
            }
        }

        /*
        cout << "model dict:" << brand_name << endl;
        for (auto item: model_dict) {
            cout << item.first << ":  ";
            for (string x: item.second) {
                cout << x << ";";
            }
            cout << endl;
        }*/




        //unordered_map<string, unordered_set<string>>
        for (auto item: model_dict) {
            vector<string> same_list {item.second.begin(), item.second.end()};
            sort(same_list.begin(), same_list.end());

//            int min_length = same_list[0].length();
//            int min_index = 0;
//            for (int i = 1; i < same_list.size(); i ++) {
//                if (same_list[i].length() < min_length) {
//                    min_length = same_list[i].length();
//                    min_index = i;
//                }
//            }
//            string tmp = same_list[min_index];
//            same_list[min_index] = same_list[0];
//            same_list[0] = tmp;


            //if (same_list.size() > 1) {
            for (int i = 0; i < same_list.size(); i++) {
                //cout << "Merge: " << same_list[i] <<  " to " << item.first << endl;
                merge(item.first, same_list[i], brand_name);
//                    if (item.first != same_list[i]) {
//                        model_index[brand_name].erase(item.first);
//                    }
            }
            //}
        }
    }
}
