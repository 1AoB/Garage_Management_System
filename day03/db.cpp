#include<iostream>
#include"db.h"
using namespace std;

sqlite3* db = nullptr;


int open_db(const char* db_path){

	int ret = sqlite3_open(db_path,&db);
	if(ret != 0){
		cout<<"<db.cpp> failed to open sql_databases."<<endl;
		return -1;
	}
	return 0;
}

void close_db(){
	sqlite3_close(db);
}

int exec_db(const char* sql){
	char *errmsg = nullptr;
	int ret = sqlite3_exec(db,sql,nullptr,nullptr,&errmsg);
	if(ret != 0){
		cout<<"<db.cpp> Failed to exec SQL,reason:"<<errmsg<<endl;
		sqlite3_free(errmsg);
	       	return -1;	
	}
	return 0;
}
int query_db(const char* sql,sqlite3_callback callback,void* arg){
	char *errmsg = nullptr;
	int ret = sqlite3_exec(db,sql,callback,arg,&errmsg);
	if(ret != 0){
		cout<<"<db.cpp> Failed to query_db,reason:"<<errmsg<<endl;
		sqlite3_free(errmsg);
		return -1;
	}
	return 0;
}
