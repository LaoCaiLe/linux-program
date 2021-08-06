#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

void sys_error(char *str)
{
    perror(str);
    exit(1);
}

void catch_son(int sino)
{
    pid_t wpid;

    //循环接受，防止僵尸进程出现
    while((wpid = wait(NULL))!=-1){
     
        printf("------------------catch %d child\n",wpid);
    }
}

int main()
{
    pid_t pid;
    int i;

    //设置阻塞，防止信号还没注册完，子进程就结束而没有被回收
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGCHLD);

    sigprocmask(SIG_BLOCK,&set,NULL);

    for(i=0; i<20; i++)
    {
        if((pid = fork())==0)
            break;        
    }

    if(i==20)
    {
        
        struct sigaction act;
        act.sa_handler = catch_son;
        sigemptyset(&act.sa_mask);
        sigaddset(&act.sa_mask,SIGCHLD);
        act.sa_flags = 1;

        //注册信号
        sigaction(SIGCHLD,&act,NULL);
        printf("I'm parent id %d\n",getpid());
        //解除阻塞
        sleep(3);
        sigprocmask(SIG_UNBLOCK,&set,NULL);
        while(1);
    }

    else
    {    
        printf("I'm child id %d\n",i);
    }
}