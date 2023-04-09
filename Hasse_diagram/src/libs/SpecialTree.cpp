#include "SpecialTree.h"


SpecialTree::SpecialTree()
{
	root = nullptr;
}
bool SpecialTree::Search(std::vector<bool> s_elem)
{
	if (root == nullptr)
		return false;
	size_t i = 0;
	auto cur = root.get();
	while (i != s_elem.size())
	{
		if (s_elem[i])
		{
			cur = cur->Right.get();
			if (cur == nullptr)
				return false;
		}
		else
		{
			cur = cur->Left.get();
			if (cur == nullptr)
				return false;
		}
		i++;
	}
	return true;
}

void SpecialTree::Insert(std::vector<bool> in_elem)
{
	if (root == nullptr)
	{
		root = std::make_unique<VecNode>(std::vector<bool>());
	}
	size_t i = 0;
	std::vector<bool> mid_vec;
	auto cur = root.get();
	while (i != in_elem.size())
	{
		mid_vec.push_back(in_elem[i]);
		if (in_elem[i])
		{

			if (cur->Right == nullptr)
			{
				cur->Right = std::make_unique<VecNode>(mid_vec);
			}
			cur = cur->Right.get();
		}
		else
		{
			if (cur->Left == nullptr)
			{
				cur->Left = std::make_unique<VecNode>(mid_vec);
			}
			cur = cur->Left.get();
		}
		i++;
	}
}
// void SpecialTree::DestroyTree(VecNode *node)
// {
// 	if (node)
// 	{
// 		DestroyTree(node->Left);
// 		DestroyTree(node->Right);
// 		delete node;
// 	}
// }
