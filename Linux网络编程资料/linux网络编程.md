

# 1.TCP/UDP



## 1.1 TCP通信和UDP通信的优缺点

### 1.1.1 TCP

> 面向连接的，可靠数据包传输。
>
> 对于不稳定的网络层，采用完全弥补的通信方式（丢包重传）。
>
> 优点：
>
> ​			数据流量稳定、速度稳定、顺序
>
> 缺点：
>
> ​			传输速度慢，效率低，开销大
>
> 使用场景：
>
> ​			数据的完整性要求较高，不追求速率。
>
> ​			大数据传输、文件传输

### 1.1.2 UDP		

>无连接的，不可靠的数据报传递。
>
>对于不稳定的网络层，采用完全不弥补的通信方式，默认还原网络状况
>
>优点:	
>
>​			传输速度快，效率高，开销小
>
>缺点：	
>
>​			不稳定
>
>使用场景：
>
>​			对时效性要求较高场合	
>
>​			游戏、视频会议、视频电话

​	

## 1.2 UDP实现的C/S模型

### 1.2.1 server:	

​	socket();  SOCK_STREAM-->SOCK_DGRAM（报式协议）

​	accept --> NULL;

​	connect--> NULL;

​	bind();

​	listen() 可有可无

​	read -->recvfrom()

​	write -->sendto()

	### client：

​	socket(); SOCK_STREAM-->SOCK_DGRAM（报式协议）

​	sendto()

​	recvfrom()

​	close()

### 1.2.2 recvfrom

```  c
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
					struct sockaddr *src_addr, socklen_t *addrlen);
```

```
	sockfd: lfd;

​	buf : 缓冲区地址
​	len : 缓冲区大小
​	flags : 0
​	src_addr: (strcut sockaddr *)&addr 传出 。对端地址结构
```

```
	返回

​	成功：接收数据字节数
​	失败：-1 errno
​	0 对端关闭
```



### 1.2.3 sendto

```c
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
                  const struct sockaddr *dest_addr, socklen_t addrlen);
```

```
	sockfd: cfd;

​	buf : 缓冲区地址
​	len : 缓冲区大小
​	flags : 0
​	src_addr: (strcut sockaddr *)&addr 传入。 目标地址结构
​ 	addrlen ：地址结构长度
```

```
	返回
​	成功：写出数据字节数
​	失败：-1 errno

```



## 1.3 本地套接字和网络套接字的比较

> IPC: pipe、fifo、 mmap、信号、本地套（domain） ----CS模型

![](F:\C Pic\Camera Roll\图片1.png)



```c
					网络套接字									本地套接字

	server：	lfd = socket(AF_INET, SOCK_STREAM, 0);		lfd = socket(AF_UNIX, SOCK_STREAM, 0);
		
    bzero() ---- struct sockaddr_in serv_addr;			bzero() ---- struct sockaddr_un serv_addr, clie_addr;

	serv_addr.sin_family = AF_INET;					   serv_addr.sun_family = AF_UNIX;	
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(8888);				   strcpy（serv_addr.sun_path, "套接字文件名"）
													len = offsetof(sockaddr_un, sun_path) + strlen();

										
	bind(lfd, (struct sockaddr *)&serv_addr, sizeof());	unlink("套接字文件名");
													bind(lfd, (struct sockaddr *)&serv_addr, len);  创建新文件

	Listen(lfd, 128);								  Listen(lfd, 128);

	cfd = Accept(lfd, ()&clie_addr, &len);			    cfd = Accept(lfd, ()&clie_addr, &len);  


	client：		
	lfd = socket(AF_INET, SOCK_STREAM, 0);				lfd = socket(AF_UNIX, SOCK_STREAM, 0);

	" 隐式绑定 IP+port"									bzero() ---- struct sockaddr_un clie_addr;
													clie_addr.sun_family = AF_UNIX;
													strcpy（clie_addr.sun_path, "client套接字文件名"）
													len = offsetof(sockaddr_un, sun_path) + strlen();
													unlink( "client套接字文件名");
													bind(lfd, (struct sockaddr *)&clie_addr, len);

	bzero() ---- struct sockaddr_in serv_addr;		    bzero() ---- struct sockaddr_un serv_addr;

	serv_addr.sin_family = AF_INET;					   serv_addr.sun_family = AF_UNIX;
																	
	inet_pton(AF_INT, "服务器IP", &sin_addr.s_addr)							
													strcpy（serv_addr.sun_path, "server套接字文件名"）
	serv_addr.sin_port = htons("服务器端口");		
										
													 len = offsetof(sockaddr_un, sun_path) + strlen();

	connect(lfd, &serv_addr, sizeof());					connect(lfd, &serv_addr, len);
```

//本地套接字比pipe强，稳定性强，能实现全双工

# 2. Libevent

## 2.1 事件event

1. 

   创建 event_base (乐高底座) 

   struct event_base event_base_new(void); 

   struct event_base *base = event_base_new(); 

   

5. 

   创建 事件 evnet 

   常规事件 event --> event_new(); 
   
   

```c
struct event *event_new(struct event_base *base, evutil_socket_t fd,short what, event_callback_fn cb, void *arg)
base  
    > event_base_new()返回值

fd
    > 定到event上的文件描述符

what: 对应事件（读、写、异常）
    > EV_READ
    > EV_WRITE
    > EV_PERSIST 持续触发，结合event_base_dispatch函数使用

cb 
    > 满足监听事件的回调函数

arg
    >回调函数的参

返回值
    > 成功创建的事件event
```

3. 将事件 添加到 base 上 

​	int event_add(struct event *ev, const struct timeval *tv) 

```c
int event_add(struct event *ev, const struct timeval *tv); 
	ev: event_new() 的返回值 
	tv：
        NULL 不会超时，一直等到事件被触发，回调函数会被调用。
        为非0，等待期间，检查事件有没有被触发，时间到，回调函数依旧会被调用。
```

4. 循环监听事件满足 

   int event_base_dispatch(struct event_base *base); 

   event_base_dispatch(base); 

``` c
int event_base_dispatch(struct event_base *base); //相当于 while(1) {epoll_wait(); }
	base: event_base_new() 的返回值
```

5. 释放 event_base 

   event_base_free(base)；

``` c
int event_free(struct event *ev);
int event_base_free(struct event_base *base);
```



## 2.2 未决和非未决

未决：	有资格被处理，但尚未被处理

非未决：没有资格被处理

![](F:\C Pic\Camera Roll\Snipaste_2021-06-06_10-53-34.bmp)



## 2.3 数据缓冲buffevent

### 2.3.1 基本原理

头文件 #include <event2/bufferevent.h>

原理：bufferevent有两个缓冲区：也是队列实现，读走没，先进先出

​	读：有数据 -->读回调函数被调用 --> 使用 bufferevent_read() -->读数据

​	写：使用 bufferevent_write() --> 向写缓冲中写数据 --> 该缓冲区有数据自动写出 --> 写完，回调函数被调用

![bufferevent](H:\Linux\Linux网络编程资料\linux网络编程资料\day6\2-其他资料\bufferevent.png)





### 2.3.2  服务器端创建TCP连接

- 创建event_base

  ``` c
  struct event_base* base;
  base = event_base_new();
  ```

  

- 创建bufferevent事件对象。bufferevent_socket_new(); 

   ``` c
  struct bufferevent *bev;
  bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
  //在监听器回调（cb_listener）中初始化
  ```

  

- 使用bufferevent_setcb() 函数给 bufferevent的 read、write、event 设置回调函数。

  ``` c
  bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);
  //在监听器回调（cb_listener）中初始化
  ```

  

- 当监听的读事件满足时，read_cb会被调用， 在其内部 bufferevent_read();

  ``` c
  void read_cb(struct bufferevent *bev, void *arg)
  {    
      bufferevent_read(bev, buf, sizeof(buf));
      bufferevent_write(bev, p, strlen(p)+1);
  }
  ```

  

- 使用 evconnlistener_new_bind 创建监听服务器， 设置其回调函数，当有客户端成功连接时，这个回调函数会被调用。

  ``` c
  struct evconnlistener* listener;
  listener = evconnlistener_new_bind(base, cb_listener, base, 
                                    LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, //LEV_OPT_REUSEABLE-->libevent标记套接字是可重用的
                                    36, (struct sockaddr*)&serv, sizeof(serv));
  ```

  

- 封装 listener_cb() 在函数内部。完成与客户端通信

  ``` c
  void cb_listener(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *addr,int len, void *ptr)
  { 
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
  ```

  

- 启动循环 ; 

  ``` c
  event_base_dispatch(base);
  ```

  

- 释放连接

  ``` c
  evconnlistener_free(listener);
  event_base_free(base);
  ```

  整体代码

  ``` c
  #include <stdio.h>
  #include <unistd.h>
  #include <stdlib.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <string.h>
  #include <event2/event.h>
  #include <event2/listener.h>
  #include <event2/bufferevent.h>
  
  void read_cb(struct bufferevent *bev, void *arg)
  {
      char buf[1024] = {0};   
      bufferevent_read(bev, buf, sizeof(buf));
      printf("client say: %s\n", buf);
  
      char *p = "我是服务器, 已经成功收到你发送的数据!";
      // 发数据给客户端
      bufferevent_write(bev, p, strlen(p)+1);
      sleep(1);
  }
  
  // 写缓冲区回调
  void write_cb(struct bufferevent *bev, void *arg)
  {
      printf("I'm服务器, 成功写数据给客户端,写缓冲区回调函数被回调...\n"); 
  }
  
  // 事件
  void event_cb(struct bufferevent *bev, short events, void *arg)
  {
      if (events & BEV_EVENT_EOF)
      {
          printf("connection closed\n");  
      }
      else if(events & BEV_EVENT_ERROR)   
      {
          printf("some other error\n");
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
  
  ```

  

### 2.3.3 客户端创建TCP连接

- 创建event_base

   ``` c
  struct event_base* base = NULL;
  base = event_base_new();
  ```

  

- 创建bufferevent事件对象。bufferevent_socket_new();

  ``` c
  struct bufferevent* bev = NULL;
  bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
  ```

  

- 使用bufferevent_setcb() 函数给 bufferevent的 read、write、event 设置回调函数

  ``` c
  bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);
  ```

  

- 连接服务器 bufferevent_socket_connect();

  ``` c
  bufferevent_socket_connect(bev, (struct sockaddr*)&serv, sizeof(serv));
  ```

  

- 设置读缓冲、写缓冲的 使能状态 enable、disable

  ``` c
  bufferevent_enable(bev, EV_READ);
  ```

  

- 启动循环 event_base_dispath();

  ``` 
  event_base_dispatch(base);
  ```

  

- 释放连接

  ``` c
  event_free(ev);    
  event_base_free(base);
  ```

  - 整体代码

    ``` c
    #include <stdio.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <string.h>
    #include <event2/bufferevent.h>
    #include <event2/event.h>
    #include <arpa/inet.h>
    
    void read_cb(struct bufferevent *bev, void *arg)
    {
        char buf[1024] = {0}; 
        bufferevent_read(bev, buf, sizeof(buf));
    
        printf("fwq say:%s\n", buf);
    
        bufferevent_write(bev, buf, strlen(buf)+1);
        sleep(1);
    }
    
    void write_cb(struct bufferevent *bev, void *arg)
    {
        printf("----------我是客户端的写回调函数,没卵用\n"); 
    }
    
    void event_cb(struct bufferevent *bev, short events, void *arg)
    {
        if (events & BEV_EVENT_EOF)
        {
            printf("connection closed\n");  
        }
        else if(events & BEV_EVENT_ERROR)   
        {
            printf("some other error\n");
        }
        else if(events & BEV_EVENT_CONNECTED)
        {
            printf("已经连接服务器...\\(^o^)/...\n");
            return;
        }
        
        // 释放资源
        bufferevent_free(bev);
    }
    
    // 客户端与用户交互，从终端读取数据写给服务器
    void read_terminal(evutil_socket_t fd, short what, void *arg)
    {
        // 读数据
        char buf[1024] = {0};
        int len = read(fd, buf, sizeof(buf));
    
        struct bufferevent* bev = (struct bufferevent*)arg;
        // 发送数据
        bufferevent_write(bev, buf, len+1);
    }
    
    int main(int argc, const char* argv[])
    {
        struct event_base* base = NULL;
        base = event_base_new();
    
        int fd = socket(AF_INET, SOCK_STREAM, 0);
    
        // 通信的fd放到bufferevent中
        struct bufferevent* bev = NULL;
        bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    
        // init server info
        struct sockaddr_in serv;
        memset(&serv, 0, sizeof(serv));
        serv.sin_family = AF_INET;
        serv.sin_port = htons(9876);
        inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr.s_addr);
    
        // 连接服务器
        bufferevent_socket_connect(bev, (struct sockaddr*)&serv, sizeof(serv));
    
        // 设置回调
        bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);
    
        // 设置读回调生效
    	bufferevent_enable(bev, EV_READ);
    
        // 创建事件
        struct event* ev = event_new(base, STDIN_FILENO, EV_READ | EV_PERSIST,
                                     read_terminal, bev);
        // 添加事件                     
        event_add(ev, NULL);
    
        event_base_dispatch(base);
    
        event_free(ev);
        
        event_base_free(base);
    
        return 0;
    }
    
    ```

    

### 2.3.4 常用函数

#### (1)创建base

``` c
struct event_base *event_base_new(void);
	返回成功创建的base
```



#### (2)创建bufferevent

``` c
struct bufferevent *bufferevent_socket_new( struct event_base *base, evutil_socket_t fd, enum bufferevent_options options);
	base: event_base_new()返回值
    fd:	文件描述符
    options：BEV_OPT_CLOSE_ON_FREE  释放时关闭事件对象
    返回值： 成功创建的 bufferevent事件对象。
```



#### (3)创建监听服务器

``` c
struct evconnlistener * evconnlistener_new_bind(struct event_base *base, evconnlistener_cb cb, void *ptr, unsigned flags, int backlog , const struct sockaddr *sa, int socklen);
        base: --
        cb: 回调函数
        ptr： 回调函数的参数
        flags： LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE //释放时关闭 | libevent标记套接字是可重用
        backlog：监听数量， -1 表最大值128
        sa：服务器自己的地址结构体
        socklen：服务器自己的地址结构体大小。
        返回值：成功创建的监听器。
```



####  (4)监听器回调函数

``` c
typedef void (*evconnlistener_cb)(struct evconnlistener *listener, evutil_socket_t sock, struct sockaddr *addr, int len, void * ptr);
	listener: 接收连接的连接监听器 。 
        sock: 新接收的套接字。 
        addr、len：接收连接的地址和地址长度。
        ptr：调用 evconnlistener_new_bind() 时用户提供的指针。
```



####  (5)设置回调函数

```c
void bufferevent_setcb(struct bufferevent *bufev, 
                       bufferevent_data_cb readcb,
                       bufferevent_data_cb writecb, 
                       bufferevent_event_cb eventcb, 
                       void *cbarg);
	bufev： bufferevent_socket_new() 返回值
	readcb： 设置 bufferevent 读缓冲，对应回调  read_cb{  bufferevent_read() 读数据  }
	writecb： 设置 bufferevent 写缓冲，对应回调 write_cb {  } -- 给调用者，发送写成功通知。  可以 NULL
	eventcb： 设置 事件回调。   也可传NULL
    cbarg：	上述回调函数使用的 参数。
```



#### (6)客户端连接函数

``` c
int bufferevent_socket_connect(struct bufferevent *bev, struct sockaddr *address, int addrlen);
		bev:bufferevent事件对象
         address: 服务器地址结构 port+IP
         addrlen：地址结构大小
```



#### (7)循环监听函数

``` c
int event_base_dispatch(struct event_base *base)
    //event_base_dispatch() 将一直运行,直到没有已经注册的事件了,或者调用了 event_base_loopbreak() 或者 event_base_loopexit() 为止。
    //相当于 while(1) epllo_wait();
```



#### (8)读写回调函数

``` c
typedef void (*bufferevent_data_cb)(struct bufferevent *bev, void *ctx);
		bev:bufferevent事件对象（为bufferevent_setcb的参数bev）
         ctx：bufferevent_setcb的传入参数
```



#### (9)事件回调函数

``` c
typedef void (*bufferevent_event_cb)(struct bufferevent *bev, short events, void *ctx);
		bev:bufferevent事件对象（为bufferevent_setcb的参数bev）
		events 参数是一个表示事件标志的位掩码
         ctx：bufferevent_setcb的传入参数
```



#### (10)监听器释放

``` c
void evconnlistener_free(struct evconnlistener *lev);
	lev:evconnlistener_new_bind()的返回值
```



#### (11)bufferevent写

```c
int bufferevent_write(struct bufferevent *bufev, const void *data, size_t size);
int bufferevent_write_buffer(struct bufferevent *bufev, struct evbuffer *buf);
```



#### (12)bufferevent读

``` c
size_t bufferevent_read(struct bufferevent *bufev, void *data, s ize_t size); 
int bufferevent_read_buffer(struct bufferevent *bufev, struct evbuffer *buf);
```







# 3.套接字编程

## 3.1值-结构参数

（1）从进程到内核传递套接字地址结构的函数：bind, connect, sendto

 ![](F:\C Pic\linux\Snipaste_2021-06-10_21-06-43.bmp)





(2)从内核到进程传递套接字地址结构的函数: accept, recvfrom, getsockname , getpeername;

![](F:\C Pic\linux\Snipaste_2021-06-10_21-11-58.png) 

​	传指针原因：当函数被调用时，结构大小是一个值，它告诉内核该结构的大小，这样内核在写该结构时不至于越界；当函数返回时，结构大小又是一个结果，他告诉进程内核在该结构中究竟存储了多少信息。



# 原子操作

1. 原子操作指的是由多步组成的一个操作
2. 任何要求多于一个函数调用的操作都不是原子操作，因为在两个函数调用之间，内核可能会临时挂起进程
