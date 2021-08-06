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
	int n,i=0;
	
	// 访问控制属性
	int	flags = fcntl(STDIN_FILENO,F_GETFL);
	if(flags == -1)
	{
		perror("fcntl error");
		exit(1);
	}
	
	flags |= O_NONBLOCK;

	int ret = fcntl(STDIN_FILENO,F_SETFL,flags);
	if(ret == -1)
	{
		perror("fcntl error");
		exit(1);
	}
	
	while(i<5)
	{
		n=read(STDIN_FILENO,buf,10);
		//没有读到数据
		if(n<0)
		{
			//读取发生错误
			if(errno != EAGAIN) 
			{
				perror("read error!");
				exit(1);
			}
			//读取发生阻塞
			else
			{
				write(STDOUT_FILENO,str_try,strlen(str_try));
				sleep(2);
				i++;
			}
		
		}
		//读到数据
		else
		{
			write(STDOUT_FILENO,buf,n);
			break;
		}

	}
	//阻塞次数超过5，直接退出
	if(i==5) write(STDOUT_FILENO,str_timeout,strlen(str_timeout));

	return 0;
}
