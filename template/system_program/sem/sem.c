#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
/*
sem_init 函数
sem_destroy 函数
sem_wait 函数
sem_trywait 函数
sem_timedwait 函数
sem_post 函数
*/
#define NUM 5

int queue[NUM]; //全局数组实现环形队列
sem_t blank_number, product_number; //空格子信号量, 产品信号量

void err_thread(int ret,char *str)
{
    if(ret != 0)
    {
        fprintf(stderr,"%s:%s\n",str,strerror(ret));
        exit(1);
    }
}

void *producer_func(void *arg)
{
    int i = 0;
    while(1)
    {
        sem_wait(&blank_number);
        queue[i] = rand()%100+1;
        printf("-Produce---%d\n", queue[i]);
        i = (i+1) % NUM;
        sem_post(&product_number);

        sleep(rand()%3);
    }
    return NULL;
}

void *cousumer_func(void *arg)
{
    int i = 0;
    while(1)
    {
        sem_wait(&product_number);
        printf("-----------Consume---%d\n", queue[i]);
        queue[i] = 0;
        i = (i+1) % NUM;
        sem_post(&blank_number);

        sleep(rand()%3);
   
    }
    return NULL;
}

int main()
{
    int ret;
    pthread_t p_tid,c_tid;
    srand(time(NULL));

    ret = sem_init(&blank_number,0,NUM);
    err_thread(ret,"sem_init blank_number");

    ret = sem_init(&product_number,0,NUM);
    err_thread(ret,"sem_init blank_number");

    ret = pthread_create(&p_tid,NULL,producer_func,NULL); //producer_init
    err_thread(ret,"pthread_create producer_func error");

    ret = pthread_create(&c_tid,NULL,cousumer_func,NULL); //cousumer_init
    err_thread(ret,"pthread_create cousumer_func error");

    ret = pthread_join(p_tid,NULL);
    err_thread(ret,"pthread_join producer_func error");

    ret = pthread_join(c_tid,NULL);
    err_thread(ret,"pthread_join cousumer_func error");

    ret = sem_destroy(&blank_number);
    err_thread(ret,"sem_destroy blank_number");

    ret = sem_destroy(&product_number);
    err_thread(ret,"sem_destroy product_number");

    return 0 ;
}