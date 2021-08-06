#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

/*
int pthread_mutex_init (pthread_mutex_t *__mutex,const pthread_mutexattr_t *__mutexattr)
int pthread_mutex_destroy (pthread_mutex_t *__mutex)
int pthread_mutex_trylock (pthread_mutex_t *__mutex) 
int pthread_mutex_lock (pthread_mutex_t *__mutex)
int pthread_mutex_unlock (pthread_mutex_t *__mutex)

以上 5 个函数的返回值都是：成功返回 0， 失败返回错误号。

pthread_mutex_t 类型，其本质是一个结构体。为简化理解，应用时可忽略其实现细节，简单当成整数看待。
pthread_mutex_t mutex; 变量 mutex 只有两种取值 1、0。
*/


/*
加锁： --操作， 阻塞线程。
解锁： ++操作， 唤醒阻塞在锁上的线程。
try 锁：尝试加锁，成功--，加锁失败直接返回错误号(如 EBUSY)，不阻塞
*/

pthread_mutex_t mutex; //互斥锁 全局变量 主子线程才都能访问

void *tfunc(void *arg)
{    
    while(1)
    {
        int ret = pthread_mutex_trylock(&mutex);
        if(errno == EBUSY)
        {
            printf("trylock failed\n");

        }
        pthread_mutex_lock(&mutex);
        sleep(rand()%2);
        printf("HELLO ");
         
        printf("WORLD\n");

        pthread_mutex_unlock(&mutex); 
        sleep(rand()%2);
    }
   

    return NULL;
}

int main()
{
    pthread_t tid;
    int ret;
    srand(time(NULL));
    ret = pthread_mutex_init(&mutex,NULL);      //互斥锁初始化
    if(ret != 0)
    {
        fprintf(stderr,"pthread_mutex_init error: %s\n",strerror(ret));
        exit(1);
    }

    ret = pthread_create(&tid,NULL,tfunc,NULL);     //创建子线程
    if(ret != 0)    
    {
        fprintf(stderr,"pthread_create error: %s\n",strerror(ret));
        exit(1);
    }

    while(1)
    {
        pthread_mutex_lock(&mutex); //加锁
        sleep(rand()%2);
        printf("hello ");
        
        printf("world\n");
        pthread_mutex_unlock(&mutex);   //解锁
              

        sleep(rand()%2);
    }

    ret = pthread_join(tid,NULL);       //回收子线程
    if(ret != 0)
    {
        fprintf(stderr,"pthread_join error: %s\n",strerror(ret));
        exit(1);
    }

    ret = pthread_mutex_destroy(&mutex);    //销毁所
    if(ret != 0)
    {
        fprintf(stderr,"pthread_mutex_destroy error: %s\n",strerror(ret));
        exit(1);
    }

    return 0;
}   