#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc,char *argv[])
{
    printf("before fork1\n");
    printf("before fork2\n");
    printf("before fork3\n");
    printf("before fork4\n");
    printf("before fork5\n");

    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        exit(1);
    }
    else if(pid==0)     //子进程的返回
    {
        printf("child pid is created!\n");
        printf("child's id is %d ,parent's id is %d",getpid(),getppid());
    }   
    else if(pid>0)    //父进程的返回
    {
        printf("parent process!\n");
        printf("my child id %d, my id is %d ,my parent's id is %d\n",pid,getpid(),getppid());
        //父进程的父进程的id为bash
    }

    //fork之后的内容父子进程都会执行
    printf("-----end-------\n");
}