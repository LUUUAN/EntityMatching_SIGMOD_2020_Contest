#include "resolve_others.h"
#include "reverse_index.h"
#include "intersection.h"
#include "index_brand.h"
#include "index_model.h"
#include "split_brand.h"
#include "merge_same.h"
#include "dataset.h"
#include "filters.h"
#include <iostream>
#include <fstream>
#include "util.h"
#include <ctime>
using namespace std;


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
    time_t now = time(0);
    char* dt = ctime(&now);
    readAll(argv[1]);

    cout << "Preprocessing start:" << dt << '\n';
    now = time(0);
    dt = ctime(&now);
    cout << "Preprocessing end:" << dt << '\n';

    index_brand();

    cout << "Brand start:" << dt << '\n';
    now = time(0);
    dt = ctime(&now);
    cout << "Brand end:" << dt << '\n';

    index_model();

    cout << "Index start:" << dt << '\n';
    now = time(0);
    dt = ctime(&now);
    cout << "Index end:" << dt << '\n';

    filtering();
    multiple_models();
    intersection();
    resolve_others();
    merge_same();
    split_brand();
    
    cout << "Filter start:" << dt << '\n';
    now = time(0);
    dt = ctime(&now);
    cout << "Filter end:" << dt << '\n';

    solve();

    cout << "Solve start:" << dt << '\n';
    now = time(0);
    dt = ctime(&now);
    cout << "Solve end:" << dt << '\n';


    return 0;
}
