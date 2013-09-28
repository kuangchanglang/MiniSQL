#include<iostream>
#include "table.h"
#include "record.h"
#include "buffer.h"

using namespace std;

// internal function
int getInt(char bytes[]);
int findEmptyRow(Block *block, int row_cnt);
int findFirstEmptyBlock(Block *block);

int insertRecord(Table table, Record record){
	// first block stores the other blocks' info
	// we have block_size*8 bits in this block, first bit 1 means the next block is full(0 empty), second bit stands the next second block
	// ...
	Block *info_block = getBlock(CURRENT_DB, table.tname, 0);
	
	int block_id = findFirstEmptyBlock(info_block);

	Block *data_block = getBlock(CURRENT_DB, table.tname, block_id);

	int row_cnt = getInt(data_block->data);
	int row_id = findEmptyRow(data_block,row_cnt);
	if(row_id != -1){
		// insert position found
		// set flag
		char flag[] = {1};
		setBlock(data_block,4+row_id,flag,1);

		// set record data
		int pos = 4+row_cnt+row_id*record.length;
		setBlock(data_block,pos,record.data,record.length);
		
		// judge if this block is full, if full, update info block

	}else{
		return -1;
	}
	return 0;
}

int findFirstEmptyBlock(Block *info_block){

	return 1;
}
int findEmptyRow(Block *block, int row_cnt){
	int row_id = -1;
	for(int i=0;i<row_cnt;i++){
		if(block->data[i+4]==0){// 4 is length of row_cnt
			row_id=i;
			break;
		}
	}
	
	return row_id;
}

vector<Record> selectAllRecords(Table table, vector<Attr> attrs){
	vector<Record> records;
	int block_id = 1;
	while(true){
		Block *block = getBlock(CURRENT_DB, table.tname, block_id);
		if(block==NULL)
			break;
		block_id++;
	}

	return records;
}

vector<Record> selectRecords(Table table, vector<Attr> attrs, vector<Condition> conditions){
}

int deleteAllRecords(Table table){
}

int deleteRecords(Table table, vector<Condition> conditions){
}

/***
  * convert char[] to int
  */
int getInt(char bytes[]){
	int addr = bytes[0] & 0xFF;
	addr |= ((bytes[1] << 8) & 0xFF00);
	addr |= ((bytes[2] << 16) & 0xFF0000);
	addr |= ((bytes[3] << 24) & 0xFF000000);
	return addr;
}
