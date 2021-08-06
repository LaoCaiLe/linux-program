#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
    int listenfd,connfd;
    socklen_t len;
    struct sockaddr_in servaddr,clitaddr;
    char buf[BUFSIZ];
    time_t ticks;

    listenfd = socket(AF_INET,SOCK_STREAM,0);
    
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13);

    bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

    listen(listenfd,128);

    while(1)
    {
        len = sizeof(clitaddr);
        printf(".....accepting.....\n");
        connfd = accept(listenfd,(struct sockaddr*)&clitaddr,&len);

        printf("connection from %s, port %d\n",
                inet_ntop(AF_INET,&clitaddr.sin_addr,buf,sizeof(buf)),
                ntohs(clitaddr.sin_port)
        );

        ticks = time(NULL);
        snprintf(buf,sizeof(buf),"%.24s\r\n",ctime(&ticks));
        write(connfd,buf,sizeof(buf));

        close(connfd);
    }
}