#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//借助线程属性实现线程分离,为线程分离后，系统会自动回收资源
// int pthread_attr_init(pthread_attr_t *attr);     初始化线程属性
// int pthread_attr_destroy(pthread_attr_t *attr);  销毁线程属性
// On success, these functions return 0; on error, they return a nonzero error number.

// int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);        设置线程属性
// int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);   获取线程属性
// On success, these functions return 0; on error, they return a nonzero error number

void *tfunc(void *arg)
{
    return NULL;
}

int main()
{
    pthread_t tid;
    pthread_attr_t attr;

    int ret;

    ret = pthread_attr_init(&attr);
    if(ret != 0)
    {
        fprintf(stderr,"pthread_attr_init error: %s\n",strerror(ret));
        exit(1);
    }

    ret = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED); //设置分离属性
    if(ret != 0)
    {
        fprintf(stderr,"pthread_attr_setdetachstate error: %s\n",strerror(ret));
        exit(1);
    }

    ret = pthread_create(&tid,&attr,tfunc,NULL);
    if(ret != 0)
    {
        fprintf(stderr,"pthread_create error: %s\n",strerror(ret));
        exit(1);
    }

    ret = pthread_attr_destroy(&attr);
    if(ret != 0)
    {
        fprintf(stderr,"pthread_attr_destroy error: %s\n",strerror(ret));
        exit(1);
    }

    ret = pthread_join(tid,NULL);
    if(ret != 0)
    {
        fprintf(stderr,"pthread_join error: %s\n",strerror(ret));
        exit(1);
    }
  
}