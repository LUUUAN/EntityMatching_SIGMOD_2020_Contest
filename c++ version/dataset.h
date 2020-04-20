#ifndef SIGMOD_CONTEST_DATASET_H
#define SIGMOD_CONTEST_DATASET_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;
struct DataContent {
    string source;
    string page_title;
    string brand;
};
/*
	所有的数据应该是一个映射，key为source(www.a.com//0)，value为json文件中有用标签的类DataContent
*/
extern map<string, struct DataContent *> all_data;

/*
	对每个商品分好品牌
	key值为品牌名称，value为该品牌下所有型号的list
*/
extern map<string, set<string>> brand_index;


/*
	二维映射：在每个商品分号品牌后，再对每个品牌分好型号
	第一维：key值为品牌的名称，value值为该品牌下所有型号及其对应所有相机的映射
	第二维：key值为型号名称，value值维该型号所有相机的list
*/
extern map<string, map<string, set<string>>> model_index;



/*
	倒排索引
	key值为specification id
	value值为（品牌型号）
*/
//extern unordered_map<string, unordered_set<string>> reverse_index;



extern map<string, set<string> *> others;

void listFiles(string& path, vector<string> &files);

void readAll(const string& path);

void brand_index_to_file();
void model_index_to_file();

#endif //SIGMOD_CONTEST_DATASET_H
