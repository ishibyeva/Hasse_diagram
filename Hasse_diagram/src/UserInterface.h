#ifndef _MYINT_H
#define _MYINT_H

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <ctime>
#include <algorithm>
#include <functional>
#include <variant>
#include <memory>

#include "libs/SpecialTree.h"
#include "libs/FacetTree.h"
#include "libs/KPf_StorStruct.h"

class Base_Interface
{
protected:
    std::vector<std::vector<size_t>> data_;
    int f_num_, v_num_, dim_;

public:
    Base_Interface(std::vector<std::vector<size_t>> &vector_form_data)
    {
        f_num_ = vector_form_data[0][0];
        v_num_ = vector_form_data[1][0];
        dim_ = vector_form_data[2][0];

        std::copy(vector_form_data.begin() + 3, vector_form_data.end(), std::back_inserter(data_));
    }
    Base_Interface(std::vector<std::vector<size_t>> &data, int v_num, int f_num, int dim)
        : data_(data), f_num_(v_num), v_num_(f_num), dim_(dim)
    {
    }

    virtual void ConvertToData() = 0;
    virtual void FindAllFace() = 0;
    virtual void Output() = 0;
    virtual ~Base_Interface() = default;
};

class Interface_KP : public Base_Interface
{
public:
    std::vector<std::list<size_t>> start_v_storage;
    std::vector<std::list<size_t>> start_f_storage;

    std::list<size_t> V_set;
    std::vector<H_Diag_Node> L;
    std::list<Vertex_set> Q;

    std::unordered_map<Vertex_set, int, KeyHasher, KeyEquals> dimersation_store;
    
    Interface_KP(std::vector<std::vector<size_t>> &vector_form_data)
        : Base_Interface(vector_form_data) {}
    void ConvertToData() override
    {
        for (auto &face : data_)
        {
            std::list<size_t> reserv;
            for (auto &str_elem : face)
            {
                reserv.push_back(str_elem);
            }
            start_f_storage.push_back(reserv);
        }
    }

    void FindAllFace() override
    {
        First_Act(v_num_, start_v_storage, start_f_storage, V_set, Q, dimersation_store);
        auto ftree = std::make_unique<F_Tree>();
        std::list<Vertex_set> min_sets;

        while (!Q.empty())
        {
            min_sets = Search_of_G_set(Q.front(), start_v_storage, start_f_storage, V_set);
            Vertex_set Q_pop = Q.front();

            while (!min_sets.empty())
            {
                if (!ftree->Search(min_sets.front().vertices, start_v_storage, start_f_storage))
                {
                    auto copy = std::make_shared<Vertex_set>(min_sets.front());
                    ftree->Insert(copy, start_v_storage, start_f_storage);
                    H_Diag_Node New_node = {Q_pop, *copy};
                    L.push_back(New_node);
                    Q.push_back(*copy);
                    auto facet_dim = dimersation_store.find(Q_pop);
                    dimersation_store.emplace(std::make_pair(*copy, facet_dim->second + 1));
                }
                min_sets.pop_front();
            }
            Q.pop_front();
        }

        std::list<size_t> full_set;
        for (size_t i = 1; i <= static_cast<size_t>(f_num_); ++i)
            full_set.push_back(i);
        auto backH = std::make_unique<Vertex_set>(full_set); 

        H_Diag_Node back_pair = {*backH, *backH};
        L.push_back(back_pair);
    }

    void Output() override
    {
        size_t k1 = 0, k2 = 0, start = 0;
        while (k2 != L.size())
        {
            start = k1;
            size_t mem_size = L[k1].Vert_adrH.Get_size();
            while (k1 != L.size() && L[k1].Vert_adrH.Get_size() == mem_size)
                k1++;
            mem_size = L[k1].Vert_adrH.Get_size();
            k2 = k1;

            while (k2 != L.size() && L[k2].Vert_adrH.Get_size() == mem_size)
                k2++;

            for (size_t i = start; i < k1; ++i)
            {
                std::cout << "(" << i + 1 << ")"
                     << "Verts:";
                L[i].Vert_adrG.Print_vert();
                std::cout << "---->{ ";
                for (size_t j = k1; j < k2; ++j)
                {
                    bool flag;
                    for (auto &elem : L[i].Vert_adrG.vertices)
                    {
                        flag = true;
                        if (find(L[j].Vert_adrG.vertices.begin(), L[j].Vert_adrG.vertices.end(), elem) == L[j].Vert_adrG.vertices.end())
                        {
                            flag = false;
                            break;
                        }
                    }
                    if (flag)
                        std::cout << " (" << j + 1 << ") ";
                }
                std::cout << "}" << '\n';
            }
        }

        std::cout << "(" << L.size() << ")Verts:";
        L[L.size() - 1].Vert_adrG.Print_vert();
        std::cout << "---->{ }" << '\n';
    }
};

#endif