#include<iostream>
#include<string>
using namespace std;

void trim(string &src);
int main(){

	string query;
	string line;

	cout<<"MiniSql>";
	//cin>>line;
	getline(cin,line);
	query = "";

	while(true){
		trim(line);
		int terminate_pos = line.find_first_of(';');
		if(terminate_pos==-1){
			query += " ";
			query += line;	
			cout<<"-->";
			getline(cin,line);
		//	cin>>line;
		}else{
			query += line.substr(0,terminate_pos);
			string tmp = line.substr(terminate_pos+1);
			//line.clear();
			line = tmp;
			cout<<"This query:"<<query<<endl;
			cout<<"Left:"<<line<<endl;
			if(query=="quit")
				break;
			if(line==""){
				cout<<"MiniSql>";
				getline(cin,line);
				//cin>>line;
			}
			query="";
		}

	}
	return 0;

}
void trim(string &src){
	int s_pos = src.find_first_not_of(' ');
	int e_pos = src.find_last_not_of(' ');

	src = src.substr(s_pos,e_pos-s_pos+1);
	return;
}
