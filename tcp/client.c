#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/wait.h>

int main(int argc, int *argv[])
{
	if(argc != 3)
	{
		printf("Usage: %s ip_address port_number\n", argv[0]);
		return 1;
	}

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("socket");
		return 2;
	}

	struct sockaddr_in remote_socket;
	bzero(&remote_socket, sizeof(remote_socket));
	remote_socket.sin_family = AF_INET;
	remote_socket.sin_addr.s_addr = inet_addr(argv[1]);
	remote_socket.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&remote_socket, sizeof(remote_socket)) < 0)
	{
		perror("connect");
		return 3;
	}

	char buf[1024];
	memset(buf, '\0', sizeof(buf));

	while(1)
	{
		printf("client > ");
		fgets(buf, sizeof(buf), stdin);
		buf[strlen(buf) - 1] = '\0';
		write(sock, buf, sizeof(buf));
		if(strncasecmp(buf, "quit", 4) == 0)
		{
			printf("quit!\n");
			break;
		}
		read(sock, buf, sizeof(buf));
	
		printf("server > %s\n", buf);
	
	}	
	close(sock);
	return 0;
}
