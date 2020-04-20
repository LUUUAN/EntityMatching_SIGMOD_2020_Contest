#include <iostream>
#include "dataset.h"
#include "util.h"
#include "merge_same.h"
#include <regex>

using namespace std;


void resolve_others_brand(string brand) {
    cout << "Resolve other: " << brand << endl;
    list<string> prefix_reg_set = prefix_dict[brand];
    set<string> del_set;

    for (string product: *others[brand]) {
        string page_title = all_data[product]->page_title;
        for (string prefix: prefix_reg_set) {
            string reg_ext_str;
            if (prefix.length() == 1) {
                reg_ext_str = "[ ,]" + prefix + "[0-9]+[a-zA-Z]*";
            } else {
                reg_ext_str = "[ ,]" + prefix + "[ -]?[0-9a-zA-Z]+";
            }
            regex reg_ext(reg_ext_str);
            smatch result;
            bool success = regex_search(page_title, result, reg_ext);
            if (success) {
                string found_str;
                for(int i = 0; i < result.size(); i ++){
                    string tmp = *(result.begin() + i);
                    if (regex_search(tmp, regex("[0-9]+"))) {
                        found_str = tmp;
                        break;
                    }
                }
                if (found_str.empty()) {
                    continue;
                }
                string model_name = found_str.substr(1, found_str.length()-1);

                model_index[brand][model_name].insert(product);
                del_set.insert(product);
            }
        }
    }
    for (string del_item: del_set) {
        others[brand]->erase(del_item);
    }

}



void collect_remains() {
    cout << "Collecting remains!" << endl;
    for (const auto& brand_item: model_index) {
        string brand = brand_item.first;
        set<string> all_set(brand_index[brand].begin(), brand_index[brand].end());
        set<string> exist_set;
        for (const auto& model_item: brand_item.second) {
            for (const string& product_id: model_item.second) {
                exist_set.insert(product_id);
            }
        }
        auto *brand_others = new set<string>;
        set_difference(all_set, exist_set, *brand_others);
        others[brand] = brand_others;
    }
}




void resolve_others() {
    collect_remains();
    cout << "Resolve others!" << endl;
    for (auto brand_item: prefix_dict) {
        string brand_name = brand_item.first;
        resolve_others_brand(brand_name);
    }
}
