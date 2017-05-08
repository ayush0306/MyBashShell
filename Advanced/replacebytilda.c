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


int replacebytilda(char *currdir,char *homedir)
{
    int p1 = 0;
    while(homedir[p1]!=0)    
    {
        if(currdir[p1]==0)          /*If the current directory is a at a lower level than home directory */
            return 0;
        if(currdir[p1]!=homedir[p1])  /*If they don't match*/
            return 0;
        p1++;
    }   
    //printf("p1 : %d\n",p1);
    currdir[--p1]='~' ;
    return p1; 
}

