/*************************************************************************
    > File Name: proc.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Mon 30 May 2016 07:58:21 PM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<string.h>

#define _SIZE_ 103

void process()
{
	char str[_SIZE_];
	memset(str,'\0',sizeof(str));
	str[0] = '[';
	str[_SIZE_-2] = ']';
	char index[5] = "-\\|/\0";
	int i = 1;
	for(; i < _SIZE_ - 2; i++)
	{
		str[i]='#';
		printf("%s[%d%%]%c\r", str, i, index[i%4]);
		usleep(100000);
	}
	printf("\n");
}

int main()
{
	process();
	return 0;
}
