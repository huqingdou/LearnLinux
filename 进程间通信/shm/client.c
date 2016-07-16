/*************************************************************************
    > File Name: client.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Sat 16 Jul 2016 11:51:56 AM CST
 ************************************************************************/

#include<stdio.h>
#include "shm.h"

int main()
{
	int shm_id = get_shm();
	char *buf = (char*)at_shm(shm_id);

	int index = 0;
	while(1)
	{
		buf[index++] = 'A';
		buf[index] = '\0';
		sleep(1);
		if(index > 5)
			break;
	}
	delete_shm(buf);
	sleep(3);
	return 0;
}
