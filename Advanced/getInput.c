#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include<pwd.h>

#define INP_LEN 10000

char *getInput()
{
  int ind = 0;
  ssize_t x=(1<<16);
  char *inp= malloc(sizeof(char) * INP_LEN);
  if (!inp)
    {
      fprintf(stderr, "lsh: allocation error\n");
      exit(EXIT_FAILURE);
    }
  getline(&inp,&x,stdin);
  return inp;
}
