/*************************************************************************
    > File Name: my_dup.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Thu 28 Jul 2016 07:31:52 PM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>

int main()
{
	int fd = open("./log", O_CREAT | O_RDWR | 0666);
	if(fd < 0)
	{
		perror("open");
		exit(1);
	}

	close(1);           //关闭想要重定向的fd
	int new_fd = dup(fd);
	if(new_fd == -1)
	{
		perror("dup");
		exit(2);
	}

	close(fd);

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
	close(new_fd);
	return 0;
}
