/*************************************************************************
    > File Name: test.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Wed 13 Jul 2016 10:26:38 AM CST
 ************************************************************************/

#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void handler(int sig)
{
	printf("get a single, num is: %d\n", sig);
}

void print_sig(sigset_t *p)
{
	int i = 1;
	for(; i< 32; i++)
	{
		if(sigismember(p, i))
		{
			printf("1");
		}
		else
		{
			printf("0");
		}
	}
	printf("\n");
}

int main()
{

	signal(2, handler);
	sigset_t s, p, o;
	sigemptyset(&s);
	sigemptyset(&o);
	sigaddset(&s, SIGINT);
	sigprocmask(SIG_SETMASK, &s, &o);

	int count = 0;
	while(1)
	{
		sigemptyset(&p);
		sigpending(&p);
		print_sig(&p);
		sleep(1);
		if(count++ == 10)
		{
			sigprocmask(SIG_SETMASK, &o, NULL);
			printf("recover block\n");
			sleep(3);
		}
	}
	return 0;
}
