#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<sys/stat.h>
#include "catalog.h"
using namespace std;

int xxmain(){
	
	Table table = getTable("default","en");
	cout<<table.primarykey_column<<endl;
	int num = table.attr_num;
	for(int i=0;i<num;i++){
		cout<<"xx "<<table.attrs[i].attr_name<<" "<<table.attrs[i].attr_type<<" "<<table.attrs[i].is_unique<<endl;
	}
	return 0;
}

bool isTableExist(string dbname, string tname){
	if(!isDatabaseExist(dbname))
		return false;
	string table_catalog_path = getTableCatalogPath(dbname,tname);
	string table_data_path = getTableDataPath(dbname,tname);
	return isFileExist(table_catalog_path) && isFileExist(table_data_path);
}

Table getTable(string dbname, string tname){
	Table table;
	string table_catalog_path = getTableCatalogPath(dbname,tname);
	stringstream sstream;
	ifstream table_file;
	table_file.open(table_catalog_path.c_str());
	string line;
	getline(table_file,line);
	table.primarykey_column = line;

	getline(table_file,line);
	sstream<<line;
	sstream>>table.attr_num;
	vector<Attr> attrs;
	for(int i=0;i<table.attr_num;i++){
		Attr attr;
		getline(table_file,line);
		sstream.clear();
		sstream<<line;
		sstream>>attr.attr_name>>attr.attr_type>>attr.is_unique;
		attrs.push_back(attr);
	}

	table.attrs = attrs;
	return table;
}

/***
  * Create table, return true if success
  ***/
bool createTable(string dbname, Table table){
	if(!isDatabaseExist(dbname))
		return false;
	if(isTableExist(dbname,table.tname))
		return false;
	ofstream table_catalog;
	string table_catalog_path = getTableCatalogPath(dbname,table.tname);
	table_catalog.open(table_catalog_path.c_str());
	//primarykey 
	table_catalog<<table.primarykey_column<<endl;
	//attr_num
	table_catalog<<table.attr_num<<endl;
	//attrs
	for(int i=0;i<table.attr_num;i++){
		Attr attr = table.attrs[i];
		table_catalog<<attr.attr_name<<" "<<attr.attr_type<<" "<<attr.is_unique<<endl;
	}
	table_catalog.flush();
	table_catalog.close();

	string table_data_path = getTableDataPath(dbname,table.tname);
	ofstream table_data(table_data_path.c_str());

}

bool isDatabaseExist(string dbname){
	string db_path = DATA_DIR+dbname;
	return isFileExist(db_path);
}

Database getDatabse(string dbname){

}

/***
  * Create database, mkdir
  ***/
bool createDatabase(string dbname){
	string data_dir = DATA_DIR;	
	if(!isFileExist(data_dir)){
		mkdir(data_dir.c_str(),0777);
	}
	if(isDatabaseExist(dbname))
		return false;
	string db_path = DATA_DIR+dbname;
	mkdir(db_path.c_str(),0777);

	return true;
}

/*** 
  * Check if file or directory exists
  ***/
bool isFileExist(string file_path){
	fstream file;
	file.open(file_path.c_str(),ios::in);
	if(file){
		file.close();
		return true;
	}
	else{
		file.close();
		return false;
	}

}

string getTableDataPath(string dbname,string tname){
	return DATA_DIR+dbname+"/"+tname+".data";
}

string getTableCatalogPath(string dbname,string tname){
	return DATA_DIR+dbname+"/"+tname+".catalog";
}
