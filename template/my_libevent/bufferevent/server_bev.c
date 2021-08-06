#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
/*
1. 创建event_base
2. 创建bufferevent事件对象。bufferevent_socket_new();
3. 使用bufferevent_setcb() 函数给 bufferevent的 read、write、event 设置回调函数。
4. 当监听的 事件满足时，read_cb会被调用， 在其内部 bufferevent_read();
5. 使用 evconnlistener_new_bind 创建监听服务器， 设置其回调函数，当有客户端成功连接时，这个回调函数会被调用。
6. 封装 listner_cb() 在函数内部。完成与客户端通信。
7. 设置读缓冲、写缓冲的 使能状态 enable、disable
7. 启动循环 event_base_dispath();
8. 释放连接。
*/
// 读缓冲区回调
void read_cb(struct bufferevent *bev, void *arg)
{
    char buf[1024] = {0};   
    bufferevent_read(bev, buf, sizeof(buf));
    printf("server 触发读回调函数\n");
    printf("client say: %s\n", buf);

    char *p = "我是服务器, 已经成功收到你发送的数据!";
    // 发数据给客户端
    //bufferevent_write(bev, p, strlen(p)+1);
    sleep(1);
}

// 写缓冲区回调
void write_cb(struct bufferevent *bev, void *arg)
{
    printf("server 触发写回调函数\n");
   // printf("I'm服务器, 成功写数据给客户端,写缓冲区回调函数被回调...\n"); 
}

// 事件
void event_cb(struct bufferevent *bev, short events, void *arg)
{
    printf("server 触发事件回调函数\n");
    if (events & BEV_EVENT_EOF)
    {
        printf("server: connection closed\n");  
    }
    else if(events & BEV_EVENT_ERROR)   
    {
        printf("server: some other error\n");
    }
    
    bufferevent_free(bev);    
    printf("buffevent 资源已经被释放...\n"); 
}



void cb_listener(
        struct evconnlistener *listener, 
        evutil_socket_t fd, 
        struct sockaddr *addr, 
        int len, void *ptr)
{
   printf("connect new client\n");

   struct event_base* base = (struct event_base*)ptr;
   // 通信操作
   // 添加新事件
   struct bufferevent *bev;
   bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

   // 给bufferevent缓冲区设置回调
   bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);

   //启用 bev 的读缓冲，默认关闭
   bufferevent_enable(bev, EV_READ);
}


int main(int argc, const char* argv[])  
{

    // init server 
    struct sockaddr_in serv;

    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(9876);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);

    struct event_base* base;
    base = event_base_new();
    // 创建套接字
    // 绑定
    // 接收连接请求
    struct evconnlistener* listener;
    listener = evconnlistener_new_bind(base, cb_listener, base, 
                                  LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, //LEV_OPT_REUSEABLE-->libevent标记套接字是可重用的
                                  36, (struct sockaddr*)&serv, sizeof(serv));

    event_base_dispatch(base);

    evconnlistener_free(listener);
    event_base_free(base);  

    return 0;
}