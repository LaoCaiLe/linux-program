#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>

int main(int argc,char *argv[])
{
	int *p=NULL;
	int val=10;
	pid_t pid;

	
	int fd = open("fork_mmap.txt",O_RDWR|O_CREAT|O_TRUNC,0644);
	
	
	if(fd == -1)
	{
		perror("open error");
		exit(1);
	}
	ftruncate(fd,20);

	p = mmap(NULL,20,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(p == NULL)
	{
		perror("mmap error");
		exit(1);
	}
	int ret = unlink("fork_mmap.txt");
	if(ret==-1)
	{
		perror("unlink error");
		exit(1);
		
	}

	pid = fork();

	if(pid<0)
	{
		perror("fork error");
		exit(1);

	}
	//*p共享 al不共享
	else if(pid == 0)	
	{
		*p=200;
		val=200;
		printf("child:*p=%d,val=%d\n",*p,val);
	}
	else if(pid>0)
	{
		sleep(1);
		printf("parent:*p=%d,val=%d\n",*p,val);
		int ret = munmap(p,20);
		if(ret == -1)
		{
			perror("munmap error");
			exit(1);
		}
	}

	return 0;
	
}
