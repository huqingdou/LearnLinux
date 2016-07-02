/*************************************************************************
    > File Name: client.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Fri 01 Jul 2016 11:36:38 PM CST
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>

#define _PATH_ "./my_fifo"
#define _SIZE_ 100

int main()
{
	int fd = open(_PATH_, O_WRONLY);
	if(fd < 0)
	{
		printf("open error\n");
		return 1;
	}
	char buf[_SIZE_];
	memset(buf, '\0', sizeof(buf));
	while(1)
	{
		printf("please write:");
		scanf("%s", buf);
		int ret = write(fd, buf, strlen(buf) + 1);
		if(ret < 0)
		{
			printf("write error\n");
			break;
		}
		if(strncmp(buf, "quit", 4) == 0)
		{
			break;
		}
	}
	close(fd);
	return 0;
}
