/*************************************************************************
    > File Name: server.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Fri 01 Jul 2016 11:29:36 PM CST
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

#define _PATH_ "./my_fifo"
#define _SIZE_ 100

int main()
{
	int ret = mkfifo(_PATH_, 0666 | S_IFIFO);
	if(ret == -1)
	{
		printf("mkfifo error\n");
		return 1;
	}
	int fd = open(_PATH_, O_RDONLY);
	if(fd < 0)
	{
		printf("open file error!\n");
		return 1;
	}
	char buf[_SIZE_];
	memset(buf, '\0', sizeof(buf));
	while(1)
	{
		int ret = read(fd, buf, sizeof(buf));
		if(ret <= 0)
		{
			printf("read and or error!\n");
			break;
		}
		printf("%s\n", buf);
		if(strncmp(buf, "quit", 4) == 0)
		{
			break;
		}
	}
	close(fd);
	return 0;
}
