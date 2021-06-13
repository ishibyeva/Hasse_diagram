	// Hasse_diagram.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
	//
#include <iostream>
	#include <fstream>
	#include <string>
	#include <set>
	#include <map>
	#include <ctime>
	#include <vector>
	#include "SpecialTree.h"
	#include "FacetTree.h"

	using namespace std;

	int v_num, f_num, dim;
	int k = 0;
	// 1 set - face set, 2 set - vertex set
	vector<vector<pair<vector<bool>, set<int>>>> HDiagramm_lvls;
	map <vector<bool>, set<int>> full_set;
	SpecialTree* sch_tree = new SpecialTree();


	// печать вершины
	void Print_Vertices(pair<vector<bool>, set<int>> sm_set)
	{
		cout << '(';
		for (auto i : sm_set.second)
		{
			cout << " ";
			cout << i;
			cout << " ";
		}
		cout << ')';
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
			cout << "}:	{";
			for (int j = 0; j < kplus_lvl.size(); j++)
			{
				if (kplus_lvl[j].first == Face_Intersection(k_lvl[i].first, kplus_lvl[j].first))
				{
					Print_Vertices(kplus_lvl[j]);
					cout << ",";
				}
			}
			cout << "}";
			cout << "\n";
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
		vector<bool> fll_set(f_num, false);

		vector<pair<vector<bool>, set<int>>> k_plus1_lvl;
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
							k_plus1_lvl.push_back(make_pair(candidat, candidat_vert));
						
				}
			}
		}
		return k_plus1_lvl;
		
	}
	//realization with special search structure 
	vector<pair<vector<bool>, set<int>>> Face_Enumeration_2(vector<pair<vector<bool>, set<int>>> & k_lvl)
	{
		vector<bool> fll_set(f_num, false);

		vector<pair<vector<bool>, set<int>>> k_plus1_lvl;
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
							k_plus1_lvl.push_back(make_pair(candidat, candidat_vert));
						
				}
			}
		}
		return k_plus1_lvl;
	}
	void View_Element_EmptySet()
	{
		cout << "{ }:	{";
		int i = 1;
		while (i != v_num + 1)
		{
			cout << "( " << i++ << " ),";
		}
		cout << "}\n";
	}
	void View_Element_FullSet(vector<pair<vector<bool>, set<int>>> fll_st)
	{
		cout << "dim " << dim << ":\n";
		cout << "{";
		Print_Vertices(fll_st[0]);
		cout << "}:	{ }\n";
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
		View_Element_EmptySet();
		while (k != dim)
		{
			cout << "dim " << k << ":\n";
			Search_of_Edges(HDiagramm_lvls[k], HDiagramm_lvls[k + 1]);
			k++;
		}
		View_Element_FullSet(HDiagramm_lvls[dim]);
	}

	void KP_Algorithm()
	{
		First_Act(f_num);
		F_Tree* ftree = new F_Tree();
		list<Vertex_set> min_sets;


		while (!Q.empty())
		{
			min_sets = Search_of_G_set(Q.front());
			Vertex_set Q_pop = Q.front();
			
			while (!min_sets.empty())
			{
				if (!ftree->Search(min_sets.front()))
				{
					Vertex_set* copy = new Vertex_set(min_sets.front());
					ftree->Insert(*copy);
					H_Diag_Node New_node = { Q_pop, *copy };
					L.push_back(New_node);
					Q.push_back(*copy);
				}
				min_sets.pop_front();
			}
			Q.pop_front();
		}

		
		list<size_t> full_set;
		for (int i = 1; i <= v_num; i++)
			full_set.push_back((size_t)i);
		Vertex_set* backH = new Vertex_set(full_set);
	
		H_Diag_Node back_pair = {*backH,*backH};
		L.push_back(back_pair);
	}

	void main()
	{
		
		cout << "enter filename" << "\n";
		string filename;
		std::cin >> filename; 
		std::ifstream infile(filename);
		std::string line;
		vector<bool> rez_face_set;	
		set<int> vertex;
		int i = 0;
		//enter matrix
		while (std::getline(infile, line))
		{
			if (line.find_first_not_of(' ') != std::string::npos)
			{
				if (i == 0)
					v_num=std::stoi(line);
				if (i == 1)
					f_num = std::stoi(line);
				if (i == 2)
				{
					dim = std::stoi(line);
					HDiagramm_lvls.resize(dim + 1);
				}
				else if (i>2)
				{
					Vert_List_Building(line);
					vertex.clear();
					rez_face_set.clear();
					rez_face_set.resize(f_num);
					vertex.insert(i-2);
					int str_pars = 0; string inc_num;
					while (line[str_pars]!='\0')
					{
						inc_num = inc_num + line[str_pars];
						if (line[str_pars] == ' ' || line[str_pars+1] == '\0')
						{
							rez_face_set[std::stoi(inc_num)-1]=true;
							inc_num.clear();
						}
						str_pars++;
					}
					HDiagramm_lvls[0].push_back(make_pair(rez_face_set, vertex));
				}
				
			}
			i++;	
		}
			
			
		// start timing 1
		unsigned int start_time = clock();
		// if bool false - use standart structure

		//Build_Diagramm(false);
		
		unsigned int end_time = clock();
		unsigned int search_time1 = end_time - start_time;
		// end timing 1
		//start timing 2
		k = 0;
		// clean vector of faces
		HDiagramm_lvls.erase(HDiagramm_lvls.begin()+1, HDiagramm_lvls.end());
		HDiagramm_lvls.resize(dim+1);
		cout << "____________________________\n\n";
		start_time = clock();
		// if bool true - use special structure 

		//Build_Diagramm(true);

		end_time = clock();
		unsigned int search_time2 = end_time - start_time;
		//end timing 2
		start_time = clock();
		KP_Algorithm();
		cout << "____________________________\n\n";
		end_time = clock();

		cout << "(0)Verts:{}---->{ ";
		for (int i = 1; i <= v_num; i++)
			cout <<"("<< i<<") ";
		cout << " }"<<'\n';

		size_t fin_size = L[L.size()-2].Vert_adrG.Get_size();
		int k1 = 0, k2=0, start=0;
		while (k2!=L.size())
		{
			start = k1;
			size_t mem_size=L[k1].Vert_adrH.Get_size();
			while (k1 != L.size() && L[k1].Vert_adrH.Get_size() == mem_size)
				k1++;
			mem_size = L[k1].Vert_adrH.Get_size();
			k2 = k1;
			
			while (k2!=L.size() && L[k2].Vert_adrH.Get_size() == mem_size )
				k2++;
			
			for (int i = start; i < k1; i++)
			{
				cout << "(" << i+1 << ")" << "Verts:";
				L[i].Vert_adrG.Print_vert();
				cout << "---->{ ";
				for (int j = k1; j < k2; j++)
				{
					bool flag;
					for (auto &elem : L[i].Vert_adrG.vertices)
					{
						 flag = true;
						if (find(L[j].Vert_adrG.vertices.begin(), L[j].Vert_adrG.vertices.end(), elem) == L[j].Vert_adrG.vertices.end())
						{
							flag = false;
							break;
						}
					}
					if (flag)
						cout << " (" << j+1 << ") ";
				}
				cout << "}" << '\n';
			}
		}

		cout << "(" << L.size() << ")Verts:";
		L[L.size() - 1].Vert_adrG.Print_vert();
		cout << "---->{ }"<<'\n';

		//end timing 3
		unsigned int search_time3 = end_time - start_time;


		cout << "____________________________\n" ; 
		cout << filename<< '\n';
		cout << "____________________________\n";

		//cout << "First algorithm with Standart RBT: " << search_time1<<"\n";
		//cout << "First algorithm with Special search tree structure: " << search_time2 << "\n";
		cout << "Keibel and Pfetsch modern algorithm: " << search_time3 << "\n";
	}