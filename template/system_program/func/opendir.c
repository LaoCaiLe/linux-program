#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

//opendir closedir
int main(int argc, char *argv[])
{
	DIR *dp;
	dp = opendir(argv[1]);
	if(dp==NULL)
	{
		perror("opendir error");
		exit(1);
	}

	struct dirent *sdp;
	while((sdp = readdir(dp))!= NULL)
	{
		printf("%s\t",sdp->d_name);
	}

	closedir(dp);
}
