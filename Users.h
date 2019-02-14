#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

extern bool decimalodds;

class Users
{
	int user_id,type;
	string username,password,fullname;
public:
	
	Users(int,string,string,string,int);
	
	int getid(){return user_id;}
	string getusername(){return username;}
	string getfullname(){return fullname;}
	string getpassword(){return password;}
	int gettype(){return type;}
	virtual string getstatus(){return "";}
	virtual double getbalance(){return 0;}
	virtual string getfreebets(){return "";}
	virtual void addmoney(double m){}
	void setid(int i){user_id=i;}
	void setusername(string usr){username=usr;}
	void setfullname(string full){fullname=full;}
	void setpassword(string pass){password=pass;}
	virtual void setstatus(string st){}
	virtual void setbalance(double bal){}
	virtual void setfreebets(string free){}
	
	virtual void print_oper(int depth);
	
	virtual void operate(string s, int depth, vector<Users*> usersvector,HierarchyNode hierroot,HierarchyNode* currentnode, vector<int>& pathstack);
};



class Guest: public Users
{
public:
	Guest():Users(0,"guest","guest","guest",0){}
};

class Punter: public Users//1
{
	string status,freebets;
	double balance;
public:
	
	Punter(int,string,string,string,int,string,double,string);
	
	string getstatus(){return status;}
	double getbalance(){return balance;}
	string getfreebets(){return freebets;}
	
	void setstatus(string st){status=st;}
	void setbalance(double bal){balance=bal;}
	void setfreebets(string free){freebets=free;}
	
	void addmoney(double m){balance+=m;}
	
	void print_oper(int depth);
	
	void account(vector<Users*> usersvector);
	
	void operate(string s, int depth, vector<Users*> usersvector,HierarchyNode hierroot,HierarchyNode* currentnode, vector<int>& pathstack);
};

class Trader: public Users//2
{
public:
	
	Trader(int,string,string,string,int);
	
	void print_oper(int depth);
	
	void operate(string s, int depth, vector<Users*> usersvector,HierarchyNode hierroot,HierarchyNode* currentnode, vector<int>& pathstack);
	
};


class Director: public Users//3
{
public:
	
	Director(int,string,string,string,int);
	
	void print_oper(int depth);
	
	void users_management(vector<Users*> usersvector);
	
	void operate(string s, int depth, vector<Users*> usersvector,HierarchyNode hierroot,HierarchyNode* currentnode, vector<int>& pathstack);
};
