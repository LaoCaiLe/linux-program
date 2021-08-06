#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
//lseek改变文件读写位置

int main()
{
	int fd,n;
	char buf;
	char msg[]="it's my vim.\n";
	fd = open("./dict.txt",O_RDWR|O_CREAT,0644);
	if(fd < 0)
	{
		perror("open dict.txt");
		exit(1);
	}
	
	write(fd,msg,strlen(msg));//文件的读和写使用同一偏移 执行完这句代码已偏移到串末

	lseek(fd,0,SEEK_SET);//重新指定下标在串头

	int strlen = lseek(fd,0,SEEK_END); //获取文件大小
	printf("strlen = %d\n",strlen);

	lseek(fd,0,SEEK_SET);


    lseek(fd,15,SEEK_END); //拓展文件大小 使用IO操作
	write(fd,'\0',1);

	lseek(fd,0,SEEK_SET);

	while(n=read(fd,&buf,1))
	{
		if(n<0)
		{
			perror("read error");
			exit(1);
		}
		write(STDOUT_FILENO,&buf,n);
	}

	close(fd);

}

