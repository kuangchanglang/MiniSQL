#include<string>
#include<iostream>
#include<fstream>
#include<sys/stat.h>
#include "catalog.h"
using namespace std;

int mzzain(){

	cout<<createDatabase("haha")<<endl;
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
