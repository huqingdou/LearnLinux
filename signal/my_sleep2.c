/*************************************************************************
    > File Name: my_sleep2.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Wed 13 Jul 2016 01:06:09 PM CST
 ************************************************************************/

#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void sig_alrm(int sig)
{

}

unsigned int my_sleep(unsigned int time)
{
	struct sigaction newact, oldact;
	sigset_t newmask, oldmask, suspmask;
	unsigned int unslept;

	newact.sa_handler = sig_alrm;
	sigemptyset(&newact.sa_mask);
	newact.sa_flags = 0;
	sigaction(SIGALRM, &newact, &oldact);

	sigemptyset(&newmask);
	sigaddset(&newmask, SIGALRM);
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);

	alarm(time);

	suspmask = oldmask;
	sigdelset(&suspmask, SIGALRM);
	sigsuspend(&suspmask);

	unslept = alarm(0);
	sigaction(SIGALRM, &oldact, NULL);

	sigprocmask(SIG_SETMASK, &oldmask, NULL);
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
