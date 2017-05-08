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


int echocall(char **args)
{
    int i;
    for(i=1;args[i]!=NULL;i++)
        printf("%s ",args[i]);
    printf("\n");

    return 0;
}
