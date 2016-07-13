/*************************************************************************
    > File Name: my_sleep.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Wed 13 Jul 2016 12:49:20 PM CST
 ************************************************************************/

#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void sig_alrm(int sig)
{

}

unsigned int my_sleep(unsigned int time)
{
	struct sigaction new, old;
	unsigned int unslept = 0;
	new.sa_handler = sig_alrm;
	sigemptyset(&new.sa_mask);
	new.sa_flags = 0;
	sigaction(SIGALRM, &new, &old);
	alarm(time);
	pause();
	unslept = alarm(0);
	sigaction(SIGALRM, &old, NULL);
	return unslept;
}

int main()
{
	while(1)
	{
		my_sleep(3);
		printf("3 senconds passed...\n");
	}
	return 0;
}
