#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int var = 0;
//---------主子线程共享全局变量

void sys_error(char *str)
{
    perror(str);
    exit(1);
}

void *tfunc(void *args)
{
    var = 200;

    return NULL;
}   

int main()
{
    pthread_t tid;


    //printf("main id = %d, tid = %lu\n",getpid(),pthread_self());
    printf("before create thread :var = %d\n",var);

    
    int ret = pthread_create(&tid,NULL,tfunc,NULL);
    if(ret != 0)
    {
        sys_error("pthread_create error");
        
    } 
    sleep(1);
    printf("after create thread :var = %d\n",var);

    sleep(1);

    return 0;

}
