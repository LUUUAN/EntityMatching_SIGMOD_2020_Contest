cmake_minimum_required(VERSION 3.10)
project(sigmod_contest)
SET(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(sigmod_contest main.cpp dataset.h dataset.cpp index_brand.cpp index_brand.h util.cpp util.h index_model.cpp index_model.h filters.cpp filters.h reverse_index.cpp reverse_index.h intersection.cpp intersection.h resolve_others.cpp resolve_others.h merge_same.cpp merge_same.h split_brand.cpp split_brand.h)
