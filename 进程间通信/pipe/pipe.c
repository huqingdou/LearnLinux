/*************************************************************************
    > File Name: pipe.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Tue 28 Jun 2016 08:09:55 PM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>

int main()
{
	int _pipe[2];
	int ret = pipe(_pipe);  //父进程调用pipe开辟管道,得到两个文件描述符指向
	                        //管道的两端
	if(ret == -1)
	{
		printf("create pipe error! error code is: %d\n", errno);
		return 1;
	}
	pid_t id = fork();   //父进程调用fork创建子进程,那么子进程也有两个文件描
	                     //述符指向同一管道
//父进程关闭管道写端,子进程关闭管道读端.子进程可以往管道里写,父进程可以从管
//道里读,管道是用环形队列实现的,数据从写端流入从读端流出,这样就实现了进程间
//通信
	if(id < 0)
	{
		printf("fork error!\n");
		return 2;
	}
	else if(id == 0)
	{
		close(_pipe[0]);
		int i = 0;
		char *mesg = NULL;
		while(i < 20)
		{
			mesg = "I am child!";
			write(_pipe[1], mesg, strlen(mesg) + 1);
			sleep(1);
			i++;
		}
	}
	else
	{
		close(_pipe[1]);
		char str[100];
		int j = 0;
		while(j < 20)
		{
			memset(str, '\0', sizeof(str));
			read(_pipe[0], str, sizeof(str));
			printf("%s\n", str);
			j++;
		}
	}
	return 0;
}
