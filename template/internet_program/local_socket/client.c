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

#define CLIT_PAHT "clit.socket"
#define SERV_PATH "serv.socket"
int main()
{
    int cfd,len;
    char buf[BUFSIZ];
    struct sockaddr_un serv_addr,clit_addr;

    cfd = Socket(AF_UNIX,SOCK_STREAM,0);

    bzero(&clit_addr, sizeof(clit_addr));
    clit_addr.sun_family = AF_UNIX;
    strcpy(clit_addr.sun_path,CLIT_PAHT);
    len = offsetof(struct sockaddr_un, sun_path)+ strlen("clit.socket");

    unlink(CLIT_PAHT);
    Bind(cfd,(struct sockaddr *)&clit_addr, len);
    
  
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path,SERV_PATH);
    len = offsetof(struct sockaddr_un, sun_path)+ strlen("serv.socket");

    connect(cfd,(struct sockaddr *)&serv_addr,len);

    while(1)
    {
        fgets(buf,sizeof(buf), stdin);
        write(cfd,buf,strlen(buf));

        len = read(cfd,buf,sizeof(buf));
        write(STDOUT_FILENO,buf,len);
    }
}