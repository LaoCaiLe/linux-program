#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "wrap.h"

#define MAXLINE 1024

void client(int readfd, int writefd)
{
    ssize_t len;
    ssize_t n;
    char buff[MAXLINE+1] = "test.txt";

    // fgets(buff, MAXLINE, stdin);

    len = strlen(buff);
    if(buff[len-1] == '\n')
        len--;

    Write(writefd, buff, len);

    while( (n=Read(readfd, buff, MAXLINE)) > 0)
        Write(STDOUT_FILENO,buff, n);
}