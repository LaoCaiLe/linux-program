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
#include <sys/un.h>
#include <stddef.h>
#include "wrap.h"

#define SERV_PATH "serv.socket"

int main()
{
    int lfd,cfd,len;
    int i,n;
    char buf[BUFSIZ];
    struct sockaddr_un serv_addr,clit_addr;

    lfd = Socket(AF_UNIX,SOCK_STREAM,0);

    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path,SERV_PATH);

    // offsetof(struct sockaddr_un, sun_path)   自动计算偏移位数//2
    len = offsetof(struct sockaddr_un, sun_path)+ strlen(serv_addr.sun_path);
  
    unlink(SERV_PATH);
    Bind(lfd,(struct sockaddr *)&serv_addr, len);
    Listen(lfd, 20);
    printf("connecting......\n");
    while (1)
    {
        len = sizeof(clit_addr);
        cfd = Accept(lfd,(struct sockaddr*)&clit_addr,(socklen_t *)&len);

        len -=  offsetof(struct sockaddr_un, sun_path);
        clit_addr.sun_path[len]='\0';

        printf("client's path is %s\n",clit_addr.sun_path);
   
        while((n = Read(cfd,buf,BUFSIZ) )>=0)
        {
            // if(n==0)
            // {
            //     printf("client %s is closed\n",clit_addr.sun_path);
            // }
            for(i=0; i<n; i++)
            {
                buf[i] = toupper(buf[i]);
            }
            Write(cfd,buf,n);
            Write(STDOUT_FILENO,buf,n);
        }
        close(cfd);
    }
    unlink(SERV_PATH);
    close(lfd);
    

}
