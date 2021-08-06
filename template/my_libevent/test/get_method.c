#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <event2/event.h>
#include <errno.h>
#include <stdlib.h>
int main()
{
	int i;

	struct event_base *base;
	base = event_base_new();
	
	const char **buf;
	char *buff;


	buff = event_base_get_method(base); //查看当前正在使用哪个多路IO
	printf("current is %s IO method\n",buff);

	buf = event_get_supported_methods();//查看当前事件适合哪些多路IO

	for(i=0; i<10; i++){
		if(strcmp(buf[i],"NULL")==0)
			break;
		printf("buf[%d]=%s\n",i,buf[i]);
	}

	return 0;
}
