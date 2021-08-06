#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <sys/wait.h>

#include "wrap.h"

#define SERV_PORT 6060

void catch_child(int sino)
{
    while((waitpid(0,NULL,WNOHANG)) >0);

    return ;
}

int main()
{
    int lfd,cfd;
    int ret,i;
    pid_t pid;
    char buf[BUFSIZ],client_addr[BUFSIZ];
    struct sockaddr_in serv_addr,clit_addr;  
    struct sigaction act;
    socklen_t clit_addr_len;

    lfd = Socket(AF_INET,SOCK_STREAM,0);

    int opt = 1;
    setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void *)&opt,sizeof(opt));   //端口复用

    // memset(&serv_addr,0,sizeof(serv_addr));  //将地址结构清零
    bzero(&serv_addr,0);    //将地址结构清零

    //配置服务器地址结构
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htons(INADDR_ANY);    

    Bind(lfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

    Listen(lfd,128);

    clit_addr_len = sizeof(clit_addr); //计算客户端地址结构长度

    act.sa_handler = catch_child; //设置回调函数
    sigemptyset(&act.sa_mask);  //清空sa_mask屏蔽字 旨在sig_catch工作时有效
    act.sa_flags = 0;           //默认值

    //注册捕捉信号函数 捕捉函数为sig_catch
    sigaction(SIGCHLD,&act, NULL);
      

    while (1)
    {   
        
        cfd = Accept(lfd,(struct sockaddr *)&clit_addr,&clit_addr_len); //阻塞等待接收客户端
        
        printf("client ip: %s, port = %d\n",
            inet_ntop(AF_INET,&clit_addr.sin_addr.s_addr,client_addr,sizeof(client_addr)),
            ntohs(clit_addr.sin_port)
            );

        pid = fork();

        if(pid < 0)
        {
            perr_exit("fork error");
        }
        else if(pid == 0)
        {
            close(lfd);
            break;
        }
        else if(pid > 0)
        {    
            close(cfd);       
        }
    }

    if(pid == 0)
    {
        for(;;)
        {
            ret = Read(cfd,buf,sizeof(buf));
            if(ret == 0)
            {
                close(cfd);
                exit(1);
            }

            for(i=0; i<ret; i++)
            {
                buf[i] = toupper(buf[i]);
            }

            write(cfd,buf,ret);
            write(STDOUT_FILENO,buf,ret);
        }
    }    
}

