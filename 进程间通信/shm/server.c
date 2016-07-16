/*************************************************************************
    > File Name: server.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Sat 16 Jul 2016 11:46:17 AM CST
 ************************************************************************/

#include<stdio.h>
#include "shm.h"

int main()
{
	int shm_id = get_shm();
	sleep(3);
	char *buf = at_shm(shm_id);
	sleep(3);
	while(1)
	{
		printf("%s\n", buf);
		sleep(1);
		if(strcmp(buf, "AAAAA") == 0)
		{
			break;
		}
	}
	delete_shm(buf);
	sleep(3);
	rm_shm(shm_id);
	return 0;
}
