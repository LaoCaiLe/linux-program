#include <stdio.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <pthread.h>
#include <ctype.h> 
#include <unistd.h> 
#include <fcntl.h>


#include "wrap.h"

#define SERV_PORT 6060

void *child_thread(void *arg)
{
    int fd = *(int *)arg;
    int ret,i;
    char buf[BUFSIZ];
    while(1)
    {
        ret = Read(fd,buf,BUFSIZ);
        if(ret == 0)
        {
            printf("the %d client is closed\n",fd);
            break;
        }

        for(i=0; i<ret; i++)
            buf[i] = toupper(buf[i]);
        
        Write(fd,buf,ret);
        Write(STDOUT_FILENO,buf,ret);
    }
    close(fd);
    pthread_exit(NULL);
}

int main()
{
    int lfd,cfd;
    pthread_t tid;
    char client_addr[BUFSIZ];
    struct sockaddr_in serv_addr,clit_addr; 
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

    while (1)
    {   
        
        cfd = Accept(lfd,(struct sockaddr *)&clit_addr,&clit_addr_len); //阻塞等待接收客户端
        
        printf("client ip: %s, port = %d\n",
            inet_ntop(AF_INET,&clit_addr.sin_addr.s_addr,client_addr,sizeof(client_addr)),
            ntohs(clit_addr.sin_port)
            );

        pthread_create(&tid,NULL,child_thread,(void *)&cfd);
        pthread_detach(tid);
        
        
    }

    close(cfd);
    close(lfd);

      
}

