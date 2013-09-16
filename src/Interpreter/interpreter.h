#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include <string>
using namespace std;

void doOneQuery(string query);

void createClause(string query);
void createDatabaseClause(string query);
void createTableClause(string query);

void insertClause(string query);

void selectClause(string query);

void deleteClause(string query);

void useClause(string query);

void dropClause(string query);


//utils
void trim(string &src);

#endif
