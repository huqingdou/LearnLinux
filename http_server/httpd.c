#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/sendfile.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<pthread.h>
#include<string.h>

#define _SIZE_ 1024

static void Usage(const char* proc)
{
	printf("Usage: %s ip port\n", proc);
}

static void bad_request(int sock)
{

	char buf[1024];
	sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
	send(sock, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(sock, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(sock, buf, strlen(buf), 0);
	sprintf(buf, "<html><br><p>your enter message is a bad request</p></br></html>\r\n");
	send(sock, buf, strlen(buf), 0);
}

void not_found(int sock)
{
	char buf[1024];
	sprintf(buf, "HTTP/1.0 404 NOT_FOUND\r\n");
	send(sock, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(sock, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(sock, buf, strlen(buf), 0);
	sprintf(buf, "<html><br><p>Not found, The server could not fulfill</p></br></html>\r\n");
	send(sock, buf, strlen(buf), 0);
}

void cannot_execute(int sock)
{

	char buf[1024];
	sprintf(buf, "HTTP/1.0 500 Internal Server Error\r\n");
	send(sock, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(sock, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(sock, buf, strlen(buf), 0);
	sprintf(buf, "<html><br><p>Error prohibited CGI execution.</p></br></html>\r\n");
	send(sock, buf, strlen(buf), 0);
}

int create_listen_sock(int ip, int port)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0)
	{
		perror("socket");
		exit(1);
	}

	int opt = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

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

static int get_line(int sock, char buf[], int len)
{
	if(!buf || len < 0)
	{
		return -1;
	}

	int i = 0;
	int ret = 0;
	char ch = '\0';
	while(i < len - 1 && ch != '\n')
	{
		ret = recv(sock, &ch, 1, 0);
		if(ret > 0)
		{
			if(ch == '\r')
			{
				recv(sock, &ch, 1, MSG_PEEK);
				if(ch == '\n')
				{
					recv(sock, &ch, 1, 0);
				}
				else
				{
					ch = '\n';
				}
			}
			buf[i++] = ch;
		}
		else
		{
			ch = '\n';
		}
	}

	buf[i] = '\0';

	return i;
}

void clear_head(int sock)
{
	int ret = 0;
	char buf[_SIZE_];
	memset(buf, '\0', sizeof(buf));
	do{
		ret = get_line(sock, buf, sizeof(buf));
	}while(ret > 0 && strcmp(buf, "\n") != 0);

}

void echo_errno(int sock, int number)
{
	switch(number)
	{
		case 400:
			bad_request(sock);
			break;
		case 404:
			not_found(sock);
			break;
		case 500:
			cannot_execute(sock);
			break;
		default:
			perror("error_code");
			break;
	}
	close(sock);
}

int echo_www(int sock, const char *path, ssize_t size)
{
	int fd = open(path, O_RDONLY);
	if(fd < 0)
	{
		echo_errno(sock, 404);
		return -1;
	}

	char buf[_SIZE_];
	memset(buf, '\0', sizeof(buf));
	sprintf(buf, "HTTP/1.0 200 OK\r\n\r\n");
	send(sock, buf, strlen(buf), 0);

	if(sendfile(sock, fd, NULL, size) < 0)
	{
		echo_errno(sock, 404);
		return -2;
	}
	close(fd);
}

static int execute_cgi(int sock, const char *path, const char *method, const char *query_string)
{
	int content_len = -1;
	int ret = 0;
	char buf[_SIZE_];

	if(strcasecmp(method, "GET") == 0)
	{
		clear_head(sock);
	}
	else          //POST
	{
		do{
			memset(buf, '\0', sizeof(buf));
			ret = get_line(sock, buf, sizeof(buf));
			if(strncasecmp(buf, "Content-Length: ", strlen("Content-Length: ")) == 0)
			{
				content_len = atoi(&buf[16]);
			}
		}while(ret > 0 && strcmp(buf, "\n") != 0);

		if(content_len == -1)
		{
			echo_errno(sock, 400);
			return -2;
		}
	}

	//cgi，
	int cgi_input[2] = {0, 0};
	int cgi_output[2] = {0, 0};
	//printf("Content_Length:%d\n",content_len);

	sprintf(buf, "HTTP/1.0 200 OK\r\n\r\n");
	send(sock, buf, strlen(buf), 0);
	if(pipe(cgi_input) < 0)
	{
		echo_errno(sock, 404);
		return -3;
	}
	if(pipe(cgi_output) < 0)
	{
		echo_errno(sock, 404);
		return -4;
	}

	char query_env[_SIZE_];
	char method_env[_SIZE_];
	char content_len_env[_SIZE_];
	memset(method_env, '\0', sizeof(method_env));
	memset(query_env, '\0', sizeof(query_env));
	memset(content_len_env, '\0', sizeof(content_len_env));

	pid_t id = fork();
	if(id == 0)
	{
		close(cgi_input[1]);
		close(cgi_output[0]);

		dup2(cgi_input[0], 0);
		dup2(cgi_output[1], 1);

		sprintf(method_env, "REQUEST_METHOD=%s", method);
		putenv(method_env);
		if(strcasecmp(method, "GET") == 0)
		{
			sprintf(query_env, "QUERY_STRING=%s", query_string);
			putenv(query_env);
		}
		else //POST
		{
			sprintf(content_len_env, "CONTENT_LENGTH=%d", content_len);
			putenv(content_len_env);
		}

		execl(path, path, NULL);
		exit(1);
	}
	else
	{
		close(cgi_input[0]);
		close(cgi_output[1]);

		char c = '\0';
		int i = 0;

		if(strcasecmp(method, "POST") == 0)
		{
			for(; i < content_len; i++)
			{
				recv(sock, &c, 1, 0);
				write(cgi_input[1], &c, 1);
			}
		}

		while(read(cgi_output[0], &c, 1) > 0)
		{
			send(sock, &c, 1, 0);
		}

		close(cgi_input[1]);
		close(cgi_output[0]);

		waitpid(id, NULL, 0);
	}
}

static void* accept_request(void *arg)
{

	int sock = (int)arg;
	char buf[_SIZE_];
	int ret = 0;

#ifdef _DEBUG_

	do{
		get_line(sock, buf, _SIZE_);
		printf("%s", buf);
		fflush(stdout);
	}while(ret > 0 && strcmp(buf, "\n"));
	close(sock);
	return NULL;
#endif

	char method[_SIZE_/10];
	char path[_SIZE_];
	char url[_SIZE_];
	

	memset(method, '\0', sizeof(method));
	memset(url, '\0', sizeof(url));
	memset(path, '\0', sizeof(path));
	memset(buf, '\0', sizeof(buf));

	ret = get_line(sock, buf, sizeof(buf));
	if(ret < 0)
	{
		echo_errno(sock, 400);
		return (void*)-1;
	}

	//get method
	int i = 0;
	int j = 0;
	while((!isspace(buf[j])) && (i < sizeof(method)-1) && (j < sizeof(buf)))
	{
		method[i] = buf[j];
		++i;
		++j;
	}

	method[i] = '\0';
//	printf("method: %s\n", method);
	
	//check method
	if((strcasecmp(method, "GET") != 0) && (strcasecmp(method, "POST") != 0))
	{
		echo_errno(sock, 400);
		return (void*)-2;
	}

	//get url
	//先越过空格
	while(isspace(buf[j]) && j < sizeof(buf))
	{
		++j;
	}

	i = 0;
	while((!isspace(buf[j])) && (i < sizeof(url) -1) && (j < sizeof(buf)))
	{
		url[i] = buf[j];
		++i;
		++j;
	}

//	printf("method: %s, url_path: %s\n", method, url);

	char *query_string = NULL;
	int cgi = 0;
	
	
	if(strcasecmp(method, "POST") == 0)
	{
		cgi = 1;
	}

	if(strcasecmp(method, "GET") == 0)
	{
		query_string = url;
		while(*query_string != '?' && *query_string != '\0')
		{
			query_string++;
		}
		if(*query_string == '?')
		{
			*query_string = '\0';
			query_string++;
			cgi = 1;
		}
	}


	//if(strcasecmp(url,"/") == 0)
	//{
	//	strcpy(url, "index.html");
	//}
	
	sprintf(path, "htdoc%s", url);
	if(path[strlen(path)-1] == '/')
	{
		strcat(path, "index.html");
	}

	//path, cgi,query_string, method
//	printf("path: %s\n", path);
	struct stat st;
	if(stat(path, &st) < 0)
	{
		echo_errno(sock, 404);
		return (void*)-3;
	}
	else
	{

		if(S_ISDIR(st.st_mode))
		{
			strcat(path, "htdoc/index.html");
		}
		else if((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))
		{
			cgi = 1;
		}
		else
		{
			//do nothing
		}
	
		if(cgi)
		{	
			execute_cgi(sock, path, method, query_string);
		}
		else
		{
			clear_head(sock);
			echo_www(sock, path, st.st_size);
		}
	}

	close(sock); //不面向连接

	return (void*)0;
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		Usage(argv[0]);
		exit(1);
	}

	int ip = inet_addr(argv[1]);
	int port = atoi(argv[2]);

	int listen_sock = create_listen_sock(ip, port);

	struct sockaddr_in peer;
	socklen_t len = sizeof(peer);
	while(1)
	{
		int newfd = accept(listen_sock, (struct sockaddr*)&peer, &len);
		if(newfd < 0)
		{
			perror("accept");
			continue;
		}

		pthread_t id;
		pthread_create(&id, NULL, accept_request, (void*)newfd);

		pthread_detach(id);
	}
	return 0;
}
