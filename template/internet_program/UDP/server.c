#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/types.h>

#include "wrap.h"

#define SERV_PORT 6060
// size_t recvfrom(int sockfd, void *buf, size_t len, int flags,struct sockaddr *src_addr, socklen_t *addrlen);
// ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
int main()
{
    int ret,lfd;
    char buf[BUFSIZ],clie_IP[BUFSIZ];
    struct sockaddr_in clit_addr,serv_addr;
    socklen_t clitaddr_len;

    lfd = Socket(AF_INET,SOCK_DGRAM,0);
    
    bzero(&serv_addr, sizeof(serv_addr));   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = ntohs(SERV_PORT);
    serv_addr.sin_addr.s_addr = ntohl(INADDR_ANY);

    Bind(lfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    while(1)
    {
        clitaddr_len = sizeof(clit_addr);
        ret = recvfrom(lfd,buf,BUFSIZ,0,(struct sockaddr *)&clit_addr,&clitaddr_len);
        if(ret == -1)
        {
            perror("recvfrom error");
            //exit(1);
        }

        printf("recv from client %s port:%d\n",
            inet_ntop(AF_INET, &clit_addr.sin_addr, clie_IP, sizeof(clie_IP)), 
            ntohs(clit_addr.sin_port));
  

        for(int i=0; i<ret; i++)
        {
            buf[i] = toupper(buf[i]);
        }

        ret = sendto(lfd,buf,ret,0,(struct sockaddr *)&clit_addr,sizeof(clit_addr));
        if(ret == -1)
        {
            perror("sendto error");
            //exit(1);
        }  
      
         
    }
    close(lfd);
}

