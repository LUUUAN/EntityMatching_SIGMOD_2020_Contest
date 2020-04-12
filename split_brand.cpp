#include <set>
#include <map>
#include <iostream>
#include <string>
#include <regex>
#include <list>
#include "dataset.h"

using namespace std;

void postfix_split(string brand, string model, set<string> postfixes) {
    map<string, set<string>> postfix_dict;
    postfix_dict[model] = {};
    for (string postfix: postfixes) {
        postfix_dict[model+postfix] = {};
    }
    set<string>& product_list = model_index[brand][model];

    for(string product: product_list) {
        bool has_postfix = false;
        for (string postfix: postfixes) {
            string reg_ext_str = model.substr(0,1) + "[ ]?" + model.substr(1,1) + "[- ]?" + postfix;
            regex reg_ext(reg_ext_str, regex_constants::icase);
            if (regex_search(all_data[product]->page_title, reg_ext)) {
                postfix_dict[model+postfix].insert(product);
                has_postfix = true;
                break;
            }
        }
        if (!has_postfix) {
            postfix_dict[model].insert(product);
        }
    }

//    for (auto item: postfix_dict) {
//        cout << item.first << endl;
//        for (string p: item.second) {
//            cout << p << "," << all_data[p]->page_title << endl;
//        }
//        cout << endl;
//    }


    for (auto item: postfix_dict) {
        model_index[brand][item.first] = item.second;
    }
}




void generation_split(string brand, string model, set<string> spec_generations = {}) {
    set<string> product_list = model_index[brand][model];
    map<string, set<string>> generation_dict;
    list<pair<string, string>> generation_transfer = {{"4", "IV"}, {"3", "III"}, {"2", "II"}};

    generation_dict[model] = {};
    for (pair<string, string> p: generation_transfer) {
        generation_dict[model + " " + p.second] = {};
    }
    for (string s: spec_generations) {
        generation_dict[model + " " + s] = {};
    }

    for (string product: product_list) {
        string page_title = all_data[product]->page_title;
        bool is_spec = false;

        for (string spec_generation: spec_generations) {
            if (regex_search(page_title, regex(spec_generation, regex_constants::icase))) {
                generation_dict[model + " " + spec_generation].insert(product);
                is_spec = true;
                break;
            }
        }
        if (is_spec) {
            continue;
        }
        bool has_generation = false;

        for (pair<string, string> g: generation_transfer) {
            string reg_ext_str = "((MARK)|(MK))[ ]?((" + g.first + ")|(" + g.second + "))";
            if (regex_search(page_title, regex(reg_ext_str, regex_constants::icase))) {
                generation_dict[model + " " + g.second].insert(product);
                has_generation = true;
                break;
            }


            string reg_ext_str_2 = model + "(([ ]?M" + g.first +")|([ ]?" + g.second + "))";
            if (regex_search(page_title, regex(reg_ext_str_2, regex_constants::icase))) {
                generation_dict[model + " " + g.second].insert(product);
                has_generation = true;
                break;
            }
        }
        if (!has_generation) {
            generation_dict[model].insert(product);
        }
    }
    /*
    for (auto item: generation_dict) {
        cout << item.first << endl;
        for (string p: item.second) {
            cout << p << "," << all_data[p]->page_title << endl;
        }
        cout << endl;
    }*/


    for (auto item: generation_dict) {
        model_index[brand][item.first] = item.second;
    }
}




void split_brand() {
    postfix_split("Canon", "1D", {"X", "C", "S"});
    postfix_split("Canon", "G7", {"X"});

    generation_split("Canon", "1D", {"Mark II N"});
    generation_split("Canon", "1DS");
    generation_split("Canon", "5D");
    generation_split("Canon", "7D");
    generation_split("Canon", "G1");
    generation_split("Sony", "A77");
    generation_split("Sony", "RX100");
    model_index["Sony"]["RX100 III"].insert("www.ebay.com//60483");
    model_index["Sony"].erase("RX1002");

}
