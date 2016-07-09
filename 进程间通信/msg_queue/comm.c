#include "comm.h"

int server_type = 1;
int client_type = 2;

static int comm_msg_queue(int flag)
{
	key_t _key = ftok(_FILE_PATH_, _PROJ_ID_);
	if(_key < 0)
	{
		printf("%d: %s\n", errno, (errno));
		return -1;
	}
	int msg_id = msgget(_key, flag);
	return msg_id;
}

int create_msg_queue()
{
	int flag = IPC_CREAT | IPC_EXCL | 0666;
	return comm_msg_queue(flag);
}

int get_msg_queue()
{ 
	int flag = IPC_CREAT;
	return comm_msg_queue(flag);
}

int send_msg(int msg_id, int send_type, const char *msg)
{
	msginfo _buf;
	_buf.mtype = send_type;
	strncpy(_buf.mtext, msg, strlen(msg) + 1);
	if(msgsnd(msg_id, &_buf, sizeof(_buf.mtext), 0) < 0)
	{
		printf("%d: %s\n", errno, strerror(errno));
		return -1;
	}
	return 0;
}

int recv_msg(int msg_id, int recv_type, char *msg_out)
{
	msginfo _buf;
	_buf.mtype = recv_type;
	memset(_buf.mtext, '\0', sizeof(_buf.mtext));
	if(msgrcv(msg_id, &_buf, sizeof(_buf.mtext), recv_type, 0) < 0)
	{
		printf("%d: %s\n", errno, strerror(errno));
		return -1;
	}
	strcpy(msg_out, _buf.mtext);
	return 0;
}

int destory_queue(int msg_id)
{
	if(msgctl(msg_id, IPC_RMID, NULL) != 0)
	{
		printf("%d: %s\n", errno, strerror(errno));
		return -1;
	}
	return 0;
}
