#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

// void pthread_exit(void *retval);

/*
retval：退出值。 无退出值时，NULL
exit(); 退出当前进程。
return: 返回到调用者那里去。
pthread_exit(): 退出当前线程
*/ 


void sys_error(char *str)
{
    perror(str);
    exit(1);
}

void *tfunc(void *args)
{
    int i=(int)args;

    sleep(i);
    if(i==2)
    {
        // exit(0);//推出进程3 4 5线程都退出
        pthread_exit(NULL); //推出当前线程
    }
    printf("I'm %dth thread id = %d, tid = %lu\n",i+1,getpid(),pthread_self());

    return NULL;
}   

int main()
{
    pthread_t tid;


    printf("main id = %d, tid = %lu\n",getpid(),pthread_self());
 
    for(int i=0;i<5;i++)
    {
        int ret = pthread_create(&tid,NULL,tfunc,(void *)i);
        if(ret != 0)
        {
            sys_error("pthread_create error");
            
        } 
    }   

    pthread_exit(NULL);

  

    sleep(5);

    return 0;

}
