#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

// 这个 daemon 进程就不会受到用户登录注销影响。
// 要想终止，就必须用 kill 命令
void syserror(char *str)
{
    perror(str);
    exit(1);
}

int main()
{
    pid_t pid;
    int fd, ret;

    pid  = fork();
    if(pid>0)
    {
        exit(0); //父进程为组长 退出
    }

    pid = setsid();   //创建新会话
    if(pid == -1)
    {
       syserror("setsid error");
    }

    ret = chdir("/home/laocaile/template"); //改变工作目录
    if(ret == -1)
    {
        syserror("chdir error");
    }

    umask(0022);    //改变文件访问权限

    close(STDIN_FILENO);

    fd = open("/dev/null",O_RDWR);
    if(fd == -1)
    {
        syserror("open error");
    }

    dup2(fd,STDOUT_FILENO);
    dup2(fd,STDERR_FILENO);

    while(1); //模拟守护进程业务
}