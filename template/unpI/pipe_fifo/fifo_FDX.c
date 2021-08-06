#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

//父子进程中的两个open函数 必须交替打开读写文件
// 1.  父进程 先打开读 再打开写 
//     子进程 先打开写 再打开读 ✔
    
// 2.  父进程 先打开写 再打开读
//     子进程 先打开读 再打开写 ✔

// 3.  都先打开读或者都先打开写 进程阻塞

//原因：
    //如果以只读方式打开就会阻塞直到有以写方式打开此文件为止，O_WRONLY
    //如果以只写方式打开就会阻塞直到有读方式打开文件为止O_RDONLY
#include "wrap.h"

#define fifo1 "./fifo1"
#define fifo2 "./fifo2"

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

void client(int, int);
void server(int, int);

int main(int argc, char *argv[])
{
    int readfd, writefd;
    pid_t child_pid;

    if((mkfifo(fifo1,FILE_MODE) < 0) && (errno != EEXIST ))
        printf("cam't create %s", fifo1);
    if((mkfifo(fifo2,FILE_MODE) < 0) && (errno != EEXIST ))
    {
        unlink(fifo1);
        printf("cam't create %s", fifo2);
    }
        

    if( (child_pid = fork()) == 0)
    {
        readfd = open(fifo1, O_RDONLY, 0);
        writefd = open(fifo2, O_WRONLY, 0);
        
        server(readfd, writefd);
        exit(1);
    }
   
    
    writefd = open(fifo1, O_WRONLY, 0);
    readfd = open(fifo2, O_RDONLY, 0);    
   
    client(readfd, writefd);

    waitpid(child_pid, NULL, 0);

    close(readfd);
    close(writefd);

    unlink(fifo1);
    unlink(fifo2);

    exit(1);
}


