#include "shm.h"

int get_shm()
{
	key_t key = ftok(_PATH_, _PROJECT_);
	int flag = IPC_CREAT | 0666;
	int shm_id = shmget(key, _SHM_SIZE_, flag);
	if(shm_id == -1)
	{
		printf("get share memory error!\n");
	}
	else
	{
		printf("get share memory success!\n");
	}
	return shm_id;
}

char *at_shm(int shm_id)
{
	return (char*)shmat(shm_id, NULL, 0);
}

int delete_shm(char *addr)
{
	return shmdt(addr);
}

int rm_shm(int shm_id)
{
	return shmctl(shm_id, IPC_RMID, NULL);
}
