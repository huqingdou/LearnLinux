#include "udp_server.h"

static void *recv_and_product(void *arg)
{
	udp_server *ser = (udp_server*)arg;
	string unuse;
	while(1)
	{
		ser->recv_data(unuse);
	}
}

static void *broadcast_and_consum(void* arg)
{
	udp_server *ser = (udp_server*)arg;
	while(1)
	{
		ser->broadcast_data();
	}
}
int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		cout<<"Usage:"<< argv[0]<<"ip, port"<<endl;
		return 1;
	}

	udp_server *ser = new udp_server(argv[1], atoi(argv[2]));

	pthread_t id1, id2;
	pthread_create(&id1, NULL, recv_and_product, (void*)ser);
	pthread_create(&id2, NULL, broadcast_and_consum, (void*)ser);

	pthread_join(id1, NULL);
	pthread_join(id2, NULL);
	//	string out;
//	while(1)
//	{
//		ser->recv_data(out);
//		cout<<"client:"<<out<<endl;
//	}
	delete ser;
	ser = NULL;
	return 0;
}
