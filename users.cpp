#include <iostream>
#include <string>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <stdio.h>
#include <iomanip>
#include "functions.h"

using namespace std;


Users::Users(int id,string usr,string full,string pass,int t)
:user_id(id),username(usr),password(pass),fullname(full),type(t)
{}
Punter::Punter(int id,string usr,string full,string pass,int t,string st,double bal,string free)
	:Users(id,usr,full,pass,t),status(st),balance(bal),freebets(free)
{}
Trader::Trader(int id,string usr,string full,string pass,int t):Users(id,usr,full,pass,t)
{}
Director::Director(int id,string usr,string full,string pass,int t):Users(id,usr,full,pass,t)
{}

void Director::users_management(vector<Users*> usersvector)
{
	string s;
	while(1)
	{
		cout<<"------------------------------------------"<<endl;
		cout<<"             USERS MANAGEMENT             "<<endl;
		cout<<"\nChoose\nV(View), to view users table\nS(Search), to search for user\nL(Lock), to toggle (un)blocked users"<<endl;
		cout<<"X(Exit), to exit\nType your choice:";
		getline(cin,s);
		if (s=="X" || s=="Exit")
			break;
		else if (s=="V" || s=="View")
		{
			cout<<"----------------------------------------------------------------------------------------------------------------------------------"<<endl;
			cout<<"|id|username         |fullname                  |password         |type|status                        |balance   |freebets       |"<<endl;
			for (int i = 0; i < usersvector.size(); i++)
			{
				printuserinfo(usersvector[i]);
			}
			cout<<"----------------------------------------------------------------------------------------------------------------------------------"<<endl;
		}
		else if (s=="S" || s=="Search")
		{
			cout<<"Type the username you want to find or a substring of this:";
			getline(cin,s);
			cout<<"----------------------------------------------------------------------------------------------------------------------------------"<<endl;
			cout<<"|id|username         |fullname                  |password         |type|status                        |balance   |freebets       |"<<endl;
			for (int i = 0; i < usersvector.size(); i++)
				if(usersvector[i]->getusername().find(s)!=s.npos)
					printuserinfo(usersvector[i]);
			cout<<"----------------------------------------------------------------------------------------------------------------------------------"<<endl;
		}
		else if (s=="L" || s=="Lock")
		{
			cout<<"Type the username you want to (un)lock:";
			getline(cin,s);
			int found=-1;
			for (int i = 0; i < usersvector.size(); i++)
				if(usersvector[i]->getusername().find(s)!=s.npos)
					found=i;
			if(found==-1)cout<<"no such username"<<endl;
			else if(usersvector[found]->gettype()!=1)cout<<"(un)lock only punters!"<<endl;
			else
			{
				if (usersvector[found]->getstatus()=="A")
				{
					cout<<"You will lock this user, type a comment:";
					stringstream ss;
					ss<<"L,";
					getline(cin,s);
					if(s=="")ss<<"N/A";
					else ss<<s;
					usersvector[found]->setstatus(ss.str());
				}
				else
				{
					cout<<"You will unlock this user"<<endl;
					usersvector[found]->setstatus("A");
				}
				update_users_file(usersvector);
			}
		}
		else
			cout<<"NO SUCH OPERATION"<<endl;
	}
	
}







	void Punter::account(vector<Users*> usersvector)
	{
		ofstream logs("audit.log",ofstream::app);
		logs.seekp(0,ios::end);
		while(1)
		{
			
			string s;
			cout<<"--------------------------------------------"<<endl;
			cout<<"               ACCOUNT INFO                 "<<endl;
			cout<<"Punter:"<<getusername()<<"\nBalance:"<<getbalance()<<"\nFreebets:"<<getfreebets()<<endl;
			cout<<"\nChoose\nR(Rename), to change your username\nP(Password), to change your password\nB(Bets), to show your bet history"<<endl;
			cout<<"D(Deposit), to add money in your account\nX(Exit), to return at your previous location\nType your choice:";
			getline(cin,s);
		
			if (s=="X" || s=="Exit")break;
			else if (s=="R" || s=="Rename")
			{
				cout<<"Give your new username:";
				getline(cin,s);
				bool available=1;
				for (int i = 0; i < usersvector.size(); i++)
					if (usersvector[i]->getusername()==s)//not available
						available=0;
				if(available==0 || s=="guest")
					cout<<"username not available"<<endl;
				else
				{
					logs<<"user with id:"<<getid()<<" renamed himself from "<<getusername()<<" to "<<s<<endl;
					setusername(s);
					update_users_file(usersvector);
				}
				
			}
			else if (s=="P" || s=="Password")
			{
				while(1)
				{
					string s1;
					cout<<"Type your new password:";
					getline(cin,s);
					cout<<"Type again your new password:";
					getline(cin,s1);
					if(s.compare(s1)==0)
					{
						setpassword(s);
						update_users_file(usersvector);
						cout<<"Password updated"<<endl;
						logs<<"user with id:"<<getid()<<" changed his password"<<endl;
						break;
					}
					else
						cout<<"Passwords are different, type again"<<endl;
				}
			}
			else if (s=="B" || s=="Bets")
			{
				int counter=1;
				ifstream bets("bets.csv");
				cout<<"--------------------------------------"<<endl;
				cout<<"|bet_id|node_id     |stake   |result |"<<endl;
				while(getline(bets,s))
				{
					if (atoi(sgetstring(s,1,'|').c_str())==getid())
					{
						cout<<"|"<<left<<setw(6)<<setfill(' ')<<counter<<"|";
						cout<<left<<setw(12)<<setfill(' ')<<sgetstring(s,2,'|')<<"|";
						cout<<left<<setw(8)<<setfill(' ')<<sgetstring(s,3,'|')<<"|";
						if(sgetstring(s,4,'|').compare("W")==0)cout<<left<<setw(7)<<setfill(' ')<<"Won"<<"|"<<endl;
						else if(sgetstring(s,4,'|').compare("L")==0)cout<<left<<setw(7)<<setfill(' ')<<"Lost"<<"|"<<endl;
						else if(sgetstring(s,4,'|').compare("V")==0)cout<<left<<setw(7)<<setfill(' ')<<"Voided"<<"|"<<endl;
						else cout<<left<<setw(7)<<setfill(' ')<<"Pending"<<"|"<<endl;
						counter++;
					}
				}
				cout<<"--------------------------------------"<<endl;
				bets.close();
				logs<<"user with id:"<<getid()<<" saw his bets history"<<endl;
			}
			else if (s=="D" || s=="Deposit")
			{
				cout<<"Type the amount of money you want to add:";
				getline(cin,s);
				if(atof(s.c_str())>0)
				{
					addmoney(atof(s.c_str()));
					update_users_file(usersvector);
					cout<<"Money added"<<endl;
					logs<<"user with id:"<<getid()<<" did a deposit ("<<atof(s.c_str())<<")"<<endl;
				}
				else
					cout<<"Give proper amount"<<endl;
			}
			else
				cout<<"NO SUCH OPERATION"<<endl;
		}
		cout<<"--------------------------------------------"<<endl;
		logs.close();
	}
	
	///////OPERATE
	void Users::operate(string s, int depth, vector<Users*> usersvector,HierarchyNode hierroot,HierarchyNode* currentnode, vector<int>& pathstack)//only for guests because it is virtual
	{
		cout<<"NO SUCH OPERATION"<<endl;
	}
	void Punter::operate(string s, int depth, vector<Users*> usersvector,HierarchyNode hierroot,HierarchyNode* currentnode, vector<int>& pathstack)
	{
		if(s=="A" || s=="Account")
			account(usersvector);
		else if (depth==4 && (s=="P" || s=="Place"))
		{
			ofstream log("audit.log",ofstream::app);
			cout<<"Your balance is "<<getbalance()<<". Type your selection, or cancel to cancel:";
			getline(cin,s);
			if(s=="cancel")cout<<"Canceled"<<endl;
			else if(atoi(s.c_str())>=1 && atoi(s.c_str())<=currentnode->getvectorsize())
			{
				int sel=atoi(s.c_str());
				cout<<"Your freebets are:"<<endl;
				int i=0;
				while(sgetstring(getfreebets(),i,',')!="")
				{
					char c='a'+i;
					cout<<c<<". "<<sgetstring(getfreebets(),i,',')<<endl;
					i++;
				}
				
				cout<<"Type a letter to use a freebet or a number to use from your balance:";
				getline(cin,s);
				if(atof(s.c_str())>0)//balance
				{
					while(1)
					{
						if(getbalance()==0)
						{
							cout<<"deposit money in your acocunt please"<<endl;
							break;
						}
						double amount=atof(s.c_str());
						if(getbalance()-amount<0)
						{
							log<<"Punter with username:"<<getusername()<<" tried to place a bet but he had not enough money LOL"<<endl;
							cout<<"not enough money"<<endl;
							
							cout<<"Type a smaller amount:";
							getline(cin,s);
						}
						else
						{
							addmoney(amount*(-1));
							ifstream bets("bets.csv");
							int counter=0;//count lines
							while(getline(bets,s))counter++;
							bets.close();
							ofstream bets1("bets.csv", ofstream::app);
							bets1<<counter<<"|"<<getid()<<"|";
							for (int i = 0; i < pathstack.size(); i++)
								bets1<<pathstack[i]+1<<".";
							bets1<<sel;
							bets1<<"|"<<amount<<"|-"<<endl;
							bets1.close();
							update_users_file(usersvector);
							log<<"Punter with username:"<<getusername()<<" placed a bet"<<endl;
							
							break;
						}
					}
				}
			}
			else cout<<"No such node"<<endl;
			log.close();
		}
		else cout<<"NO SUCH OPERATION"<<endl;
	}
	void Trader::operate(string s, int depth, vector<Users*> usersvector,HierarchyNode hierroot,HierarchyNode* currentnode, vector<int>& pathstack)
	{
		if(s=="B" || s=="Bets")
			printlast20bets();
		else if (s=="F" || s=="Freebets")
			givefreebets(usersvector);
		else if (depth==4 && (s=="V" || s=="Void"))
		{
			
			//void
		}
		else if (depth==4 && (s=="S" || s=="Settle"))
		{
			int sel;
			cout<<"Type the id that won:";
			getline(cin,s);
			sel=atoi(s.c_str());
			for (int i = 0; i < currentnode->getvectorsize(); i++)
				currentnode->getnextnode(i)->setstate('L');
			currentnode->getnextnode(sel-1)->setstate('W');
			
			//give to user
			stringstream ss;
			for (int i = 0; i < pathstack.size(); i++)
				ss<<pathstack[i]+1<<".";
			ss<<sel;
			ifstream bets("bets.csv");
			while(getline(bets,s))
				if(ss.str()==sgetstring(s,2,'|') && sgetstring(s,4,'|')=="-")//searching for bets at this selection
					for (int j = 0;j < usersvector.size(); j++)
						if (usersvector[j]->getid()==atoi(sgetstring(s,1,'|').c_str()))//give him money
							usersvector[j]->addmoney(atof(sgetstring(s,3,'|').c_str())*currentnode->getnextnode(sel-1)->getpr());
			bets.close();
			update_users_file(usersvector);
			
			//update bets file
		}
		else cout<<"NO SUCH OPERATION"<<endl;
		
	}
	void Director::operate(string s, int depth, vector<Users*> usersvector,HierarchyNode hierroot,HierarchyNode* currentnode, vector<int>& pathstack)
	{
		if(s=="B" || s=="Bets")
			printlast20bets();
		else if (s=="F" || s=="Freebets")
			givefreebets(usersvector);
		else if (s=="S" || s=="Save")
		{
			update_users_file(usersvector);
			update_hierarchy_file(hierroot);
			cout<<"Saved succesfully"<<endl;
		}
		else if (s=="L" || s=="Logs")
			printlast25logs();
		else if (s=="U" || s=="Users")
			users_management(usersvector);
		else if (s=="V" || s=="Visibility")
		{
			cout<<"Type number of node you want to change visibility:";
			getline(cin,s);
			if(currentnode->getnextnode(atoi(s.c_str())-1)==0)
				cout<<"NO SUCH NODE"<<endl;
			else
				currentnode->getnextnode(atoi(s.c_str())-1)->togglevisible();
		}
		else if (s=="R" || s=="Rename")
		{
			currentnode->print_level_dir();
			cout<<"Type the node's id you want to rename, or cancel/abort to cancel:";
			getline(cin,s);
			int id=atoi(s.c_str());
			if(id>=1 && id<=currentnode->getvectorsize())
			{
				cout<<"Type the the new name, or cancel/abort to cancel:";
				getline(cin,s);
				if(!boost::iequals(s, "cancel")&&!boost::iequals(s, "abort")&&!boost::iequals(s, "c")&&!boost::iequals(s, "a"))
				{
					currentnode->getnextnode(id-1)->setname(s);
					update_hierarchy_file(hierroot);
					cout<<"New name set succesfully"<<endl;
				}
				else cout<<"Canceled"<<endl;
			}
			else if(!boost::iequals(s, "cancel")&&!boost::iequals(s, "abort")&&!boost::iequals(s, "c")&&!boost::iequals(s, "a")) cout<<"No such node"<<endl;
			else cout<<"Canceled"<<endl;
		}
		else if (s=="N" || s=="New")
		{
			cout<<"Type the name of the new node, or cancel/abort to cancel:";
			getline(cin,s);
			if(!boost::iequals(s, "cancel")&&!boost::iequals(s, "abort")&&!boost::iequals(s, "c")&&!boost::iequals(s, "a"))
			{
				currentnode->newnode(s,1);
				update_hierarchy_file(hierroot);
				cout<<"New node added succesfully"<<endl;
			}
			else cout<<"Canceled"<<endl;
		}
		else if (s=="C" || s=="Copy")/////////PROBLEMS
		{
			cout<<"Type the node's id you want to copy, or cancel/abort to cancel:";
			getline(cin,s);
			int id=atoi(s.c_str());
			if(id>=1 && id<=currentnode->getvectorsize())
			{
				stringstream ss;
				ss<<currentnode->getnextnode(id-1)->getname()<<"-Copied";
				
				//create temp file for sub-hierarchy
				ofstream temp("temp.dat", ios::out | ios::trunc);
				ifstream hier("hierarchy.dat");
				while(getline(hier,s))if(currentnode->getnextnode(id-1)->getname()==sgetstring(s,1,' '))break;
				while(getline(hier,s))
					temp<<s<<endl;
				
				temp.close();
				hier.close();
				
				fstream temp1("temp.dat");
				currentnode->copynewnode(temp1,ss.str());
				temp1.close();
				//update hierarchy
				
				update_hierarchy_file(hierroot);
				cout<<"New node copied succesfully"<<endl;
			}
			else if(!boost::iequals(s, "cancel")&&!boost::iequals(s, "abort")&&!boost::iequals(s, "c")&&!boost::iequals(s, "a")) cout<<"No such node"<<endl;
			else cout<<"Canceled"<<endl;
		}
		else if (s=="D" || s=="Delete")
		{
			cout<<"Type the node's id you want to delete, or cancel/abort to cancel:";
			getline(cin,s);
			int id=atoi(s.c_str());
			if(id>=1 && id<=currentnode->getvectorsize())
			{
				cout<<"Are you sure? Y/N:";
				getline(cin,s);
				if(s=="Y")
				{
					currentnode->erase(id-1);
					update_hierarchy_file(hierroot);
					cout<<"Deleted succesfully"<<endl;
				}
				else cout<<"Canceled"<<endl;
			}
			else if(!boost::iequals(s, "cancel")&&!boost::iequals(s, "abort")&&!boost::iequals(s, "c")&&!boost::iequals(s, "a")) cout<<"No such node"<<endl;
			else cout<<"Canceled"<<endl;
		}
		else cout<<"NO SUCH OPERATION"<<endl;
		
	}
	
	
	///////PRINT OPERATIONS
	void Users::print_oper(int depth)
	{
		cout<<"H(Home), for return to home"<<endl;
		cout<<"T(Toggle), to view odds in ";if(decimalodds)cout<<"fractional type\n";else cout<<"decimal type\n";
		cout<<"X(Exit), to exit from the system"<<endl;
	}
	void Punter::print_oper(int depth)
	{
		cout<<"H(Home), for return to home"<<endl;
		cout<<"T(Toggle), to view odds in ";if(decimalodds)cout<<"fractional type\n";else cout<<"decimal type\n";
		cout<<"X(Exit), to exit from the system"<<endl;
		cout<<"A(Account), to manage your account's information"<<endl;
		if(depth==4)cout<<"P(Place), to place a bet"<<endl;
	}
	void Trader::print_oper(int depth)
	{
		cout<<"H(Home), for return to home"<<endl;
		cout<<"T(Toggle), to view odds in ";if(decimalodds)cout<<"fractional type\n";else cout<<"decimal type\n";
		cout<<"X(Exit), to exit from the system"<<endl;
		cout<<"B(Bets), to show the 20 last bets"<<endl;
		cout<<"F(Freebets), to give a freebet to a player"<<endl;
		if(depth==4)cout<<"V(Void), to cancel a selection"<<endl<<"S(Settle), to settle the market"<<endl;
	}
	void Director::print_oper(int depth)
	{
		cout<<"H(Home), for return to home"<<endl;
		cout<<"T(Toggle), to view odds in ";if(decimalodds)cout<<"fractional type\n";else cout<<"decimal type\n";
		cout<<"X(Exit), to exit from the system"<<endl;
		cout<<"B(Bets), to show the 20 last bets"<<endl;
		cout<<"F(Freebets), to give a freebet to a player"<<endl;
		cout<<"S(Save), to save and synchronize the files"<<endl;
		cout<<endl<<"Management:"<<endl;
		cout<<"L(Logs), to show the last 25 actions in the system"<<endl;
		cout<<"U(Users), to manage users\nV(Visibility), to change visibility of nodes"<<endl;
		cout<<"R(Rename), to rename node\nN(New), to create a node\nC(Copy), to copy a node (as non-visible)"<<endl;
		cout<<"D(Delete), to delete a node"<<endl;
	}
