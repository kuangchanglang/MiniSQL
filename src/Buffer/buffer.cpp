#include "buffer.h"
#include "catalog.h"
#include <fstream>
#include <string.h>

using namespace std;

Block* getBlock(string dbname, string tname, int block_id){
	string file_path = getTableDataPath(dbname,tname);
	ifstream data(file_path.c_str(), ios::binary);

	data.seekg(block_id*BLOCK_SIZE);
	if(data.eof()){
		return NULL;
	}

	Block *block = new Block();
	block->dbname = dbname;
	block->tname = tname;
	block->block_id = block_id;
	block->dirty = false;
	data.read(block->data,BLOCK_SIZE);

	data.close();

	return block;
}
 
void setBlock(Block *block, int pos, char *data, int length){
	memcpy(block->data+pos,data,length);
	block->dirty = true;

	return ;
}

void writeBlock(Block *block){
	string file_path = getTableDataPath(block->dbname,block->tname);
	ofstream data(file_path.c_str(), ios::binary);
	
	data.seekp(block->block_id*BLOCK_SIZE);
	data.write(block->data,BLOCK_SIZE); 

	data.flush();
	data.close();
} 
