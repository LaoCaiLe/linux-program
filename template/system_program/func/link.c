#include <stdio.h>
#include <fcntl.h>  
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

//int link(const char *oldpath, const char *newpath);

int main(int argc, char *argv[])
{
	int fd,ret;

	ret = link(argv[1],argv[2]);
	if(ret == -1)
	{
		perror("link error");
		exit(1);
	}

	unlink(argv[1]);


	return 0;
}
