#ifndef _MYINT_H
#define _MYINT_H

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <ctime>
#include <vector>

#include "SpecialTree.h"
#include "FacetTree.h"
#include "KPf_StorStruct.h"

class Base_Interface {
protected:
    std::vector<std::vector<size_t>> data_;
    int v_num_, f_num_, dim_;
    vector<list<pair<vector<bool>, set<int>>>> Diagramm_lvl;
    int k = 0;

public:
    Base_Interface(std::vector<std::vector<size_t>>& vector_form_data) {
        v_num_ = vector_form_data[0][0];
        f_num_ = vector_form_data[1][0];
        dim_ = vector_form_data[2][0];

        std::copy(vector_form_data.begin() + 3, vector_form_data.end(), std::back_inserter(data_));
        Diagramm_lvl.resize(dim_ + 1);
    }
    Base_Interface(std::vector<std::vector<size_t>> data, int v_num, int f_num, int dim)
    : data_(data), v_num_(v_num), f_num_(f_num), dim_(dim) {

    }
    Base_Interface(const Base_Interface& base_interface) {
        v_num_ = base_interface.v_num_;
        f_num_ = base_interface.f_num_;
        dim_ = base_interface.dim_;
        data_ = base_interface.data_;
    }
    
    virtual void ConvertToData() = 0;
    virtual void FindAllFace() = 0;
    virtual void Output() = 0;
    //virtual clock_t Timing() = 0; 
    virtual ~Base_Interface() {};
};

class Interface_FR : public Base_Interface {
public:
    Interface_FR(std::vector<std::vector<size_t>>& vector_form_data) 
    : Base_Interface(vector_form_data) {}
    void ConvertToData() override {
        int facet_iterate = 0;
        for (auto& face : data_) {
            vector<bool> binary_face(f_num_, false);
            set<int> vertex;
            vertex.insert(facet_iterate++);

            for (auto& number : face) {
                binary_face[(int)number - 1] = true;
            }
            Diagramm_lvl[0].push_back(make_pair(binary_face, vertex));
        }

    }

    void FindAllFace() override {

    }

    void Output() override {

    }

};

class Interface_KP : public Base_Interface {
public:
    Interface_KP(std::vector<std::vector<size_t>>& vector_form_data) 
    : Base_Interface(vector_form_data) {}
    void ConvertToData() override {
        for (auto& face : data_) {
            list<size_t> reserv;
            for (auto& str_elem : face) {
                reserv.push_back(str_elem);
            }
            start_v_storage.push_back(reserv);
        }
    }
    
    void FindAllFace() override {

    }

    void Output() override {

    }

};

#endif