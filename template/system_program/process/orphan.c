#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc,char *argv[])
{
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        exit(1);
    }
    else if(pid==0)     //子进程返回0
    {
        while(1)
        {
            printf("I am child, my parent pid = %d\n",getppid());
            sleep(1);
        }
    }   
    else if(pid>0)    //父进程返回子进程id
    {
        printf("I am parent, my pid is %d\n",getpid());
        sleep(9);
        printf("parent is going to die\n");
    }

    //fork之后的内容父子进程都会执行
    printf("-----end-------\n");
}
/*
执行结果
父进程结束后子进程id发生变化 成为孤儿进程

I am parent, my pid is 8522
I am child, my parent pid = 8522
I am child, my parent pid = 8522
I am child, my parent pid = 8522
I am child, my parent pid = 8522
I am child, my parent pid = 8522
I am child, my parent pid = 8522
I am child, my parent pid = 8522
I am child, my parent pid = 8522
I am child, my parent pid = 8522
parent is going to die
-----end-------
laocaile@laocaile-machine ~/template/system_program/exec$ I am child, my parent pid = 1
I am child, my parent pid = 1
I am child, my parent pid = 1
I am child, my parent pid = 1
I am child, my parent pid = 1
I am child, my parent pid = 1
I am child, my parent pid = 1

*/