#include <list>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iostream>
#include "index_brand.h"
#include "dataset.h"
#include <regex>
#include "util.h"
#include <set>

using namespace std;
typedef basic_regex<char> regex;

string special_brand[] = {"Hikvision", "Dahua", "Konica", "Cannon", "Coolpix", "Vista Quest", "Go Pro","Minolta"};
string ban_list[] = {"Tamron", "SHOOT"};


list<string> brand_list;
map<string, set<string> *> brand_items;


bool is_valid_brand(string brand) {
    if (brand == "unknown") {
        return false;
    }
    vector<string> tmp;
    //boost::split(tmp, brand, boost::is_any_of(" "), boost::token_compress_on);
    split(brand, tmp);
    if (tmp.size() > 2) {
        return false;
    }
    for (const string& word: tmp) {
//        regex all_alphabet("^[a-zA-Z]+$", regex::icase);
//        smatch sm;
//        regex_match(brand, sm, all_alphabet);
        for(char ch: word) {
            if (!is_alphabet(ch)) {
                return false;
            }
        }
    }
    return true;
}



void remove_low_frequency() {
    int threshold = brand_items.size();
    list<string> del_list;
    for (const auto& item: brand_items) {
        if ((item.second)->size() <= 0.01 * threshold) {
            del_list.push_back(item.first);
        }
        else {
            brand_list.push_back(item.first);
        }
    }
    for (const string& del_item: del_list) {
        brand_items.erase(del_item);
    }
}

void remove_duplicate() {
    list<string> del_list;
    for (string brand: brand_list) {
        vector<string> tmp;
        split(brand, tmp);
        if (tmp.size() > 1 && (brand_items.find(tmp[0]) != brand_items.end())) {
            del_list.push_back(brand);
        }
    }
    for (const string& brand: del_list) {
        brand_list.remove(brand);
    }

}

void add_special() {
    for (const string& brand: special_brand) {
        brand_list.push_back(brand);
    }
}


void get_brand_list() {
    cout << "Getting brand list..." << endl;
    for (const auto& item: all_data) {
        string product_id = item.first;
        auto *product_content = item.second;
        string brand = product_content->brand;
        if (brand.empty() || !is_valid_brand(brand)) {
            continue;
        }
        bool is_banned = false;
        for (const string& ban: ban_list) {
            if (str_lower(brand) == str_lower(ban)) {
                is_banned = true;
                break;
            }
        }
        if (is_banned) {
            continue;
        }

        if (brand_items.find(brand) == brand_items.end()) {
            auto *se = new set<string>;
            brand_items[brand] = se;
        }
        brand_items[brand]->insert(product_id);
    }
    remove_low_frequency();
    remove_duplicate();
    add_special();

}



bool match(const string& brand, const string& page_title) {
    if (brand.length() < 4) {
        vector<string> tmp;
        split(page_title, tmp);
        for (const string& word: tmp) {
            if (word == brand) {
                return true;
            }
        }
        return false;
    }
    else {
        return str_lower(page_title).find(str_lower(brand)) != string::npos;
    }
}



void blocking() {
    cout << "Blocking according to the brands..." << endl;
    for (const string& brand: brand_list) {
        set<string> visit;
        bool flag = !(brand == "Hikvision" || brand == "Dahua");
        set<string> ids;
        for (const auto& item: all_data) {
            auto *data_content = item.second;
            string key = item.first;
            string page_title = data_content->page_title;
            if (match(brand, page_title)) {
                if (flag && key.find("www.alibaba.com") != key.npos) {
                    continue;
                }
                if (key.find("www.ebay.com") != key.npos && str_lower(page_title).find("lot of") != page_title.npos) {
                    continue;
                }
                visit.insert(key);
                ids.insert(key);
            }
        }
        if (brand_items.find(brand) != brand_items.end()) {
            for (const string& brand_item: *brand_items[brand]) {
                if (visit.find(brand_item) == visit.end()) {
                    visit.insert(brand_item);
                    ids.insert(brand_item);
                }
            }
        }
        brand_index[brand] = ids;
    }
}



void merge(const string& to_brand, const string& from_brand) {
    if (brand_index.find(from_brand) != brand_index.end() &&
        brand_index.find(to_brand) != brand_index.end()) {
        brand_index[to_brand].insert(brand_index[from_brand].begin(), brand_index[from_brand].end());
        brand_index.erase(from_brand);
        cout << "Merge brand: from " << from_brand << " to " << to_brand << endl;
    }

}


void merge_pair() {
    vector<string> brands(brand_list.begin(), brand_list.end());
    set<string> del_set;
    sort(brands.begin(), brands.end());
    for (int i = 0; i < brands.size(); i++) {
        for (int j = i + 1; j < brands.size(); j++) {
            int edit_distance = str_edit_distance(brands[i], brands[j]);
            if (edit_distance < 2 ||
                brands[i].find(brands[j]) != string::npos ||
                brands[j].find(brands[i]) != string::npos) {
                if (del_set.find(brands[i]) == del_set.end()) {
                    merge(brands[j], brands[i]);
                    del_set.insert(brands[i]);
                }
            }
        }
    }

    merge("Nikon", "Coolpix");
    merge("Minolta", "Konica");
}



void index_brand() {
    cout << "Indexing brand!" << endl;
    get_brand_list();
    blocking();
    merge_pair();
}
