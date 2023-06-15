#include<iostream>
#include<cstring>
#include<ctime>
#include<sstream>
#include"db.h"

using namespace std;

char* max_id ;
std::tm parse_datetime(const char* datetime_str) {//解析日期时间字符串,返回对应的时间结构体
    std::tm tm = {};
    std::istringstream ss(datetime_str);
    ss >> tm.tm_year; // 年份
    ss.ignore(); // 忽略分隔符 '-'
    ss >> tm.tm_mon; // 月份
    ss.ignore(); // 忽略分隔符 '-'
    ss >> tm.tm_mday; // 日
    ss.ignore(); // 忽略分隔符 ' '
    ss >> tm.tm_hour; // 时
    ss.ignore(); // 忽略分隔符 ':'
    ss >> tm.tm_min; // 分
    ss.ignore(); // 忽略分隔符 ':'
    ss >> tm.tm_sec; // 秒
    tm.tm_year -= 1900; // 年份需要减去 1900
    tm.tm_mon -= 1; // 月份需要减去 1
    return tm;
}
int diff_datetime(const char* datetime1, const char* datetime2) {
    // 解析日期时间字符串，转换为时间戳
    std::tm tm1 = parse_datetime(datetime1);
    std::tm tm2 = parse_datetime(datetime2);
    std::time_t t1 = std::mktime(&tm1);
    std::time_t t2 = std::mktime(&tm2);
    // 计算时间差
    int diff = std::difftime(t2, t1);
    return diff;
}
void menu(){

	cout<<"***********************************"<<endl;

	cout<<"1:insert  2:update  3:query  4:exit"<<endl;

	cout<<"***********************************"<<endl;

}


string string_time(){

	time_t now;
	time(&now);


	struct tm *tm_time = localtime(&now);

	char buf[256]={0};
	strftime(buf,sizeof(buf),"%4Y-%02m-%02d %02H:%02M:%02S",tm_time);


	return string(buf);

}

void do_insert(){
	cout<<"Now do insert!"<<endl;
	
	string plate;
	cout<<"Please input license plate"<<endl;
	cin >> plate;

	string str_time = string_time();//now time
	
	char sql[512] = {0};
	sprintf(sql,
		"insert into parking values(NULL,'%s','%s','%s');",
		str_time.c_str(),//now time = car in time
		"",//car out time
		plate.c_str());
	int ret = exec_db(sql);
	if(ret != 0)
	{
		cout<<"<parking.cpp> Failed to  do_insert"<<endl;
	}
}

int parking_callback(void * para,int f_num,char **f_value,char **f_name)
{
	cout<<"parking_callback is used"<<endl;
	if(f_value[0]==nullptr){
		cout<<"<parking.cpp> parking_callback f_value[0] is null"<<endl;
		return -1;
	}
	max_id = f_value[0];
	cout << f_name[0] <<":"<<f_value[0]<<endl;
	return 0;
}

char inTime[64]={0};
char outTime[64]={0};

int parking_callback1(void * para,int f_num,char **f_value,char **f_name)
{
	cout<<"parking_callback1 is used"<<endl;
	if(f_value[0]==nullptr){
		cout<<"<parking.cpp> parking_callback f_value[0] is null"<<endl;
		return -1;
	}
	
	cout << f_name[0] <<":"<<f_value[0]<<endl;
	if(strcmp(f_name[0],"in_time") == 0){
		cout<<"字段是"<<f_name[0]<<endl;
		strcpy(inTime , f_value[0]);
	}
	if(strcmp(f_name[0],"out_time") == 0 ){
	
		cout<<"字段是"<<f_name[0]<<endl;
		strcpy(outTime , f_value[0]);
	}	
	return 0;
}


void do_update(string plate){
	char* id;
	cout<<"Now do update!"<<endl;

	string str_time = string_time();//now time
	
	
	char sql[512] = {0};
	sprintf(sql,
	  "select max(id) from parking where license_plate='%s';",
	   plate.c_str());
	int ret = query_db(sql,parking_callback,nullptr);
	if(ret != 0){
		cout<<"<parking.cpp> Failed to do_query"<<endl;
		return ;
	}
	strcpy(id,max_id);
	cout<<"max_id = "<<max_id<<"111111111111111111111111"<<endl;
	cout<<"id = "<<id<<"11111111111111111111111111111"<<endl;
	memset(sql,0,sizeof(sql));

	
	string now_time = string_time();
	sprintf(sql,
	   "update parking set out_time ='%s' where id = '%s';",
	   now_time.c_str(),
	   id);	
	ret = query_db(sql,parking_callback,nullptr);
	if(ret != 0){
		cout<<"<parking.cpp> Failed to do_query"<<endl;
		return ;
	}
	cout<<"max_id = "<<max_id<<"22222222222222222222222222222"<<endl;
	cout<<"id = "<<id<<"2222222222222222222222222222222"<<endl;
	
	
	
	memset(sql,0,sizeof(sql));
	cout<<"id:"<<id<<endl;
	sprintf(sql,
	   "select in_time from parking where id = '%s';",
	   id);	
	ret = query_db(sql,parking_callback1,nullptr);
	cout<<sql<<endl;
	cout<<"2222222222222222222"<<endl;
	if(ret != 0){
		cout<<"<parking.cpp> Failed to do_query"<<endl;
		return ;
	}
	
	memset(sql,0,sizeof(sql));
	cout<<"id:"<<id<<endl;
	sprintf(sql,
	   "select out_time from parking where id = '%s';",
	   id);	
	ret = query_db(sql,parking_callback1,nullptr);
	cout<<sql<<endl;
	cout<<"333333333333333333333"<<endl;
	if(ret != 0){
		cout<<"<parking.cpp> Failed to do_query"<<endl;
		return ;
	}
	cout<<inTime<<","<<outTime<<endl;	
	int seconds = diff_datetime(inTime,outTime);
	cout<<"在车库中待了"<<seconds<<"seconds,应收费"<<seconds*0.1<<"块"<<endl;
}


void do_query(){
	cout<<"Now do query!"<<endl;
	string plate;
	cout<<"Please input license plate"<<endl;
	cin>>plate;

	char sql[512] = {0};
	sprintf(sql,
	  "select max(id) from parking where license_plate='%s';",
	  plate.c_str());

	int ret = query_db(sql,parking_callback,nullptr);
	
	if(ret != 0){
		cout<<"<parking.cpp> Failed to do_query"<<endl;
	}
}

int main(){
	int ret = open_db("parking.db");
	if(ret != 0){
		cout<<"<parking.cpp> failed to open_db"<<endl;
		return -1;
	}
	cout<<"<parking.cpp> congratulate to success to open_db!"<<endl;
	string sql = "create table if not exists parking(id integer primary key autoincrement , in_time text not null,out_time text,license_plate text not null)" ;
	ret = exec_db( sql.c_str());
	if(ret != 0){
		cout<<"<parking.cpp> failed to exec_db"<<endl;
		close_db();
		return -1;
	}
	cout<<"<parking.cpp> congratulate to success to exec_db!"<<endl;


	int op;

	while(1){
		menu();
		string plate="";
		cout<<"Please to input your option:"<<endl;
		cin>>op;
		switch(op){
			case 1:
				do_insert();
				break;
			case 2:
				cout<<"Please intput plate"<<endl;
				cin>>plate;
				do_update(plate);
				break;
			case 3:
				do_query();
				break;
			case 4:
				cout<<"Now do close database!"<<endl;
				close_db();
				exit(0);
				break;
			default:
				break;
		}

	}

	return 0;
}
