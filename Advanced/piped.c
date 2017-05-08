#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
struct command
{
	const char **argv;
};

#define DELIM " \t\r\n\a\"\'"
#define PATH_LEN 1000
#define INP_LEN 10000

int spawn_proc (int in, int out, char *com,char *home)
{
	// printf("%s\n",com);
	pid_t pid;
	char **args ;
	char currdir[PATH_LEN];
	args = segregate(com,DELIM);
	int tostart = getcurrdir(currdir,home);         /*Since we are replacing the path of the shell program with ~, to start defines where it is replaced, so we print form that place*/
	if(strcmp(args[0],"pwd")==0)
	{
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
		printf("%s\n",currdir+tostart);
	}
	else if(strcmp(args[0],"pinfo")==0)
	{
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
		pinfo(args,home);
	}
	else if((pid = fork ()) == 0)
	{
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
		return execvp (args[0], args);
	}
	return pid;
}
int piping(char * comd,char *home)
{
	int i;
	int in, fd [2];
	int saved = dup(1);
	char * split;
	char **pipedOnes ;
	if(comd[strlen(comd)-1] == '\n')
	{
		comd[strlen(comd)-1] = '\0';
	}
	pipedOnes = segregate(comd,"|");
	int temp = 0;
	while(pipedOnes[temp] != NULL)
		temp++;
	// printf("No of piped commands are : %d\n",temp);
	if(temp==1)
		return -1;
	char inpredirect[INP_LEN];
	strcpy(inpredirect,pipedOnes[0]);
	int result = findInputFile(segregate(inpredirect,DELIM),dup(0));
	if(result==1)
	{
		int j = 0;
		for(j=0;j<INP_LEN;j++)
		{
			if(pipedOnes[0][j]=='<')
			{
				pipedOnes[0][j]=0;
				break;
			}
		}
	}
	in = dup(0);
	for (i = 0; i < temp-1; ++i)
	{
		pipe (fd);
		spawn_proc (in, fd [1], pipedOnes[i],home);
		close (fd [1]);
		in = fd [0];
	}
	dup2(saved,1);
	char **par = segregate(pipedOnes[i],DELIM);
	findOutputFile(par,dup(1));
	// printf("Check1\n");
	char currdir[PATH_LEN];
	// printf("Check2\n");
	pid_t pid ;
	int tostart = getcurrdir(currdir,home);         /*Since we are replacing the path of the shell program with ~, to start defines where it is replaced, so we print form that place*/
	if(strcmp(par[0],"pwd")==0)
		printf("%s\n",currdir+tostart);
	else if(strcmp(par[0],"cd")==0)
	{
		if(!par[1])
		{
			par[1]=(char *)malloc((sizeof(char)*(3)));
			strcpy(par[1],"~");
		}
		changedir(par[1],home);
	}
	else if(strcmp(par[0],"quit")==0)
		_exit(-1);
	else if(strcmp(par[0],"echo")==0)
		echocall(par);
	else if(strcmp(par[0],"pinfo")==0)
	{
		pinfo(par,home);
	}
	else
	{
		pid = fork();
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
			execvp(par[0], par);
			char* error = strerror(errno);
			printf("shell: %s: %s\n", par[0], error);
			return 0;
		}

		// Parent process
		else {
			int childStatus;
			waitpid(pid, &childStatus, 0);
			return 1;
		}
	}
	return 0;
}
