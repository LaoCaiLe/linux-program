#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

// int pthread_detach(pthread_t thread); //实现线程分离 （有回收子线程的作用）
// On success, pthread_detach() returns 0; on error, it returns an error number.

void *tfunc(void *arg)
{
    printf("thread id = %d, tid = %lu\n",getpid(),pthread_self());

    return NULL;
}

int main()
{
    pthread_t tid;

    printf("main id = %d, tid = %lu\n",getpid(),pthread_self());
    
    int ret = pthread_create(&tid,NULL,tfunc,NULL);
    if(ret != 0)
    {
        fprintf(stderr,"pthread_create error: %s\n",strerror(ret));
        exit(1);
    }  

    ret = pthread_detach(tid);  //设置线程分离 线程终止 会自动清理pcb 无需回收
    if(ret != 0)
    {
        fprintf(stderr,"pthread_detach error: %s\n",strerror(ret));
        exit(1);
    }  

    sleep(1);

    ret = pthread_join(tid,NULL);   //发生错误 子线程分离时已设置自动回收
    if(ret != 0)
    {
        fprintf(stderr,"pthread_join error: %s\n",strerror(ret));
        exit(1);
    }  
  

    return 0;
}
