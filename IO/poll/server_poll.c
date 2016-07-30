/*************************************************************************
    > File Name: server_select.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Thu 30 Jul 2016 09:52:08 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<poll.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>

#define _NUM_ 10

int create_listen_sock(int ip, int port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("socket");
		exit(1);
	}

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = ip;

	if(bind(sock, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		perror("bind");
		exit(2);
	}

	if(listen(sock, 5) < 0)
	{
		perror("listen");
		exit(3);
	}

	return sock;
}

void select_server(int ip, int port)
{
	int listen_sock = create_listen_sock(ip, port);
	struct sockaddr_in client;
	socklen_t client_len = sizeof(client);

	struct pollfd fds[_NUM_];
	fds[0].fd = listen_sock;
	fds[0].events = POLLIN;
	fds[0].revents = 0;

	size_t i = 1;
	for(; i < _NUM_; i++)
	{
		fds[i].fd = -1;
		fds[i].events = 0;
		fds[i].revents = 0;
	}
	int max_fd = 1;
	int timeout = 3000;

	while(1)
	{
		switch(poll(fds, max_fd, timeout))
		{
			case -1:
				perror("poll");
				break;
			case 0:
				printf("timeout\n");
				break;
			default:
				{
					size_t i = 0;
					for(; i < _NUM_; i++)
					{
						if((fds[i].fd == listen_sock) && (fds[i].revents == POLLIN))
						{
							int accept_sock = accept(listen_sock, (struct sockaddr*)&client, &client_len);
							if(accept_sock < 0)
							{
								perror("accept");
								continue;
							}
							printf("get a client, ip is: %s, port is: %d\n",inet_ntoa(client.sin_addr), ntohs(client.sin_port));
							size_t j = 0;
							for(; j < _NUM_; j++)
							{
								if(fds[j].fd == -1)
								{
									fds[j].fd = accept_sock;
									fds[j].events = POLLIN;
									max_fd++;
									break;
								}
							}
							if(j == _NUM_)
							{
								close(accept_sock);
							}
						}

						else if((fds[i].fd > 0) && (fds[i].revents == POLLIN))
						{
							char buf[1024];
							memset(buf, '\0', sizeof(buf));
							ssize_t _s = read(fds[i].fd, buf, sizeof(buf)-1);
							if(_s < 0)
							{
								perror("read");
							}
							else if(_s == 0)
							{
								printf("client closed\n");
								struct pollfd tmp = fds[i];
								fds[i] = fds[max_fd - 1];
								fds[max_fd - 1] = tmp;

								close(fds[max_fd-1].fd);
								fds[max_fd-1].events = 0;
								fds[max_fd-1].revents = 0;
								--max_fd;
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
