#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>

/* int mq_send(mqd_t mqdes, const char *msg_ptr,size_t msg_len, unsigned int msg_prio);
    msg_prio: 待发送消息的优先级
    返回失败：-1 errno

ssize_t mq_receive(mqd_t mqdes, char *msg_ptr,size_t msg_len, unsigned int *msg_priop);
    msg_priop：返回消息的优先级，通过该指针存放
    返回失败：-1 errno

如果不须指定优先级，则设置mq_send()的 msg_prio 为0； mq_receive()的msg_priop 为NULL

*/
#define mq_name "/test"
int main(int argc, char *argv[])
{
    int ret;
    char buf[BUFSIZ];
    unsigned int priop;
    mqd_t fd;
    fd = mq_open(mq_name,O_RDWR | O_NONBLOCK | O_CREAT, 0644, NULL);
    if(fd == -1)
    {
        perror("open mq error");
        exit(1);
    }

    ret = mq_receive(fd, buf, sizeof(buf), &priop);
    if(ret == -1)
    {
        perror("mq_receive error");
        exit(1);
    }
    write(STDOUT_FILENO,buf,ret);
    printf(" priority = %d \n",priop);
    

    mq_close(fd);
}

//消息队列会接收最高优先级的最早消息