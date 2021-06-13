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

using namespace std;


class Facet_set
{
public:

	list<size_t> facet;

	Facet_set();
	Facet_set(list<size_t> vlist);
	void Print_Facet();
	size_t Get_size();
	~Facet_set();

};


class Vertex_set
{ 
public:
	
	list<size_t> vertices;

	Vertex_set();
	Vertex_set(list<size_t> vlist);
	Facet_set F_operation(list<size_t> &lst);
	Vertex_set Cl_operation(list<size_t> &lst);
	Facet_set F_operation();
	Vertex_set Cl_operation();
	void Print_vert();
	size_t Get_size();
	~Vertex_set();

};

struct H_Diag_Node
{
	Vertex_set Vert_adrH;
	Vertex_set Vert_adrG;
};

extern vector<list<size_t>> start_v_storage;
extern vector<list<size_t>> start_f_storage;
extern list<size_t> V_set;

extern vector<H_Diag_Node> L;
extern list<Vertex_set> Q;


void First_Act(int f_dim);
void Vert_List_Building(string str);
void Facet_List_Building(int f_dim);
list<Vertex_set> Search_of_G_set(Vertex_set &vset);



#endif
