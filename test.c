#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#define DELIM " \t\r\n\a\"\'"

struct command
{
	const char **argv;
};

int spawn_proc (int in, int out, char *com)
{
	pid_t pid;
	char **args ;
	args = segregate(com,DELIM);
	if (in != 0)
	{
		dup2 (in, 0);
		close (in);
	}
	if (out != 1)
	{
		dup2 (out, 1);
		close (out);
	}
	// int tostart = getcurrdir(currdir,home);
	// if(strcmp(args[0],"pwd")==0)
	// 	printf("%s\n",currdir+tostart);
	// else if(strcmp(args[0],"cd")==0)
	// {
	// 	if(!args[1])
	// 	{
	// 		args[1]=(char *)malloc((sizeof(char)*(3)));
	// 		strcpy(args[1],"~");
	// 	}
	// 	changedir(args[1],home);
	// }
	// else if(strcmp(args[0],"exit")==0)
	// 	_exit(-1);
	// else if(strcmp(args[0],"echo")==0)
	// 	echocall(args);
	// else if(strcmp(args[0],"pinfo")==0)
	// {
	// 	pinfo(args,home);
	// }
	if ((pid = fork ()) == 0)
	{
		return execvp (args[0], args);
	}
	return pid;
}
int piping(char * comd)
{
	int i;
	int in, fd [2];
	char * split;
	char **pipedOnes ;
	if(comd[strlen(comd)-1] == '\n')
	{
		comd[strlen(comd)-1] = '\0';
	}
	pipedOnes = segregate(comd,"|");
	int len = 0;
	while(pipedOnes[len] != NULL)
		len++;
	findInputFile(comd,dup(0));
	in = dup(0);
	for (i = 0; i < len-1; ++i)
	{
		pipe (fd);
		spawn_proc (in, fd [1], pipedOnes[i]);
		close (fd [1]);
		in = fd [0];
	}

	// char *par[1000];
	// char * Split_temp = strtok(com[i]," ");
	// for(int i = 0; i < 1000; i++)
	// {
	// 	par[i] = Split_temp;
	// 	Split_temp = strtok(NULL," ");
	// 	if(par[i] == NULL) break;
	// }
	char **args = segregate(pipedOnes[i],DELIM);
	findOutputFile(args,dup(1));
	pid_t pid = fork();

	// Error
	if (pid == -1) {
		char* error = strerror(errno);
		printf("fork: %s\n", error);
		return 1;
	}

	// Child process
	else if (pid == 0) {
		// execute execvp
		if (in != 0)
			dup2 (in, 0);
		execvp(args[0], args);
		char* error = strerror(errno);
		printf("shell: %s: %s\n", args[0], error);
		return 0;
	}

	// Parent process
	else {
		int childStatus;
		waitpid(pid, &childStatus, 0);
		return 1;
	}
	return 0;
}
