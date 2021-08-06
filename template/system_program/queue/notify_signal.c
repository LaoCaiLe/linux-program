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
#include <signal.h>

#define mq_name "/test"

mqd_t fd;
void *buff;
struct mq_attr attr;
struct sigevent sigev;

/*  int mq_notify(mqd_t mqdes, const struct sigevent *sevp);
    成功： 0
    失败：-1

    struct sigevent {
        
        //signal
        int          sigev_notify; 
        int          sigev_signo;  

        //thread
        union sigval sigev_value;  
        void       (*sigev_notify_function) (union sigval);                        
        void        *sigev_notify_attributes;                            
        pid_t        sigev_notify_thread_id;                       
    };
    sigev_notify: SIGRV_NONE/SIGEV_SIGNAL/SIGEV_THREAD
    sigev_signo: signal_number if SIGEV_SIGNAL

    union sigval {          Data passed with notification 
           int     sival_int;         Integer value 
           void   *sival_ptr;         Pointer value 
    };

*/

static void sig_usr1(int signo)
{
    ssize_t n;
    mq_notify(fd, &sigev);
    n = mq_receive(fd,buff,attr.mq_msgsize,NULL);
    if(n<=0)
    {
        perror("rece error");
        exit(1);
    }
    printf("sigusr1 received, read data %s\n",(char *)buff);
    return ;
}

int main(int argc,char *argv[])
{

    int ret;
    fd = mq_open(argv[1],O_RDONLY|O_CREAT, 0644, NULL);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }
    mq_getattr(fd,&attr);
    buff = malloc(attr.mq_msgsize);

    signal(SIGUSR1,sig_usr1);
    sigev.sigev_notify = SIGEV_SIGNAL;
    sigev.sigev_signo = SIGUSR1;
    ret = mq_notify(fd,&sigev);
    if(ret ==-1)
    {
        perror("notify error");
        exit(1);
    }

    for(;;)
    {
        pause();
    }
    
    exit(0);
}