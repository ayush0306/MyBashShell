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


int getcurrdir(char *arr,char *homedir)
{
    getcwd(arr,PATH_LEN);
    int offset = replacebytilda(arr,homedir);
    return offset;
}

