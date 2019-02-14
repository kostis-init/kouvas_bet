#include <stdlib.h>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>
using namespace std;
extern bool decimalodds;
class HierarchyNode
{
	bool visible;
	int id,depth;
	vector<HierarchyNode*> nextnode;
	string name;
public:


	
	HierarchyNode(fstream& file,int i,string n,int d);
	HierarchyNode(string n,int d,int i,bool v)//for newnode operation
	{
		name=n;depth=d;id=i;visible=v;
	}
	void togglevisible(){visible=!visible;}
	void setvisible(bool v){visible=v;}
	bool getvisible(){return visible;}
	int getid(){return id;}
	virtual double getpr(){}
	virtual void setstate(char c){}
	virtual char getstate(){return '-';}
	int iddown(){id--;}
	int getdepth(){return depth;}
	virtual string getname(){return name;}
	int getvectorsize(){return nextnode.size();}
	virtual string getprice(){return "";}
	void newnode(string nam,bool v);
	void erase(int i)
	{
		for (int j = i+1; j < nextnode.size(); j++)
		{
			nextnode[j]->iddown();
		}
		nextnode.erase(nextnode.begin()+i);
	}
	void copynewnode(fstream& file,string nam);//copy
	virtual void setname(string s){name=s;}
	
	HierarchyNode* getnextnode(int i);
	
	void print_level();
	void print_level_dir();
	
	
};

class Selection: public HierarchyNode
{
	double pr;
	string name, price;//price always at fractional
	char state;
public:
	Selection(fstream& file,int id, string n);
	string getname(){return name;}
	string getprice();
	void setname(string s){name=s;}
	void setprice(string p){price=p;}
	double getpr(){return pr;}
	void setstate(char c){state=c;}
	char getstate(){return state;}
	
};

class Market: public HierarchyNode
{
	
public:
	
	Market(fstream& file,int id,string n);
	
};

class Event: public HierarchyNode
{
	
public:
	
	Event(fstream& file,int id,string n);
	
};

class Subcategory: public HierarchyNode
{
	
public:
	
	Subcategory(fstream& file,int id,string n);
	
};

class Category: public HierarchyNode
{
	
public:
	
	Category(fstream& file,int id,string n);
	
};


class HierarchyRoot: public HierarchyNode
{

public:
	
	HierarchyRoot(fstream& file);
};
