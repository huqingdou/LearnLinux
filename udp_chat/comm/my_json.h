#pragma once

#include<iostream>
using namespace std;
#include "json/json.h"
#include<string.h>

class my_json
{
public:
	my_json();
	~my_json();
	void serialize(Json::Value &_v, string &_out);
	void unserialize(string &_in, Json::Value &_v);
};
