#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
/*
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
how: SIG_BLOCK: 设置阻塞
	SIG_UNBLOCK: 取消阻塞
	SIG_SETMASK: 用自定义 set 替换 mask。
set： 自定义 set
oldset：旧有的 mask。

查看未决信号集：
int sigpending(sigset_t *set);
	set： 传出的 未决信号集
*/

void printf_set(sigset_t *set)
{
	int i;
	for(i=1;i<32;i++)
	{
		//查看信号是否在这个集合中
		if(sigismember(set, i))
			putchar('1');
		else
			putchar('0');
	}
	printf("\n");
}

int main()
{
	int ret;

	sigset_t set,oldset,tempset;

	//清空信号集
	sigemptyset(&set);
	
	//添加信号到阻塞信号集（信号屏蔽字）
	sigaddset(&set,SIGINT); //ctrl+c
	sigaddset(&set,SIGQUIT); //ctrl+\'
	sigaddset(&set,SIGBUS); 
	sigaddset(&set,SIGKILL); 

	//屏蔽信号
	//SIG_BLOCK 阻塞  位或
	//SIG_UNBLOCK 非阻塞 取反再位与
	ret = sigprocmask(SIG_BLOCK,&set,&oldset);
	if(ret==-1)
	{
		perror("sigprocmask error");
		exit(1);
	}


	while(1)
	{
		//获取未决信号集
		ret = sigpending(&tempset);
		if(ret == -1)
		{
			perror("sigpending error");
			exit(1);
		}
		sleep(1);	
		printf_set(&tempset);
	}

	return 0;
}
