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

	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
	{
		perror("socket");
		return 2;
	}

	struct sockaddr_in server_socket;
	bzero(&server_socket, sizeof(server_socket));
	server_socket.sin_family = AF_INET;
	server_socket.sin_addr.s_addr = inet_addr(argv[1]);
	server_socket.sin_port = htons(atoi(argv[2]));

	if(bind(sock, (struct sockaddr*)&server_socket, sizeof(server_socket)) < 0)
	{
		perror("bind");
		close(socket);
		return 3;
	}

	struct sockaddr_in client_socket;
	socklen_t len = sizeof(client_socket);

	char buf[1024];
	while(1)
	{
		memset(buf, '\0', sizeof(buf));

		recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&client_socket, &len);
		printf("get a client,ip is: %s, port is: %d\n", inet_ntoa(client_socket.sin_addr), ntohs(client_socket.sin_port));
		printf("client > %s\n", buf);
	
		sendto(sock, buf, sizeof(buf), 0, (struct sockaddr*)&client_socket, len);
	}
	close(sock);
	return 0;
}
