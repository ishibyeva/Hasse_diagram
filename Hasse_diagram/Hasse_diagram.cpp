// Hasse_diagram.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//



#include <iostream>
#include <set>
#include <map>
#include <ctime>
#include <vector>
#include "SpecialTree.h"

using namespace std;

int v_num, f_num, dim;
int k = 0;
// 1 set - face set, 2 set - vertex set
vector<vector<pair<vector<bool>, set<int>>>> HDiagramm_lvls;



// печать вершины
void Print_Vertices(pair<vector<bool>, set<int>> sm_set)
{
		for (auto i : sm_set.second)
			cout << i ;	
}


vector<bool> Face_Intersection(vector<bool> vec1, vector<bool> vec2)
{
	vector<bool> int_vec;
	for (int i = 0; i < f_num; i++)
		int_vec.push_back(vec1[i] && vec2[i]);
	return int_vec;
}

// if vertices incedent - true 
void Search_of_Edges(vector<pair<vector<bool>, set<int>>> k_lvl, vector<pair<vector<bool>, set<int>>> kplus_lvl)
{
	for (int i = 0; i < k_lvl.size(); i++)
	{
		cout << "{";
		Print_Vertices(k_lvl[i]);
		cout << "}: ";
		for (int j = 0; j < kplus_lvl.size(); j++)
		{
			if (kplus_lvl[j].first == Face_Intersection(k_lvl[i].first, kplus_lvl[j].first))
			{
				Print_Vertices(kplus_lvl[j]);
				cout << "\t";
			}
		}
		cout << endl;
	}
}

set<int> Get_New_Face(set<int> st1, set<int> st2)
{
	set<int> st3;
	st3 = st1;
	st3.insert(st2.begin(), st2.end());
	return st3;
}

// realization with RBT 

vector<pair<vector<bool>, set<int>>> Face_Enumeration_1(vector<pair<vector<bool>, set<int>>> &k_lvl)
{
	vector<pair<vector<bool>, set<int>>> k_plus1_lvl;
	map <vector<bool>, set<int>> full_set;


	for (int i=0; i<k_lvl.size()-1; i++)
	{
		for (int j = i+1; j<k_lvl.size(); j++)
		{
			vector<bool> candidat=Face_Intersection(k_lvl[i].first, k_lvl[j].first);
			set<int> candidat_vert=Get_New_Face(k_lvl[i].second, k_lvl[j].second);

			if (full_set.find(candidat) == full_set.end())
			{
				
					if (candidat == k_lvl[i].first)
					{
						k_lvl.erase(k_lvl.begin() + i);
					}
					if (candidat == k_lvl[j].first)
					{
						k_lvl.erase(k_lvl.begin() + j);
					}
				

					full_set.insert(make_pair(candidat, candidat_vert));
					k_plus1_lvl.emplace_back(candidat, candidat_vert);
			}

		}
	}

	return k_plus1_lvl;
	
}

//realization with special search structure 

vector<pair<vector<bool>, set<int>>> Face_Enumeration_2(vector<pair<vector<bool>, set<int>>> & k_lvl)
{
	vector<pair<vector<bool>, set<int>>> k_plus1_lvl;
	SpecialTree* sch_tree = new SpecialTree();


	for (int i = 0; i < k_lvl.size() - 1; i++)
	{
		for (int j = i + 1; j < k_lvl.size(); j++)
		{
			vector<bool> candidat = Face_Intersection(k_lvl[i].first, k_lvl[j].first);
			set<int> candidat_vert = Get_New_Face(k_lvl[i].second, k_lvl[j].second);

			if (sch_tree->Search(candidat) == false)
			{
				
					if (candidat == k_lvl[i].first)
					{
						k_lvl.erase(k_lvl.begin() + i);
					}
					if (candidat == k_lvl[j].first)
					{
						k_lvl.erase(k_lvl.begin() + j);
					}


				sch_tree->Insert(candidat);
				k_plus1_lvl.emplace_back(candidat, candidat_vert);


			}

		}
	}

	return k_plus1_lvl;

}

void Build_Face_Set(bool struc_ind)
{
	while (k != dim)
	{
		vector<pair<vector<bool>, set<int>>> & ref = HDiagramm_lvls[k];
		if (struc_ind)
			HDiagramm_lvls[k + 1] = Face_Enumeration_2(ref);
		else
			HDiagramm_lvls[k + 1] = Face_Enumeration_1(ref);
		k++;
	}
}

// build special lists of incedence for H diagramm
void Build_Diagramm(bool struct_ind)
{
	Build_Face_Set(struct_ind);
	k = 0;
	while (k != dim)
	{
		Search_of_Edges(HDiagramm_lvls[k], HDiagramm_lvls[k + 1]);
		k++;
	}
}


void main()
{
	
	cout << "enter dim and number of vertices and faces" << endl;
	cin >> dim >> v_num >> f_num;
	bool mat_cell;
	vector<bool> rez_face_set;	set<int> vertex;


	HDiagramm_lvls.resize(dim + 1);

	//enter matrix

	for (int i = 0; i < v_num; i++)
	{
		rez_face_set.clear();
		vertex.clear();
		for (int j = 0; j < f_num; j++)
		{
			cin >> mat_cell;
			rez_face_set.push_back(bool(mat_cell));
		}
		vertex.insert(i + 1);
		HDiagramm_lvls[0].push_back(make_pair(rez_face_set, vertex));
	}
	

	// start timing 1
	unsigned int start_time = clock();

	// if bool false - use standart structure
	Build_Diagramm(false);
	
	unsigned int end_time = clock();
	unsigned int search_time1 = end_time - start_time;
	// end timing 1
	//start timing 2
	k = 0;
	// clean vector of faces
	HDiagramm_lvls.erase(HDiagramm_lvls.begin()+1, HDiagramm_lvls.end());
	HDiagramm_lvls.resize(dim+1);

	start_time = clock();

	// if bool true - use special structure 
	Build_Diagramm(true);

	end_time = clock();
	unsigned int search_time2 = end_time - start_time;
	//end timing 2
	

	cout << "____________________________" << endl; 
	cout << "Standart RBT: " << search_time1 << endl;
	cout << "Special structure: " << search_time2 << endl;
}


