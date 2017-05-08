#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<linux/limits.h>
#include<signal.h>
#include<errno.h>
#include "mylib.h"

extern Node head;
Node * lead, * follow;

void handler(int signum)
{
	pid_t proc_id;
	int   status;
	(proc_id = waitpid(-1, &status, WNOHANG));
	if (proc_id>0)
	{
		if(head.next != NULL)
			follow = head.next;

		if(follow->next != NULL && follow->pid != proc_id)
		{
			lead = follow->next;

			while(lead != NULL)
			{
				if(lead->pid == proc_id)
				{
					printf("%s [%d] %d\n", lead->pname, proc_id,WEXITSTATUS(status));
					follow->next = lead->next;
					free(lead);
					break;
				}
				else
				{
					follow = lead;
					lead = follow->next;
				}
			}
			return;
		}

		else if(follow->pid == proc_id)
		{
			printf("%s [%d] %d\n", follow->pname, proc_id,WEXITSTATUS(status));
			head.next = follow->next;
			free(follow);
			return;
		}

	}
	return;
}


void terminal_init()
{
	int flag;
	char hostname[100],username[100], **commandInput, **commandSegregated, buffer[PATH_MAX + 1];
	gethostname(hostname, sizeof(hostname));
	getlogin_r(username,sizeof(username));
	char *initialDir = getcwd(buffer, PATH_MAX + 1);
	do{
		int i,j;
		char *line = NULL;
		ssize_t bufsize = 0;
		char *dir = getCurrentDir(initialDir);
		printf("<%s@%s: %s > ",username,hostname,dir);

		getline(&line,&bufsize,stdin);
		commandInput = split(line, ";");
		for(i= 0; commandInput[i]!=NULL; ++i)
		{
			int tempin=dup(0);
			int tempout=dup(1);
			commandSegregated = split(commandInput[i], " \t\n\r\a");
			setOutput(commandSegregated, tempout);
			setInput(commandSegregated, tempin);
			j=0;
			while(commandSegregated[j]!=NULL && strcmp(commandSegregated[j],"<")!=0 && strcmp(commandSegregated[j],">")!=0 && strcmp(commandSegregated[j],">>")!=0)
				j++;
			if(commandSegregated[j]!=NULL)
				commandSegregated[j]=NULL;
			flag = commandExecute(commandSegregated, initialDir);
			signal(SIGCHLD, handler);
			dup2(tempin,0);
			dup2(tempout,1);
			close(tempin);
			close(tempout);
			if(flag == 0)
				break;
		}

	}while(flag);
}
