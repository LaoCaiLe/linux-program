#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define SERV_PORT 6060
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
    struct sockaddr_in serv_addr;

    cfd = socket(AF_INET,SOCK_STREAM,0);
    if(cfd == -1) sys_err("socket error");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);    
    ret = inet_pton(AF_INET,"127.1",&(serv_addr.sin_addr.s_addr));

    ret = connect(cfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    if(ret != 0) sys_err("connect error");

    while(i--)
    {
        write(cfd,"hello\n",7);

        ret = read(cfd,buf,sizeof(buf));
        write(STDOUT_FILENO,buf,ret);
        sleep(1);
    }

    close(cfd);

}
