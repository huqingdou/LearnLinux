/*************************************************************************
    > File Name: special_pipe3.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Thu 30 Jun 2016 09:47:16 PM CST
	如果所有指向管道读端的文件描述符都关闭了(管道读端的引⽤用计数等于0),这时
有进程向管道的写端write,那么该进程会收到信号SIGPIPE,通常会导致进程异常终止。
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/wait.h>

int main()
{
	int _pipe[2];
	int ret = pipe(_pipe);
	if(ret == -1)
	{
		printf("create pipe error! error code is: %d\n", errno);
		return 1;
	}
	pid_t id = fork();
	if(id < 0)
	{
		printf("fork error!\n");
		return 2;
	}
	else if(id == 0)
	{
		close(_pipe[0]);
		char *mesg = NULL;
		int i = 0;
		while(i < 20)
		{
			if(i < 10)
			{
				mesg = "I am child!";
				write(_pipe[1], mesg, strlen(mesg) + 1);
			}
			sleep(1);
			i++;
		}
	}
	else
	{
		close(_pipe[1]);
		char _mesg[100];
		int j = 0;
		while(j < 3)
		{
			memset(_mesg, '\0', sizeof(_mesg));
			int ret = read(_pipe[0], _mesg, sizeof(_mesg));
			printf("%s: code is: %d\n", _mesg, ret);
			j++;
		}
		close(_pipe[0]);
		sleep(10);
		if(waitpid(id, NULL, 0) < 0)
		{
			return 3;
		}
	}
	return 0;
}
