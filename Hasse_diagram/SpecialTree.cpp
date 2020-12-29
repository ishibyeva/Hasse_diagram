
#include "SpecialTree.h"

VecNode* newNode(vector<bool> data)
{
	VecNode* temp = new VecNode;

	temp->Data = data;
	temp->Left = nullptr;
	temp->Right = nullptr;

	return temp;
}

SpecialTree::SpecialTree()
{
	root = nullptr;
	
}
bool SpecialTree::Search(vector<bool> s_elem)
{
	if (root== nullptr)
		return false;
	int i = 0;
	VecNode* cur = root;
	while (i != s_elem.size())
	{
		if (s_elem[i] == true)
		{
			cur = cur->Right;
			if (cur == NULL)
				return false;

		}
		else
		{
			cur = cur->Left;
			if (cur == NULL)
				return false;
		}
		i++;
	}
	return true;
}

void SpecialTree::Insert(vector<bool> in_elem)
{
	int i = 0;
	VecNode* cur;
	VecNode* newnode;
	vector<bool> mid_vec;
	if (root == nullptr)
	{
		vector<bool> root_vec = {};
		newnode = newNode(root_vec);
		root = newnode;
	}

	cur = root;
	while (i != in_elem.size())
	{
		mid_vec.push_back(in_elem[i]);
		if (in_elem[i] == true)
		{
			
			if (cur->Right == nullptr)
			{
				newnode = newNode(mid_vec);
				cur->Right = newnode;
			}
			cur = cur->Right;
		}
		else
		{
			
			if (cur->Left == nullptr)
			{
				newnode = newNode(mid_vec);
				cur->Left = newnode;
			}
			cur = cur->Left;
		}

		i++;
	}
}
void SpecialTree::DestroyTree(VecNode* node)
{
	if (node)
	{
		DestroyTree(node->Left);
		DestroyTree(node->Right);
		delete node;
	}
}


