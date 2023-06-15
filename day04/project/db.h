#ifndef _DB_H_
#define _DB_H_

#include<iostream>
#include<sqlite3.h>



int open_db(const char* db_path);

void close_db();

int exec_db(const char* sql);

int query_db(const char * sql,sqlite3_callback callback ,void* arg);
#endif
