#include <algorithm>
#include <iostream>
#include "dataset.h"
#include "util.h"
#include <string>
#include <thread>
#include <list>




using namespace std;


set<string> ban_set = {"1080p", "720p", "3d"};
set<string> prefix_ban_set = {"under", "with", "camera", "full", "black", "pink", "for", "body",
                                        "canon", "ef", "top", "w", "led", "tv", "kit", "lens", "edition",
                                        "only", "pro", "ii", "iii"};
map<string, list<string>> special_model = {{"Canon", {"xt", "xti", "xs", "xsi", "eos m", "eos m2", "1ds"}},
                                                     {"Nikon", {"df", "d1x", "d2x", "d3x", "coolpix A"}},
                                                     {"GoPro", {"Hero 3", "Hero 2", "Hero3+"}},
                                                     {"SVP", {"20MP", "18MP"}},{"Olympus",{"EM 5","EM-5"}},
                                                     {"Hasselblad", {"Hasselblad"}}};



bool word_rule(string word) {
    if (word.size() < 2) {
        return false;
    }
    if (word.size() <= 3 && (word[word.length()-1] == 'x' || word[word.length()-1] == 'X')) {
        return false;
    }
    bool flag = true;
    for (int i = 0; i < word.size()-1; i++) {
        if (!is_digit(word[i])) {
            flag = false;
            break;
        }
    }
    if (flag && is_alphabet(word[word.size() - 1])) {
        return true;
    }
    if (!is_alphabet(word[0])) {
        return false;
    }
    int score = 0;
    for (char c: word) {
        if (is_alphabet(c)) score |= 1;
        else if (is_digit(c)) score |= 2;
        else if (c != '-') score |= 4;
    }
    return score == 3;
}

bool is_valid_prefix(const string& word) {
    if (word.length() < 1) {
        return false;
    }
    for (char c: word) {
        if (!is_alphabet(c)) {
            return false;
        }
    }
    return true;
}

bool is_valid_postfix(string word) {
    if (word.size() < 2 || word.size() > 7) {
        return false;
    }
    for (int i = 0; i < word.size()-1; i++) {
        if (!is_digit(word[i])) {
            return false;
        }
    }
    if (!is_alphabet(word[word.size()-1]) && !is_digit(word[word.size()-1])) {
        return false;
    }
    return str_lower(word)[word.size() - 1] != 'x';
}


void collecting_models(const string& page_title, set<string>& model_set) {
    vector<string> tmp;
    split(page_title, tmp);
    //boost::split(tmp, page_title, boost::is_any_of(" "), boost::token_compress_on);
    //tmp = split(std::move(page_title));

    for (int i = 0; i < tmp.size()-1; i++) {
        for (int step = 0; step <= 1; step ++) {
            string word = tmp[i+step];
            if (word_rule(word) && ban_set.find(str_lower(word)) == ban_set.end()) {
                if (model_set.find(str_upper(word)) != model_set.end()) {
                    return;
                }
                model_set.insert(str_upper(word));
            }
        }
        if (is_valid_prefix(tmp[i]) && (prefix_ban_set.find(str_lower(tmp[i])) == prefix_ban_set.end())
            && is_valid_postfix(tmp[i+1])) {
            string word = tmp[i] + ' ' + tmp[i+1];
            if (model_set.find(str_upper(word)) != model_set.end()) {
                return;
            }
            model_set.insert(str_upper(word));
            return;
        }
    }
}


void matching(map<string, set<string>>& model_index_each, const set<string>& model_set,
              const set<string>& product_set) {
    for (const string& model: model_set) {
        //model_index_each[model] = new unordered_set<string>;
        for (const string& product: product_set) {
            string page_title = all_data[product]->page_title;
            //vector<string> tmp = split(page_title);
            vector<string> tmp;
            //boost::split(tmp, page_title, boost::is_any_of(" "), boost::token_compress_on);
            split(page_title, tmp);
            for (const string& word: tmp) {
                if (str_lower(word) == str_lower(model)) {
                    model_index_each[model].insert(product);
                }
            }
            for (int i = 0; i + 1 < tmp.size(); i++) {
                string word = tmp[i] + " " + tmp[i+1];
                if (str_lower(word) == str_lower(model)) {
                    model_index_each[model].insert(product);
                }
            }
        }
    }
}



inline void merge(string& to_model, string& from_model,
                  map<string, set<string>>& model_index_each) {
    model_index_each[to_model].insert(model_index_each[from_model].begin(), model_index_each[from_model].end());
}



void merge_pairs(const set<string> &model_set,
                 map<string, set<string>> &model_index_each) {
    set<string> remove_models;

    vector<string> models(model_set.begin(), model_set.end());
    sort(models.begin(), models.end());
    for (int i = 0; i < models.size(); i++) {
        for (int j = i+1; j < models.size(); j++) {
            if (str_remove_space(models[i]) == str_remove_space(models[j])) {
                if (remove_models.find(models[i]) == remove_models.end() ||
                    remove_models.find(models[j]) == remove_models.end()) {
                    continue;
                }
                merge(models[i], models[j], model_index_each);
                remove_models.insert(models[j]);
            }
        }
    }
    for (const string& rm_model: remove_models) {
        model_index_each.erase(rm_model);
    }
}


void kill_no_contribute(map<string, set<string>>& model_index_each) {
    list<string> del_list;
    for (auto item: model_index_each) {
        if (item.second.size() < 2) {
            del_list.push_back(item.first);
        }
    }
    for (const string& del_model: del_list) {
        model_index_each.erase(del_model);
    }
}


void index_model_brand(const string& brand, const set<string> product_set) {
    cout << "Indexing model: " << brand << endl;
    map<string, set<string>> each_model_index;
    set<string> model_set;
    // add special
    if (special_model.find(brand) != special_model.end()) {
        for (string model: special_model[brand]) {
            model_set.insert(str_upper(model));
        }
    }

    // find models
    for (const string& product: product_set) {
        string page_title = all_data[product]->page_title;
        collecting_models(page_title, model_set);
    }
    matching(each_model_index, model_set, product_set);
    merge_pairs(model_set, each_model_index);
    kill_no_contribute(each_model_index);
    model_index[brand] = each_model_index;
}

string important_brands[] = {"Canon", "Nikon", "Fujifilm","Sony","Panasonic","Samsung","Olympus"};
void index_model() {
    cout << "Indexing Models!" << endl;
    auto **th = new thread * [4];
    for(int i = 0;i<4;i++){
        th[i] = new thread(index_model_brand,important_brands[i],brand_index[important_brands[i]]);
    }
    for (auto& brand: brand_index) {
        string brand_name = brand.first;
        bool flag = false;
        for(int i=0;i<4;i++){
            if(brand_name == important_brands[i]){
                flag = true;
                break;
            }
        }
        if(flag) continue;
        set<string> set = brand.second;
        index_model_brand(brand_name, set);
    }
    for(int i = 0;i<4;i++){
        th[i]->join();
    }




//    ThreadPool pool(4);
//    std::vector< std::future<int> > results;
//
//
//    for (auto& brand: brand_index) {
//        string brand_name = brand.first;
//        bool flag = false;
//        unordered_set<string> set = brand.second;
//        results.emplace_back( pool.enqueue(index_model_brand, brand_name, set) );
//    }
//
//    for(auto && result: results)
//        std::cout << result.get() << ' ';
//    std::cout << std::endl;



}
