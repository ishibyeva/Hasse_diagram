#include "KPf_StorStruct.h"

vector<list<size_t>> start_v_storage;
vector<list<size_t>> start_f_storage;
list<size_t> V_set;
vector<H_Diag_Node> L;
list<Vertex_set> Q;

Vertex_set::Vertex_set() {
	list<size_t> lst;
	this->vertices = lst;
}

Vertex_set::~Vertex_set() {
}

Vertex_set::Vertex_set(list<size_t> vlist) : vertices(vlist) {
}

Facet_set::Facet_set() {
	list<size_t> lst;
	this->facet = lst;
}

Facet_set::Facet_set(list<size_t> vlist) : facet(vlist) {
}

Facet_set::~Facet_set() {
}

size_t Vertex_set::Get_size() {
	return vertices.size();
}

size_t Facet_set::Get_size() {
	return facet.size();
}

void Vertex_set::Print_vert()
{
	cout << "{";
	for (auto &elem : vertices)
		cout << elem<< ",";
	cout << '\b';
	cout << "}";
}

void Facet_List_Building(int f_dim) {

	for (int i = 0; i < start_v_storage.size(); i++)
		V_set.push_back((size_t)(i+1));


	for (int i = 0; i < f_dim; i++)
	{
		list<size_t> temp;
		size_t s_num = (size_t)(i + 1);
		for (int j = 0; j < start_v_storage.size(); j++)
		{
			auto result = find(begin(start_v_storage[j]), end(start_v_storage[j]), s_num);
			if (result != end(start_v_storage[j]))
				temp.push_back(j+1);
			
		}
		temp.sort();
		start_f_storage.push_back(temp);
	}
}

Vertex_set Vertex_set::Cl_operation(list<size_t> &lst) {
	Facet_set F_rezlt;
	F_rezlt = this->F_operation(lst);
	if (F_rezlt.facet.empty()) {
		Vertex_set v;
		return v;
	}
	list<size_t> intersec= start_f_storage[F_rezlt.facet.front() - 1];
	
	std::list<size_t>::const_iterator i; 
	i = F_rezlt.facet.begin();
	++i;
	for (; i!= F_rezlt.facet.end(); ++i)
	{
		unordered_set<size_t> search_str;
		for (auto &num : intersec)
			search_str.insert(num);
		intersec.clear();
		
		for (auto &num1 : start_f_storage[*i - 1]) {
			if (search_str.find(num1) != search_str.end())
				intersec.push_back(num1);
		}
	}

	Vertex_set A(intersec);
	return A;
}

Vertex_set Vertex_set::Cl_operation() {
	Facet_set F_rezlt;
	F_rezlt = this->F_operation();
	if (F_rezlt.facet.empty())
	{
		Vertex_set v;
		return v;
	}
	list<size_t> intersec = start_f_storage[F_rezlt.facet.front() - 1];
	
	std::list<size_t>::const_iterator i;
	i = F_rezlt.facet.begin();
	++i;
	for (; i != F_rezlt.facet.end(); ++i) {
		unordered_set<size_t> search_str;
		for (auto &num : intersec)
			search_str.insert(num);
		intersec.clear();

		for (auto &num1 : start_f_storage[*i - 1]) {
			if (search_str.find(num1) != search_str.end())
				intersec.push_back(num1);
		}
	}

	Vertex_set A(intersec);
	return A;
}

Facet_set Vertex_set::F_operation(list<size_t> &lst) {
	list<size_t> resrv;
	resrv = lst;

	for (auto &i : vertices) {
		
		unordered_set<size_t> search_str;
		for (auto &num : resrv)
			search_str.insert(num);
		resrv.clear();
		for (auto &num1 : start_v_storage[i-1]) {
			if (search_str.find(num1) != search_str.end())
				resrv.push_back(num1);
		}
	}

	Facet_set A(resrv);
	return A;
}

Facet_set Vertex_set::F_operation() {
	list<size_t> resrv;
	resrv = start_v_storage[vertices.front()-1];
	list<size_t>::iterator it = ++vertices.begin();
	for (; it!=vertices.end();it++)
	{

		unordered_set<size_t> search_str;
		for (auto &num : resrv)
			search_str.insert(num);
		resrv.clear();
		for (auto &num1 : start_v_storage[*it - 1])
		{
			if (search_str.find(num1) != search_str.end())
				resrv.push_back(num1);
		}
	}


	Facet_set A(resrv);
	return A;
}

void First_Act(int f_dim) {
	Facet_List_Building(f_dim);
	list<size_t> vert_l = {};
	Vertex_set * first_set = new Vertex_set(vert_l);
	
	Q.push_back(vert_l);
}

vector<pair<Vertex_set, size_t>> Compute_H_Collection(Vertex_set &vset) {
	vector<pair<Vertex_set, size_t>> H_set;
	list<size_t> set_wt_vset = V_set;
	for (auto &i : vset.vertices)
	{
		set_wt_vset.erase(find(set_wt_vset.begin(), set_wt_vset.end(), i));
	}
	for (auto &j : set_wt_vset)
	{
		Vertex_set res = vset.Cl_operation(start_v_storage[(int)j - 1]);
		if (!res.vertices.empty())
			H_set.push_back(make_pair(res, j));
	}

	return H_set;
}

list<Vertex_set> Search_of_G_set(Vertex_set &vset) {

	list<Vertex_set> res_collection;
	vector<pair<Vertex_set, size_t>> H_set = Compute_H_Collection(vset);
	bool flag;
	
	unordered_map<size_t, bool> Candidate_list;

	for (auto &vert : H_set)
		Candidate_list.insert(make_pair(vert.second, false));

	for (int i = 0; i < H_set.size(); i++) {
		auto vert = Candidate_list.find(H_set[i].second);
			if (vert != Candidate_list.end() && !(*vert).second) {
 				flag = false;
				for (auto &w : H_set[i].first.vertices) {
					if (w != H_set[i].second) {
						if (Candidate_list.find(w)!= Candidate_list.end())
							flag = true;
					}
				
				}
				if (!flag) {
					(*vert).second = true;
				}
				else
					vert = Candidate_list.erase(vert);
			}

	}
	

	for (int i = 0; i < H_set.size(); i++)
	{
		auto vert = Candidate_list.find(H_set[i].second);
		if (vert != Candidate_list.end()) {
			res_collection.push_back(H_set[i].first);
		}
	}

	return res_collection;
}