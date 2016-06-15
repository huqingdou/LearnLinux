#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>

int main()
{
	uid_t uid;
	uid_t euid;
	pid_t pid;
	pid_t ppid;
	pid = fork();
	if(pid < 0)
	{
		printf("%d\n", errno);
		exit(2);
	}
	else if(pid == 0)   //child
	{
		printf("child->pid:%d, ppid:%d, uid:%d, euid:%d\n",getpid(),getppid(),getuid(),geteuid());
		exit(0);
	}
	else //father
	{	
		printf("father->pid:%d, ppid:%d, uid:%d, euid:%d\n",getpid(),getppid(),getuid(),geteuid());
		sleep(2);
	}
	return 0;
}
