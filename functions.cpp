#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <vector>
#include "functions.h"
using namespace std;

bool searchColumn(string s,int j,string str)	//searches column j for the string str in string s
{
	char c;
	int i=0,f=0,k=0;
	bool found=1;
	
	while(i++<j)//move f to the j column 
	{
		do
		{
			c=s[f++];
		}while(c!='|');
		
	}
	do//compare strings
	{
		c=s[f];
		if((str[k]!=s[f++] && c!='|')||(c=='|' && str[k]!=0))
		{
			found=0;
			break;
		}
		k++;
	}while(c!='|');
	
	if(found)
		return 1;
	return 0;
}

string sgetstring(string s,int j,char delim)//returns the string of the j column in string s
{
	char c;
	int i=0,f=0,k=0;
	char* buffer=new char[60];
	
	while(i++<j)//move f to the j column 
	{
		do
		{
			c=s[f++];
		}while(c!=delim && c!='\n' && c!=0);
		
	}
	
	do//create the string we want
	{
		c=s[f];
		if(c!=delim && c!='\n' && c!=0)buffer[k++]=s[f++];
	}while(c!=delim && c!='\n' && c!=0);
	buffer[k]=0;
	return buffer;
	
}

string sgetstring_after(string s,int j,char delim)//returns the string after the j column in string s
{
	char c;
	int i=0,f=0,k=0;
	char* buffer=new char[80];
	
	while(i++<j)//move f to the j column 
	{
		do
		{
			c=s[f++];
		}while(c!=delim && c!='\n' && c!=0);
		
	}
	
	do//create the string we want
	{
		buffer[k++]=s[f++];
	}while(s[f]);
	buffer[k]=0;
	return buffer;
	
}

void update_users_file(vector<Users*> usersvector)
{
	ofstream users ("users.csv", ios::out | ios::trunc);
	users<<"user_id|username|fullname|password|type|status|balance|freebets"<<endl;
	for (int i = 0; i < usersvector.size(); i++)
	{
		users<<usersvector[i]->getid()<<"|";
		users<<usersvector[i]->getusername()<<"|";
		users<<usersvector[i]->getfullname()<<"|";
		users<<usersvector[i]->getpassword()<<"|";
		users<<usersvector[i]->gettype()<<"|";
		if(usersvector[i]->gettype()==1)
		{
			users<<usersvector[i]->getstatus()<<"|";
			users<<usersvector[i]->getbalance()<<"|";
			users<<usersvector[i]->getfreebets()<<endl;
		}
		else
			users<<"||"<<endl;
	}
	users.close();
}

void printlast20bets()
{
	string s;
	ifstream bets("bets.csv");
	cout<<"         LAST 20 BETS TABLE            "<<endl;
	cout<<"---------------------------------------"<<endl;
	cout<<"|user_id|node_id     |stake   |result |"<<endl;
	int counter=0;
	getline(bets,s);//ignore first line
	while(getline(bets,s))counter++;//count bets
	bets.clear();
	bets.seekg(0);
	getline(bets,s);//ignore first line
	for (int i = 0; i < counter-20; i++)//move getpointer of file
		getline(bets,s);
	while(getline(bets,s))
	{
		cout<<"|"<<left<<setw(7)<<setfill(' ')<<sgetstring(s,1,'|')<<"|";
		cout<<left<<setw(12)<<setfill(' ')<<sgetstring(s,2,'|')<<"|";
		cout<<left<<setw(8)<<setfill(' ')<<sgetstring(s,3,'|')<<"|";
		if(sgetstring(s,4,'|').compare("W")==0)cout<<left<<setw(7)<<setfill(' ')<<"Won"<<"|"<<endl;
		else if(sgetstring(s,4,'|').compare("L")==0)cout<<left<<setw(7)<<setfill(' ')<<"Lost"<<"|"<<endl;
		else if(sgetstring(s,4,'|').compare("V")==0)cout<<left<<setw(7)<<setfill(' ')<<"Voided"<<"|"<<endl;
		else cout<<left<<setw(7)<<setfill(' ')<<"Pending"<<"|"<<endl;
	}
	cout<<"---------------------------------------"<<endl;
	bets.close();
}

void printlast25logs()
{
	string s;
	ifstream audit("audit.log");
	cout<<"         LAST 25 LOGS          "<<endl;
	cout<<"-------------------------------"<<endl;
	int counter=0;
	while(getline(audit,s))counter++;//count logs
	audit.clear();
	audit.seekg(0);
	for (int i = 0; i < counter-25; i++)//move getpointer of file
		getline(audit,s);
	while(getline(audit,s))
		cout<<s<<endl;
	cout<<"-------------------------------"<<endl;
	audit.close();
}

void givefreebets(vector<Users*> usersvector)
{
	string s;
	cout<<"Type the username that takes the freebet:";
	getline(cin,s);
	int found=-1;
	for (int i = 0; i < usersvector.size(); i++)
		if(s.compare(usersvector[i]->getusername())==0)
			{found=i;break;}
	if(found!=-1 && usersvector[found]->gettype()==1)
	{
		cout<<"Type the amount of freebet you want to give:";
		getline(cin,s);
		if(atof(s.c_str())>0)
		{
			//create ss with freebets
			stringstream ss;
			ss<<usersvector[found]->getfreebets();
			//if it isn't empty put a comma
			if(ss.rdbuf()->in_avail())ss<<",";
			ss<<s;
			usersvector[found]->setfreebets(ss.str());
			update_users_file(usersvector);
			cout<<"freebet was given succesfully"<<endl;
		}
		else
			cout<<"non-acceptable freebet"<<endl;
	}
	else
		cout<<"username does not exist or this user is not a punter"<<endl;
}

void update_hierarchy_file(HierarchyNode hierroot)
{
	ofstream file("hierarchy.dat", ios::out | ios::trunc);
	for (int i = 0; i < hierroot.getvectorsize(); i++)//categories
	{
		file<<i+1<<". "<<hierroot.getnextnode(i)->getname();
		
		file<<endl;
		for (int j = 0; j < hierroot.getnextnode(i)->getvectorsize(); j++)//subcategories
		{
			file<<i+1<<"."<<j+1<<". "<<hierroot.getnextnode(i)->getnextnode(j)->getname();
			file<<endl;
			for (int k = 0; k < hierroot.getnextnode(i)->getnextnode(j)->getvectorsize(); k++)//events
			{
				file<<i+1<<"."<<j+1<<"."<<k+1<<". "<<hierroot.getnextnode(i)->getnextnode(j)->getnextnode(k)->getname();
				file<<endl;
				for (int l = 0; l < hierroot.getnextnode(i)->getnextnode(j)->getnextnode(k)->getvectorsize(); l++)//markets
				{
					file<<i+1<<"."<<j+1<<"."<<k+1<<"."<<l+1<<". "<<hierroot.getnextnode(i)->getnextnode(j)->getnextnode(k)->getnextnode(l)->getname();
					file<<endl;
					for (int m = 0; m < hierroot.getnextnode(i)->getnextnode(j)->getnextnode(k)->getnextnode(l)->getvectorsize(); m++)//selections
					{
						file<<i+1<<"."<<j+1<<"."<<k+1<<"."<<l+1<<"."<<m+1<<". "<<hierroot.getnextnode(i)->getnextnode(j)->getnextnode(k)->getnextnode(l)->getnextnode(m)->getname()<<"#"<<hierroot.getnextnode(i)->getnextnode(j)->getnextnode(k)->getnextnode(l)->getnextnode(m)->getprice()<<endl;
					}
				}
			}
		}
	}
	file.close();
}

void printuserinfo(Users* user)
{
	cout<<"|"<<left<<setw(2)<<setfill(' ')<<user->getid()<<"|";
	cout<<left<<setw(17)<<setfill(' ')<<user->getusername()<<"|";
	cout<<left<<setw(26)<<setfill(' ')<<user->getfullname()<<"|";
	cout<<left<<setw(17)<<setfill(' ')<<user->getpassword()<<"|";
	cout<<left<<setw(4)<<setfill(' ')<<user->gettype()<<"|";
	if(user->getstatus().compare("")==0||user->getstatus().compare("A")==0)cout<<"                              |";
	else if(sgetstring_after(user->getstatus(),1,',')=="")cout<<"N/A                           |";
	else cout<<left<<setw(30)<<setfill(' ')<<sgetstring_after(user->getstatus(),1,',')<<"|";
	if(user->gettype()==1)cout<<left<<setw(10)<<setfill(' ')<<user->getbalance()<<"|";
	else cout<<"          |";
	if(user->gettype()==1)cout<<left<<setw(15)<<setfill(' ')<<user->getfreebets()<<"|"<<endl;
	else cout<<"               |"<<endl;
}
