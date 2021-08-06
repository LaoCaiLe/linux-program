#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

// pid_t waitpid(pid_t pid, int *wstatus, int options);
/*
pid：指定回收某一个子进程 pid
    >0: 待回收的子进程 pid
    -1：任意子进程
    0：同组的子进程。
status：（传出） 回收进程的状态。
options：WNOHANG 指定回收方式为，非阻塞。
返回值：
    >0 : 表成功回收的子进程 pid
    0 : 函数调用时， 参3指定了WNOHANG, 并且，没有子进程结束。
    -1: 失败。errno
*/
// 回收指定pid的子进程

/**
waitpid():  on  success, returns the process ID of the child whose state has changed; if
    WNOHANG was specified and one or more child(ren) specified by pid exist,  but  have  not
    yet changed state, then 0 is returned.  On error, -1 is returned.
*/

int main()
{
    pid_t pid,wpid;
    int i;

    for(i=0; i<5; i++)
    {
        pid=fork();
        if(pid == 0)
        {            
            sleep(i);
            printf("I am the %dth child:%d\n",i+1,getpid());
            break;
        }
       
      
    }

    if(5 == i) //父进程
    {
        while((wpid=waitpid(-1,NULL,0)) != -1)
        {
            if(wpid == -1)
            {
                perror("waitpid error");
                exit(1);
            }
      
            printf("parent wait the %dth child\n",wpid);
          
        }
    }


    return 0;
}