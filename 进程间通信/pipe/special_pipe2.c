/*************************************************************************
    > File Name: special_pipe2.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Thu 30 Jun 2016 09:31:05 PM CST
 ************************************************************************/
//如果有指向管道写端的文件描述符没关闭(管道写端的引⽤用计数⼤大于0),而持有
//管道写端的进程也没有向管道中写数据,这时有进程从管道读端读数据,那么管道中剩
//余的数据都被读取后,再次read会阻塞,直到管道中有数据可读了才读取数据并返回。

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
		close(_pipe[1]);
	}
	else
	{
		close(_pipe[1]);
		char _mesg[100];
		int j = 0;
		while(j < 20)
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
