#include <algorithm>
#include <iostream>
#include "dataset.h"
#include "util.h"
using namespace std;


void intersection() {
    cout << "Merge brands: same models with different representations!" << endl;
    for (auto brand_item: model_index) {
        set<string> del_set;
        string brand_name = brand_item.first;
        cout << "Merge brand: " << brand_name << endl;
        //<model_name, product_set>
        vector<pair<string, set<string>>> model_items =
                {brand_item.second.begin(), brand_item.second.end()};

        sort(model_items.begin(), model_items.end(), [](pair<string, set<string>> x,
                                                        pair<string, set<string>> y) { return x.first < y.first; });

        for (int i = 0; i < model_items.size(); i++) {
            if (del_set.find(model_items[i].first) != del_set.end()) {
                continue;
            }
            for (int j = i + 1; j < model_items.size(); j ++) {
                if (del_set.find(model_items[j].first) != del_set.end()) {
                    continue;
                }
                if(set_share_more_than(model_items[i].second, model_items[j].second)) {

                    cout << "Merge: " << model_items[j].first << " to " << model_items[i].first << endl;
                    model_index[brand_name][model_items[i].first].
                            insert(model_items[j].second.begin(), model_items[j].second.end());
                    del_set.insert(model_items[j].first);
                }
            }

        }
        for (const string& del_model: del_set) {
            model_index[brand_name].erase(del_model);
        }
    }
}
