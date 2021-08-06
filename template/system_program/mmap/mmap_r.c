#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <error.h>
#include <sys/types.h>
#include <fcntl.h>

struct people
{
    int age;
    char name[256];
};
 

int main(int argc, char*argv[])
{

	int fd;
	struct people temp;
	struct people *mm;

	fd = open("test.txt",O_RDONLY);
	if(fd == -1)
	{
		perror("open error");
		exit(1);
	}
	
	mm = mmap(NULL,sizeof(temp),PROT_READ,MAP_SHARED,fd,0);
	if(mm == MAP_FAILED)
	{
		perror("mmap error");
		exit(1);
	}
	
	close(fd);

	while(1)
	{
		printf("%d:%s\n",mm->age,mm->name);
		sleep(1);
	}

	munmap(mm,sizeof(temp));

	return 0;
}
