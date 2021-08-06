#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
//杀死线程
//  int pthread_cancel(pthread_t thread);
//On  success,  pthread_cancel() returns 0; on error, it returns a nonzero error number.

// void pthread_testcancel(void); 添加cancel取消点 检查是否有cancel函数在杀自己 
void *tfunc(void *arg)
{
    while(1)
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
        perror("pthread_create error");
        exit(1);
    }  

    sleep(5);//5秒后杀死子线程

    ret = pthread_cancel(tid);
    if(ret != 0)
    {
        perror("pthread_cancel error");
        exit(1);
    } 
  
      

    

    return 0;
}
