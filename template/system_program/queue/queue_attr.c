#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>

  /*


    int mq_getattr(mqd_t mqdes, struct mq_attr *attr);  //获取attr结构体

    int mq_setattr(mqd_t mqdes, const struct mq_attr *newattr,struct mq_attr *oldattr); //设置attr结构体
    此时attr只指向mq_attr中的mq_flags成员，其他三个成员被忽略
    每个队列的最大消息树和每个消息的最大字节数只能在创建队列时设置

    struct mq_attr {
               long mq_flags;        Flags (ignored for mq_open()) 
               long mq_maxmsg;       Max. number of messages on queue 
               long mq_msgsize;      Max. message size (bytes) 
               long mq_curmsgs;      number of messages currently in queue
                                       (ignored for mq_open()) 
           }

   */

#define mq_name "/msg_queue"

int main()
{
    mqd_t fd;
    fd = mq_open(mq_name,O_RDWR|O_CREAT,0644,NULL);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }

    struct mq_attr mqAttr;
    if (mq_getattr(fd, &mqAttr) < 0)
    {
        perror("getattr error");
        return -1;
    }
    printf("mq_flags:%ld\n",mqAttr.mq_flags);    
    printf("mq_msgsize;:%ld\n",mqAttr.mq_msgsize);
    printf("mq_maxmsg:%ld\n",mqAttr.mq_maxmsg);
    printf("mq_curmsgs;:%ld\n",mqAttr.mq_curmsgs);

    printf("-----------------------\n");
    struct mq_attr set_Attr;
    set_Attr.mq_flags = O_NONBLOCK;
    set_Attr.mq_curmsgs = 1;
    set_Attr.mq_maxmsg = 1024;
    set_Attr.mq_msgsize = 64;

    int ret = mq_setattr(fd, &set_Attr, &mqAttr);
    if(ret == -1)
    {
        perror("setattr error");
        exit(1);
    }
    
    //除mq_flags之外其他成员变量不会改变
    // 每个队列的最大消息树和每个消息的最大字节数只能在创建队列时设置
    printf("mq_flags:%ld\n",mqAttr.mq_flags);    
    printf("mq_msgsize;:%ld\n",mqAttr.mq_msgsize);
    printf("mq_maxmsg:%ld\n",mqAttr.mq_maxmsg);
    printf("mq_curmsgs;:%ld\n",mqAttr.mq_curmsgs);

    mq_close(fd);
}