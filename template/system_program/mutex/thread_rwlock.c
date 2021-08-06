#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

/*
pthread_rwlock_init 函数
pthread_rwlock_destroy 函数
pthread_rwlock_rdlock 函数 

pthread_rwlock_tryrdlock 函数
pthread_rwlock_trywrlock 函数
pthread_rwlock_unlock 函数
*/

pthread_rwlock_t mutex;
int counter; //全局资源

void *thread_r(void* arg)
{
    int i = (int)arg;
    while(1)
    {
        pthread_rwlock_rdlock(&mutex);
        printf("--------%dth read thread counter = %d\n",i,counter);
        pthread_rwlock_unlock(&mutex);
        usleep(1000);// 给 w 锁提供机会
    }
    return NULL;
}

void *thread_w(void* arg)
{
    int i = (int)arg;

    while(1)
    {
        pthread_rwlock_wrlock(&mutex);
        printf("========%dth write thread counter = %d, counter++ = %d\n",i,counter,++counter);
        pthread_rwlock_unlock(&mutex);
        usleep(9100);// 给 r 锁提供机会
    }

    return NULL;
}

int main()
{
    int i;
    pthread_t tid[6];

    pthread_rwlock_init(&mutex,NULL);       //读写锁初始化

    for(i=0;i<3;i++)
    {
        pthread_create(&tid[i],NULL,thread_r,(void*)i);  //创建读线程
    }

    for(i=0;i<3;i++)
    {
        pthread_create(&tid[i+3],NULL,thread_w,(void*)i);  //创建写线程
    }
    
    for(i=0;i<6;i++)
    {
        pthread_join(tid[i],NULL);  //回收线程
    }

    pthread_rwlock_destroy(&mutex);

    return 0;


}