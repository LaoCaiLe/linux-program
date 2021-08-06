#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>


// typedef void (*sighandler_t)(int);
// sighandler_t signal(int signum, sighandler_t handler);

//捕捉函数 返回值为空 参数变量有int
void sig_catch(int sigo)
{
    if(sigo == SIGINT)
    {
        printf("catch SIGINT! \n");
        sleep(2);
    }
    else if(sigo == SIGQUIT)
        printf("catch SIGQUIT! \n");
}

int main()
{
    //注册捕捉信号函数 捕捉函数为sig_catch
    signal(SIGINT,sig_catch);
    signal(SIGQUIT,sig_catch);

    while(1);

    return 0;
}