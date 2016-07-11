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
			fflush(stdout);
			usleep(30000);
			printf("A");
			fflush(stdout);
			usleep(30000);
			V_sem_set(c_sem_id);
		}
	}
	else
	{
		int count = 3;
		while(1)
		{
			P_sem_set(sem_id);
			printf("B");
			fflush(stdout);
			usleep(30000);
			printf("B");
			fflush(stdout);
			usleep(30000);
			V_sem_set(sem_id);
		}
	}
}
