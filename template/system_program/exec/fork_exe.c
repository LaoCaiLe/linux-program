#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

/*
int execlp(const char *file, const char *arg, ...);借助 PATH 环境变量找寻待执行程序

int execl(const char *path, const char *arg, ...); 自己指定待执行程序路径。

参 1： 程序名
参 2： argv0
参 3： argv1
...： argvN
哨兵：NULL
*/

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
        printf("child pid is created!\n");
        //execlp("date","date",NULL);           //执行命令
        //execlp("ls","ls","-l","-h"NULL);      //执行命令
        //execl("/bin/ls","ls","-lh",NULL);       
        //execl("./test","./test",NULL);  //执行可执行文件
        perror("exec error");
        exit(1);
    }   
    else if(pid>0)    //父进程返回子进程id
    {
        sleep(1);
        // printf("parent process: my child id %d\n",pid);
    }

    //fork之后的内容父子进程都会执行
    printf("-----end-------\n");
}