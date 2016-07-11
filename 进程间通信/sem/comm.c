#include "comm.h"

static int comm_sem_set(int num, int flag)
{
	key_t _key = ftok(_PATH_NAME_, _PROJ_ID_);
	if(_key < 0)
	{
		printf("%d: %s\n", errno, strerror(errno));
		return -1;
	}
	int sem_id = semget(_key, num, flag);
	return sem_id;
}

static int comm_sem_op(int sem_id, int op)
{
	struct sembuf _buf;
	_buf.sem_num = 0;
	_buf.sem_op = op;
	_buf.sem_flg = 0;
	if( semop(sem_id, &_buf, 1) < 0)
	{
		printf("%d： %s\n", errno, strerror(errno));
		return -1;
	}
	return 0;
}

int create_sem_set(int num)
{
	int flag = IPC_CREAT | IPC_EXCL | 0666;
	return comm_sem_set(num, flag);
}

int get_sem_set()
{
	int flag = IPC_CREAT;
	int num = 0;
	return comm_sem_set(num, flag);
}

int init_sem_set(int sem_id, int which, int val)
{
	union semun _un;
	_un.val = val;
	if(semctl(sem_id, which, SETVAL, _un) < 0)
	{
		printf("%d :%s\n", errno, strerror(errno));
		return -1;
	}
	return 0;
}

int P_sem_set(int sem_id)
{
	return comm_sem_op(sem_id, -1);
}

int	V_sem_set(int sem_id)
{
	return comm_sem_op(sem_id, 1);
}

int destory_sem_set(int sem_id)
{
	if(semctl(sem_id, 0, IPC_RMID) < 0)
	{
		printf("%d: %s\n", errno, strerror(errno));
		return -1;
	}
	return 0;
}
