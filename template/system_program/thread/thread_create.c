#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

// int pthread_create(pthread_t *tid, const pthread_attr_t *attr, void*(*start_rountn)(void *), void *arg); 创建子线程。

// 参 1：传出参数，表新创建的子线程 id
// 参 2：线程属性。传 NULL 表使用默认属性。
// 参 3：子线程回调函数。创建成功，ptherad_create 函数返回时，该函数会被自动调用。
// 参 4：参 3 的参数。没有的话，传 NULL

// 返回值：成功：0
// 失败：errno

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
        perror("pthread_create error");
        exit(1);
    }  
  
      

    sleep(1);

    return 0;
}
