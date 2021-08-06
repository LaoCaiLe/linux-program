#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

//   int sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);
/*
struct sigaction {
    void     (*sa_handler)(int);
    void     (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t   sa_mask; //只作用于信号捕捉函数执行期间
    int        sa_flags;
    void     (*sa_restorer)(void);
};
*/


//捕捉函数 返回值为空 参数类型为int
void sig_catch(int sigo)
{
    if(sigo == SIGINT)
    {
        printf("catch SIGINT! \n");
        sleep(5);
        printf("catch SIGINT finish\n");
    }
    else if(sigo == SIGQUIT)
         printf("catch SIGQUIT! \n");
}

int main()
{
    struct sigaction act,oldact;

    act.sa_handler = sig_catch; //设置回调函数
    sigemptyset(&act.sa_mask);  //清空sa_mask屏蔽字 旨在sig_catch工作时有效
   // sigaddset(&act.sa_mask, SIGQUIT);
    act.sa_flags = 0;           //默认值

    //注册捕捉信号函数 捕捉函数为sig_catch
    int ret = sigaction(SIGINT,&act, &oldact);
    if(ret == -1)
    {
        perror("sigaction error");
        exit(1);
    }

    ret = sigaction(SIGQUIT,&act, &oldact);
    if(ret == -1)
    {
        perror("sigaction error");
        exit(1);
    }



    while(1);

    return 0;
}