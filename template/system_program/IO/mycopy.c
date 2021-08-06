#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int buf[1024];
	int n;


	int fd1 = open(argv[1],O_RDONLY);
	if(fd1 == -1)
	{
		perror("open argv1 error!\r\n");
		exit(1);
	}

	int fd2 = open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0644);
	if(fd2 == -1)
	{
		perror("open argv1 error!\r\n");
		exit(1);
	}
	while((n = read(fd1, buf, 1024))!= 0)
	{
		if(n<0) 
		{
				perror("read error");
				break;
		}
		write(fd2,buf,n);
	}
		
	close(fd1);
	close(fd2);
}
