#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>

#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)
int main()
{
    umask(0);
    if(creat("foo",RWRWRW)<0)
    {
        perror("creat foo error");
        exit(1);
    }
    umask(S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
    if(creat("bar",RWRWRW)<0)
    {
        perror("creat bar error");
        exit(1);
    }
    return 0;
}