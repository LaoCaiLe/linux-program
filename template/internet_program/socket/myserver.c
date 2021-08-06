#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define SERV_PORT 5050
/*
socket->bind->listen->accept->read->write->close
int socket(int domain, int type, int protocol);
*/

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

int main()
{
    int ret,i;
    int cfd,lfd;
    char buf[BUFSIZ];
    char client_ip[BUFSIZ];
    struct sockaddr_in serv_addr;
    struct sockaddr_in clit_addr;
    socklen_t clit_addr_len;

    lfd = socket(AF_INET,SOCK_STREAM, 0); //创建socket套接字

    int opt = 1;
    setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void *)&opt,sizeof(opt));   //端口复用

    serv_addr.sin_family=AF_INET;           //配置服务器地址结构
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
    ret = bind(lfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr));   //绑定一个地址结构 (IP+port)
    if(ret != 0) sys_err("bind error");

    ret = listen(lfd,128);      //设置监听上险
    if(ret != 0) sys_err("listen error");

    clit_addr_len =sizeof(clit_addr);

    printf("wait for client connect ...\n");
    cfd = accept(lfd,(struct sockaddr *)&clit_addr,&clit_addr_len); //阻塞等待客户端连接
    if(cfd == -1) sys_err("accept error");

    printf("client id: %s, port: %d\n", //打印客户端ip和port
            inet_ntop(AF_INET,&clit_addr.sin_addr.s_addr,client_ip,sizeof(client_ip)),
            ntohs(clit_addr.sin_port));

    while(1)
    {
        ret = read(cfd,buf,sizeof(buf));
        write(STDOUT_FILENO,buf,ret);

        for(i=0; i<ret; i++)
            buf[i] = toupper(buf[i]);

        write(cfd,buf,ret);
        
    }

    close(cfd);
    close(lfd);
}