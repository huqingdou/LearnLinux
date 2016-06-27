/*************************************************************************
    > File Name: 等待多个子进程.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Mon 27 Jun 2016 12:01:44 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<time.h>

#define _PROC_NUM_ 10
#define _DEFAULT_PID_ -1

int child_run()
{
	srand(time(NULL));
	int _time = rand()%30;
	printf("this child pid is: %d, sleep time is: %d\n", getpid(), _time);
	sleep(_time);
	return 0;
}

int creat_proc(pid_t *pid, int num)
{
	if(pid != NULL && num > 0)
	{
		int i = 0;
		for(; i < num; i++)
		{
			pid_t id = fork();
			if(id < 0)
			{
				printf("%s: creat %d proc failed\n", __FUNCTION__, i);
				return 1;
			}
			else if(id == 0)
			{
				int child_ret = child_run();
				exit(1);
			}
			else
			{
				pid[i] = id;
			}
		}
	}
	return 0;
}

int wait_proc(pid_t *pid, int num)
{
	int wait_ret = 0;
	if(pid != NULL && num > 0)
	{
		int i = 0;
		for(; i < num; i++)
		{
			if(pid[i] == _DEFAULT_PID_)
			{
				continue;
			}
			int status = 0;
			int ret = waitpid(pid[i], &status, 0);
			if(WIFEXITED(status) && ret == pid[i])
			{
				printf("wait child pid %d success, return code is: %d\n", pid[i], WEXITSTATUS(status));
			}
			else
			{
				printf("wait child pid %d failed\n", pid[i]);
				wait_ret = 1;
			}
		}
	}
	return wait_ret;
}
//wait子进程传入的status，它的低8位为零，次低8位为真正退出码.

int main()
{
	pid_t pid_list[_PROC_NUM_];
	int i = 0;
	for(; i < _PROC_NUM_; i++)
	{
		pid_list[i] = _DEFAULT_PID_;
	}
	if(creat_proc(pid_list, sizeof(pid_list)/sizeof(pid_list[0])) == 0)
	{
		printf("%s: create all proc successs!\n", __FUNCTION__);
	}
	else
	{
		printf("%s: not all proc create success!\n", __FUNCTION__);
	}

	if(wait_proc(pid_list, sizeof(pid_list)/sizeof(pid_list[0])) == 0)
	{
		printf("%s: wait all proc success!\n", __FUNCTION__);
	}
	else
	{
		printf("%s: not all proc wait success!\n", __FUNCTION__);
	}

	return 0;
}
