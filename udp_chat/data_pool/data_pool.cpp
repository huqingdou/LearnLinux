#include "data_pool.h"

data_pool::data_pool(int _cap)
	:capacity(_cap)
	 ,pool(_cap)
{
	put_step = 0;
	get_step = 0;
	sem_init(&sem_space, 0, capacity);
	sem_init(&sem_data, 0, 0);
}

bool data_pool::get_data(string &out)
{
	sem_wait(&sem_data);

	//lock
	out = pool[get_step];

	sem_post(&sem_space);

	get_step++;
	get_step %= capacity;
}

bool data_poll::put_data(string &in)
{
	sem_wait(&sem_space);

	pool[put_step] = in;

	sem_post(&sem_data);

	put_step++;
	put_step %= capacity;
}

data_pool::~data_pool()
{
	sem_destroy(&sem_space);
	sem_destroy(&sem_data);
}
