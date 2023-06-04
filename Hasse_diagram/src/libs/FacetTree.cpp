#include "FacetTree.h"

F_Tree::F_Tree()
{
	root = std::make_unique<FT_Node>(std::make_shared<Vertex_set>());
}

// F_Tree::~F_Tree()
// {
// 	Destroy_Tree(root);
// }

void F_Tree::Insert(std::shared_ptr<Vertex_set> vst, std::vector<std::list<size_t>> &start_v_storage,
					std::vector<std::list<size_t>> &start_f_storage)
{
	auto cur = root.get();
	std::list<size_t> cur_l = vst->vertices;
	if (cur_l.size() < 3)
	{
		if (vst->vertices.size()==1)
			{
			if (root->edges.find(*cur_l.begin()) == root->edges.end())
			{
				auto new_node = std::make_unique<FT_Node>(vst);
				cur->Add_Child(cur_l.front(), std::move(new_node));
			}
			else
			{
				if (cur->C_S->vertices.empty())
					cur->Add_MinSet(vst);
			}
			}
		
		else
		{
			if (root->edges.find(*cur_l.begin()) == root->edges.end())
			{
				auto buff = std::make_shared<Vertex_set>();
				auto new_node = std::make_unique<FT_Node>(buff);
				new_node->Add_MinSet(buff);
				cur->Add_Child(cur_l.front(), std::move(new_node));
			}
			cur = cur->edges.find(*cur_l.begin())->second.get();
			// FT_Node* new_node = new FT_Node(vst);
			auto new_node = std::make_unique<FT_Node>(vst);
			cur->Add_Child(cur_l.back(), std::move(new_node));

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
				CS.Cl_operation(start_v_storage[*it], start_v_storage, start_f_storage).Get_size())
				CS.vertices.push_back(*it);
		}

		for (auto &it : CS.vertices)
		{
			if (cur->edges.find(it) == cur->edges.end())
			{
				// Vertex_set* buff = new Vertex_set();
				auto new_node = std::make_unique<FT_Node>(std::make_shared<Vertex_set>());
				cur->Add_Child(it, std::move(new_node));
			}
			cur = cur->edges.find(it)->second.get();
		}
		cur->Add_MinSet(vst);

	}
}
bool F_Tree::Search(std::list<size_t> &vst, std::vector<std::list<size_t>> &start_v_storage,
					std::vector<std::list<size_t>> &start_f_storage)
{
	if (root->edges.empty())
		return false;
	auto current = root.get();
	std::list<size_t> cur_l = vst;
	if (vst.size() < 3)
	{
		for (auto &it : cur_l)
		{
			if (current->edges.empty() || current->edges.find(it) == current->edges.end())
				return false;
			current = current->edges.find(it)->second.get();
		}
		if (current->GetCS() == vst)
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
				CS.Cl_operation(start_v_storage[*it], start_v_storage, start_f_storage).Get_size())
				CS.vertices.push_back(*it);
		}
		for (auto &it : CS.vertices)
		{
			if (current->edges.empty() || current->edges.find(it) == current->edges.end())
				return false;
			current = current->edges.find(it)->second.get();
		}
		if (current->GetCS() == vst)
			return true;
		else return false;
	}
}

void F_Tree::Destroy_Tree(FT_Node* root)
{
	if (root)
	{
		std::map<size_t, std::unique_ptr<FT_Node>>::iterator it;
		for (it = root->edges.begin(); it != root->edges.end(); it++)
		{
			Destroy_Tree((*it).second.get());
			root->edges.erase(++it);
		}
		delete root;
	}
}


