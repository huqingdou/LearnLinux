/*************************************************************************
    > File Name: my_dup2.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Thu 28 Jul 2016 07:52:39 PM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>

int main()
{
	int fd = open("./log", O_CREAT | O_RDWR | 0666);
	if(fd < 0)
	{
		perror("open");
		exit(1);
	}

	close(1);

	int ret = dup2(fd, 1);

	char buf[1024];
	while(1)
	{
		memset(buf, '\0', sizeof(buf));
		fgets(buf, sizeof(buf), stdin);
		if(strncmp("quit", buf, 4) == 0)
		{
			break;
		}
		printf("%s", buf);
		fflush(stdout);
	}
	return 0;
}
