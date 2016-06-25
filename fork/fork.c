#include<stdio.h>
#include<unistd.h>

int glob = 6;
char buf[] = "a write to stdout\n";

int main()
{
	int var;
	pid_t pid;

	var = 88;
	if(write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1)
		perror("write error");
	printf("before fork\n");

	pid = fork();
	if(pid < 0)
	{
		perror("fork error");
	}
	else if(pid == 0) //child
	{
		glob++;
		var++;
	}
	else //parent
	{
		sleep(2);
	}

	printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
	return 0;
}

/*一般来说fork之后父进程和子进程的执行顺序是不确定的，这取决于内核的调度算法.
在上面的程序中，父进程是自己休眠2秒钟，以使子进程先执行。 程序中fork与I/O函数
之间的关系：write是不带缓冲的,因为在fork之前调用write，所以其数据只写到标准输
出一次。标准I/O是缓冲的，如果标准输出到终端设备，则它是行缓冲，否则它是全缓冲。
当以交互方式运行该程序时，只得到printf输出的行一次，因为标准输出到终端缓冲区由
换行符冲洗。但将标准输出重定向到一个文件时，由于缓冲区是全缓冲，遇到换行符不输
出，当调用fork时,其printf的数据仍然在缓冲区中，该数据将被复制到子进程中，该缓冲
区也被复制到子进程中.于是父子进程的都有了带改行内容的标准I/O缓冲区，所以每个进
程终止时，会冲洗其缓冲区中的数据，得到第一个printf输出两次.*/

