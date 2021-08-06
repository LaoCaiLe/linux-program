#include <event2/event.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

void write_cb(evutil_socket_t fd, short what, void *arg)
{
    char buf[20] = "hello libevent!\n";
    write(fd,buf,strlen(buf)+1);
    sleep(1);
}
int main()
{
    int fd = open("lib.fifo",O_WRONLY| O_NONBLOCK);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }

    //创建event_base
    struct event_base *base;
    base = event_base_new();

    //创建事件
    struct event *event;
    event = event_new(base, fd, EV_WRITE | EV_PERSIST, write_cb, NULL);

    //添加事件到base
    event_add(event,NULL);

    //监听事件
    event_base_dispatch(base);

    //释放事件和base
    event_base_free(base);
    event_free(event);

    //关闭管道
    close(fd);
}