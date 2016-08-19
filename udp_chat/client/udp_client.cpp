#include "udp_client.h"

udp_client::udp_client(const string &_ip, const int &_port)
	:remote_ip(_ip)
	,remote_port(_port)
{
	init();
}

int udp_client::init()
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
	{
		print_log("create sock error", __FUNCTION__, __LINE__);
		exit(1);
	}
	return 0;
}

int udp_client::recv_data(string &out)
{
	char buf[SIZE];
	struct sockaddr_in remote;
	socklen_t len = sizeof(remote);

	ssize_t s = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&remote, &len);
	if(s > 0)
	{
		buf[s] = '\0';
		out = buf;
	}
	else
	{
		print_log("recv error", __FUNCTION__, __LINE__);
	}
	return s;
}

int udp_client::send_data(const string &in)
{
	struct sockaddr_in remote;
	remote.sin_fami	ly = AF_INET;
	remote.sin_port = htons(remote_port);
	remote.sin_addr.s_addr = inet_addr(remote_ip.c_str());

	ssize_t s = sendto(sock, in.c_str(), in.size(), 0, (struct sockaddr*)&remote, sizeof(remote));
	if(s < 0)
	{
		print_log("send error", __FUNCTION__, __LINE__);
	}
	return s;
}

bool udp_client::is_friend_exit(const string &_key)
{
	vector<string>::iterator iter = flist.begin();
	while(iter != flist.end())
	{
		if(*iter == _key)
		{
			return true;
		}
		iter++;
	}
	return false;
}

void udp_client::add_friend(string &f)
{
	if(!is_friend_exit(f))
	{
		flist.push_back(f);
	}
}

void udp_client::del_friend(string &f)
{
	if(is_friend_exit(f))
	{
		vector<string>::iterator iter = flist.begin();
		while(iter != flist.end())
		{
			if(*iter == f)
			{
				iter = flist.erase(iter);
				break;
			}
			iter++;
		}
	}
}

udp_client::~udp_client()
{
	if(sock > 0)
	{
		close(sock);
	}
}
