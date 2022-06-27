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

class Base_Interface {
protected:
    std::string data_;
    int v_num_, f_num_, dim_;
    int k = 0;

public:
    Base_Interface();
    void Print();
    void Input() {
        cout << "Enter filename:" << "\n";
        string filename;
        std::cin >> filename; 

        std::ifstream infile(filename);
        std::string line;
        vector<bool> rez_face_set;
        set<int> vertex;
        int i = 0;
        //enter matrix
        while (std::getline(infile, line)) {
            if (line.find_first_not_of(' ') != std::string::npos) {
                if (i == 0)
                    v_num_ = std::stoi(line);
                if (i == 1)
                    f_num_ = std::stoi(line);
                if (i == 2) {
                    dim_ = std::stoi(line);
                    HDiagramm_lvls.resize(dim_ + 1);
                }
                else if (i > 2) {
                    Vert_List_Building(line);
                    vertex.clear();
                    rez_face_set.clear();
                    rez_face_set.resize(f_num_);
                    vertex.insert(i - 2);
                    int str_pars = 0;
                    std::string inc_num;
                    while (line[str_pars] != '\0') {
                        inc_num = inc_num + line[str_pars];
                        if (line[str_pars] == ' ' || line[str_pars + 1] == '\0') {
                            rez_face_set[std::stoi(inc_num) - 1] = true;
                            inc_num.clear();
                        }
                        str_pars++;
                    }
                    HDiagramm_lvls[0].push_back(make_pair(rez_face_set, vertex));
                }

            }
            i++;
        }
    }
    
    virtual void ConvertToData() = 0;
    virtual void FindDiagramm() = 0;
    virtual ~Base_Interface() {};

};

class Interface_FR : public Base_Interface {
    void ConvertToData() override {

    }
};

class Interface_KP : public Base_Interface {
    void ConvertToData() override {

    }
};

#endif