#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "wrap.h"
#define SERV_PORT 6060

#define CLIT_PORT 6080
/*
socket->connect->write->read->close

*/

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

int main()
{
    int ret,i=7;
    int cfd;
    char buf[BUFSIZ];
    struct sockaddr_in serv_addr,clit_addr;
    socklen_t serv_addr_len;

    cfd = socket(AF_INET,SOCK_STREAM,0);
    if(cfd == -1) sys_err("socket error");

    int opt = 1;
    setsockopt(cfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));//允许ip地址复用

    bzero(&serv_addr, sizeof(serv_addr));       //清空服务器地址结构
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);    
    ret = inet_pton(AF_INET,"192.1",&(serv_addr.sin_addr.s_addr));

    bzero(&clit_addr, sizeof(clit_addr));       //清空客户端地址结构
    clit_addr.sin_family = AF_INET;
    clit_addr.sin_port = htons(CLIT_PORT);    
    ret = inet_pton(AF_INET,"127.1",&(clit_addr.sin_addr.s_addr));
    Bind(cfd,(struct sockaddr *)&clit_addr, sizeof(clit_addr));    //绑定地址结构

    // serv_addr_len = sizeof(serv_addr);
    // printf("getsockname = %d\n",getsockname(cfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)));

    ret = connect(cfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    if(ret == -1) sys_err("connect error");

    while(i--)
    {
        write(cfd,"hello\n",7);

        ret = read(cfd,buf,sizeof(buf));
        write(STDOUT_FILENO,buf,ret);
        sleep(1);
    }

    close(cfd);

}
