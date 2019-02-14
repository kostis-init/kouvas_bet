#include "Hierarchy.h"
#include "Users.h"
#include <iostream>
using namespace std;

bool searchColumnOfThisLine(string s,int j,string str);
string sgetstring(string s,int j,char delim);
string sgetstring_after(string s,int j,char delim);
void update_users_file(vector<Users*>);
void printlast20bets();
void givefreebets(vector<Users*> usersvector);
void update_hierarchy_file(HierarchyNode hierroot);
void printlast25logs();
void printuserinfo(Users*);
