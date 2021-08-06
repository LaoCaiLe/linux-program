#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>

#include <stdlib.h>
#include <signal.h>

#include <unistd.h>

int main()
{
	int i;
	pid_t pid;
	int id;

	for(i=0; i<5; i++)
	{
		pid = fork();		
		if(pid==0)
		{
			break;
		}
		else if(pid == -1)
		{
			printf("fork error");
			exit(1);
		}
		else if (pid>0)
		{
			if(i==2)
			{
				id = pid;
			}
		}
	}


	if(i==5)
	{
		sleep(5);
		printf("I'm dad, id = %d\n",getpid());
		int ret = kill(id,SIGKILL);
		if(ret == 0)
		{
			printf("kill ok!\n");
		}
	}
	else
	{
		printf("I'm the %dth child, id = %d\n",i+1,getpid());
		while(1);
	}

	return 0;
}
