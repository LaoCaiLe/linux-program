#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char* argv[])
{
		int fd;
		fd = open("./dct.txt",O_RDWR,0444);//rw-r--r--
		if(fd == -1)
		{
			printf("file open error!\n[%d]error:%s!\n",errno,strerror(errno));
			exit(1);
		}
		close(fd);


		return 0;
}

