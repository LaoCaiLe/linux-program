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
//一个进程最多打开1024个文件描述符
/*
int select(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, struct timeval *timeout); 
nfds：监听的所有文件描述符中，最大文件描述符+1 
readfds： 读 文件描述符监听集合。 传入、传出参数 
writefds：写 文件描述符监听集合。 传入、传出参数 NULL 
exceptfds：异常 文件描述符监听集合 传入、传出参数 NULL 
timeout： 
    > 0: 设置监听超时时长。
    NULL: 阻塞监听 
    0： 非阻塞监听，轮询 
返回值： 
    > 0: 所有监听集合（3 个）中， 满足对应事件的总数。 
    0： 没有满足监听条件的文件描述符 
    -1： errno
*/

int main(int argc, char* argv[])
{
    int ret,i,j,n;
    int listenfd,connfd,maxfd;      
    //int client[FD_SETSIZE];     //FD_SETSIZE 1024
    fd_set inset,outset;
    char buf[BUFSIZ],client_ip[INET_ADDRSTRLEN]; //INET_ADDRSTRLEN 16
    struct sockaddr_in clit_addr,serv_addr;
    socklen_t clit_addr_len;

    listenfd = Socket(AF_INET,SOCK_STREAM,0 );
    maxfd = listenfd;   //最大文件描述符 

    int opt = 1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));//允许ip地址复用

    bzero(&serv_addr, sizeof(serv_addr));       //清空服务器地址结构
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);    
    //INADDR_ANY 其值一般为0，告知内核去选择IP地址，适用于ipv4

    Bind(listenfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr));    //绑定地址结构

    Listen(listenfd,128);   //设置监听上限

    FD_ZERO(&inset);            //清空读集合
    FD_SET(listenfd,&inset);    //将监听fd添加到读集合中
    
    while(1)
    {
        outset = inset;
        ret = Select(maxfd+1, &outset,NULL,NULL,NULL);  //使用select监听
        if(ret == -1)
        {
            perror("select error");
            exit(1);
        }
        if(FD_ISSET(listenfd,&outset))
        {
            clit_addr_len = sizeof(clit_addr);
            connfd = Accept(listenfd,(struct sockaddr *)&clit_addr, &clit_addr_len); //建立连接 不会阻塞
            printf("connect client Ip: %s, port:%d\n",
                    inet_ntop(AF_INET,(void *)&clit_addr.sin_addr.s_addr,client_ip,sizeof(client_ip)),
                    ntohs(clit_addr.sin_port)
                );

            FD_SET(connfd,&inset);      //将新产生的fd添加到读集合中，监听数据读事件

            if(maxfd < connfd) maxfd = connfd;
             
            if(ret == 1) continue;      //ret=1只有listenfd事件 
        }

        for(i=listenfd+1; i<=maxfd; i++)       //处理读事件的fd
        {
            if(FD_ISSET(i,&outset))
            {
                n = Read(i,buf,sizeof(buf));    //不会阻塞
                if(n==0)     //检车客户端是否关闭
                {
                    Close(i);       
                    printf("client:%d is closed\n",i);
                    FD_CLR(i,&inset);       //将关闭的fd移除读集合
                }
                if(n>0)
                {
                    for(j=0; j<n; j++)
                    {
                        buf[j] = toupper(buf[j]);
                    }
                    write(i,buf, n);
                    write(STDOUT_FILENO,buf, n);
                }               
            }           
        }
    }    
}

//select
    // 优点： 跨平台 --win,linux,maxOS,__uintmax_t

    // 缺点： 监听文件上限受文件描述符限制（一个进程）最大为1024
    //       检测满足条件的fd，自己添加业务逻辑提高小，提高了编码难度