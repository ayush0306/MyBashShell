#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<linux/limits.h>
#include<signal.h>
#include<errno.h>
#include "mylib.h"
char** split(char *commandInput, char *delims)
{
	const char *delim = delims;
	int numOfTokens = 64, position = 0, size = 64;
	char **tokens = malloc(numOfTokens*sizeof(char*));
	char *token;

	if(!tokens)
	{
		fprintf(stderr, "bash: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(commandInput, delim);
	while(token!=NULL)
	{
		tokens[position] = token;
		position++;

		if(position>=numOfTokens)
		{
			numOfTokens += size;
			tokens = realloc(tokens, numOfTokens*sizeof(char*));
			if(!tokens)
			{
				fprintf(stderr, "bash: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, delim);
	}

	tokens[position] = NULL;
	return tokens;
}
