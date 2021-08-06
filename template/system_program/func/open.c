#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    int fd = open(argv[1],O_RDWR |O_CLOEXEC );

    sleep(2);

    close(fd);

    exit(1);
}