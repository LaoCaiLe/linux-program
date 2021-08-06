#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int val=0;

void *func(void *args)
{
    for(int i=0; i<10; i++)
    {
        printf("id=%ld,i=%d,val=%d\n",pthread_self()%100,i,val);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t tid;

	pthread_cancel(tid);
    val =1;
    int ret = pthread_create(&tid,NULL,func,NULL);
    if(ret != 0)
    {
        perror("thread_create error!");
        exit(1);
    }
    sleep(2);
	pthread_cancel(tid);
    val=2;
    ret = pthread_create(&tid,NULL,func,NULL);
    if(ret != 0)
    {
        perror("thread_create error!");
        exit(1);
    }
    sleep(3);
	pthread_cancel(tid);
	val=3;
    ret = pthread_create(&tid,NULL,func,NULL);
    if(ret != 0)
    {
        perror("thread_create error!");
        exit(1);
    }
	sleep(10);
    return 0;
}
