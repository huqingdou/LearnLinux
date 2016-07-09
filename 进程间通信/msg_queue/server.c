#include "comm.h"

int main()
{
	int msg_id = create_msg_queue();
	if( msg_id < 0)
	{
		printf("%d: %s\n", errno, strerror(errno));
		return 1;
	}

	char buf[_MSG_SIZE_];
	while(1)
	{
		memset(buf, '\0', sizeof(buf));
		recv_msg(msg_id, client_type, buf);
		if(strncasecmp(buf, "quit", 4) == 0)
		{
			printf("server quit!\n");
			break;
		}
		printf("client: > %s\n", buf);
		printf("please enter:");
		fflush(stdout);
		ssize_t s = read(0 , buf, sizeof(buf) - 1);
		if(s > 0)
		{
			buf[s-1] = '\0';
		}
		send_msg(msg_id, server_type, buf);
	}

	destory_queue(msg_id);
	return 0;
}
