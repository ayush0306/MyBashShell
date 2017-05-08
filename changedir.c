#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include<pwd.h>

#define INP_LEN 10000
#define PATH_LEN 1000
#define DELIM " \t\r\n\a\"\'"


int changedir(char *path,char *homedir)
{
    int ret,i=0;
    if(path[0]=='~')
    {   
        ret = chdir(homedir);
        i++;
        if(strncmp(path+i,"/",1)==0)
            i++;
        else
            perror(path);
    }
    if(i!=1)
        ret = chdir(path+i);
    if(ret==-1)
        perror(path);
    return 0;
}

