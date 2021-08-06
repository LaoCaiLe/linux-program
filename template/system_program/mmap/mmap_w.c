#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <error.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

struct people
{
	
	int age;
	char name[256];
};


int main(int argc, char*argv[])
{

	int fd,ret;
	struct people temp = {10,"ck"};
	char *p;

	
	fd = open("test.txt",O_RDWR|O_CREAT,0644);
	if(fd == -1)
	{
		perror("open error");
		exit(1);
	}
	ftruncate(fd, sizeof(temp));	

	p = mmap(NULL,sizeof(temp),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(p == MAP_FAILED)
	{
		perror("mmap error");
		exit(1);
	}
	

	while(1)
	{
		
		memcpy(p,&temp,sizeof(temp));
		temp.age++;
		sleep(1);
	}

	munmap(p,sizeof(temp));

	return 0;
}
