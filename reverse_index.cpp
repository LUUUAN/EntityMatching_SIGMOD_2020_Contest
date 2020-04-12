#include <unordered_map>
#include <iostream>
#include "dataset.h"
#include <list>

using namespace std;


map<string, list<pair<string, string>>> reversed_index;
list<string> rm_list {
        "www.ebay.com//56876", "www.ebay.com//45419", "www.ebay.com//23996", "www.ebay.com//23901", "www.ebay.com//48556",
        "www.ebay.com//59616", "www.ebay.com//48556", "www.ebay.com//54133", "www.ebay.com//55586", "www.ebay.com//42549",
        "www.ebay.com//47424", "www.ebay.com//44323", "www.ebay.com//46513", "www.ebay.com//45893", "www.ebay.com//47037",
        "www.ebay.com//55515"
};


void build_reverse_index() {
    for (auto brand_item: model_index) {
        string brand_name = brand_item.first;
        for (auto model_item: brand_item.second) {
            string model_name = model_item.first;
            for (string product_id: model_item.second) {
                reversed_index[product_id].push_back({brand_name, model_name});
            }
        }
    }
}



void multiple_models() {
    cout << "Remove product with multiple models!" << endl;
    build_reverse_index();
    for (const string& product: rm_list) {
        list<pair<string, string>> rm_tuples = reversed_index[product];
        for (pair<string, string> p: rm_tuples) {
            model_index[p.first][p.second].erase(product);
        }
    }
//    for (auto item: reversed_index) {
//        if (item.second.size() >= 3) {
//            for (pair<string, string> p: item.second) {
//                model_index[p.first][p.second].erase(item.first);
//                cout << "Remove multiple: " << item.first << " from " << p.first << "," << p.second << endl;
//            }
//        }
//    }
}