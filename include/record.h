#ifndef __RECORD_H__
#define __RECORD_H__
#include "table.h"

extern string CURRENT_DB;

struct Record{
	vector<Attr> attrs;
	int length;
	char *data;
};

int insertRecord(Table table, Record record);

vector<Record> selectAllRecords(Table table, vector<Attr> attrs);

vector<Record> selectRecords(Table table, vector<Attr> attrs, vector<Condition> conditions);

int deleteAllRecords(Table table);

int deleteRecords(Table table, vector<Condition> conditions);



#endif
