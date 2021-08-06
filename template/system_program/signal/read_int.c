#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

//中断系统调用：
    // 系统调用阻塞时捕捉到信号，则系统中断就被中断不再执行
    // 支持自动重启动的函数：ioctl read write readv writev

// 可修改 sa_flags 参数来设置被信号中断后系统调用是否重启
// SA_INTERRURT 不重启。 SA_RESTART 重启

void catch_sig(int signo)
{
    if(signo==SIGINT)
    {
        printf("catch SIGINT\n");
    }
}

int main()
{
    int ret;
    char buf[20];
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_handler=catch_sig;
    act.sa_flags=SA_INTERRUPT;       //支持中断不重启
    // act.sa_flags=SA_RESTART;            //中断重启

    // signal(SIGINT,catch_sig);
    sigaction(SIGINT,&act, NULL);

    ret=read(STDIN_FILENO,buf,sizeof(buf));
    if(ret == -1)
    {
        perror("read error");
        exit(1);
    }
    // pause();
    printf("over\n");
}