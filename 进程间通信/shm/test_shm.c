/*************************************************************************
    > File Name: test_shm.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Wed 13 Jul 2016 01:44:17 PM CST
 ************************************************************************/

#include "shm.h"

int main()
{
	int shm_id = get_shm();
	pid_t id = fork();
	if(id < 0)
	{
		printf("fork error\n");
	}
	else if(id == 0)
	{
		char *buf = at_shm(shm_id);
		int i = 0;
		while(i < 4096)
		{
			buf[i++] = 'X';
		}
		buf[4095] = '\0';
		delete_shm(buf);
	}
	else
	{
		char *buf = at_shm(shm_id);
		sleep(3);
		printf("%s\n", buf);
		delete_shm(buf);
		waitpid(id, NULL, 0);
		rm_shm(shm_id);
	}
	return 0;
}
