#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/types.h>

#include "wrap.h"

#define SERV_PORT 6060
#define SERV_IP "127.0.0.1"

int main()
{
    int connfd,ret;
    struct sockaddr_in serv_addr;
    // socklen_t servaddr_len;
    char buf[BUFSIZ];  

    connfd = Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));                       
    serv_addr.sin_family = AF_INET;                             
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);    
    serv_addr.sin_port = htons(SERV_PORT);                      
    while(1)
    {        
        fgets(buf, sizeof(buf), stdin);
        ret = sendto(connfd, buf, strlen(buf),0,(struct sockaddr *)&serv_addr,sizeof(serv_addr));       ;
        if(ret == -1)
        {
            perror("sendto error");
            exit(1);
        }
        
        ret = recvfrom(connfd, buf, ret,0,NULL,0);   
        if(ret == -1)
        {
            perror("recvfrom error");
            exit(1);
        }
 
        Write(STDOUT_FILENO, buf, ret);
    }
    close(connfd);
}


