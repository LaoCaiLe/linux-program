#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

mqd_t fd;
struct mq_attr attr;
struct sigevent sigev;

static void notify_thread(union sigval arg)
{
    ssize_t n;
    void *buf;

    printf("notify_thread started\n");
    buf = malloc(attr.mq_msgsize);

    mq_notify(fd,&sigev);

    while(( n = mq_receive(fd,buf,attr.mq_msgsize,NULL)) >= 0)
    {
        printf("read date : %s\n",(char *)buf);
    }

    if(errno != EAGAIN)
    {
        perror("mq_receive error");
        exit(1);
    }

    free(buf);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    fd = mq_open(argv[1],O_RDONLY | O_CREAT, 0644, NULL);
    if(fd == -1)
    {
        perror("mq_open error");
        exit(1);
    }

    mq_getattr(fd,&attr);
    
    sigev.sigev_notify = SIGEV_THREAD;
    sigev.sigev_value.sival_ptr = NULL;
    sigev.sigev_notify_function = notify_thread;
    sigev.sigev_notify_attributes = NULL;

    mq_notify(fd, &sigev);

    while(1)
    {
        pause();
    }
    exit(0);

}