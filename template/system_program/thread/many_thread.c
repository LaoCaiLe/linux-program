#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


void sys_error(char *str)
{
    perror(str);
    exit(1);
}

void *tfunc(void *args)
{
    int i=(int)args;

    //进程id一样 线程id不一样
    printf("I'm %dth thread id = %d, tid = %lu\n",i+1,getpid(),pthread_self());

    return NULL;
}   

int main()
{
    pthread_t tid[5];
    int ret;

    printf("main id = %d, tid = %lu\n",getpid(),pthread_self());

    for(int i=0;i<5;i++)
    {
        ret = pthread_create(&tid[i],NULL,tfunc,(void *)i);
        if(ret != 0)
        {
            sys_error("pthread_create error");
            
        } 
    }   

    for(int i=0;i<5;i++)
    {
        ret = pthread_join(tid[i],NULL);
        if(ret != 0)
        {
             sys_error("pthread_join error");
        }
        printf("thread join %dth child id=%ld\n",i+1,tid[i]);
    }

    sleep(1);
    pthread_exit(NULL);

    return 0;

}
