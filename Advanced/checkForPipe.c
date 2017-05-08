#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#define DELIM " \t\r\n\a\"\'"

int checkForPipe(char *commands)
{
  // char **tmp = segregate(commands,DELIM);
  int i;
  while(commands[i]==0)
  {
    if(commands[i]=='|')
      return 1;
  }
  return 0;
}
