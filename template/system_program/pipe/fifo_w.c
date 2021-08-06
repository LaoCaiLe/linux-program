#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
// 写管道

/// fifo 实现非血缘关系进程间通信 fifo_r.c------fifo_w.c
int main(int argc, char *argv[])
{
    if(argc<2)
    {
        printf("input error");
        return -1;
    }

    int fd,ret;
    int i=0;
    char buf[1024];

    fd = open(argv[1],O_WRONLY);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }
    while(1)
    {
        sprintf(buf,"fifo %d\n",i++);
        ret = write(fd,buf,strlen(buf));
        if(ret == -1)
        {
            perror("write error");
            exit(1);
        }

        sleep(1);
    }
    close(fd);
    return 0;
    
}