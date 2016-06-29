/*************************************************************************
    > File Name: special_pipe1.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Wed 29 Jun 2016 02:41:09 PM CST
	如果所有指向管道写端的文件描述符都关闭了(管道写端的引⽤用计数等于0),
	而仍然有进程从管道的读端读数据,那么管道中剩余的数据都被读取后,再次read
	会返回0,就像读到文件末尾一样。 
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
		while(i < 10)
		{
			mesg = "I am child";
			write(_pipe[1], mesg, strlen(mesg) + 1);
			sleep(1);
			i++;
		}
		close(_pipe[1]);
	}
	else
	{
		close(_pipe[1]);
		char _mesg[100];
		int j = 0;
		while(j < 10)
		{
			memset(_mesg, '\0', sizeof(_mesg));
			int ret = read(_pipe[0], _mesg, sizeof(_mesg));
			printf("%s: code is: %d\n", _mesg, ret);
			j++;
		}
		if(waitpid(id, NULL, 0) < 0)
		{
			return 3;
		}
	}
	return 0;
}
