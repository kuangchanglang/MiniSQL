#ifndef __TABLE_H__
#define __TABLE_H__
#include<string>
#include<vector>
using namespace std;
struct attr{
	string attr_name;
	int attr_type;
	bool is_unique;
	attr(string name,int type,bool unique){
		attr_name=name;
		attr_type=type;
		is_unique=unique;
	}

};

struct table{
	string tname;
	int attr_num;
	vector<attr> attrs;
	int primarykey_pos;
	table(string name, vector<attr> attr_vec, int pos=-1){
		tname = name;
		attrs = attr_vec;
		attr_num = attrs.size();
		primarykey_pos = pos;
	}

	// free memory
	~table(){
	}
};

#endif
