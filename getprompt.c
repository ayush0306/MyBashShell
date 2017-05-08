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


typedef struct twoStr{
    char username[PATH_LEN];
    char hostname[PATH_LEN];
}twoStr;

twoStr * getprompt()
{
    struct passwd *uname ;
    char host[PATH_LEN];
    twoStr * prompt;
    prompt = (twoStr*)malloc(sizeof(twoStr));
    uname = getpwuid(getuid());
    if(uname==NULL)
    {
        printf("Error in retrieving username\n");
        _exit(0);
    }
    strcpy(prompt->username,uname->pw_name);
    int check = gethostname(host,1000);
    if(check!=0)
    {
        printf("Error in retrieving hostname\n");
        _exit(0);
    }
    strcpy(prompt->hostname,host);
    return prompt ;
}

