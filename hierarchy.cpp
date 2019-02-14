#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include "functions.h"
using namespace std;
void HierarchyNode::copynewnode(fstream& file,string nam)//copy
	{
		if(depth==0)
			nextnode.push_back(new Category(file,getvectorsize()+1,nam));
		else if(depth==1)
			nextnode.push_back(new Subcategory(file,getvectorsize()+1,nam));
		else if(depth==2)
			nextnode.push_back(new Event(file,getvectorsize()+1,nam));
		else if(depth==3)
			nextnode.push_back(new Market(file,getvectorsize()+1,nam));
		else if(depth==4)
			nextnode.push_back(new Selection(file,getvectorsize()+1,nam));
	}
void HierarchyNode::newnode(string nam,bool v)
	{
		nextnode.push_back(new HierarchyNode(nam,depth+1,getvectorsize()+1,v));
	}

HierarchyNode* HierarchyNode::getnextnode(int i)
	{
		if(i<nextnode.size())
			return nextnode[i];
		else
			return 0;
	}
	
	
	void HierarchyNode::print_level()
	{
		if(visible)
		if(depth==4)
			for (int i = 0; i < nextnode.size(); i++)
				cout<<nextnode[i]->getid() <<". " <<nextnode[i]->getname()<<" - "<<nextnode[i]->getprice()<<endl;
		else 
			for (int i = 0; i < nextnode.size(); i++)
				cout<<nextnode[i]->getid() <<". " <<nextnode[i]->getname()<<endl;
	}

void HierarchyNode::print_level_dir()
	{
		
		if(depth==4)
			for (int i = 0; i < nextnode.size(); i++)
			{
				if(nextnode[i]->getvisible()==0)cout<<"[Hidden]"<<nextnode[i]->getid() <<". " <<nextnode[i]->getname()<<" - "<<nextnode[i]->getprice()<<endl;
				else cout<<"\t"<<nextnode[i]->getid() <<". " <<nextnode[i]->getname()<<" - "<<nextnode[i]->getprice()<<endl;
			}
		else 
			for (int i = 0; i < nextnode.size(); i++)
			{
				if(nextnode[i]->getvisible()==0)cout<<"[Hidden]"<<nextnode[i]->getid() <<". " <<nextnode[i]->getname()<<endl;
				else cout<<"\t"<<nextnode[i]->getid() <<". " <<nextnode[i]->getname()<<endl;
			}
	}



string Selection::getprice()
	{
		if(decimalodds)
		{//make it decimal
			stringstream ss;
			int a,b;
			a=atoi(sgetstring(price,0,'/').c_str());
			b=atoi(sgetstring(price,1,'/').c_str());
			ss <<fixed<<setprecision(2)<< (double)a/b+1;
			return ss.str();
		}
		return price;
	}



	Selection::Selection(fstream& file,int id, string n):HierarchyNode(file,id,n,5)//:name(n),fullid(id)
	{
		name=sgetstring(n,0,'#');
		price=sgetstring_after(n,1,'#');
		state='-';
		
		//keep the actual price in decimal
		stringstream ss;
		int a,b;
		a=atoi(sgetstring(price,0,'/').c_str());
		b=atoi(sgetstring(price,1,'/').c_str());
		ss <<fixed<<setprecision(2)<< (double)a/b+1;
		pr=atof(ss.str().c_str());
		
	}
	
	Market::Market(fstream& file,int id,string n):HierarchyNode(file,id,n,4)//:name(n),fullid(id)
	{}
	
	
	Event::Event(fstream& file,int id,string n):HierarchyNode(file,id,n,3)//:name(n),fullid(id)
	{}
	
	
	Subcategory::Subcategory(fstream& file,int id,string n):HierarchyNode(file,id,n,2)//:name(n),fullid(id)
	{}
	
	
	Category::Category(fstream& file,int id,string n):HierarchyNode(file,id,n,1)//:name(n),fullid(id)//create vector of subcategories
	{}
	
	
	HierarchyRoot::HierarchyRoot(fstream& file):HierarchyNode(file,0,"Home",0)//create vector of categories
	{}
	
	HierarchyNode::HierarchyNode(fstream& file,int i,string n,int d):name(n),id(i),depth(d)//create vector of nextnodes
	{
		visible=1;
		string s;
		//cout<<"depth "<<depth<<" "<<id<<" "<<name<<endl;
		if(depth==5) return;
		while (1)
		{
			
				streampos pos=file.tellg();
				getline(file,s);
				
				if(file.eof() && depth<4)
					break;
				if(sgetstring(s,depth,'.').c_str()[0] && atoi(sgetstring(s,depth,'.').c_str()))
				{
					if(depth==0)
						nextnode.push_back(new Category(file,atoi(sgetstring(s,depth,'.').c_str()),sgetstring_after(s,1,' ')));
					else if(depth==1)
						nextnode.push_back(new Subcategory(file,atoi(sgetstring(s,depth,'.').c_str()),sgetstring_after(s,1,' ')));
					else if(depth==2)
						nextnode.push_back(new Event(file,atoi(sgetstring(s,depth,'.').c_str()),sgetstring_after(s,1,' ')));
					else if(depth==3)
						nextnode.push_back(new Market(file,atoi(sgetstring(s,depth,'.').c_str()),sgetstring_after(s,1,' ')));
					else if(depth==4)
						nextnode.push_back(new Selection(file,atoi(sgetstring(s,depth,'.').c_str()),sgetstring_after(s,1,' ')));
					if(file.eof())break;
				}
				else
				{
					if(file.eof())break;
					file.clear();
					file.seekg(pos);
					break;
				}
		}
	}
