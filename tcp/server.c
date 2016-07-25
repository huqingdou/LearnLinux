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

	struct sockaddr_in server_socket;
	bzero(&server_socket, sizeof(server_socket));
	server_socket.sin_family = AF_INET;
	server_socket.sin_addr.s_addr = htonl(atoi(argv[1]));
	server_socket.sin_port = htons(atoi(argv[2]));

	if(bind(sock, (struct sockaddr*)&server_socket, sizeof(server_socket)) < 0)
	{
		perror("bind");
		close(socket);
		return 3;
	}

	if(listen(sock, 5) < 0)
	{
		perror("listen");
		close(sock);
		return 4;
	}

	printf("bind and listen success, will accept...\n");

	while(1)
	{
		socklen_t len = 0;
		struct sockaddr_in client_socket;
		int client_sock = accept(sock, (struct sockaddr*)&client_socket, &len);
		if(client_sock < 0)
		{
			perror("accept");
			close(sock);
			return 5;
		}

	    char buf_ip[INET_ADDRSTRLEN];
		memset(buf_ip, '\0', sizeof(buf_ip));
		inet_ntop(AF_INET, &client_socket.sin_addr, buf_ip, sizeof(buf_ip));
	    printf("get connect, ip is: %s, port is: %d\n", buf_ip, ntohs(client_socket.sin_port));

	  	pid_t id = fork();
		if(id == 0)
		{	
			while(1)
			{
				char buf[1024];
				memset(buf, '\0', sizeof(buf));
	
				read(client_sock, buf, sizeof(buf));
				printf("client > %s\n", buf);
	
				printf("server > ");
	
				memset(buf, '\0', sizeof(buf));
				fgets(buf, sizeof(buf), stdin);
				buf[strlen(buf) - 1] = '\0';
				write(client_sock, buf, strlen(buf));
			}	
		}
	    else if(id > 0)
		{
			waitpid(-1, 0, WNOHANG);
		}
	}
	close(sock);
	return 0;
}
