#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

/*
wait 函数： 回收子进程退出资源
---------阻塞回收任意一个---------

pid_t wait(int *status)
参数：（传出） 回收进程的状态。
返回值：成功： 回收进程的 pid
失败： -1， errno
函数作用 1： 阻塞等待子进程退出
函数作用 2： 清理子进程残留在内核的 pcb 资源
函数作用 3： 通过传出参数，得到子进程结束状
*/

/*
获取子进程正常终止值：
WIFEXITED(status) --》 为真 --》调用 WEXITSTATUS(status) --》 得到子进程退出值
获取导致子进程异常终止信号：
WIFSIGNALED(status) --》 为真 --》调用 WTERMSIG(status) --》 得到导致子进程异常终止的信号编号
*/

int main()
{    
   
    pid_t pid,w_pid;
    int status;

    pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        exit(1);
    }
    else if(pid==0)     //子进程返回0
    {
       printf("child pid is doing\n");
       sleep(10);
       printf("child die!\n");
       return 16;
    }   
    else if(pid>0)    //父进程返回子进程id
    {
        w_pid = wait(&status);// 如果子进程未终止,父进程阻塞在这个函数上
        if(w_pid == -1)
        {
            perror("wait error");
            exit(1);
        }
        if(WIFEXITED(status))   //为真,说明子进程正常终止.
        {
            printf("child exit with %d\n",WEXITSTATUS(status));
        }
        if(WIFSIGNALED(status)) //为真,说明子进程是被信号终止.(kill -9 xxx)
        {
            printf("child exit with signal %d\n",WTERMSIG(status));
        }

        printf("parent wait finish!\n");
    }

    return 0;

}