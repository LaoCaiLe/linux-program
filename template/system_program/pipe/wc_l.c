#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

//int pipe(int pipefd[2]); fd[0]为读端 fd[1]为写端
// pipe dup2 exec

int main(int argc, char *argv[])
{
    int ret;
    pid_t pid;
    int fd[2];

    ret = pipe(fd);
    if(ret == -1)
    {
        perror("pipe error");
        exit(1);
    }

    pid = fork();
    if(pid==-1)
    {
        perror("forl error");
        exit(1);
    }
    else if(pid == 0)//子进程 read
    {
        close(fd[0]);   //关闭读端
        
        dup2(fd[1],STDOUT_FILENO);
        
        execlp("ls","ls",NULL);
        perror("execlp error");
        exit(1);

        //close(fd[1]);
        

    }
    else if(pid > 0) //父进程 write
    {
        close(fd[1]); //关闭写端

        dup2(fd[0],STDIN_FILENO);
        
        execlp("wc","wc","-l",NULL);
        perror("execlp error");
        exit(1);

        //close(fd[0]);
    }
}
