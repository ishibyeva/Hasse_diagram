cmake_minimum_required(VERSION 3.1)

project(enum_libs)

add_library(enum_libs SHARED
            FacetTree.h FacetTree.cpp
            KPf_StorStruct.h KPf_StorStruct.cpp
            SpecialTree.h SpecialTree.cpp)

target_include_directories(enum_libs PUBLIC .)