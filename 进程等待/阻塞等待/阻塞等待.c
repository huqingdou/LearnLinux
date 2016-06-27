#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

int main()
{
	pid_t pid;
	pid = fork();
	if(pid < 0)
	{
		printf("%s fork error\n", __FUNCTION__);
		return 1;
	}
	else if(pid == 0)
	{
		printf("child is run, pid is: %d\n", getpid());
		sleep(3);
		exit(257);
	}
	else
	{
		int status = 0;
		pid_t ret = waitpid(-1, &status, 0); //阻塞式等待
		printf("this is test for wait\n");
		if(WIFEXITED(status) && ret == pid) // WIFEXITED(status): 若为正常终止子进程返回的状态,则为真.(查看进程是否是正常退出) 
		{
			printf("wait child  success, child return code is: %d\n", WEXITSTATUS(status)); // WEXITSTATUS(status): 若WIFEXITED非0，提取子进程退出码.（查看进程的退出码)
		}
		else
		{
			printf("wait child failed, return.\n");
			return 1;
		}
	}
	return 0;
}
