#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
// 使进程执行某一程序。成功无返回值，失败返回 -1
//open execl dup2
int main()
{
    int fd;
    pid_t pid ;
    
    fd = open("ps_aux.txt",O_RDWR|O_CREAT|O_TRUNC,0644);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }

    dup2(fd,STDOUT_FILENO);

    pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        exit(1);
    }
    else if(pid==0)     //子进程返回0
    {
       execlp("ps","ps","aux",NULL);
       perror("execl error");
       exit(1);
    }   
    else if(pid>0)    //父进程返回子进程id
    {
        
    }

    close(fd);

    return 0;
}