#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* int pthread_join(pthread_t thread, void **retval);
阻塞回收线程（不杀死）

返回： 
    成功: 0
    失败: errno
*/

struct thrd{
    int val;
    char str[256];
};

void sys_error(char *str)
{
    perror(str);
    exit(1);
}

void *tfunc(void *arg)
{
    struct thrd *p={100,"hello thread_join"};

    p = malloc(sizeof(p));
    p->val = 100;
    strcpy(p->str,"hello thread_join");

    
    return (void*)(p);
}

int main(int argc,char*argv[])
{
    pthread_t tid; 
    struct thrd *retval;


    int ret = pthread_create(&tid,NULL,tfunc,NULL);
    if(ret != 0)
    {
        sys_error("pthread_create");
    }

    ret = pthread_join(tid,(void **)(&retval));
    if(ret != 0)
    {
        sys_error("pthread_join");
    }

    printf("Sub thread callback with val = %d, str = %s\n",retval->val,retval->str);

    pthread_exit(NULL);

    return 0;
}