#pragma once

#include<iostream>
using namespace std;
#include<map>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<pthread.h>
#include<string.h>
#include "data_pool.h"
#include "comm.h"
#include "udp_data.h"

#define SIZE 1024

class udp_server
{
public:
	udp_server(const string &_ip = "0", const int &_port = 8888);
	int init();
	int recv_data(string &out);
	int broadcast_data();
	~udp_server();
private:
	bool is_user_exit(string &key);
	void add_user(string &ip, struct sockaddr_in &sk);
	int send_data(struct sockaddr_in &client, socklen_t &len, string &in);
	void del_user(string &key);
private:
	int sock;
	int port;
	string ip;
	map<string, struct sockaddr_in> online_user;
	data_pool pool;
};
