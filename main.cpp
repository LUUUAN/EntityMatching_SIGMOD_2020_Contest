// main.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string.h>
#include <exception>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#ifdef __APPLE__
           #include <sys/uio.h>
#else
           #include <sys/io.h>
#endif

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "dataset.h"
#include "index_brand.h"
#include "index_model.h"
#include <unordered_set>
// #include <direct.h>
#include <sys/stat.h>
#include "filters.h"
#include "util.h"
#include "reverse_index.h"
#include "intersection.h"
#include "resolve_others.h"
#include <algorithm>
#include <regex>
#include "merge_same.h"
#include "split_brand.h"

using namespace std;
namespace pt = boost::property_tree;


void solve(const string& output_path = "./submission.csv") {
    cout << "Output the result file" << endl;
    ofstream output_stream;
    output_stream.open(output_path, ios::out);
    output_stream << "left_spec_id" << "," << "right_spec_id" << endl;
    for (const auto& brand_item: model_index) {
        for (auto model_item: brand_item.second) {
            vector<string> models (model_item.second.begin(), model_item.second.end());
            for (int i = 0; i < models.size(); i++) {
                for (int j = i+1; j < models.size(); j++) {
                    output_stream << models[i] << "," << models[j] << endl;
                }
            }
        }
    }
    output_stream.flush();
    output_stream.close();
}




int main(int argc, char** argv) {
    readAll(argv[1]);
    index_brand();
    index_model();
    filtering();
    multiple_models();
    intersection();
    resolve_others();
    merge_same();
    split_brand();
    model_index_to_file();
    solve();
    return 0;


}


