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
    else if(pid>0)    //父进程返回子进程id 
    {
        while(1)
        {
            printf("I am parent, my pid is %d\n",getpid());
            sleep(1);
        }
    }   
    else if(pid==0)    //子进程返回0
    {
        printf("I am child, my pid is %d\n",getpid());
        sleep(9);
        printf("child is going to die\n");
        
    }

    //fork之后的内容父子进程都会执行
    printf("-----end-------\n");
}

//子进程死亡 父进程未死亡子进程无法被回收 进入僵尸态

// 子进程 僵尸态
// 8800   8801   8800   8552 pts/5      8800 Z+    1000   0:00 [zombie] <defunct>