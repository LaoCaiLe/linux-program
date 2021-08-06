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
#include <sys/epoll.h>

#include "wrap.h"

#define MAXLINE 80
#define SERV_PORT 6060
#define OPEN_MAX 1024

// int epoll_create(int size);创建红黑树监听节点数量
// int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event); 操作监听红黑树
// int epoll_wait(int epfd, struct epoll_event *events,int maxevents, int timeout); 阻塞监听  
        //events 从内核得到的事件集合
// struct epoll_event {
//                uint32_t     events;    /* Epoll events */
//                epoll_data_t data;      /* User data variable */
//            };

int main(int argc, char* argv[])
{
    int ret,i,j,n;
    int listenfd,connfd,sockfd;      
    char buf[BUFSIZ],client_ip[INET_ADDRSTRLEN]; //INET_ADDRSTRLEN 16
    struct epoll_event temp, ep[1024];  
    struct sockaddr_in clit_addr,serv_addr;
    socklen_t clit_addr_len;    

    listenfd = Socket(AF_INET,SOCK_STREAM, 0);
   
    int opt = 1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    bzero(&serv_addr, sizeof(serv_addr));       //清空服务器地址结构
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);    

    Bind(listenfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr));

    Listen(listenfd,128);

    int epfd = epoll_create(1024);  //创建一颗监听红黑树

    temp.data.fd = listenfd;
    temp.events = EPOLLIN;

    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &temp);    //操作监听红黑树

    while(1)
    {       
        ret = epoll_wait(epfd, ep, 1024, -1);
        for(i=0; i<ret; i++) //有客户端链接请求
        {
            if(ep[i].data.fd == listenfd)
            {
                clit_addr_len = sizeof(clit_addr);
                connfd = Accept(listenfd,(struct sockaddr *)&clit_addr,&clit_addr_len);
                printf("connect client Ip: %s, port:%d\n",
                    inet_ntop(AF_INET,(void *)&clit_addr.sin_addr.s_addr,client_ip,sizeof(client_ip)),
                    ntohs(clit_addr.sin_port)
                );

                temp.data.fd = connfd;
                temp.events = EPOLLIN;
                epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&temp);
            }
            else
            {
                sockfd = ep[i].data.fd;
                n = Read(sockfd,buf,BUFSIZ);
                if(n==0)
                {
                    epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,&temp);
                    printf("client[%d] closed connection\n",i);
                    Close(sockfd);
                    continue;
                }

                for(j=0; j<n; j++)
                    buf[j] = toupper(buf[j]);
                write(sockfd,buf,n);
                write(STDOUT_FILENO,buf,n);
            }
        }

          
    }    
    return 0;
}

//poll
    // 优点：
        // 高效

    // 缺点： 
        // 不能跨平台 只能在Linux系统使用
        
