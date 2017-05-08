#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include<pwd.h>

#define INP_LEN 10000

char **segregate(char *line,char del[])
{
  int status,ind,position;
  status=ind=1;
  position=0;
  char **tokens = malloc(INP_LEN*sizeof(char*)) ;
  char *token;
  if (!tokens)
    {
      fprintf(stderr, "Allocation error\n");
      exit(EXIT_FAILURE);
    }
  token = strtok(line,del);
  do
    {
      tokens[position++] = token;
      token = strtok(NULL,del);
    }while(token);
  tokens[position]=NULL; free(token);
  return tokens;
}
