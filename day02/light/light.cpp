#include<iostream>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

int main(){

	int fd = open("/sys/class/leds/led3/brightness",O_WRONLY);

	if(fd==-1){
		perror("Failed to open file.\n");
		return -1;
	}

	char data1[]="1";
	char data0[]="0";
	while(1){
		write(fd,data1,sizeof(data1));//1
		sleep(2);
		write(fd,data0,sizeof(data0));//0
		sleep(2);
	}
	close(fd);
	return 0;
}
