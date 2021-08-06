#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
//如果以只读方式打开就会阻塞直到有以写方式打开此文件为止，O_WRONLY
//如果以只写方式打开就会阻塞直到有读方式打开文件为止O_RDONLY
int main()
{
    pid_t child_pid;

    if((mkfifo("./test_fifo",FILE_MODE) < 0) && (errno != EEXIST ))
        printf("cam't create %s", "fifo1");
    if((mkfifo("./test_fifo2",FILE_MODE) < 0) && (errno != EEXIST ))
        printf("cam't create %s", "fifo2");
  
   


    if( (child_pid = fork()) == 0)
    {
        open("./test_fifo2",O_RDWR,0);
    
        printf("test_fifo open with child success\n");
        exit(1);
    }
    sleep(1);
    // open("./test_fifo2",O_WRONLY,0);
  
    printf("test_fifo open with dad success\n");

    unlink("./test_fifo");
    unlink("./test_fifo2");

    exit(1);
    
}