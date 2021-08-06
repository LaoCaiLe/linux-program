#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>

#define mq_name "/mqueue.1234"

/*  
创建队列或打开已经存在的队列
    mqd_t mq_open(const char *name, int oflag);
    mqd_t mq_open(const char *name, int oflag, mode_t mode,struct mq_attr *attr);
    name: 路径名
    oflag：O_RDWR/O_RDONLY/O_WRONLY
    返回值：
        成功：返回一个消息队列描述符
        失败：-1、errno
        
关闭队列
    int mq_close(mqd_t mqdes);
    成功返回0
    失败返回-1 errno
    */


 
int main()
{
    // struct mq_attr attr;
    mqd_t qfd;
    qfd = mq_open(mq_name,O_RDWR|O_CREAT,0644,NULL);
    if(qfd == -1)
    {
        perror("mq_open error");
    }
    
    mq_close(qfd);

    // int msgid;
    // msgid = msgget(IPC_PRIVATE,0777);
    // if(msgid < 0)
    // {
    //     perror("msgget error");
    //     exit(1);
    // }
    // printf("creat message success msgid = %d\n",msgid);
    // system("ipcs -q");

    return 0;
}