//读取按键状态
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
using namespace std;
int main()
{
	//打开按键设备文件
	int fd = open( "/dev/input/event1",O_RDONLY );
	if (fd < 0){
		perror( "open error");
		return -1;
	}
	while (true)
       	{
		//阻塞读取按键事件
		struct input_event ev;
		read(fd, &ev,sizeof(ev));
		//处理按键事件
		if (ev.type != EV_KEY) {
			continue;
		}
		//按下volum+
		if (ev.code == KEY_UP) {
			//按键按下
			if ( ev.value == 1){
				cout<<"按下volum+" << endl;
			}
			sleep(10);
			//按键抬起
			//if ( ev.value == 0) {
			cout<<"抬起volum+" << endl;
			//}
		}
		//按下volum-
		if (ev.code == KEY_DOWN){
			//按键按下
			if (ev.value == 1){
				cout <<"按下volum-" << endl;
			}
			sleep(10);
			//按键抬起
			//if ( ev.value == 0){
			cout <<"抬起volum-" << endl;
			//}	
		}
	}
	return 0;
}
