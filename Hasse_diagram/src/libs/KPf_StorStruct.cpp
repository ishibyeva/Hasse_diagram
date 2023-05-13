#include "KPf_StorStruct.h"

Vertex_set::Vertex_set()
{
	std::list<size_t> lst;
	this->vertices = lst;
}

Vertex_set::~Vertex_set()
{
}

Vertex_set::Vertex_set(std::list<size_t> vlist) : vertices(vlist)
{
}

Vertex_set::Vertex_set(const Vertex_set& v_set)
{
	vertices = v_set.vertices;
}

Facet_set::Facet_set()
{
	std::list<size_t> lst;
	this->facet = lst;
}

Facet_set::Facet_set(std::list<size_t> vlist) : facet(vlist)
{
}

Facet_set::~Facet_set()
{
}

size_t Vertex_set::Get_size()
{
	return vertices.size();
}

size_t Facet_set::Get_size()
{
	return facet.size();
}

void Vertex_set::Print_vert()
{
	std::cout << "{";
	for (auto &elem : vertices)
		std::cout << elem << ",";
	std::cout << '\b';
	std::cout << "}";
}

void Facet_List_Building(int f_dim,
						 std::vector<std::list<size_t>> &start_v_storage,
						 std::vector<std::list<size_t>> &start_f_storage,
						 std::list<size_t> &V_set)
{
	for (size_t i = 0; i < start_v_storage.size(); i++)
		V_set.push_back((size_t)(i + 1));

	for (int i = 0; i < f_dim; i++)
	{
		std::list<size_t> temp;
		size_t s_num = (size_t)(i + 1);
		for (size_t j = 0; j < start_v_storage.size(); j++)
		{
			auto result = find(begin(start_v_storage[j]), end(start_v_storage[j]), s_num);
			if (result != end(start_v_storage[j]))
				temp.push_back(j + 1);
		}
		temp.sort();
		start_f_storage.push_back(temp);
	}
}

Vertex_set Vertex_set::Cl_operation(std::list<size_t> &lst,
									std::vector<std::list<size_t>> &start_v_storage,
									std::vector<std::list<size_t>> &start_f_storage)
{
	Facet_set F_rezlt;
	F_rezlt = this->F_operation(lst, start_v_storage);
	if (F_rezlt.facet.empty())
	{
		Vertex_set v;
		return v;
	}
	std::list<size_t> intersec = start_f_storage[F_rezlt.facet.front() - 1];

	std::list<size_t>::const_iterator i;
	i = F_rezlt.facet.begin();
	++i;
	for (; i != F_rezlt.facet.end(); ++i)
	{
		std::unordered_set<size_t> search_str;
		for (auto &num : intersec)
			search_str.insert(num);
		intersec.clear();

		for (auto &num1 : start_f_storage[*i - 1])
		{
			if (search_str.find(num1) != search_str.end())
				intersec.push_back(num1);
		}
	}

	Vertex_set A(intersec);
	return A;
}

Vertex_set Vertex_set::Cl_operation(std::vector<std::list<size_t>> &start_v_storage, std::vector<std::list<size_t>> &start_f_storage)
{
	Facet_set F_rezlt;
	F_rezlt = this->F_operation(start_v_storage);
	if (F_rezlt.facet.empty())
	{
		Vertex_set v;
		return v;
	}
	std::list<size_t> intersec = start_f_storage[F_rezlt.facet.front() - 1];

	std::list<size_t>::const_iterator i;
	i = F_rezlt.facet.begin();
	++i;
	for (; i != F_rezlt.facet.end(); ++i)
	{
		std::unordered_set<size_t> search_str;
		for (auto &num : intersec)
			search_str.insert(num);
		intersec.clear();

		for (auto &num1 : start_f_storage[*i - 1])
		{
			if (search_str.find(num1) != search_str.end())
				intersec.push_back(num1);
		}
	}

	Vertex_set A(intersec);
	return A;
}

Facet_set Vertex_set::F_operation(std::list<size_t> &lst,
								  std::vector<std::list<size_t>> &start_v_storage)
{
	std::list<size_t> resrv;
	resrv = lst;

	for (auto &i : vertices)
	{
		std::unordered_set<size_t> search_str;
		for (auto &num : resrv)
			search_str.insert(num);
		resrv.clear();
		for (auto &num1 : start_v_storage[i - 1])
		{
			if (search_str.find(num1) != search_str.end())
				resrv.push_back(num1);
		}
	}

	Facet_set A(resrv);
	return A;
}

Facet_set Vertex_set::F_operation(std::vector<std::list<size_t>> &start_v_storage)
{
	std::list<size_t> resrv;
	resrv = start_v_storage[vertices.front() - 1];
	std::list<size_t>::iterator it = ++vertices.begin();
	for (; it != vertices.end(); it++)
	{
		std::unordered_set<size_t> search_str;
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

void First_Act(int f_dim, std::vector<std::list<size_t>> &start_v_storage,
			   std::vector<std::list<size_t>> &start_f_storage,
			   std::list<size_t> &V_set,
			   std::list<Vertex_set> &Q)
{
	Facet_List_Building(f_dim, start_v_storage, start_f_storage, V_set);
	std::list<size_t> vert_l = {};
	Q.push_back(vert_l);
}

std::vector<std::pair<Vertex_set, size_t>> Compute_H_Collection(Vertex_set &vset,
													  std::vector<std::list<size_t>> &start_v_storage,
													  std::vector<std::list<size_t>> &start_f_storage,
													  std::list<size_t> &V_set)
{
	std::vector<std::pair<Vertex_set, size_t>> H_set;
	std::list<size_t> set_wt_vset = V_set;
	for (auto &i : vset.vertices)
	{
		set_wt_vset.erase(find(set_wt_vset.begin(), set_wt_vset.end(), i));
	}
	for (auto &j : set_wt_vset)
	{
		Vertex_set res = vset.Cl_operation(start_v_storage[(int)j - 1], start_v_storage, start_f_storage);
		if (!res.vertices.empty())
			H_set.push_back(std::make_pair(res, j));
	}

	return H_set;
}

std::list<Vertex_set> Search_of_G_set(Vertex_set &vset, std::vector<std::list<size_t>> &start_v_storage,
								 std::vector<std::list<size_t>> &start_f_storage, std::list<size_t> &V_set)
{

	std::list<Vertex_set> res_collection;
	std::vector<std::pair<Vertex_set, size_t>> H_set = Compute_H_Collection(vset, start_v_storage,
																	   start_f_storage, V_set);
	bool flag;

	std::unordered_map<size_t, bool> Candidate_list;

	for (auto &vert : H_set)
		Candidate_list.insert(std::make_pair(vert.second, false));

	for (size_t i = 0; i < H_set.size(); i++)
	{
		auto vert = Candidate_list.find(H_set[i].second);
		if (vert != Candidate_list.end() && !(*vert).second)
		{
			flag = false;
			for (auto &w : H_set[i].first.vertices)
			{
				if (w != H_set[i].second)
				{
					if (Candidate_list.find(w) != Candidate_list.end())
						flag = true;
				}
			}
			if (!flag)
			{
				(*vert).second = true;
			}
			else
				vert = Candidate_list.erase(vert);
		}
	}

	for (size_t i = 0; i < H_set.size(); i++)
	{
		auto vert = Candidate_list.find(H_set[i].second);
		if (vert != Candidate_list.end())
		{
			res_collection.push_back(H_set[i].first);
		}
	}

	return res_collection;
}