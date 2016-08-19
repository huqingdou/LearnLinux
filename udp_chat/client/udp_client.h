#pragma once

#include<iostream>
using namespace std;

#include<stdio.h>
#include<vector>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string>
#include "comm.h"
#include "udp_data.h"


class udp_client
{
public:
	udp_client(const string &_ip = "0", const int &_port = 8888);
	int init();
	int recv_data(string &out);
	int send_data(const string &in);
	void add_friend(string &f);
	void del_friend(string &f);
	~udp_client();
private:
	int sock;
	int remote_port;
	string remote_ip;
pulic:
	vector<string> flist;
};
