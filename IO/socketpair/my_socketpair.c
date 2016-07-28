/*************************************************************************
    > File Name: my_socketpair.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Thu 28 Jul 2016 07:15:01 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>

int main()
{
	int fd[2];
	int ret = socketpair(PF_LOCAL, SOCK_STREAM, 0, fd);
	if(ret < 0)
	{
		perror("socketpir");
		exit(1);
	}

	pid_t id = fork();
	if(id < 0)
	{
		perror("fork");
		exit(2);
	}
	else if(id == 0)
	{
		close(fd[0]);
		char buf[128];
		while(1)
		{
			memset(buf, 0, sizeof(buf));
			strcpy(buf, "hello");
			write(fd[1], buf, sizeof(buf));
			read(fd[1], buf, sizeof(buf));
			sleep(1);
			printf("%s\n", buf);
		}
	}
	else
	{
		close(fd[1]);
		char buf[128];
		while(1)
		{
			memset(buf, 0, sizeof(buf));
			read(fd[0], buf, sizeof(buf));
			printf("%s\n", buf);
			strcpy(buf, "hello too");
			write(fd[0], buf, sizeof(buf));
		}
	}
	return 0;
}
