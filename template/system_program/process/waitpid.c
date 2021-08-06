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
    pid_t pid,wpid,tmpid;
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
        if(i==2)
        {
            wpid = pid; //在父进程接收子进程pid
            printf("%d will be called \n",wpid);
        }
    }

    if(5 == i) //父进程
    {
        //sleep(2);
        printf("parent before wait %d\n",wpid);
        tmpid = waitpid(wpid,NULL,0);//发生阻塞回收 等到指定wpid出现时回收
        //tmpid =waitpid(wpid,NULL,WNOHANG); //不阻塞时不sleep时 wpid为0
        printf("parent call the child whose pid is %d\n",tmpid);
    }


    return 0;
}