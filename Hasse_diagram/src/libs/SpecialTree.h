#ifndef _MYINCL1_H
#define _MYINCL1_H

#include <vector>

using namespace std;

struct VecNode
{
	vector<bool> Data;
	VecNode* Left;
	VecNode* Right;
};


class SpecialTree
{
	VecNode* root;

public:
	SpecialTree();
	bool Search(vector<bool> s_elem);
	void Insert(vector<bool> in_elem);
	void DestroyTree(VecNode* node);
	~SpecialTree() { DestroyTree(root); }

};





#endif