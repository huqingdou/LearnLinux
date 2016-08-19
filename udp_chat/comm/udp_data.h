#pragma once

#include<iostream>
using namespace std;
#include "my_json.h"

class udp_data
{
public:
	udp_data();
	void to_string(string &out);
	void to_value(string &in);
	~udp_data();
public:
	string nick_name;
	string school;
	string msg;
	string type;
};
