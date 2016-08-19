#pragma once

#include<iostream>
using namespace std;
#include "json.h"

class my_json
{
	void serialize(Json::Value &_v, string &_out);
	void unserialize(string &_in, Json::Value &_v);
};
