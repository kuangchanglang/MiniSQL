#include<iostream>
#include<vector>
#include<set>
#include<stdlib.h>
#include<string>
#include "table.h"
#include "interpreter.h"
#include "catalog.h"
using namespace std;

void doOneQuery(string query){

	int space_pos = query.find_first_of(' ');
	string first_word = "";	
	string remaining = "";
	if(space_pos!=-1||query!=""){
		first_word = query.substr(0,space_pos);
		remaining = query.substr(space_pos+1);
		trim(remaining);
	}
	if(first_word=="create"){
		createClause(remaining);
	}else if(first_word=="use"){
		useClause(remaining);
	}else if(first_word=="insert"){
		insertClause(remaining);
	}else if(first_word=="select"){
		selectClause(remaining);
	}else if(first_word=="delete"){
		deleteClause(remaining);
	}else if(first_word=="drop"){
		dropClause(remaining);
	}else if(first_word=="execfile"){
		execFileClause(remaining);
	}else if(first_word=="quit"){
		cout<<"Bye"<<endl;
		exit(0);
	}else if(first_word=="help"){
		cout<<"No help yet."<<endl;
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
	if(space_pos==-1){
		cout<<"You have an error in your SQL syntax near "<<query<<endl;
		cout<<"Too few parameter."<<endl;
		return ;
	}
	string c_what = query.substr(0,space_pos);
	string remaining = query.substr(space_pos+1);
	trim(remaining);
	if(c_what=="database"){
		createDatabaseClause(remaining);
	}else if(c_what=="table"){
		createTableClause(remaining);
	}else if(c_what=="index"){
		createIndexClause(remaining);
	}else{
		// undefined 
		cout<<"You have an error in your SQL syntax near "<<c_what<<endl;
		return;
	}

}

/***
  * Create database clause
  ***/
void createDatabaseClause(string query){
	if(!isOneWord(query)){
		// too many words for create database;
		cout<<"You have an error in your SQL syntax near "<<query<<"."<<endl;
		cout<<"Usage: create database dbname."<<endl;
	}else{
		// create database name query	
		if(createDatabase(query))
			cout<<"Database created: "<<query<<"."<<endl;
		else
			cout<<"Database already exist: "<<query<<endl;
	}
	return ;

}

/***
  * Create index clause
  ***/
void createIndexClause(string query){
	int s_pos = query.find_first_of(' ');
	if(s_pos==-1){
		cout<<"You have an error in your SQL syntax near "<<query<<endl;
		return;
	}
	string idxname = query.substr(0,s_pos);
	//cut idxname
	query = query.substr(s_pos+1);
	trim(query);
	int on_pos = query.find("on ");
	if(on_pos==query.npos){
		cout<<"You have an error in your SQL syntax near "<<query<<endl;
		cout<<"Usage create index idxname on tablename (attr)."<<endl;
		return;
	}
	query = query.substr(on_pos+3);
	trim(query);
	int left_bracket_pos = query.find_first_of('(');
	if(left_bracket_pos==-1){
		// no left bracket
		cout<<"You have an error in your SQL syntax."<<endl;
		cout<<"Expect '(' after tablename."<<endl;
		return ;
	}
	string tname = query.substr(0,left_bracket_pos);
	if(tname==""){
		cout<<"You have an error in your SQL syntax."<<endl;
		cout<<"Expect tablename."<<endl;
		return ;
	}
	int right_bracket_pos = query.find_last_of(')');
	if(right_bracket_pos==-1 || right_bracket_pos!=query.length()-1){
		cout<<"You have an error in your SQL syntax near."<<query<<endl;
		return ;
	}
	string attr_name = query.substr(left_bracket_pos+1,right_bracket_pos-left_bracket_pos-1);
	return ;
}

void createTableClause(string query){
	int left_bracket_pos = query.find_first_of('(');
	if(left_bracket_pos==-1){
		// no left bracket
		cout<<"You have an error in your SQL syntax."<<endl;
		cout<<"Expect '(' after tablename."<<endl;
		return ;
	}else{
		//
		if(left_bracket_pos==0){
			cout<<"You have an error in your SQL syntax."<<endl;
			cout<<"Table name can not be null."<<endl;
			return ;
		}

		string table_name = query.substr(0,left_bracket_pos);
		vector<Attr> attrs;
		string primarykey_column="";

		trim(table_name);
		int right_bracket_pos = query.find_last_of(')');
		
		if(right_bracket_pos==-1){
			//no right bracket
			cout<<"You have an error in your SQL syntax."<<endl;
			cout<<"Expect ')'."<<endl;
			return ;

		}
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
				Attr one_attr;
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
			Attr one_attr;
			try{
				one_attr = parseAttribute(create_definition,is_primarykey_defined,primarykey_column);
				attrs.push_back(one_attr);
			}catch(int msg){
				if(msg==-1)
					return ;
			}

			if(containsDuplicate(attrs)){
				cout<<"Duplicate attribute name."<<endl;
				return ;
			}

			if(attrs.size()>22){
				cout<<"Attribute num can not exceed 22."<<endl;
				return ;
			}
			//everything ok!
			Table table(table_name,attrs,primarykey_column);
			if(createTable(CURRENT_DB,table)){
				cout<<"Table created: "<<table_name<<endl;
			}else{
				cout<<"Table create failed!"<<endl;
			}

		}else{
			string remaining = query.substr(right_bracket_pos+1);
			trim(remaining);
			cout<<"You have an error in your SQL syntax near "<<remaining<<"."<<endl;
			cout<<" No more content expected after )."<<endl;
			return ;

		}
	}


}


/***
 *parse each attribute: attr_name attr_type attr_constraint
 ***/
Attr parseAttribute(string attr_string, bool &is_primarykey_defined, string &primarykey_column){
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

	struct Attr one_attr(attr_name,attr_type,unique);
	return one_attr;

}

/***
  * use clause
  ***/
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
		if(isDatabaseExist(query)){
			CURRENT_DB = query;
			cout<<"Database changed: "<<query<<"."<<endl;
		}else{
			cout<<"Database not exist: '"<<query<<"'."<<endl;
		}
	}


	return ;
}

/*=======================insert clause===========================*/
void insertClause(string query){
	//get word into
	int s_pos = query.find_first_of(' ');
	if(s_pos==-1){
		cout<<"You have an error in your SQL syntax."<<endl;
		cout<<"Usage: insert into tablename values(...);"<<endl;
		return;
	}
	string into = query.substr(0,s_pos);
	if(into!="into"){
		cout<<"You have an error in your SQL syntax."<<endl;
		cout<<"Usage: insert into tablename values(...);"<<endl;
		return;
	}
	//cut query after into
	query = query.substr(s_pos+1);
	trim(query);

	//get word table name
	s_pos = query.find_first_of(' ');
	if(s_pos==-1){
		cout<<"You have an error in your SQL syntax."<<endl;
		cout<<"Usage: insert into tablename values(...);"<<endl;
		return;
	}
	string tname = query.substr(0,s_pos);
	query = query.substr(s_pos+1);
	trim(query);

	//get word values
	int left_bracket_pos = query.find_first_of('(');
	if(left_bracket_pos==-1){
		cout<<"You have an error in your SQL syntax."<<endl;
		cout<<"Usage: insert into tablename values(...);"<<endl;
		return;
	}
	string values_str = query.substr(0,left_bracket_pos);
	if(values_str!="values"){
		cout<<"You have an error in your SQL syntax."<<endl;
		cout<<"Usage: insert into tablename values(...);"<<endl;
		return;
	}

	int right_bracket_pos = query.find_last_of(')');
	if(right_bracket_pos==-1||right_bracket_pos!=query.length()-1){
		cout<<"You have an error in your SQL syntax."<<endl;
		cout<<"Expect ')'."<<endl;
		return;
	}

	string content = query.substr(left_bracket_pos+1,right_bracket_pos-left_bracket_pos-1);
	trim(content);
	//add values into vector
	vector<string> values;
	int comma_pos = content.find_first_of(',');
	while(comma_pos!=-1){
		string one_value = content.substr(0,comma_pos);
		trim(one_value);
		values.push_back(one_value);
		
		content = content.substr(comma_pos+1);
		trim(content);
		comma_pos = content.find_first_of(',');
	}
	values.push_back(content);

	if(!isTableExist(CURRENT_DB,tname)){
		cout<<"Table not exist : "<<tname<<endl;
		return ;
	}
	Table table = getTable(CURRENT_DB,tname);
	if(values.size()!=table.attr_num){
		cout<<"Attribute num does not match."<<endl;
		return ;
	}
	for(int i=0;i<table.attr_num;i++){
		if(!checkValueMatchType(table.attrs[i].attr_type,values[i])){
			cout<<"Type does not match for attribute:"<<table.attrs[i].attr_name<<endl;
			return ;
		}
	}

	return ;
}
/*===================end of insert=======================*/

/*===================delete clause=======================*/
void deleteClause(string query){
	int s_pos = query.find_first_of(' ');
	if(s_pos==-1){
		cout<<"you have an error in your sql syntax."<<endl;
		cout<<"usage: delete from tablename [where] [condition]."<<endl;
		return ;
	}
	//get word "from"
	string from = query.substr(0,s_pos);
	if(from!="from"){
		cout<<"You have an error in your SQL syntax."<<endl;
		cout<<"Usage: delete from tablename [where] [condition]."<<endl;
		return ;
	}
    query = query.substr(s_pos+1);
	trim(query);
	//get table name
	s_pos = query.find_first_of(' ');
	if(s_pos==-1){
		cout<<"you have an error in your sql syntax."<<endl;
		cout<<"usage: delete from tablename [where] [condition]."<<endl;
		return ;
	}
	string tname = query.substr(0,s_pos);
    query = query.substr(s_pos+1);
	trim(query);
	vector<Condition> conditions;
	if(query!=""){
		//get word where
		s_pos = query.find_first_of(' ');
		string where = query.substr(0,s_pos);
		if(where!="where"){
			cout<<"you have an error in your sql syntax near"<<where<<endl;
			cout<<"Expect where"<<endl;
			return ;
		}
		query = query.substr(s_pos+1);//get content after where
		trim(query);
		try{
			conditions = getConditions(query);
		}catch(int msg){
			if(msg==-1){
				cout<<"you have an error in your sql syntax in where condition"<<endl;
				return ;
			}
		}

	}
	return ;
}

/***
  * get condtions from string "xxx op value1 and yyy op value2..." 
  ***/
vector<Condition> getConditions(string cond_str){
	cout<<cond_str<<endl;
	vector<Condition> conditions;
	int and_pos = cond_str.find(" and ");
	while(and_pos!=cond_str.npos){

		string one_cond_str = cond_str.substr(0,and_pos);
		Condition one_condition = getCondition(one_cond_str);	
		conditions.push_back(one_condition);
		cond_str = cond_str.substr(and_pos+5);//3 is length of and
		trim(cond_str);
		and_pos = cond_str.find(" and ");
	}
	Condition one_condition = getCondition(cond_str);	
	conditions.push_back(one_condition);
	return conditions;
}

/***
  * input string 'attr op value'
  * return struct condition if op is legal
  * otherwise throw exception
  ***/
Condition getCondition(string one_cond_str){
	cout<<one_cond_str<<endl;
	trim(one_cond_str);
	// get attr
	string attr;
	string value;
	int op;
	int op_pos = one_cond_str.find("<>");
	if(op_pos!=one_cond_str.npos){
		attr = one_cond_str.substr(0,op_pos);
		trim(attr);
		if(!isOneWord(attr))
			throw -1;
		op = 1;
		value = one_cond_str.substr(op_pos+2);//2 is length of <>
		trim(value);
		if(!isOneWord(value))
			throw -1;
		Condition one_condition(attr,op,value);
		return one_condition;
	}
	op_pos = one_cond_str.find("<=");
	if(op_pos!=one_cond_str.npos){
		attr = one_cond_str.substr(0,op_pos);
		trim(attr);
		if(!isOneWord(attr))
			throw -1;
		op = 4;
		value = one_cond_str.substr(op_pos+2);//2 is length of <=
		trim(value);
		if(!isOneWord(value))
			throw -1;
		Condition one_condition(attr,op,value);
		return one_condition;
	}
	op_pos = one_cond_str.find(">=");
	if(op_pos!=one_cond_str.npos){
		attr = one_cond_str.substr(0,op_pos);
		trim(attr);
		if(!isOneWord(attr))
			throw -1;
		op = 5;
		value = one_cond_str.substr(op_pos+2);//2 is length of <=
		trim(value);
		if(!isOneWord(value))
			throw -1;
		Condition one_condition(attr,op,value);
		return one_condition;
	}
	op_pos = one_cond_str.find_first_of("<");
	if(op_pos!=-1){
		attr = one_cond_str.substr(0,op_pos);
		trim(attr);
		if(!isOneWord(attr))
			throw -1;
		op = 2;
		value = one_cond_str.substr(op_pos+1);
		trim(value);
		if(!isOneWord(value))
			throw -1;
		Condition one_condition(attr,op,value);
		return one_condition;
	}
	op_pos = one_cond_str.find_first_of(">");
	if(op_pos!=-1){
		attr = one_cond_str.substr(0,op_pos);
		trim(attr);
		if(!isOneWord(attr))
			throw -1;
		op = 3;
		value = one_cond_str.substr(op_pos+1);
		trim(value);
		if(!isOneWord(value))
			throw -1;
		Condition one_condition(attr,op,value);
		return one_condition;
	}
	op_pos = one_cond_str.find_first_of("=");
	if(op_pos!=-1){
		attr = one_cond_str.substr(0,op_pos);
		trim(attr);
		if(!isOneWord(attr))
			throw -1;
		op = 0;
		value = one_cond_str.substr(op_pos+1);
		trim(value);
		if(!isOneWord(value))
			throw -1;
		Condition one_condition(attr,op,value);
		return one_condition;
	}

}

void selectClause(string query){
	int from_pos = query.find(" from ");
	if(from_pos == query.npos){
		cout<<"You have an error in your SQL syntax."<<endl;
		cout<<"Expect from in select sql."<<endl;
		return ;
	}
	string content = query.substr(0,from_pos);
	trim(content);
	vector<string> attrs;
	bool all_attr = false;
	if(content=="*")
	{
		all_attr = true;
	}else{
		int comma_pos = content.find_first_of(',');
		while(comma_pos!=-1){
			string one_value = content.substr(0,comma_pos);
			trim(one_value);
			if(!isOneWord){
				cout<<"You have an error in your SQL syntax near "<<one_value<<endl;
				cout<<"Usage: select attr1,attr2,... from tname where conditions"<<endl;
				return ;
			}
			attrs.push_back(one_value);

			content = content.substr(comma_pos+1);
			trim(content);
			comma_pos = content.find_first_of(',');
		}
		attrs.push_back(content);

	}

	query = query.substr(from_pos+6);//6 is length of ' from '
	trim(query);
	string tname;
	int s_pos = query.find_first_of(' ');
	if(s_pos==-1){
		if(query==""){	
			cout<<"You have an error in your SQL syntax."<<endl;
			cout<<"Expect table name after from."<<endl;
			return ;
		}else{
			tname = query;
		}
	}else{
		tname = query.substr(0,s_pos);
		// cut table name
		query = query.substr(s_pos+1);
		trim(query);

		vector<Condition> conditions;
		if(query!=""){
			//get word where
			s_pos = query.find_first_of(' ');
			string where = query.substr(0,s_pos);
			if(where!="where"){
				cout<<"you have an error in your sql syntax near"<<where<<endl;
				cout<<"Expect where"<<endl;
				return ;
			}
			query = query.substr(s_pos+1);//get content after where
			trim(query);
			try{
				conditions = getConditions(query);
			}catch(int msg){
				if(msg==-1){
					cout<<"you have an error in your sql syntax in where condition"<<endl;
					return ;
				}
			}

		}


	}
	return ;
}

/*=================Drop clause=================*/
void dropClause(string query){
	int space_pos = query.find_first_of(' ');
	string d_what = query.substr(0,space_pos);
	string remaining = query.substr(space_pos+1);
	trim(remaining);
	if(d_what=="database"){
		dropDatabaseClause(remaining);
	}else if(d_what=="table"){
		dropTableClause(remaining);
	}else if(d_what=="index"){
		dropIndexClause(remaining);
	}else{
		// undefined 
		cout<<"You have an error in your SQL syntax near "<<d_what<<endl;
		cout<<"Unkown key word."<<endl;
	}
	return ;
}

void dropDatabaseClause(string dbname){
	if(dbname.find(" ")!=dbname.npos){
		cout<<"You have an error in your SQL syntax. Too many argument for drop database:"<<dbname<<"."<<endl; 
		return;
	}
	//drop database;
}

void dropTableClause(string tname){
	if(tname.find(" ")!=tname.npos){
		cout<<"You have an error in your SQL syntax. Too many argument for drop table:"<<tname<<"."<<endl; 
		return;
	}
	//drop table;
}
void dropIndexClause(string iname){
	if(iname.find(" ")!=iname.npos){
		cout<<"You have an error in your SQL syntax. Too many argument for drop index:"<<iname<<"."<<endl; 
		return;
	}
	//drop table;

}
/*=================End of Drop clause==================*/

void execFileClause(string query){
	
}


/***
  * check if value of string match type
  * type 1 means int
  * type 2 means float
  * type n>2 means char(t) where t=n-2
  
  */
bool checkValueMatchType(int type, string value){
	if(type==1){
		return isInt(value);		
	}else if(type==2){
		return isFloat(value);
	}else if(type>2){
		int size = value.size();
		if(size!=type-2){//'value'
			return false;
		}
		if(value[0]!='\'' || value[size-1]!='\'')
			return false ;
		return true;
	}else{
		return false;
	}

}


/***
  * check if attrs contains duplicate attrname
  ***/
bool containsDuplicate(vector<Attr> attrs){
	set<string> attr_names;
	for(int i=0;i<attrs.size();i++){
		string attr_name = attrs[i].attr_name;
		if(attr_names.find(attr_name)!=attr_names.end()){
			//this attr name already exist. 
			return true;
		}else{
			attr_names.insert(attr_name);
		}
	}

	return false;
}

/*=================Utils functions=====================*/
/***
 * Delete extra space at head and tail 
 ***/
void trim(string &src){
	if(src=="")
		return;
	int s_pos = src.find_first_not_of(' ');
	if(s_pos==-1)
		return ;
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
		int right_bracket_pos = type.find_last_of(')');

		if(right_bracket_pos==-1||right_bracket_pos!=type.length()-1)
			return -1;
		string content = type.substr(5,right_bracket_pos-5);// 5 is length of char(
		trim(content);
		int t = toInt(content);
		if(t<=0 ||t>255)
			return -1;
		return t+2;
	}
	return -1;
}

/***
  *return -1 if this is not a positive interger
  ***/
int toInt(string value){
	int length = value.length();
	const char *ch = value.c_str();

	int result = 0;
	for(int i=0;i<length;i++){
		if(ch[i]<'0' || ch[i]>'9')
			return -1;
		result = result * 10 + ch[i]-'0';
	}
	return result;
}

/***
  * check value is integer
  ***/
bool isInt(string value){
	if(value=="")
		return false;
	const char *ch = value.c_str();
	for(int i=0;i<value.size();i++){
		if(i==0){
			if(ch[i]=='-')
				continue;
			if(ch[i]<'0' || ch[i]>'9')
				return false;
		}else{
			if(ch[i]<'0' || ch[i]>'9')
				return false;
		}
	}
	return true;
}

/***
  * check value is float 
  ***/
bool isFloat(string value){
	bool point = false;
	if(value=="")
		return false;
	const char *ch = value.c_str();
	for(int i=0;i<value.size();i++){
		if(i==0){
			if(ch[i]=='-')
				continue;
			if(ch[i]<'0' || ch[i]>'9')
				return false;
		}else{
			if(ch[i]=='.'){
				if(point)
					return false;
				point = true;
			}else if(ch[i]<'0' || ch[i]>'9')
				return false;
		}
	}
	return true;
}

/***
  * check if line contains only one word
  ***/
bool isOneWord(string line){
	trim(line);
	if(line==""||line.find(' ')!=line.npos)
		return false;
	return true;
}




