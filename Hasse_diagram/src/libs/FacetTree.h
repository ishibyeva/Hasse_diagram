#ifndef FTR_H
#define FTR_H

#include "KPf_StorStruct.h"

class FT_Node {
public:
	Vertex_set& C_S;
	std::map<size_t, FT_Node*> edges;

	FT_Node(Vertex_set& vrt) : C_S(vrt) { C_S = vrt; };
	void Add_Child(size_t num, FT_Node* ptr) {
		edges.insert(std::make_pair(num,ptr));
	};
	void Add_MinSet(Vertex_set& new_cs) {
		C_S = new_cs;
	};
	std::list<size_t> GetCS() {
		return C_S.vertices;
	};
	~FT_Node() {};

};

class F_Tree {
public:
	FT_Node* root;

	F_Tree();
	void Insert(Vertex_set &vst, std::vector<std::list<size_t>> &start_v_storage,
				std::vector<std::list<size_t>> &start_f_storage);
	bool Search(Vertex_set &vst, std::vector<std::list<size_t>> &start_v_storage,
				std::vector<std::list<size_t>> &start_f_storage);
	void Destroy_Tree(FT_Node* root);
	~F_Tree();
};

#endif