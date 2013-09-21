#ifndef __TABLE_H__
#define __TABLE_H__
#include<string>
#include<vector>
using namespace std;
struct Attr{
	string attr_name;
	int attr_type;
	bool is_unique;
	Attr(){}
	Attr(string name,int type,bool unique){
		attr_name=name;
		attr_type=type;
		is_unique=unique;
	}

};

struct Table{
	string tname;
	int attr_num;
	vector<Attr> attrs;
	string primarykey_column;
	Table(string name, vector<Attr> attr_vec, string column=""){
		tname = name;
		attrs = attr_vec;
		attr_num = attrs.size();
		primarykey_column = column;
	}

	// free memory
	~Table(){
	}
};


struct Condition{
	string attr_name;
	int op;
	//op map
	//0 =
	//1 <>
	//2 <
	//3 >
	//4 <=
	//5 >=
	//-1 undefined
	string value;
	Condition(){}
	Condition(string _attr,int _op, string _value){
		attr_name = _attr;
		op = _op;
		value = _value;
	}
};
#endif
