#include"comm.h"

int main()
{
	int sem_id = create_sem_set(1);
	init_sem_set(sem_id, 0, 1);
	pid_t id = fork();
	if(id == 0)
	{
		int c_sem_id = get_sem_set();
		while(1)
		{
			P_sem_set(c_sem_id);
			printf("A");
			usleep(30000);
			fflush(stdout);
			printf("A");
			usleep(30000);
			fflush(stdout);
			V_sem_set(c_sem_id);
		}
	}
	else
	{
		while(1)
		{
			P_sem_set(sem_id);
			printf("B");
			usleep(30000);
			fflush(stdout);
			printf("B");
			usleep(30000);
			fflush(stdout);
			V_sem_set(sem_id);
		}
		pid_t ret = waitpid(id, NULL, 0);
		if(ret == id)
		{
			printf("wait success!\n");
		}
		else
		{
			printf("wait failed!\n");
		}
	}
	return 0;
}
