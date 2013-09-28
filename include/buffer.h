#ifndef __BUFFER_H__
#define __BUFFER_H__
#include "record.h"
#define BLOCK_SIZE 4096

// two kind of block, one is info block, the other is data block
// info block stores info about whether the next block_size*8 blocks is full
// data block stores table data in format: row_cnt flag1,flag2,...,flagn data1,data2,...,datan
// where flag1 set 1 means data1 is effective, 0 means data1 is deleted
struct Block{
	string dbname;
	string tname;
	int block_id;
	bool dirty;
	char data[BLOCK_SIZE];

	Block(){
	
	}
	~Block(){
	}
};

Block* getBlock(string db, string tname, int block_id);

void setBlock(Block *block, int pos, char *data, int length);

void writeBlock(Block *block);

#endif
