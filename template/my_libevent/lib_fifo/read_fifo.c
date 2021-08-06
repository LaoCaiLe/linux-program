#include <event2/event.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

void read_cb(evutil_socket_t fd, short what, void *arg)
{
    char buf[20];    
    read(fd,buf,sizeof(buf));
    printf("event read_mode is %s\n",what & EV_READ ? "yes":"no");
    printf("read_buf is %s\n",buf);
}
int main()
{
    unlink("lib.fifo");
    //创建管道文件
    mkfifo("lib.fifo",0644);

    int fd = open("lib.fifo",O_RDONLY| O_NONBLOCK);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }

    //创建event_base
    struct event_base *base = event_base_new(); ;

    //创建事件
    struct event *event = event_new(base, fd, EV_READ | EV_PERSIST, read_cb, NULL);

    //添加事件到base
    event_add(event,NULL);

    //监听事件
    event_base_dispatch(base);  //while(1) epoll_wait();

    //释放事件和base
    event_base_free(base);
    event_free(event);

    //关闭管道
    close(fd);
}