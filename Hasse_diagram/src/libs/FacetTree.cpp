#include "FacetTree.h"

F_Tree::F_Tree()
{
	Vertex_set* root_list = new Vertex_set();
	FT_Node* v_nd = new FT_Node(*root_list);
	root = v_nd;
}

F_Tree::~F_Tree()
{
	Destroy_Tree(root);
}

void F_Tree::Insert(Vertex_set &vst, std::vector<std::list<size_t>> &start_v_storage,
					std::vector<std::list<size_t>> &start_f_storage)
{
	FT_Node* cur = root;
	std::list<size_t> cur_l = vst.vertices;
	if (cur_l.size() < 3)
	{
		if (vst.vertices.size()==1)
			{
			if (root->edges.find(*cur_l.begin()) == root->edges.end())
			{
				FT_Node* new_node = new FT_Node(vst);
				cur->Add_Child(cur_l.front(), new_node);
			}
			else
			{
				if (cur->C_S.vertices.empty())
					cur->Add_MinSet(vst);
			}
			}
		
		else
		{
			
			if (root->edges.find(*cur_l.begin()) == root->edges.end())
			{
				Vertex_set* buff = new Vertex_set();
				FT_Node* new_node = new FT_Node(*buff);
				new_node->Add_MinSet(*buff);
				cur->Add_Child(cur_l.front(), new_node);
			}
			cur = cur->edges.find(*cur_l.begin())->second;
			FT_Node* new_node = new FT_Node(vst);
			cur->Add_Child(cur_l.back(), new_node);

		}
		
	}
	else
	{
		cur_l.sort();
		Vertex_set CS;
		CS.vertices.push_back(*cur_l.begin());
		std::list<size_t>::iterator it = cur_l.begin()++;
		for (; it != cur_l.end(); it++)
		{
			if (CS.Cl_operation(start_v_storage, start_f_storage).Get_size() <
				CS.Cl_operation(start_v_storage[*it - 1], start_v_storage, start_f_storage).Get_size())
				CS.vertices.push_back(*it);
		}

		for (auto &it : CS.vertices)
		{
			if (cur->edges.find(it) == cur->edges.end())
			{
				Vertex_set* buff = new Vertex_set();
				FT_Node* new_node = new FT_Node(*buff);
				cur->Add_Child(it, new_node);
			}
			cur = cur->edges.find(it)->second;
		}
		cur->Add_MinSet(vst);

	}
}
bool F_Tree::Search(Vertex_set &vst, std::vector<std::list<size_t>> &start_v_storage,
					std::vector<std::list<size_t>> &start_f_storage)
{
	if (root->edges.empty())
		return false;
	FT_Node* current = root;
	std::list<size_t> cur_l = vst.vertices;
	if (vst.vertices.size() < 3)
	{
		for (auto &it : cur_l)
		{
			if (current->edges.empty() || current->edges.find(it) == current->edges.end())
				return false;
			current = current->edges.find(it)->second;
		}
		if (current->GetCS() == vst.vertices)
			return true;
		else return false;

	}
	else
	{
		cur_l.sort();
		Vertex_set CS;
		CS.vertices.push_back(*cur_l.begin());
		std::list<size_t>::iterator it = cur_l.begin()++;
		for (; it != cur_l.end(); it++)
		{
			if (CS.Cl_operation(start_v_storage, start_f_storage).Get_size() <
				CS.Cl_operation(start_v_storage[*it - 1], start_v_storage, start_f_storage).Get_size())
				CS.vertices.push_back(*it);
		}
		for (auto &it : CS.vertices)
		{
			if (current->edges.empty() || current->edges.find(it) == current->edges.end())
				return false;
			current = current->edges.find(it)->second;
		}
		if (current->GetCS() == vst.vertices)
			return true;
		else return false;
	}
}

void F_Tree::Destroy_Tree(FT_Node* root)
{
	if (root)
	{
		std::map<size_t, FT_Node*>::iterator it;
		for (it = root->edges.begin(); it != root->edges.end(); it++)
		{
			Destroy_Tree((*it).second);
			root->edges.erase(++it);
		}
		delete root;
	}
}


