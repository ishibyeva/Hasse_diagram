#ifndef _KPSTRCT_H
#define _KPSTRCT_H

#include <vector>
#include <list>
#include <algorithm>
#include <map>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <stdlib.h>
#include <string>
#include <memory>

class Facet_set
{
public:
	std::list<size_t> facet;

	Facet_set();
	Facet_set(std::list<size_t> vlist);
	void Print_Facet();
	size_t Get_size();
	~Facet_set();
};

class Vertex_set
{
public:
	std::list<size_t> vertices;

	Vertex_set();
	Vertex_set(std::list<size_t>& vlist);
	Vertex_set(const Vertex_set& v_set);
	Facet_set F_operation(std::list<size_t> &lst, std::vector<std::list<size_t>> &start_v_storage);
	Vertex_set Cl_operation(std::list<size_t> &lst, std::vector<std::list<size_t>> &start_v_storage,
							std::vector<std::list<size_t>> &start_f_storage);
	Facet_set F_operation(std::vector<std::list<size_t>> &start_v_storage);
	Vertex_set Cl_operation(std::vector<std::list<size_t>> &start_v_storage,
							std::vector<std::list<size_t>> &start_f_storage);
	void Print_vert();
	size_t Get_size();
	~Vertex_set();
};

struct H_Diag_Node
{
	Vertex_set Vert_adrH;
	Vertex_set Vert_adrG;
};

void First_Act(int f_dim, std::vector<std::list<size_t>> &start_v_storage,
			   std::vector<std::list<size_t>> &start_f_storage,
			   std::list<size_t> &V_set,
			   std::list<Vertex_set> &Q);
void Facet_List_Building(int f_dim, std::vector<std::list<size_t>> &start_v_storage,
						 std::vector<std::list<size_t>> &start_f_storage,
						 std::list<size_t> &V_set);
std::list<Vertex_set> Search_of_G_set(Vertex_set &vset, std::vector<std::list<size_t>> &start_v_storage,
									  std::vector<std::list<size_t>> &start_f_storage,
									  std::list<size_t> &V_set);

#endif
