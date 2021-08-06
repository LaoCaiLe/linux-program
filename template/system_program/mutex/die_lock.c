#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

//use lock wrong with 2


void test1(void)
{
   
    pthread_mutex_t mutex;
    int ret;

    ret = pthread_mutex_init(&mutex,NULL);      //互斥锁初始化
    if(ret != 0)
    {
        fprintf(stderr,"pthread_mutex_init error: %s\n",strerror(ret));
        exit(1);
    }

    ret = pthread_mutex_lock(&mutex);
    if(ret != 0)
    {
        fprintf(stderr,"pthread_mutex_lock error: %s\n",strerror(ret));
        exit(1);
    }
    for(int i = 0;i<5; i++)
    {
        printf("i am working-%d\n",i+1);

        sleep(1);
    }   

    ret = pthread_mutex_lock(&mutex);
    if(ret != 0)
    {
        fprintf(stderr,"pthread_mutex_lock error: %s\n",strerror(ret));
        exit(1);
    }

    for(int i = 0;i<5; i++)
    {
        printf("i am working-%d\n",i+1);

        sleep(1);
    }   
    
    ret = pthread_mutex_unlock(&mutex);
    if(ret != 0)
    {
        fprintf(stderr,"pthread_mutex_unlock error: %s\n",strerror(ret));
        exit(1);
    }
    
   

    // ret = pthread_mutex_unlock(&mutex);
    // if(ret != 0)
    // {
    //     fprintf(stderr,"pthread_mutex_unlock error: %s\n",strerror(ret));
    //     exit(1);
    // }

    // for(int i = 0;i<5; i++)
    // {
    //     printf("i am working-%d\n",i+1);

    //     sleep(1);
    // }  
}

int val1,val2;
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

void *tfunc(void *arg)
{
    int ret;
    pthread_mutex_init(&mutex2,NULL);      //互斥锁初始化

    ret = pthread_mutex_lock(&mutex2);
    if(ret !=0)
    {
        fprintf(stderr,"pthread_mutex_lock error: %s\n",strerror(ret));
        exit(1);
    }
    else
    {
        printf("T2 lock mutex2\n");
    }

    sleep(1);
    ret = pthread_mutex_lock(&mutex1);
    if(ret !=0)
    {
        fprintf(stderr,"pthread_mutex_lock error: %s\n",strerror(ret));
        exit(1);
    }
    else
    {
        printf("T2 lock mutex1\n");
    }

    return NULL;
}

void test2()
{
    int ret;
  
    pthread_t tid;

    pthread_create(&tid,NULL,tfunc,NULL);//创建子线程

    pthread_mutex_init(&mutex1,NULL);      //互斥锁初始化

    ret = pthread_mutex_lock(&mutex1);
    if(ret !=0)
    {
        fprintf(stderr,"pthread_mutex_lock error: %s\n",strerror(ret));
        exit(1);
    }
    else
    {
        printf("T1 lock mutex1\n");
    }

    sleep(1);
    ret = pthread_mutex_lock(&mutex2);
    if(ret !=0)
    {
        fprintf(stderr,"pthread_mutex_lock error: %s\n",strerror(ret));
        exit(1);
    }
    else
    {
        printf("T1 lock mutex2\n");
    }
 }

int main(int argc,char *argv[])
{
    //1. 对一个互斥量反复加锁 反复解锁不会死锁
    test1();

    //2. 两个线程，各自持有一把锁，请求另一把
    // test2();

    return 0;
}