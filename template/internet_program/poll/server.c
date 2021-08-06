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
#include <poll.h>

#include "wrap.h"

#define MAXLINE 80
#define SERV_PORT 6060
#define OPEN_MAX 1024

//int poll(struct pollfd *fds, nfds_t nfds, int timeout);
// struct pollfd {
//                int   fd;         /* 待监听的文件描述符 */
//                short events;     /* 待监听的文件描述符对映的监听事件POLLIN POLLOUT POLLERR */
//                short revents;    /* 传入时 给0 如果满足对应事件， 返回 非0--> POLLIN POLLOUT POLLERR*/
//    };

// POLLIN			普通或带外优先数据可读,即POLLRDNORM | POLLRDBAND
// POLLRDNORM		数据可读
// POLLRDBAND		优先级带数据可读
// POLLPRI 		    高优先级可读数据
// POLLOUT		    普通或带外数据可写
// POLLWRNORM		数据可写
// POLLWRBAND		优先级带数据可写
// POLLERR 		    发生错误
// POLLHUP 		    发生挂起
// POLLNVAL 		描述字不是一个打开的文件

int main(int argc, char* argv[])
{
    int ret,i,j,n;
    int listenfd,connfd,maxi,sockfd;      
    char buf[BUFSIZ],client_ip[INET_ADDRSTRLEN]; //INET_ADDRSTRLEN 16
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in clit_addr,serv_addr;
    socklen_t clit_addr_len;

    listenfd = Socket(AF_INET,SOCK_STREAM,0 );
   
    int opt = 1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    bzero(&serv_addr, sizeof(serv_addr));       //清空服务器地址结构
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);    

    Bind(listenfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr));

    Listen(listenfd,128);

    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;  /* listenfd监听普通读事件 */

    for(i=1; i<OPEN_MAX; i++)
    {
        client[i].fd = -1;
    }
    maxi = 0;
    
    while(1)
    {
        
        ret = poll(client,maxi+1,-1);  //阻塞监听

        if(client[0].revents & POLLRDNORM) //有客户端链接请求
        {
            clit_addr_len = sizeof(clit_addr);
            connfd = Accept(listenfd,(struct sockaddr *)&clit_addr,&clit_addr_len);
            printf("connect client Ip: %s, port:%d\n",
                inet_ntop(AF_INET,(void *)&clit_addr.sin_addr.s_addr,client_ip,sizeof(client_ip)),
                ntohs(clit_addr.sin_port)
            );

            for(i=1; i<OPEN_MAX; i++)
            {
                if(client[i].fd < 0)
                {
                    client[i].fd = connfd;
                    client[i].events = POLLRDNORM;
                    break;
                }
            }

            if(i == OPEN_MAX) perr_exit("too many clients");

            if(i > maxi) maxi = i;

            if(--ret <= 0) continue;            
        }

        for(i=1; i<=maxi; i++)
        {
            if((sockfd = client[i].fd) <0) continue;

            if(client[i].revents & (POLLRDNORM | POLLERR))      //有读事件请求
            {
                if((n = Read(sockfd, buf, MAXLINE)) < 0 )
                {
                    if(errno == ECONNRESET)
                    {
                        printf("client[%d] aborted connection\n",i);
                        Close(sockfd);
                        client[i].fd = -1;
                    }
                    else
                    {
                        perr_exit("read error");
                    }
                }
                else if(n == 0)
                {
                    printf("client[%d] closed connection\n",i);
                    Close(sockfd);
                    client[i].fd = -1;
                }
                else
                {
                    for(j=0; j<n; j++)                    
                        buf[j] = toupper(buf[j]);
                    Writen(sockfd, buf, n);
                    Write(STDOUT_FILENO, buf, n);
                }

                if(--ret <= 0)
                    break;
            }
        }     
    }    
    return 0;
}

//poll
    // 优点：
        // 自带数组结构  
        // 可以将监听事件和返回事件分离
        // 拓展监听上限

    // 缺点： 
        // 不能跨平台 只能在Linux系统使用
        // 无法直接定位监听事件文件描述符，编码难度大

