#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>

struct itimerval new,old;

//捕捉函数 返回值为空 参数变量有int
void func(int sigo)
{
	//printf("hello world!\n");
	printf("old.it_interval.tv_sec = %ld\n",old.it_interval.tv_sec);
	printf("old.it_interval.tv_usec = %ld\n", old.it_interval.tv_usec);

}


int main()
{

	signal(SIGALRM,func);		//注册捕捉信号函数

	new.it_interval.tv_sec = 4;	//周期定时为4s 0us
	new.it_interval.tv_usec = 0;

	new.it_value.tv_sec = 0;	//第一次定时为2s 0us
	new.it_value.tv_usec = 300;

	if(setitimer(ITIMER_REAL, &new, &old)==-1)
	{
		perror("setitimer error");
		exit(1);
	}

	while(1);

	return 0;
}
