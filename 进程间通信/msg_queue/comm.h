#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<string.h>
#include<time.h>
#include<errno.h>

#define _MSG_SIZE_ 1024
#define _FILE_PATH_ "/tmp"
#define _PROJ_ID_ 0x666

extern int server_type;
extern int client_type;

typedef struct _msginfo
{
	long mtype;
	char mtext[_MSG_SIZE_];
}msginfo;

int create_msg_queue();
int get_msg_queue();
int send_msg(int msg_id, int send_tyype, const char* msg);
int recv_msg(int msg_id, int recv_type, char *msg_out);
int destory_queue(int msg_id);

