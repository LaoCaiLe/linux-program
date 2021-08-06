#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// 读管道
/// fifo 实现非血缘关系进程间通信 fifo_r.c------fifo_w.c
int main(int argc, char *argv[])
{
    int fd,ret;
    int i=0;
    char buf[1024];

    if(argc<2)
    {
        printf("input error");
        return -1;
    }
    
    fd = open(argv[1],O_RDONLY);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }
    
    while(1)
    {
       
        ret = read(fd,buf,sizeof(buf));
        if(ret == -1)
        {
            perror("read error");
            exit(1);
        }

        write(STDOUT_FILENO,buf,ret);

        sleep(1);
    }
    close(fd);
    return 0;
    
}