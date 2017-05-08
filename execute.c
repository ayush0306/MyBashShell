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


pid_t execute(char *command, char **args,int flag, char *home)
{
    char currdir[PATH_LEN];
    pid_t pid ;
    int tostart = getcurrdir(currdir,home);         /*Since we are replacing the path of the shell program with ~, to start defines where it is replaced, so we print form that place*/
    if(strcmp(command,"pwd")==0)
        printf("%s\n",currdir+tostart);
    else if(strcmp(command,"cd")==0)
    {
        if(!args[1])
        {
            args[1]=(char *)malloc((sizeof(char)*(3)));
            strcpy(args[1],"~");
        }
        changedir(args[1],home);
    }
    else if(strcmp(command,"exit")==0)
        _exit(-1);
    else if(strcmp(command,"echo")==0)
        echocall(args);
    else if(strcmp(command,"pinfo")==0)
    {
        pinfo(args,home);
    }
    else
    {
        pid=fork();
        if(pid<0)
        {
            perror("Child process could not be created\n");
            _exit(-1);
        }
        else if(pid==0)
        {
            if(flag==1)
                setpgid(0,0);
            int ret=execvp(args[0],args); //execute the command and the success value is stored in ret variable
            if(ret!=0)
            {
                perror(args[0]);
                _exit(-1);
            }
            _exit(0);
        }
        if(flag==0)
            wait();
    }
    return pid;
}
