#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define str_timeout "time_out\n"
#define str_try "try again\n"

int main(int argc,char *argv[])
{
	char buf[10];
	int n,i;
	int fd = open("/dev/tty",O_NONBLOCK|O_RDWR);
	if(fd==-1)
	{
		perror("open /dev/tty");
		exit(1);
	}

	for(i=0;i<5;i++)
	{
		n=read(fd,buf,10);
		//读到数据
		if(n>0)
		{
			write(STDOUT_FILENO,buf,n);
			break;
		}
		//没有读到数据
	
		//出错
		if(errno != EAGAIN)
		{
			perror("read /dev/tty!");
			exit(1);
		}
		//发生非阻塞
		else
		{
			write(STDOUT_FILENO,str_try,strlen(str_try));
			sleep(1);								
		}
	}
	if(i==5) write(STDOUT_FILENO,str_timeout,strlen(str_timeout));
	//else write(STDOUT_FILENO,buf,n);

	close(fd);

	return 0;
}
