//读取按键状态
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
using namespace std;
int main()
{

	while (true)
       	{
		int a = -1,b = -1;
		while(1){
			scanf("%d %d",&a,&b);
			if(a != -1 && b != -1)break;
		}


		
		if (a == 0) {
			//按键按下
			if ( b == 0){
				cout<<"按下volum+" << endl;
			}
			sleep(10);
			//按键抬起
			//if ( b == 1) {
			cout<<"抬起volum+" << endl;
			//}
		}
		//按下volum-
		if (a == 1){
			//按键按下
			if (b == 0){
				cout <<"按下volum-" << endl;
			}
			sleep(10);
			//按键抬起
			//if ( b == 1){
			cout <<"抬起volum-" << endl;
			//}	
		}
	}
	return 0;
}
