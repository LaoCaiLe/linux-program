#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main()
{
	pid_t pid = fork();

	if(pid>0)
	{
		while(1)
		{
			sleep(1);
			printf("parent's pid is %d\n",getpid());
		}
	}
	else if(pid == 0)
	{
		sleep(2);
		printf("child pid is %d, dad pid is %d\n",getpid(),getppid());
		//kill(getppid(),SIGKILL);
		kill(getppid(),SIGCHLD); //父进程会忽略这个信号
	}
	else if(pid<0)
	{
		perror("fork error");
		exit(1);
	}
}
