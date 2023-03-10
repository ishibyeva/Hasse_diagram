#ifndef _MYINT_H
#define _MYINT_H

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <ctime>
#include <vector>
#include <algorithm>
#include <functional>
#include <variant>
#include <memory>

#include "SpecialTree.h"
#include "FacetTree.h"
#include "KPf_StorStruct.h"

class Base_Interface
{
protected:
    std::vector<std::vector<size_t>> data_;
    int v_num_, f_num_, dim_;
    vector<list<pair<vector<bool>, set<int>>>> Diagramm_lvl;

public:
    Base_Interface(std::vector<std::vector<size_t>> &vector_form_data)
    {
        v_num_ = vector_form_data[0][0];
        f_num_ = vector_form_data[1][0];
        dim_ = vector_form_data[2][0];

        std::copy(vector_form_data.begin() + 3, vector_form_data.end(), std::back_inserter(data_));
        Diagramm_lvl.resize(dim_ + 1);
    }
    Base_Interface(std::vector<std::vector<size_t>> data, int v_num, int f_num, int dim)
        : data_(data), v_num_(v_num), f_num_(f_num), dim_(dim)
    {
    }
    Base_Interface(const Base_Interface &base_interface)
    {
        v_num_ = base_interface.v_num_;
        f_num_ = base_interface.f_num_;
        dim_ = base_interface.dim_;
        data_ = base_interface.data_;
    }

    virtual void ConvertToData() = 0;
    virtual void FindAllFace() = 0;
    virtual void Output() = 0;
    virtual ~Base_Interface(){};
};

class Interface_FR : public Base_Interface
{
public:
    Interface_FR(std::vector<std::vector<size_t>> &vector_form_data)
        : Base_Interface(vector_form_data) {}
    void ConvertToData() override
    {
        int facet_iterate = 0;
        for (auto &face : data_)
        {
            vector<bool> binary_face(f_num_, false);
            set<int> vertex;
            vertex.insert(facet_iterate++);

            for (auto &number : face)
            {
                binary_face[(int)number - 1] = true;
            }
            Diagramm_lvl[0].push_back(make_pair(binary_face, vertex));
        }
    }

    std::set<int> Get_New_Face(std::set<int> set1, std::set<int> set2)
    {
        std::set<int> intersection;
        std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(),
                              std::back_inserter(intersection));
        return intersection;
    }

    void InsertNewFace(std::variant<map<vector<bool>, set<int>> &, SpecialTree &> storage_struct,
                       std::vector<bool> candidat)
    {
    }

    list<pair<vector<bool>, set<int>>> Enumeration(list<pair<vector<bool>, set<int>>> &k_lvl,
                                                   std::variant<map<vector<bool>, set<int>>, SpecialTree> storage_struct)
    {
        list<pair<vector<bool>, set<int>>> k_plus1_lvl;
        list<pair<vector<bool>, set<int>>>::iterator it = k_lvl.begin();

        for (; (*it) != k_lvl.back(); it++)
        {
            for (auto j = it++; j != k_lvl.end(); j++)
            {
                vector<bool> candidat = Face_Intersection((*it).first, (*j).first);
                set<int> candidat_vert = Get_New_Face((*it).second, (*j).second);
                if (full_set.find(candidat) == full_set.end())
                {

                    if (candidat == (*it).first)
                    {
                        it = k_lvl.erase(it);
                    }
                    if (candidat == (*j).first)
                    {
                        it = k_lvl.erase(j);
                    }
                    sch_tree->Insert(candidat);
                    k_plus1_lvl.push_back(make_pair(candidat, candidat_vert));
                }
                else
                {
                    for (auto it = k_lvl.begin(); it != k_lvl.end();)
                    {
                        if ((*it).first == candidat)
                            it = k_lvl.erase(it);
                        else
                            it++;
                    }

                    k_plus1_lvl.push_back(make_pair(candidat, candidat_vert));
                }
            }
        }
        return k_plus1_lvl;
    }

    void FRAlgorithmIteration(std::variant<map<vector<bool>, set<int>>, SpecialTree> storage_struct)
    {
        int k_layer = 0; // first layer of facet
        while (k_layer != dim_)
        {
            list<pair<vector<bool>, set<int>>> &ref = HDiagramm_lvls[k_layer];
            HDiagramm_lvls[k_layer + 1] = Enumeration(ref);
            k_layer++;
        }
    }
    void FindAllFace(bool type)
    {
        // type true = special tree, type false = red black tree realisation
        if (type)
        {
            std::unique_ptr<SpecialTree> spec_tree(new SpecialTree());
            FRAlgorithmIteration(std::move(spec_tree));
        }
        else
        {
            std::unique_ptr<vector<bool>, set<int>> rb_tree;
            FRAlgorithmIteration(rb_tree);
        }
    }

    void Output() override
    {
    }
};

class Interface_KP : public Base_Interface
{
public:
    Interface_KP(std::vector<std::vector<size_t>> &vector_form_data)
        : Base_Interface(vector_form_data) {}
    void ConvertToData() override
    {
        for (auto &face : data_)
        {
            list<size_t> reserv;
            for (auto &str_elem : face)
            {
                reserv.push_back(str_elem);
            }
            start_v_storage.push_back(reserv);
        }
    }

    void FindAllFace() override
    {
        First_Act(f_num);
        F_Tree *ftree = new F_Tree();
        list<Vertex_set> min_sets;

        while (!Q.empty())
        {
            min_sets = Search_of_G_set(Q.front());
            Vertex_set Q_pop = Q.front();

            while (!min_sets.empty())
            {
                if (!ftree->Search(min_sets.front()))
                {
                    Vertex_set *copy = new Vertex_set(min_sets.front());
                    ftree->Insert(*copy);
                    H_Diag_Node New_node = {Q_pop, *copy};
                    L.push_back(New_node);
                    Q.push_back(*copy);
                }
                min_sets.pop_front();
            }
            Q.pop_front();
        }

        list<size_t> full_set;
        for (int i = 1; i <= v_num; i++)
            full_set.push_back((size_t)i);
        Vertex_set *backH = new Vertex_set(full_set);

        H_Diag_Node back_pair = {*backH, *backH};
        L.push_back(back_pair);
    }

    void Output() override
    {
    }
};

#endif