#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc,char *argv[])
{
    int i;
    for(i=0; i<5; i++)
    {
        if(fork()==0)
        {           
            break;     
        }        
    }
    if(i==5)
    {
        sleep(5);
        printf("I'm parent process\n");        
    }
    else
    {
        sleep(i);
        printf("I'm the %dth child\n",i+1); 
    }
    return 0;
}