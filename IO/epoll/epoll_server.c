/*************************************************************************
    > File Name: epoll_server.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Fri 29 Jul 2016 02:40:06 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<fcntl.h>

int create_socket(int port, int ip)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("socket");
		exit(1);
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = port;
	local.sin_addr.s_addr = ip;

	if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
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

static int set_noblock(int sock)
{
	int fl = fcntl(sock, F_GETFL);
	return fcntl(sock, F_SETFL, fl | O_NONBLOCK);
}

void epoll_server(int port, int ip)
{
	int listen_socket = create_socket(port, ip);
	int epfd = epoll_create(256);
	if(epfd < 0)
	{
		perror("epoll_create");
		exit(1);
	}

	struct epoll_event eve;
	eve.events = EPOLLIN;
	eve.data.fd = listen_socket;
	epoll_ctl(epfd, EPOLL_CTL_ADD, listen_socket, &eve);

	struct epoll_event ready_ev[128];
	int ready_ev_size = 128;
	int timeout = -1;
	int nums = 0;
	while(1)
	{
		switch((nums = epoll_wait(epfd, ready_ev, ready_ev_size, timeout)))
		{
			case -1:
				perror("epoll_wait");
				break;
			case 0:
				printf("time out\n");
				break;
			default:
				{
					int i = 0;
					for(; i < nums; i++)
					{
						int fd = ready_ev[i].data.fd;

						if(fd == listen_socket && (ready_ev[i].events & EPOLLIN))
						{
							struct sockaddr_in peer;
							socklen_t len = sizeof(peer);
							int newsock = accept(listen_socket, (struct sockaddr*)&peer, &len);
							if(newsock > 0)
							{
								printf("get a client, ip is:%s, port is: %d\n", inet_ntoa(peer.sin_addr), ntohs(peer.sin_port));
								eve.events = EPOLLIN | EPOLLET;
								eve.data.fd = newsock;

								set_noblock(newsock);

								epoll_ctl(epfd, EPOLL_CTL_ADD, newsock, &eve);

							}
						}
						else
						{
							if(ready_ev[i].events & EPOLLIN)
							{
								char buf[102400];
								memset(buf, '\0', sizeof(buf));
								ssize_t s = recv(fd, buf, sizeof(buf)-1, 0);
								if(s > 0)
								{
									printf("client > %s", buf);
									eve.events = EPOLLOUT | EPOLLET;
									eve.data.fd = fd;
									epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &eve);
								}
								else if(s == 0)
								{
									printf("client close\n");
									epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
									close(fd);
								}
								else
								{
									perror("recv");
								}
							}
							else if(ready_ev[i].events & EPOLLOUT)
							{
								const char *msg = "HTTP/1.1 200 OK\r\n\r\n<h1>hello world</h1>\r\n";
								send(fd, msg, strlen(msg), 0);
								epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
								close(fd);
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
		printf("%s ip port\n", argv[0]);
		exit(1);
	}

	int port = htons(atoi(argv[2]));
	int ip = inet_addr(argv[1]);
	epoll_server(port, ip);
	return 0;
}
