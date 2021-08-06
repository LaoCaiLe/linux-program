#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

// int dup(int oldfd);
#define str1 "this is test1\n"  
#define str2 "this is test2\n"  
#define str3 "this is test3\n"  

void test01(char *name)
{
    int fd = open(name,O_RDWR);

    int newfd = dup(fd);

    printf("fd=%d\n",fd);       //fd=3
    printf("newfd=%d\n",newfd); //newfd=4

    close(fd);
}

void test02(char *name1,char *name2)
{
    int fd1 = open(name1,O_RDWR|O_TRUNC);
	if(fd1 == -1)
	{
		perror("open name1 error");
		exit(1);
	}
    int fd2 = open(name2,O_RDWR|O_TRUNC);
	if(fd2 == -1)
	{
		perror("open name2 error");
		exit(1);
	}

    int fdret = dup2(fd2,fd1);

    printf("fd1=%d\n",fd1);       //fd1=3
    printf("fd2=%d\n",fd2);       //fd2=4
    printf("fdret=%d\n",fdret);   //fdret = 4

    int ret1 = write(fd1,str1,sizeof(str1));
    int ret2 = write(fd2,str2,sizeof(str2));
    int ret3 = write(fdret,str3,sizeof(str3));

    printf("ret1=%d\n",ret1);
    printf("ret2=%d\n",ret2);
    printf("ret3=%d\n",ret3);

    close(fd1);
    close(fd2);
}

void test03(char *name)
{
    int fd = open(name,O_RDWR);

    dup2(fd,STDOUT_FILENO);

    printf("shiyuan if my wife");

    close(fd);
}

int main(int argc, char *argv[])
{
    //test03(argv[1]);

     test02(argv[1],argv[2]);
}
