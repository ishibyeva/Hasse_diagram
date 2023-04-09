#ifndef _MYINCL1_H
#define _MYINCL1_H

#include <vector>
#include <memory>

class SpecialTree
{
	struct VecNode
	{
		std::vector<bool> Data;
		std::unique_ptr<VecNode> Left;
		std::unique_ptr<VecNode> Right;
		explicit VecNode(const std::vector<bool> &val) : Data(val) {}
	};
	std::unique_ptr<VecNode> root;

public:
	SpecialTree();
	bool Search(std::vector<bool> s_elem);
	void Insert(std::vector<bool> in_elem);
	void DestroyTree(VecNode *node);
	// ~SpecialTree() { DestroyTree(root); }
};

#endif