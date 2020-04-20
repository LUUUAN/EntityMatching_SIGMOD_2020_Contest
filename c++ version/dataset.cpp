#include "dataset.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <map>
#include <cstring>
#include <dirent.h>
#ifdef __APPLE__
           #include <sys/uio.h>
#else
           #include <sys/io.h>
#endif
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
using namespace std;

map<string, struct DataContent *> all_data;
map<string, set<string>> brand_index;
map<string, map<string, set<string>>> model_index;
map<string, set<string> *> others;

struct Attribute{
    string id,page_title,brand;
    Attribute(string a, string b, string c) {
        id = a;
        page_title = b;
        brand = c;
    }
};
vector<vector<Attribute*>> table(25);
/*
void listFiles(const string& path, vector<string> &files) {
    string in_path = path + "/*";
    long handle;
    struct _finddata_t fileinfo{};
    handle = _findfirst(in_path.c_str(), &fileinfo);
    if (handle != -1) {
        do {
            if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, ".."))
                files.push_back(fileinfo.name);
        } while (!_findnext(handle, &fileinfo));
    }
    _findclose(handle);
}*/
void listFiles(const string& path, vector<string> &files) {
    string in_path = path + "/";

    struct dirent *direntp;

    DIR *dirp = opendir(in_path.c_str());

    if (dirp != NULL) {

        while ((direntp = readdir(dirp)) != NULL)

            if (strcmp(direntp->d_name, ".") && strcmp(direntp->d_name, ".."))
                files.push_back(direntp->d_name);

    }

    closedir(dirp);

    return;
}

string get_attribute(string line){
    string res;
    bool Flag = false;
    for(int i=0;i<line.length()-2;i++){
        if(line[i] == ':'){
            i += 3;
            Flag = true;
        }
        if(Flag){
            res += line[i];
        }
    }
    if (line[line.length()-2] != ',' && line[line.length()-2] != '\"') {
        res += line[line.length()-2];
    }
    return res;
}


void preprocessing(vector<string> product_files, string path, string source, int x){
    vector<Attribute*> res;
    for (const string& product_file: product_files) {
        string json_path = path + "/" + source + "/" + product_file;
        string spec_id = source + "//" + product_file.substr(0, product_file.length()-5);
        ifstream in(json_path);
        string line;
        string page_title;
        string brand;
        while (getline (in, line)){
            if(line.find("<page title>")!=-1){
                page_title = get_attribute(line);
            }
            if(line.find("\"brand\"")!=-1){
                brand = get_attribute(line);
                break;
            }
        }
        auto* temp = new Attribute(spec_id,page_title,brand);
        res.push_back(temp);
    }
    table[x] = res;
}


/**
 * Read all data from given path to the map all_data.
 * @param path ABSOLUTE PATH of the data source.
 */
string important_websites[] = {"www.ebay.com", "www.alibaba.com", "buy.net"};
void readAll(const string& path) {
    cout << "Read All Data into Memory!" << endl;
    vector<string> source_dir;
    listFiles(path, source_dir);
    auto **th = new thread * [3];
    for(int i = 0;i<3;i++){
        vector<string> product_files;
        listFiles(path + "/" + important_websites[i], product_files);
        th[i] = new thread(preprocessing,product_files,path,important_websites[i],i);
    }
    int cnt = 2;
    for (const string& source: source_dir) {
        cout << "Read All: " << source << endl;
        bool flag = false;
        for(int i=0;i<3;i++){
            if(source == important_websites[i]){
                flag = true;
                break;
            }
        }
        if(flag) continue;
        vector<string> product_files;
        listFiles(path + "/" + source, product_files);
        preprocessing(product_files,path,source,++cnt);
    }
    for(int i =0;i<3;i++){
        th[i] -> join();
    }
    for(int i=0;i<table.size();i++){
        for(int j=0;j<table[i].size();j++) {
            auto *data_content = new DataContent;
            data_content->source = table[i][j] -> id;
            data_content->brand = table[i][j] -> brand;
            data_content->page_title = table[i][j] ->page_title;
            all_data.insert({table[i][j] -> id, data_content});
        }
    }

}



/*
void brand_index_to_file() {
    cout << "Writing brand index to file" << endl;
    //_mkdir(output_dir.c_str());
    for (const auto& item: brand_index) {
        string brand_name = item.first;
        unordered_set<string> data_set = item.second;
        std::string prefix = "./brand";
        string output_path = "./brand/" + brand_name + ".csv";
        if (_access(prefix.c_str(), 0) == -1)	//濡傛灉鏂囦欢澶逛笉瀛樺湪
            _mkdir(prefix.c_str());
        ofstream output_stream;
        output_stream.open(output_path, ios::out);
        output_stream << "id, <page title>" << endl;
        for (const string& product: data_set) {
            output_stream << product << "," << all_data[product]->page_title << endl;
        }
        output_stream.flush();
        output_stream.close();
    }
}


void model_index_to_file() {
    cout << "Writing model index to file" << endl;
    //_mkdir(output_dir.c_str());
    for (const auto& item: model_index) {

        string brand_name = item.first;
        auto model_index_each = item.second;

        string grandparent_dir = "./model";
        if (_access(grandparent_dir.c_str(), 0) == -1)
            _mkdir(grandparent_dir.c_str());

        string parent_dir = "./model/" + brand_name;
        if (_access(parent_dir.c_str(), 0) == -1)
            _mkdir(parent_dir.c_str());

        for (const auto& brand_item: model_index_each) {
            string model_name = brand_item.first;
            auto &product_set = brand_item.second;
            string output_path = parent_dir + "/" + model_name + ".csv";
            ofstream output_stream;
            output_stream.open(output_path, ios::out);
            output_stream << "id, <page title>" << endl;
            for (const string& product: product_set) {
                output_stream << product << "," << all_data[product]->page_title << endl;
            }
            output_stream.flush();
            output_stream.close();
        }
        if (others.find(brand_name) != others.end()) {
            string output_path = parent_dir + "/others.csv";
            ofstream output_stream;
            output_stream.open(output_path, ios::out);
            output_stream << "id, <page title>" << endl;
            for (const string& product: *others[brand_name]) {
                output_stream << product << "," << all_data[product]->page_title << endl;
            }
            output_stream.flush();
            output_stream.close();

        }
    }
}
*/
