#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <string.h>
#include <unistd.h>

// int dup(int oldfd);
#define str1 "this is test1\n"  
#define str2 "this is test2\n"  
#define str3 "this is test3\n"  

void test01(char *name)
{
    int fd = open(name,O_RDWR);
    int ret;

    ret = fcntl(fd,F_DUPFD,0);//0 ????fcntl ???????????????????

    printf("ret = %d\n",ret);

    ret = fcntl(fd,F_DUPFD,7);//??? 7?? 7 ????????? >=7
    printf("ret = %d\n",ret);

    write(fd,"hello",6);

    close(fd);
}



int main(int argc, char *argv[])
{
    test01(argv[1]);

    return 0;
}