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

#include "wrap.h"

#define SERV_PORT 6060

//socket->connct->wirte->read

int main()
{
    int ret,i=10;
    int cfd;
    struct sockaddr_in clit_addr;
    socklen_t clit_addr_len;
    char buf[BUFSIZ];
    

    cfd = Socket(AF_INET,SOCK_STREAM,0);

    clit_addr.sin_family = AF_INET;
    clit_addr.sin_port = htons(SERV_PORT);   
    inet_pton(AF_INET,"127.1",&clit_addr.sin_addr.s_addr);

    clit_addr_len = sizeof(clit_addr);
    Connect(cfd,(struct sockaddr *)&clit_addr,clit_addr_len);

    while(i--)
    {
        fgets(buf, sizeof(buf), stdin);
        
        Write(cfd,buf,strlen(buf));
 
        ret = Read(cfd,buf,BUFSIZ);

        Write(STDOUT_FILENO,buf,ret);

        sleep(1);
    }

    close(cfd);


}