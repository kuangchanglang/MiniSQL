#ifndef __TABLE_H__
#define __TABLE_H__
struct attr{
	string attr_name;
	int attr_type;
	bool is_unique;
};

struct table{
	string tname;
	int attr_num;
	attr attrs[];
	int primarykey_pos;
	table(string name, int num, int pos=-1){
		tname = name;
		attr_num = num;
		attrs = new attr[attr_num];
		primarykey_pos = pos;
	}

	// free memory
	~table(){
		delete []attrs;
	}
};

#endif
