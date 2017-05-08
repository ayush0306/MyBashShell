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


void pinfo(char ** params, char * basePath)
{
    char procPath[PATH_LEN];
    strcpy(procPath, "/proc/");

    if(params[1]) 
        strcat(procPath, params[1]);
    else 
        strcat(procPath, "self");

    char statPath[PATH_LEN];
    strcpy(statPath, procPath); strcat(statPath, "/stat");

    errno = 0;
    FILE * stat = fopen(statPath, "r");
    if(errno)
    {
        fprintf(stderr, "Error reading %s: %s\n", statPath, strerror(errno));
        return ;
    }
    int pid; char status; char name[20];
    fscanf(stat, "%d", &pid); fscanf(stat, "%s", name); fscanf(stat, " %c", &status);

    fprintf(stdout, "pid: %d\n", pid);
    fprintf(stdout, "Process Status: %c\n", status);
    fclose(stat);
    
    errno = 0;
    strcpy(statPath, procPath); strcat(statPath, "/statm");
    FILE * mem = fopen(statPath, "r");
    if(errno)
    {
        fprintf(stderr, "Error reading %s: %s\n", statPath, strerror(errno));
        return ;
    }
    int memSize; fscanf(mem, "%d", &memSize);
    fprintf(stdout, "Memory: %d\n", memSize);
    fclose(mem);
    
    char exePath[PATH_LEN];
    strcpy(statPath, procPath); strcat(statPath, "/exe");
    errno = 0;
    readlink(statPath, exePath, sizeof(exePath));
    if(errno)
    {
        fprintf(stderr, "Error reading symbolic link %s: %s\n", statPath, strerror(errno));
        return;
    }

    int sameChars = 0, baseL = strlen(basePath);
    for(sameChars = 0; sameChars < baseL; sameChars++)
        if(basePath[sameChars] != exePath[sameChars]) break;;
    
    char relPath[PATH_LEN];
    if(sameChars == baseL) 
    {
        relPath[0] = '~'; relPath[1] = '\0';
        strcat(relPath, (const char *)&exePath[baseL]);
    }
    else
    {
        strcpy(relPath, exePath);
        relPath[strlen(exePath)] = '\0';
    }

    int i = 0;
    while(exePath[i]) exePath[i++] = '\0';

    fprintf(stdout, "Executable Path: %s\n", relPath);
    return;
}
