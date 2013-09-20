#include<iostream>
#include<vector>
#include<stdlib.h>
#include<string>
#include "table.h"
#include "interpreter.h"
using namespace std;

void doOneQuery(string query){

	int space_pos = query.find_first_of(' ');
	string first_word = query.substr(0,space_pos);
	string remaining = query.substr(space_pos+1);
	trim(remaining);
	if(first_word=="create"){
		createClause(remaining);
	}else if(first_word=="use"){
		useClause(remaining);
		cout<<"use clause"<<endl;
	}else if(first_word=="insert"){
		insertClause(remaining);
	}else if(first_word=="select"){
		selectClause(remaining);
	}else if(first_word=="delete"){
		deleteClause(remaining);
	}else if(first_word=="drop"){
		dropClause(remaining);
	}else if(first_word=="execfile"){

	}else if(first_word=="quit"){
		cout<<"Bye"<<endl;
		exit(0);

	}else if(first_word=="help"){
		cout<<"No help yet"<<endl;
	}else{
		cout<<"You have an error in your SQL syntax near "<<first_word<<endl;
	}

	return;
}

/***
  * create clause
  ***/
void createClause(string query){
	int space_pos = query.find_first_of(' ');
	string c_what = query.substr(0,space_pos);
	string remaining = query.substr(space_pos+1);
	trim(remaining);
	if(c_what=="database"){
		createDatabaseClause(remaining);
	}else if(c_what=="table"){
		createTableClause(remaining);
	}else if(c_what=="index"){

	}else{
		// undefined 
	}

}

/***
  * Create database clouse
  ***/
void createDatabaseClause(string query){
	if(query.find(' ')!=-1){
		// too many words for create database;
		cout<<"You have an error in your SQL syntax. Too many argument for create database: "<<query<<"."<<endl;
	}else{
		// create database name query	
		cout<<"Database created: "<<query<<"."<<endl;
	}
	return;

}

void createTableClause(string query){
	int left_bracket_pos = query.find_first_of('(');
	if(left_bracket_pos==-1){
		// no left bracket
		cout<<"You have an error in your SQL syntax."<<endl;
		cout<<" Expect '(' after tablename."<<endl;
		return ;
	}else{
		//
		if(left_bracket_pos==0){
			cout<<"You have an error in your SQL syntax."<<endl;
			cout<<"Table name can not be null."<<endl;
			return ;
		}

		string table_name = query.substr(0,left_bracket_pos);
		vector<attr> attrs;
		string primarykey_column="";

		trim(table_name);
		int right_bracket_pos = query.find_last_of(')');
		
		if(right_bracket_pos==-1){
			//no right bracket
			cout<<"You have an error in your SQL syntax."<<endl;
			cout<<" Expect ')'."<<endl;
			return ;

		}else{
			// create table tablename(create_definition);
			string create_definition = query.substr(left_bracket_pos+1,right_bracket_pos-left_bracket_pos-1);
			trim(create_definition);
			//cout<<create_definition<<endl;
			int comma_pos = create_definition.find_first_of(",");
			if(right_bracket_pos+1>=query.length()){
				// no content after right bracket, normal situation
				int attr_pos = 0;
				bool is_primarykey_defined = false;
				while(comma_pos!=-1 ){
					//parse each attribute: attr_name attr_type attr_constraint
					string attr_str = create_definition.substr(0,comma_pos);
					attr one_attr;
					try{
						one_attr = parseAttribute(attr_str,is_primarykey_defined,primarykey_column);
						attrs.push_back(one_attr);
					}catch(int msg){
						if(msg==-1)
							return ;
					}

					create_definition = create_definition.substr(comma_pos+1);
					trim(create_definition);
					comma_pos = create_definition.find_first_of(",");
					attr_pos ++;

				}
				
				//last attribute
				attr one_attr;
				try{
					one_attr = parseAttribute(create_definition,is_primarykey_defined,primarykey_column);
					attrs.push_back(one_attr);
				}catch(int msg){
					if(msg==-1)
						return ;
				}

				for(int i=0;i<attrs.size();i++){
					cout<<attrs[i].attr_name<<" "<<attrs[i].attr_type<<" "<<attrs[i].is_unique<<endl;
				}
				cout<<primarykey_column<<endl;


			}else{
				string remaining = query.substr(right_bracket_pos+1);
				trim(remaining);
				cout<<"You have an error in your SQL syntax near "<<remaining<<"."<<endl;
				cout<<" No more content expected after )."<<endl;
				return ;

			}
		}

	}

}


/***
  *parse each attribute: attr_name attr_type attr_constraint
***/
attr parseAttribute(string attr_string, bool &is_primarykey_defined, string &primarykey_column){
	trim(attr_string);
	if(attr_string.find("primary key")==0){
		// primary key 
		string column = attr_string.substr(11);//11 is length of primary key
		trim(column);
		int left_bracket_pos = column.find_first_of('(');
		if(left_bracket_pos==-1){
			// no left bracket
			cout<<"You have an error in your SQL syntax."<<endl;
			cout<<" Expect '(' after primary key."<<endl;
			throw -1;
		}else{
			int right_bracket_pos = column.find_last_of(')');

			if(right_bracket_pos==-1){
				//no right bracket
				cout<<"You have an error in your SQL syntax."<<endl;
				cout<<" Expect ')'."<<endl;
				throw -1;
			}else{
				// create table tablename(create_definition);
				string col_str = column.substr(left_bracket_pos+1,right_bracket_pos-left_bracket_pos-1);
				trim(col_str);
				if(right_bracket_pos+1<column.length()){
					cout<<"You have an error in your SQL syntax."<<endl;
					cout<<" No more content after ')'."<<endl;
					throw -1;
				}

				if(is_primarykey_defined){
					cout<<"You have an error in your SQL syntax."<<endl;
					cout<<"Multipule primary key defined."<<endl;
					throw -1;
				}
				primarykey_column = col_str;
				is_primarykey_defined = true;

			}
			throw 0;
		}

	}
	int s_pos = attr_string.find_first_of(" ");
	if(s_pos==-1){
		cout<<"You have an error in your SQL syntax near in attribute definition."<<endl;
		throw -1;
	}
	string attr_name = attr_string.substr(0,s_pos);
	string type_and_constraint = attr_string.substr(s_pos+1);
	trim(type_and_constraint);
	s_pos = type_and_constraint.find_first_of(" ");

	string type_str;
	string constraint="";
	if(s_pos==-1){
		// no constraint
		type_str = type_and_constraint;

	}else{
		// with constraint
		type_str = type_and_constraint.substr(0,s_pos);
		constraint = type_and_constraint.substr(s_pos+1);
		trim(constraint);

	}

	int attr_type = getType(type_str);
	bool unique = false;
	if(attr_type==-1){
		cout<<"You have an error in your SQL syntax near "<<type_str<<"."<<endl;
		cout<<"Unknown attribute type."<<endl;
		throw -1;
	}
	if(constraint=="primary key"){
		if(is_primarykey_defined){
			cout<<"You have an error in your SQL syntax."<<endl;
			cout<<"Multipule primary key defined."<<endl;
			throw -1;
		}
		primarykey_column = attr_name;
		is_primarykey_defined = true;

	}else if(constraint=="unique"){
		unique = true;
	}else if(constraint!=""){
		cout<<"You have an error in your SQL syntax near "<<constraint<<"."<<endl;
		cout<<"Unknown attribute constraint. Only 'primary key' and 'unique' support."<<endl;
		string aaa="hehe";
		throw -1;
	}

	struct attr one_attr(attr_name,attr_type,unique);
	return one_attr;

}

void useClause(string query){
	if(query.find(' ')!=-1){
		// too many words for use;
		cout<<"You have an error in your SQL syntax. Too many argument for use : "<<query<<"."<<endl;
		return ;
	}else if(query==""){
		// database name cannot be null
		cout<<"You have an error in your SQL syntax."<<endl;
		cout<<"Database name can not be null"<<endl;
		return ;
	}else{
		cout<<"Database changed: "<<query<<"."<<endl;
	}


	return ;
}

void insertClause(string query){

	return ;
}

void deleteClause(string query){

	return ;
}

void selectClause(string query){

	return ;
}

void dropClause(string query){
	int space_pos = query.find_first_of(' ');
	string c_what = query.substr(0,space_pos);
	string remaining = query.substr(space_pos+1);
	trim(remaining);
	if(c_what=="database"){
	//	createDatabaseClause(remaining);
	}else if(c_what=="table"){
	//	createTableClause(remaining);
	}else if(c_what=="index"){

	}else{
		// undefined 
	}
	return ;
}

/***
 * Delete extra space at head and tail 
 ***/
void trim(string &src){
	int s_pos = src.find_first_not_of(' ');
	int e_pos = src.find_last_not_of(' ');

	src = src.substr(s_pos,e_pos-s_pos+1);
	return;
}

/***
  * get type 
  * return 1 if type is int
  * return 2 if type is float
  * return n+2 if type is char(n)
  ***/
int getType(string type){

	if(type=="int"){
		return 1;
	}
	if(type=="float"){
		return 2;
	}

	if(type.find("char(")==0){


	}
	return -1;
}


