#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "wrap.h"
#define MAXLINE 1024
//假如 FIFO 里没有数据，调用 read() 函数从 FIFO 里读数据时会阻塞
void server(int readfd, int writefd)
{
    int fd;
    ssize_t n;
    char buff[MAXLINE+1];

    if( (n = Read(readfd, buff, MAXLINE))== 0)
        printf("end of line while reading pathname");

    buff[n] = '\0';

    if( (fd = open(buff, O_RDONLY)) < 0)
    {
        snprintf(buff+n, sizeof(buff)-n, ":can't open %s\n",strerror(errno));
        n = strlen(buff);
        Write(writefd, buff , n);
    }

    else
    {
        while(( n = Read(fd, buff, MAXLINE)) > 0)
        {
            Write(writefd, buff, n);
        }
        close(fd);
    }

}