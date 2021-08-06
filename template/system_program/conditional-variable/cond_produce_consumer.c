#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void err_thread(int ret,char *str)
{
    if(ret != 0)
    {
        fprintf(stderr,"%s:%s\n",str,strerror(ret));
        exit(1);
    }
}

//公共数据链表
struct msg
{
    int val;
    struct msg *next;  
};

struct msg *head;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // 初始化一个互斥量
pthread_cond_t Is_data = PTHREAD_COND_INITIALIZER; // 初始化一个条件变量

void *producer_func(void *arg)
{     
    while(1)
    {
        struct msg *mp = malloc(sizeof(mp));

        mp->val = rand()%100+1; //插入数据
        printf("--produce %d\n", mp->val);        

        mp->next = head;
        head = mp;

        pthread_mutex_unlock(&mutex); 
        pthread_cond_signal(&Is_data);

        sleep(rand()%3);
    }
    return NULL;
}

void *cousumer_func(void *arg)
{
    while(1)
    {
        struct msg *mp;

        pthread_mutex_lock(&mutex);
        while(head == NULL)
        {
            pthread_cond_wait(&Is_data,&mutex);

        }         
        mp = head;  //读出数据
        head = mp->next;
        printf("---------consumer:%d\n", mp->val);

        pthread_mutex_unlock(&mutex);     
        
        free(mp);
        sleep(rand()%3);
    }
    
    return NULL;
}

int main(int argc,char* argv[])
{
    int ret;
    pthread_t p_tid,c_tid;
    srand(time(NULL));

    ret = pthread_create(&p_tid,NULL,producer_func,NULL); //producer_init
    err_thread(ret,"pthread_create producer_func error");

    ret = pthread_create(&c_tid,NULL,cousumer_func,NULL); //cousumer_init
    err_thread(ret,"pthread_create cousumer_func error");

    ret = pthread_join(p_tid,NULL);
    err_thread(ret,"pthread_join producer_func error");

    ret = pthread_join(c_tid,NULL);
    err_thread(ret,"pthread_join cousumer_func error");

    return 0 ;
}