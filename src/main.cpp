#include<iostream>
#include<string>
#include "interpreter.h"
#include "catalog.h"
using namespace std;
string current_db;

void initDb(){
	current_db = "default";
	if(!isDatabaseExist(current_db)){
		createDatabase(current_db);
	}
}


int main(){

	string query;//one query 
	string line;//accept one line input
	
	//init db
	initDb();
	

	cout<<"MiniSql>";//hint
	getline(cin,line);
	query = "";

	while(true){
		trim(line);
		int terminate_pos = line.find_first_of(';');
		if(terminate_pos==-1){
			query += " ";
			query += line;	
			cout<<"      ->";
			getline(cin,line);
		}else{
			query += line.substr(0,terminate_pos);//end one query
			line = line.substr(terminate_pos+1);
			//cout<<"This query:"<<query<<endl;
			//cout<<"Left:"<<line<<endl;
			trim(query);
			doOneQuery(query);
			if(line==""){//if end with '' means no more content left
				cout<<"MiniSql>";
				getline(cin,line);
			}
			query="";//reinitialize
		}

	}
	return 0;

}


