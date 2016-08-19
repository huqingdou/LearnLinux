#include "udp_data.h"

udp_data::udp_data()
{}

void udp_data::to_string(string &out)
{
	Json::Value root;
	root["nick_name"] = nick_name;
	root["school"] = school;
	root["msg"] = msg;
	root["type"] = type;

	my_json::serialize(root, out);
}

void udp_data::to_value(string &in)
{
	Json::Value root;
	my_json::unserialize(in, root);
	nick_name = root["nickname"].asString();
	school = root["school"].asString();
	msg = root["msg"].asString();
	type = root["type"].asString();

}

udp_data::~udp_data()
{}


//int main()
//{
//	string out;
//	udp_data data;
//	data.nick_name = "baobao";
//
//
//	udp_data::to_string(out);
//	cout << out << endl;
//}
