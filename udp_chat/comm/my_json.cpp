#include "my_json.h"

my_json::my_json()
{}

void my_json::serialize(Json::Value &_v, string &_out)
{
#ifdef FAST
	Json::FastWriter _w;
#else
	_out = _w.write(_v);
#endif
}


void unserialize(string &_in, Json::Value &_v)
{
	Json::Reader _r;
	_r.parse(_in, _v, false);
}

my_json::~my_json()
{}

//int main()
//{
//	Json::Value _val;
//	string _out;
//	_val["nick_name"] = "haha";
//	_val["school"] = "Oxford";
//	_val["msg"] = "hello";
//	my_json::serialize(_val, _out);
//	cout << _out << endl;
//}
