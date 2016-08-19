#include "udp_server.h"

static void print_log(const char *err, const char *fun, const int &line)
{
	cerr << "[" << err << "]:" << fun << ":" <<line<<endl;
}

udp_server::udp_server(const string &_ip, const int &_port)
	:ip(_ip)
	,port(_port)
{
	init();
}

int udp_server::init()
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
	{
		print_log("create sock error", __FUNCTION__, __LINE__);
		exit(1);
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip.c_str());

	if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
	{
		print_log("bind sock error", __FUNCTION__, __LINE__);
		exit(2);
	}
	return 0;
}

int udp_server::recv_data(string &out)
{
	char buf[SIZE];
	struct sockaddr_in peer;
	socklen_t len = sizeof(peer);
	ssize_t s = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&peer, &len);
	if(s > 0)
	{
		buf[s] = '\0';
		out = buf;
		string ip = inet_ntoa(peer.sin_addr);
		
		udp_data data;
		data.to_value(out);
		if(data.type == "QUIT")
		{
			del_user(ip);
		}
		else
		{
			add_user(ip, peer);
		}
		pool.put_data(buf);
	}
	else
	{
		print_log("recv error", __FUNCTION__, __LINE__);
	}
	return s;
}

int udp_server::send_data(struct sockaddr_in &client, socklen_t &len, string &in)
{
	ssize_t s = sendto(sock, in.c_str(), in.size(), 0, (struct sockaddr*)&client, len);
	if(s < 0)
	{
		print_log("send error", __FUNCTION__, __LINE__);
	}
	return s;
}

bool udp_server::is_user_exit(string &key)
{
	map<string, struct sockaddr_in>::iterator iter = online_user.find(key);
	return iter == online_user.end() ? false:true;
}

void udp_server::add_user(string &ip, struct sockaddr_in &sk)
{
	if(!is_user_exit(ip))
	{
		online_user.insert(pair<string, struct sockaddr_in>(ip, sk));
	}
}

void udp_server::del_user(string &key)
{
	if(is_user_exit(key))
	{
		online_user.erase(key);
	}
}

int udp_server::broadcast_data()
{
	string msg;
	pool.get_data(msg);
	map<string, struct sockaddr_in>::iterator iter = online_user.begin();
	socklen_t len = 0;
	for(; iter != online_user.end(); iter++)
	{
		len = sizeof(iter->second);
		send_data(iter->second, len, msg);
	}
}

udp_server::~udp_server()
{
	if(sock > 0)
	{
		close(sock);
	}
}
