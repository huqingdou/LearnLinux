#pragma once

#include<iostream>
using namespace std;
#include <mysql.h>
#include<string>


class cgi_sql
{
public:
	cgi_sql(const string &host, const string &user, const string &passwd, const string &db);
	bool begin_connect();
	bool close_connect();
	bool insert_sql(const string &data);
	void show_info();
	bool select_sql(string field_name[], string _out_data[][5], int &_out_row);
	~cgi_sql();
private:
	MYSQL *mysql_base;
	MYSQL_RES *res;
	string host;
	string user;
	string passwd;
	string db;
};
