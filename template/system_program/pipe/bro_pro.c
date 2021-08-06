#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

//int pipe(int pipefd[2]); fd[0]为读端 fd[1]为写端
// pipe dup2 exec

int main(int argc, char *argv[])
{
    int fd[2],ret;
    pid_t pid;
    int i;

    ret = pipe(fd);
    if(ret == -1)
    {
        perror("pipe error");
        exit(1);
    }

    for(i=0; i<2;i++)
    {
        pid = fork();
        if(pid<0)
        {
            perror("fork error");
            exit(1);
        }
        if(pid == 0)
        {
            break;
        }
    }

    switch(i)
    {
        case 0: //兄    ls
            printf("I'm bro %d\n",getpid());
            close(fd[0]);//关闭读

            ret = dup2(fd[1],STDOUT_FILENO);            
            if(ret == -1)
            {
                perror("dup2 error");
                exit(1);
            }

            execlp("ls","ls",NULL);
            perror("execlp ls error");
            exit(1);
     

        case 1: //弟    wc -l
            close(fd[1]); //关闭写

            printf("I'm didi %d\n",getpid());

            ret = dup2(fd[0],STDIN_FILENO);   
            if(ret == -1)
            {
                perror("dup2 error");
                exit(1);
            }

            execlp("wc","wc","-l",NULL);
            perror("execlp wc error");
            exit(1);
     

        case 2: //爹
        //---------------------
            close(fd[1]);
            close(fd[0]);
        //---------------------
        //管道只能有一个读端一个写端 保证单向流动
        
            printf("I'm dad %d\n",getpid());
            while((ret=waitpid(-1,NULL,0))!=-1)
            {
                if(ret>0)
                    printf("dad wait %dth child\n",ret);
                else if(ret == -1)
                {
                    perror("waitpid error");
                    exit(1);
                }
            }
            break;

    }

    return 0;
}