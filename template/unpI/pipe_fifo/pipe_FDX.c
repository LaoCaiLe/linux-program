#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include "wrap.h"
//P35 使用两个管道的客户-服务器程序

void client(int, int);
void server(int, int);

int main()
{
    int fd1[2], fd2[2];
    pid_t child_pid;

    pipe(fd1);
    pipe(fd2);

    if((child_pid = fork()) == 0)
    {
        close(fd1[1]);
        close(fd2[0]);

        server(fd1[0],fd2[1]);
        exit(1);
    }
    close(fd1[0]);
    close(fd2[1]);

    client(fd2[0],fd1[1]);

    waitpid(child_pid, NULL, 0);

    return 0;
}

