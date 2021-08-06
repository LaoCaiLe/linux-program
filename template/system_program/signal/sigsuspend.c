#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

void sig_int(int signo)
{
    printf("in sig_int: \n");
}

int main()
{
    sigset_t newset,oldset,waitset;

    struct sigaction act;
    act.sa_flags=0;
    act.sa_handler = sig_int;
    sigemptyset(&act.sa_mask);

    sigaction(SIGINT,&act,NULL);

    sigemptyset(&waitset);
    sigaddset(&waitset,SIGUSR1);
    sigaddset(&waitset,SIGQUIT);

    sigprocmask(SIG_BLOCK, &newset,&oldset);

    sigsuspend(&waitset);

    if(sigprocmask(SIG_SETMASK, &oldset, NULL) < 0)
    {
        perror("sigprocmask error");
        exit(1);
    }

    exit(1);
    
    
}