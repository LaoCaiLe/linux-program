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
    else if(pid==0)     //子进程返回0
    {
        printf("child pid is created!\n");
    }   
    else if(pid>0)    //父进程返回子进程id
    {
        printf("parent process: my child id %d\n",pid);
    }

    //fork之后的内容父子进程都会执行
    printf("-----end-------\n");
}

