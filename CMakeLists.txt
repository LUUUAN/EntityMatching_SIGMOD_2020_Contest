cmake_minimum_required(VERSION 3.14)
project(sigmod_contest)

set(CMAKE_CXX_STANDARD 14)


# add_executable(sigmod_contest main.cpp util.cpp util.h)



#添加头文件搜索路径
#include_directories(D:\\env\\boost_1_72_0)

#添加库文件搜索路径
#link_directories(D:\\env\\boost_1_72_0\\libs)
#link_directories(/usr/local/Cellar/boost/1.68.0_1/include)

find_package(Boost REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(sigmod_contest main.cpp dataset.h dataset.cpp index_brand.cpp index_brand.h util.cpp util.h index_model.cpp index_model.h filters.cpp filters.h reverse_index.cpp reverse_index.h intersection.cpp intersection.h resolve_others.cpp resolve_others.h merge_same.cpp merge_same.h split_brand.cpp split_brand.h)

target_link_libraries(sigmod_contest ${Boost_LIBRARIES})