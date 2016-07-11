#pragma once

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<errno.h>
#include<string.h>

#define _PATH_NAME_ "."
#define _PROJ_ID_ 0x666

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *_buf;
};

int create_sem_set(int num);
int get_sem_set();
int destory_sem_set(int sem_id);
int init_sem_set(int sem_id, int which, int val);
int P_sem_set(int sem_id);
int V_sem_set(int sem_id);
