#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

int g_val = 0;

void fun()
{
	printf("child exit\n");
}

int main()
{
	int val = 0;
	pid_t id = vfork();
	if(id < 0)
	{
		exit(1);
	}
	else if(id == 0) //child
	{
		atexit(fun);
		printf("this is child process.\n");
	//	++g_val;
	//	++val;
		sleep(3);
		exit(0);
	}
	else
	{
		printf("this is father process\n");
	//	printf("father exit, g_val = %d, val = %d\n", g_val, val);
	}
	return 0;
}
