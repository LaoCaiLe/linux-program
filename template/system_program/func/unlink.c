#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
	int fd,ret;
	char *buf="the test of unlink!";

	fd = open("./test.txt",O_CREAT|O_RDWR,0644);
	if(fd<0)
	{
		perror("open error");
		exit(1);
	}
	
	//这时test.txt已经被标记为可释放文件 数据写在内核空间的缓冲区
	ret = unlink("./test.txt");
	if(ret<0)
	{
		perror("unlink error");
		exit(1);
	}
	

	ret = write(fd,buf,strlen(buf));
	if(ret<0)
	{
		perror("write error");
		exit(1);
	}

	printf("please enter any key:\n");
	getchar();
	
	//buf[3] = 'a';

	

	close(fd);

	return 0;


}
// /*
//  * unlink 函数的特征：清除文件时，如果文件的硬链接数到 0 了，没有 dentry 对应，但该文件仍不会马上被释放，要等到所有打开文件的进程关闭该文件，系统才会挑时间将该文件释放掉。
//  *
//  */ 


