/*************************************************************************
    > File Name: server_select.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Thu 28 Jul 2016 09:52:08 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/select.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>


int fds[20];

int create_listen_sock(int ip, int port)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0)
	{
		perror("socket");
		exit(1);
	}

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = ip;

	if(bind(fd, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		perror("bind");
		exit(2);
	}

	if(listen(fd, 5) < 0)
	{
		perror("listen");
		exit(3);
	}

	return fd;
}

void select_server(int ip, int port)
{
	int listen_sock = create_listen_sock(ip, port);
	struct sockaddr_in client;
	socklen_t client_len = sizeof(client);
	size_t size = sizeof(fds)/sizeof(fds[0]);
	size_t i = 0;
	for(; i < size; i++)
	{
		fds[i] = -1;
	}
	fds[0] = listen_sock;
	fd_set read_fd;
	int max_fd = fds[0];

	while(1)
	{
		FD_ZERO(&read_fd);
		struct timeval timeout = {3, 0};
		size_t i = 0;
		for(; i < size; i++)
		{
			if(fds[i] > 0)
			{
				FD_SET(fds[i], &read_fd);
				if(fds[i] > max_fd)
				{
					max_fd = fds[i];
				}
			}
		}

		switch(select(max_fd+1, &read_fd, NULL, NULL, &timeout))
		{
			case -1:
				perror("select");
				break;
			case 0:
				printf("time out\n");
				break;
			default:   //至少一个IO事件就绪
				{
					size_t i = 0;
					for(; i < size; i++)
					{
						if(FD_ISSET(fds[i], &read_fd) && fds[i] == listen_sock)
						{
							int accept_sock = accept(listen_sock, (struct sockaddr*)&client, &client_len);
							if(accept_sock < 0)
							{
								perror("accept");
								continue;
							}
							printf("get a client, ip is: %s, port is: %d\n",inet_ntoa(client.sin_addr), ntohs(client.sin_port));
							size_t j = 0;
							for(; j < size; j++)
							{
								if(fds[j] == -1)
								{
									fds[j] = accept_sock;
									break;
								}
							}
							if(j == size)
							{
								close(accept_sock);
							}
						}
						else if(FD_ISSET(fds[i], &read_fd) && fds[i] > 0)
						{
							char buf[1024];
							memset(buf, '\0', sizeof(buf));
							ssize_t _s = read(fds[i], buf, sizeof(buf)-1);
							if(_s < 0)
							{
								perror("read");
							}
							else if(_s == 0)
							{
								printf("client closed\n");
								close(fds[i]);
								fds[i] = -1;
							}
							else
							{
								printf("client > %s", buf);
							}
						}
					}
				}
				break;
		}
	}
}

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("Usge: %s ip port\n", argv[0]);
		return 1;
	}
	int ip = inet_addr(argv[1]);
	int port = atoi(argv[2]);

	select_server(ip, port);
	return 0;
}
