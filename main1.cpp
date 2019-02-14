#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <vector>
#include "functions.h"
bool decimalodds=0;//default odds type to fractional
const string name="KouvasBet";//name of system

using namespace std;
int main(int args,char** argv)
{
	bool reg=0;
	string username,password,s;
	fstream users("users.csv"),hierarchy("hierarchy.dat");
	Users* currentuser=0;
	HierarchyNode* currentnode;
	
	////load file hierarchy in objects
	HierarchyRoot hierroot(hierarchy);
	
	////load file users in objects
	vector<Users*> usersvector;
	getline(users,s);//ignore first line
	while(getline(users,s))
	{
		int type=atoi(sgetstring(s,4,'|').c_str());
		switch(type)
		{
			case 1:
				usersvector.push_back(new Punter(usersvector.size()+1,sgetstring(s,1,'|'),sgetstring(s,2,'|'),sgetstring(s,3,'|'),1,sgetstring(s,5,'|'),atof(sgetstring(s,6,'|').c_str()),sgetstring(s,7,'|')));
				break;
			case 2:
				usersvector.push_back(new Trader(usersvector.size()+1,sgetstring(s,1,'|'),sgetstring(s,2,'|'),sgetstring(s,3,'|'),2));
				break;
			case 3:
				usersvector.push_back(new Director(usersvector.size()+1,sgetstring(s,1,'|'),sgetstring(s,2,'|'),sgetstring(s,3,'|'),3));
				break;
		}
	}
	
	
	/////// registration (-R)
	if(args==2) if(argv[1][0]=='-' && argv[1][1]=='R')
	{
		reg=1;
		cout<<"Username:";
		getline(cin,username);
		cout<<"Password:";
		getline(cin,password);
		
		bool available=1;
		for (int i = 0; i < usersvector.size(); i++)
			if (usersvector[i]->getusername()==username)//not available
				available=0;
		if(available==1)
		{
			//add object user
			usersvector.push_back(new Punter(usersvector.size()+1,username,username,password,1,"A",0,""));
			//add to file
			update_users_file(usersvector);
			//make current user
			currentuser=usersvector[usersvector.size()-1];
		}
		else
		{
			cout<<"Username not available"<<endl;
			return 0;
		}
	}
	////if no registration made, login
	if(reg==0)
	{
		cout<<"Welcome to "<<name<<"!"<<endl;
		cout<<"Please give your username and password for login"<<endl;
		cout<<"Alternatively,press enter or type guest:guest to entry as a guest"<<endl;
		while(currentuser==0)//until right combination
		{
			cout<<"Username:";
			getline(cin,username);
			if(username=="")
				currentuser=new Guest;
			else
			{
				cout<<"Password:";
				getline(cin,password);
				if(username=="guest" && password=="guest")
					currentuser=new Guest;
				else
				{
					for (int i = 0; i < usersvector.size(); i++)//search vector for correspondent strings
						if (username==usersvector[i]->getusername() && password==usersvector[i]->getpassword())
							currentuser=usersvector[i];
					if(currentuser==0)//not found 
						cout<<"Wrong username or password, type again"<<endl;
					else if (currentuser->getstatus()!="A" && currentuser->getstatus()!="")//banned
					{
						cout<<"Sorry, you are banned!"<<endl;
						return 0;
					}
				}
			}
		}
	}
	users.close();
	hierarchy.close();
	/*-----currentuser now points at the current user obviously--------*/
	if(currentuser->getid()==0)
		cout<<"---------------Logged in as a guest-------------------"<<endl;
	else
		cout<<"----------Welcome "<<currentuser->getusername()<<"----------"<<endl;
	
	vector<int> pathstack;//keeps the sequence of numbers that express the current location int the tree
	while(1){
		
		//print location
		cout<<"Location:/"<<name;
		
		//make currentnode to point at current node
		currentnode=&hierroot;
		for (int i = 0; i < pathstack.size(); i++)
		{
			currentnode=currentnode->getnextnode(pathstack[i]);
			cout<<"/"<<currentnode->getname();
		}
		cout<<endl;
		cout<<"-------------------------------------------------------------------"<<endl;
		
		//print odds type
		if(decimalodds)cout<<"Odds type: Decimal"<<endl;
		else cout<<"Odds type: Fractional"<<endl;
		//print operations
		cout<<"Choose"<<endl<<"ID of the node you want to go"<<endl;
		currentuser->print_oper(currentnode->getdepth());
		cout<<"Available choices at "<<currentnode->getname()<<endl;
		if(currentuser->gettype()!=3)currentnode->print_level();
		else currentnode->print_level_dir();
		cout<<"Type your choice:";
		getline(cin,s);
		
		if(s=="X" || s=="Exit")return 0;
		else if(s=="T" || s=="Toggle")decimalodds=!decimalodds;
		else if(s=="H" || s=="Home")pathstack.clear();
		else if(atoi(s.c_str())>=1)
		{
			if(currentnode->getnextnode(atoi(s.c_str())-1)==0)
				cout<<"NO SUCH NODE"<<endl;
			else
				pathstack.push_back(atoi(s.c_str())-1);
		}
		else currentuser->operate(s,currentnode->getdepth(),usersvector,hierroot,currentnode,pathstack);
		update_hierarchy_file(hierroot);
		update_users_file(usersvector);
		cout<<"========================================================================================="<<endl;
	}
	
	
	
	
	return 0;
}
