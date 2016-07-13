#pragma once

#include<stdio.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/wait.h>
#include<unistd.h>

#define _PATH_ "."
#define _PROJECT_ 8888
#define _SHM_SIZE_ 4*1024

int get_shm();
char *at_shm(int shm_id);
int delete_shm(char *addr);
int rm_shm(int shm_id);

