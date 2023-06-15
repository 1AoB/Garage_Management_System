#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
using namespace std;

void tone( int hz){
	int fd = open("/dev/input/event0",O_WRONLY);
	if ( fd <0){
		perror( "open" );
		return;
	}
	struct input_event ev;
	ev.type = EV_SND;
	ev.code = SND_TONE;
	ev.value = hz;
	write(fd,&ev,sizeof(ev));
	close( fd);
}

int main( )
{
	while (true) {
		tone ( 500);
		sleep( 1);
		tone( 0);
		sleep( 1);
	}
	return 0;
}

