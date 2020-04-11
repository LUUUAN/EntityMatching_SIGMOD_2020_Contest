#include "dataset.h"
#ifdef __APPLE__
           #include <sys/uio.h>
#else
           #include <sys/io.h>
#endif
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <dirent.h>
// #include <direct.h>
// #include <boost/filesystem.hpp>

using namespace std;
namespace pt = boost::property_tree;

unordered_map<string, struct DataContent *> all_data;
unordered_map<string, unordered_set<string> > brand_index;
unordered_map<string, unordered_map<string, unordered_set<string> > > model_index;

unordered_map<string, unordered_set<string> *> others;


// void listFiles(string path, vector<string> &files) {
//     string in_path = path + "/*";
//     long handle;
//     struct _finddata_t fileinfo;
//     handle = _findfirst(in_path.c_str(), &fileinfo);
//     if (handle != -1) {
//         do {
//             if (strcmp(fileinfo.name, ".") && strcmp(fileinfo.name, ".."))
//                 files.push_back(fileinfo.name);
//         } while (!_findnext(handle, &fileinfo));
//     }
//     _findclose(handle);
// }

void listFiles(string path, vector<string> &files) {
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



/**
 * Read all data from given path to the map all_data.
 * @param path ABSOLUTE PATH of the data source.
 */
void readAll(const string& path) {
    cout << "Read All Data into Memory!" << endl;
    vector<string> source_dir;
    listFiles(path, source_dir);
    for (const string& source: source_dir) {
        cout << "Read All: " << source << endl;
        vector<string> product_files;
        listFiles(path + "/" + source, product_files);
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
            auto *data_content = new DataContent;
            data_content->source = spec_id;
            data_content->brand = brand;
            data_content->page_title = page_title;
            all_data.insert({spec_id, data_content});
            /*
            pt::ptree root;
            pt::read_json(json_path, root);
            string spec_id = source + "//" + product_file.substr(0, product_file.length()-5);
            string page_title = root.get<string>("<page title>");
            string brand;
            boost::optional<string> has_brand = root.get_optional<string>("brand");
            if (has_brand) {
                brand = has_brand.get();
            }
            else {
                brand = "";
            }
            auto *data_content = new DataContent;
            data_content->source = spec_id;
            data_content->brand = brand;
            data_content->page_title = page_title;
            all_data.insert({spec_id, data_content});
            */
        }
    }
}



void brand_index_to_file() {
    cout << "Writing brand index to file" << endl;
    //_mkdir(output_dir.c_str());
    for (const auto& item: brand_index) {
        string brand_name = item.first;
        unordered_set<string> data_set = item.second;
        std::string prefix = "./brand";
        string output_path = "./brand/" + brand_name + ".csv";
        if (access(prefix.c_str(), F_OK) == -1)	//如果文件夹不存在
            mkdir(prefix.c_str(),S_IRWXU|S_IRWXG|S_IRWXO);
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
        if (access(grandparent_dir.c_str(), F_OK) == -1)
            mkdir(grandparent_dir.c_str(),S_IRWXU|S_IRWXG|S_IRWXO);

        string parent_dir = "./model/" + brand_name;
        if (access(parent_dir.c_str(), F_OK) == -1)
            mkdir(parent_dir.c_str(),S_IRWXU|S_IRWXG|S_IRWXO);

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