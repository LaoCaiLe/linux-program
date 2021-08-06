#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#define SERV_IP "192.168.21.1"

int main(int argc, char* argv[])
{
    int ret,connfd;

    struct sockaddr_in servaddr;
    char buf[BUFSIZ];


    connfd = socket(AF_INET,SOCK_STREAM,0);
    
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    // clitaddr.sin_addr.s_addr = htonl(SERV_IP);
    servaddr.sin_port = htons(13);    
    inet_pton(AF_INET,SERV_IP,&servaddr.sin_addr.s_addr);

    ret = connect(connfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    if(ret == -1) 
    {
        perror("connect error");
        exit(1);
    }

    while((ret = read(connfd,buf,sizeof(buf)))>0)
    {
        // write(connfd,"hello\n",7);

        buf[ret]=0;
        if(fputs(buf,stdout) == EOF)
        {
            perror("puts error");
            exit(1);
        }
        // write(STDOUT_FILENO,buf,ret);
        // sleep(1);
    }

    if(ret < 0 )
    {
        perror("read error");
        exit(1);
    }

    close(connfd);

}