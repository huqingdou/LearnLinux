#pragma once

#include<iostream>
using namespace std;

#include<vector>
#include<semaphore.h>
#include<pthread.h>

class data_pool
{
public:

	data_pool(int _cap = 64);
	bool get_data(string &out);
	bool put_data(string &in);
	~data_pool();

private:
	vector<string> pool;
	int capacity;
	sem_t sem_space;
	sem_t sem_data;
	int put_step;
	int get_step;
};
