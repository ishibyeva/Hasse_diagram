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

#include "libs/SpecialTree.h"
#include "libs/FacetTree.h"
#include "libs/KPf_StorStruct.h"

class Base_Interface
{
protected:
    std::vector<std::vector<size_t>> data_;
    int v_num_, f_num_, dim_;
    std::vector<list<pair<vector<bool>, set<int>>>> Diagramm_lvl;

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

typedef list<pair<vector<bool>, set<int>>> facet_list;

class Interface_FR : public Base_Interface
{
public:
    Interface_FR(std::vector<std::vector<size_t>> &vector_form_data)
        : Base_Interface(vector_form_data) {}
    void ConvertToData() override
    {
        int facet_iterate = 1;
        for (auto &face : data_)
        {
            std::vector<bool> binary_face(f_num_, false);
            std::set<int> vertex;
            vertex.insert(facet_iterate++);

            for (auto &number : face)
            {
                binary_face[(int)number - 1] = true;
            }
            Diagramm_lvl[0].push_back(make_pair(binary_face, vertex));
        }
    }

    std::vector<bool> FaceIntersection(std::vector<bool> &vec1, std::vector<bool> &vec2)
    {
        std::vector<bool> int_vec;
        for (int i = 0; i < f_num_; i++)
            int_vec.push_back(vec1[i] && vec2[i]);
        return int_vec;
    }

    std::set<int> GetNewFace(std::set<int> &set1, std::set<int> &set2)
    {
        std::set<int> new_face(set1);
        new_face.insert(set2.begin(), set2.end());
        return new_face;
    }

    void Output() override
    {
    }
};

class Interface_FR_Standart : public Interface_FR
{

public:
    Interface_FR_Standart(std::vector<std::vector<size_t>> &vector_form_data)
        : Interface_FR(vector_form_data) {}
    facet_list Enumeration(facet_list &k_lvl, map<vector<bool>, set<int>> &full_set)
    {
        facet_list k_plus1_lvl;
        facet_list::iterator it;

        for (it = k_lvl.begin(); it != k_lvl.end(); ++it)
        {
            for (auto j = std::next(it); j != k_lvl.end(); ++j)
            {
                std::vector<bool> candidat = FaceIntersection((*it).first, (*j).first);
                std::set<int> candidat_vert = GetNewFace((*it).second, (*j).second);
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
                    full_set.insert(make_pair(candidat, candidat_vert));
                    k_plus1_lvl.push_back(make_pair(candidat, candidat_vert));
                }
                else
                {
                    auto remove_it = std::remove_if(k_lvl.begin(), k_lvl.end(),
                                                   [&](const std::pair<std::vector<bool>, std::set<int>> &element)
                                                   { return element.first == candidat; });
                    k_lvl.erase(remove_it, k_lvl.end());
                    k_plus1_lvl.push_back(make_pair(candidat, candidat_vert));
                }
            }
        }
        return k_plus1_lvl;
    }
    void FindAllFace()
    {
        std::map<std::vector<bool>, std::set<int>> full_set;
        int k_layer = 0; // first layer of facet
        while (k_layer != dim_)
        {
            facet_list &ref = Diagramm_lvl[k_layer];
            Diagramm_lvl[k_layer + 1] = Enumeration(ref, full_set);
            k_layer++;
        }
    }
};

class Interface_FR_Modern : public Interface_FR
{
public:
    Interface_FR_Modern(std::vector<std::vector<size_t>> &vector_form_data)
        : Interface_FR(vector_form_data) {}
    facet_list Enumeration(facet_list &k_lvl, SpecialTree &sch_tree)
    {
        facet_list k_plus1_lvl;
        facet_list::iterator it;

        for (it = k_lvl.begin(); it != k_lvl.end(); ++it)
        {
            for (auto j = std::next(it); j != k_lvl.end(); ++j)
            {
                vector<bool> candidat = FaceIntersection((*it).first, (*j).first);
                set<int> candidat_vert = GetNewFace((*it).second, (*j).second);
                if (!sch_tree.Search(candidat))
                {
                    if (candidat == (*it).first)
                    {
                        it = k_lvl.erase(it);
                    }
                    if (candidat == (*j).first)
                    {
                        it = k_lvl.erase(j);
                    }
                    sch_tree.Insert(candidat);
                    k_plus1_lvl.push_back(make_pair(candidat, candidat_vert));
                }
                else
                {
                    auto remove_it = std::remove_if(k_lvl.begin(), k_lvl.end(),
                                                   [&](const std::pair<std::vector<bool>, std::set<int>> &element)
                                                   { return element.first == candidat; });
                    k_lvl.erase(remove_it, k_lvl.end());
                    k_plus1_lvl.push_back(make_pair(candidat, candidat_vert));
                }
            }
        }
        return k_plus1_lvl;
    }
    void FindAllFace()
    {
        SpecialTree *sch_tree = new SpecialTree();
        int k_layer = 0; // first layer of facet
        while (k_layer != dim_)
        {
            facet_list &ref = Diagramm_lvl[k_layer];
            Diagramm_lvl[k_layer + 1] = Enumeration(ref, *sch_tree);
            k_layer++;
        }
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
            std::list<size_t> reserv;
            for (auto &str_elem : face)
            {
                reserv.push_back(str_elem);
            }
            start_v_storage.push_back(reserv);
        }
    }

    void FindAllFace() override
    {
        First_Act(f_num_);
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
        for (int i = 1; i <= v_num_; i++)
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