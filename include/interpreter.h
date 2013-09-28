#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include <string>
#include <vector>
#include "table.h"
using namespace std;

extern string CURRENT_DB;

void doOneQuery(string query);

void createClause(string query);
void createDatabaseClause(string query);
void createTableClause(string query);
void createIndexClause(string query);

void insertClause(string query);

void selectClause(string query);

void deleteClause(string query);

void useClause(string query);

void dropClause(string query);
void dropDatabaseClause(string dbname);
void dropTableClause(string tname);
void dropIndexClause(string iname);

void execFileClause(string query);

Attr parseAttribute(string attr_str,bool &is_primarykey_defined,string &primarykey_column);

vector<Condition> getConditions(string cond_str);
Condition getCondition(string one_cond_str);

bool containsDuplicate(vector<Attr> attrs);
bool checkValueMatchType(int type, string value);

//utils
void trim(string &src);
int getType(string type);
int toInt(string value);
bool isInt(string value);
bool isFloat(string value);
bool isOneWord(string line);
#endif
