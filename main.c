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

int main()
{
    pid_t pid;
    char home[PATH_LEN];
    twoStr *prompt ;
    prompt = (twoStr*)malloc(sizeof(twoStr));
    prompt = getprompt();
    getcwd(home,PATH_LEN);
    while(1)
    {
        char currdir[PATH_LEN];
        int tostart = getcurrdir(currdir,home);         /*Since we are replacing the path of the shell program with ~, to start defines where it is replaced, so we print form that place*/
        printf("<%s@%s:%s> ",prompt->username,prompt->hostname,currdir+tostart);
        char *line ;
        line = getInput();
        char *commands ;
        char **args ;
        while(line)
        {
          int flag = 0;
          commands = strsep(&line,";");
          // int check = checkForPipe(args);
          int check = 1;
          int dupStdout = dup(1);
          int dupStdin = dup(0);
          if(check==0)
          {
            args = segregate(commands,DELIM);
            findOutputFile(args,dupStdout);
            findInputFile(args,dupStdin);
            int len = 0;
            while(args[len] != NULL)
              len++;
            if(args[len-1][0] == '&')
            {
              flag = 1;
              args[len-1]=NULL;
            }
            pid = execute(args[0],args,flag,home);

          }
          else
          {
            piping(commands);
          }
          dup2(dupStdin,0);
          dup2(dupStdout,1);
          close(dupStdout);
          close(dupStdin);
        }
        free(args);
        free(line);
    }
    return 0;
}
