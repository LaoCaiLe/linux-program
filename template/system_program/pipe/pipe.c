#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

//int pipe(int pipefd[2]); fd[0]为读端 fd[1]为写端
// On success, zero is returned.  On error, -1 is returned, and  errno  is set appropriately.

//int pipe2(int pipefd[2], int flags);

int main(int argc,char* argv[])
{
    int ret;
    int fd[2];
    pid_t pid;
    char *buf="hello pipe\n";
    char readbuf[1024];

    ret = pipe(fd);
    if(ret==-1)
    {
        perror("pipe error");
        exit(1);

    }

    pid = fork();
    if(pid==0)//子进程 read
    {
        
        close(fd[1]);

        ret = read(fd[0],readbuf,sizeof(readbuf)); //read 会阻塞等待
        printf("child ret = %d\n",ret);
        //printf("child read buf %s\n",readbuf);
        //write(STDOUT_FILENO,readbuf,ret);
        close(fd[0]);
    }
    else if(pid>0)//父进程 write
    {
        
        close(fd[0]);
        sleep(2);
        //ret = write(fd[1],buf,strlen(buf));

        close(fd[1]);

    }
    else
    {
        perror("fork error");
        exit(1);
    }

    return 0;
    

}