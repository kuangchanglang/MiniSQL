#ifndef __CATALOG_H__
#define __CATALOT_H__

#define DATA_DIR "./data/"
#include<iostream>
#include<string>
#include "table.h"
#include "database.h"
using namespace std;

bool isTableExist(string dbname, string tname);

Table getTable(string dbname, string tname);

bool createTable(string dbname, Table table);

bool isDatabaseExist(string dbname);

Database getDatabse(string dbname);

bool createDatabase(string dbname);

bool isFileExist(string file_path);

string getTableCatalogPath(string dbname, string tname);
string getTableDataPath(string dbname, string tname);
#endif
