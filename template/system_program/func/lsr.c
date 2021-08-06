#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdarg.h>



void show_dir(char *name)
{
    char dir_name[256];
    struct stat sbuf;
    int ret;
    DIR *dp;
    struct dirent *sdp;

    ret = stat(name,&sbuf);
    if(ret == -1)
    {
        perror("stat error");
        exit(1);
    }

    if(S_ISDIR(sbuf.st_mode))
    {
        dp = opendir(name);
        printf("%s:\n",name);
        while((sdp = readdir(dp)) != NULL)
        {       
            if(strcmp(sdp->d_name,".")==0||strcmp(sdp->d_name,"..")==0) continue;
            
            printf("%s\n",sdp->d_name);            
        }
        printf("\n");
        closedir(dp);

        dp = opendir(name);
        while((sdp = readdir(dp)) != NULL)
        {
            if(sdp->d_type==DT_DIR)
            {
                if(strcmp(sdp->d_name,".")==0||strcmp(sdp->d_name,"..")==0) continue;
                sprintf(dir_name,"%s/%s",name,sdp->d_name);
                show_dir(dir_name);
            }
        }
    }
  

    closedir(dp);

}


int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        show_dir(".");
    }
    else
    {
        show_dir(argv[1]);
    }

    return 0;

}
