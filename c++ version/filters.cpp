#include <unordered_map>
#include <unordered_set>
#include <string>
#include <list>
#include <iostream>
#include "dataset.h"
#include "util.h"
#include <set>

using namespace std;

map<string, set<string>> ban_dict = {
        {"Canon", {"4l", "8f", "e1", "e3", "e7", "efs 18", "eos-1d", "is 24", "lens 18",
                          "lens 55", "quantaray 18", "f3", "s 18", "s 55", "sigma 18", "sigma 70",
                          "slr 10", "stm 18", "tamron 18", "v1", "usm 75"}},
        {"Fujifilm", {"f1", "p1", "v1"}},
        {"Magnavox", {"F7"}},
        {"Nikon", {"af 18", "af 70", "f1", "f3", "f4", "f5", "nikkor 18", "nikkor 35", "nikkor 55",
                          "nikon 16", "p1", "sigma 70", "u1", "vr10", "y1","v1","v2"}},
        {"Olympus", {"e1", "f2", "ihs 12", "ihs 14", "olympus 14", "p1", "v2"}},
        {"Panasonic", {"f2", "f3", "lumix 14"}},
        {"Pentax", {"al 18", "da 18", "dal 18", "l 18"}},
        {"Philips", {"F7"}},
        {"Sony", {"and 18", "b 16", "class 10", "digital 20", "e 18", "f1", "f2", "f3", "p1",
                          "pz 16", "sony 16", "sony18", "v1"}}
};


void filter_ban_list() {
    cout << "Filtering: Ban Set!" << endl;
    for (auto item: model_index) {
        string brand = item.first;
        if (ban_dict.find(brand) != ban_dict.end()) {
            const set<string>& del_set = ban_dict[brand];
            for (string del_item: del_set) {
                string upper = str_upper(del_item);
                if (model_index[brand].find(upper) != model_index[brand].end()) {
                    model_index[brand].erase(upper);
                }
            }
        }
    }
}

void filter_Dahua() {
    string brand = "Dahua";
    cout << "Filtering: " << brand << endl;
    list<string> del_list;
    for (const auto& model: model_index[brand]) {
        string model_name = model.first;
        if (model_name.length() <= 4 || model_name.find(' ') != model_name.npos) {
            del_list.push_back(model_name);
        }
    }
    for (const string& del_model: del_list) {
        model_index[brand].erase(del_model);
    }
}


void filter_Hikivision() {
    string brand = "Hikvision";
    cout << "Filtering: " << brand << endl;
    list<string> del_list;
    for (const auto& model: model_index[brand]) {
        string model_name = model.first;
        if (model_name[0] == 'I' || model_name.find('-') == model_name.npos) {
            del_list.push_back(model_name);
        }
    }
    for (const string& del_model: del_list) {
        model_index[brand].erase(del_model);
    }
}


void filter_SVP() {
    string brand = "SVP";
    cout << "Filtering: " << brand << endl;
    list<string> del_list;
    for (const auto& model: model_index[brand]) {
        string model_name = model.first;
        if (model_name.substr(0,3) == "500") {
            del_list.push_back(model_name);
        }
    }
    for (const string& del_model: del_list) {
        model_index[brand].erase(del_model);
    }
}


void filter_Kodak() {
    string brand = "Kodak";
    cout << "Filtering: " << brand << endl;
    map<string, string> del_map;
    for (const auto& model: model_index[brand]) {
        string model_name = model.first;
        for (const string& product_id: model.second) {
            string page_title = all_data[product_id]->page_title;
            if (page_title.find("Printer Dock") != page_title.npos
                || page_title.find("2 Digital Cameras with") != page_title.npos) {
                del_map[product_id] = model_name;
            }
        }
    }
    for (auto item: del_map) {
        model_index[brand][item.second].erase(item.first);
    }
}


void filtering() {
    cout << "Filtering the specific brand!" << endl;
    filter_ban_list();
    filter_Dahua();
    filter_Hikivision();
    filter_SVP();
    filter_Kodak();
}
