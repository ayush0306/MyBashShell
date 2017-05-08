#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int checkForPipe(char **commands)
{
  int i;
  for(i=0;i<1000;i++)
  {
    if(commands[i]==NULL)
      return 0;
    if(strcmp(commands[i],"|")==0)
      return 1;
  }
  return 2;
}
