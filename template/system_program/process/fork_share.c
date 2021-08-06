#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

/**
 * 1）fork()： 父子进程的执行次序不确定。
   vfork()：保证子进程先运行,在它调用 exec（进程替换） 或 exit（退出进程）之后父进程才可能被调度运行。

2）fork()： 子进程拷贝父进程的地址空间，子进程是父进程的一个复制品。
   vfork()：子进程共享父进程的地址空间（准确来说，在调用 exec（进程替换） 或 exit（退出进程） 之前与父进程数据是共享的）

 */



/*
父子进程相同：
刚 fork 后。 data 段、text 段、堆、栈、环境变量、全局变量、宿主目录位置、进程工作目录位
置、信号处理方式

父子进程不同：
进程 id、返回值、各自的父进程、进程创建时间、闹钟、未决信号集
*/

/*
 共享内容
 1. 文件描述符 2. mmap 映射区

 读时共享 写时复制 ----全局变量
 */
int var=8;
int main(int arhc,char *arhv[])
{
    pid_t pid = fork();

    if(pid == -1)
    {
        perror("fork error");
        exit(1);
    }
    else if(pid==0)     //子进程的返回
    {
         var = 100;
        sleep(1);
        // printf("child pid is created!\n");
        printf("child's var = %d\n",var);
        exit(1);
        // sleep(1);
    }   
    else if(pid>0)    //父进程的返回
    {
       // var = 200;
       sleep(2);
        // printf("parent process: my child id %d\n",pid);
        printf("Dad's var = %d\n",var); sleep(2);
    }

    //fork之后的内容父子进程都会执行
    printf("-----end-------\n");

    return 0;
}