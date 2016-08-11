#include "cgi_sql.h"

cgi_sql::cgi_sql(const string &_host, 
		const string &_user, 
		const string &_passwd, 
		const string &_db)
			: host(_host)
			, user(_user)
			, passwd(_passwd)
			, db(_db)
			, res(NULL)
{
	mysql_base = mysql_init(NULL);
}

bool cgi_sql::begin_connect()
{
	//连接MYSQL服务器
	if(mysql_real_connect(mysql_base, host.c_str()
				, user.c_str(), passwd.c_str()
				, db.c_str(), 3306, NULL, 0) == NULL)
	{
		cerr<<"connect error"<<endl;
		return false;
	}
	else
	{
		cout << "connect done" << endl;
	}
	return true;
}

bool cgi_sql::select_sql(string field_name[], string _out_data[][5], int &_out_row)
{
	string sql = "SELECT * FROM STUDENT";
	if(mysql_query(mysql_base, sql.c_str()) == 0)
	{
		cout << "query sucess" << endl;
	}
	else
	{
		cerr << "query failed" << endl;
		return false;
	}
	res = mysql_store_result(mysql_base);
	//检查my_sql_store_result()的返回值
	int row_num = mysql_num_rows(res);  //行数
	int field_num = mysql_num_fields(res);  //字段数
	_out_row = row_num;

	MYSQL_FIELD *fd = NULL;

	for(int i = 0; fd = mysql_fetch_field(res); )
	{
		field_name[i++] = fd->name;
	}

	for(int index = 0; index < row_num; index++)
	{
		MYSQL_ROW _row = mysql_fetch_row(res);
		if(_row)
		{
			int start = 0;
			for(; start < field_num; ++start)
			{
				_out_data[index][start] = _row[start];
				cout << _row[start] << "\t";
			}
			cout<<endl;
		}
	}
	return true;
}

bool cgi_sql::insert_sql(const string &data)
{
	string sql = "INSERT INTO STUDENT(name, age, school, hobby) values";
	sql += "(";
	sql += data;
	sql += ");";
	if(mysql_query(mysql_base, sql.c_str()) == 0)
	{
		cout << "query success" << endl;
		return true;
	}
	else
	{
		cerr << "query failed" << endl;
		return false;
	}
}

bool cgi_sql::close_connect()
{
	//关闭连接
	mysql_close(mysql_base);
	cout << "connect close" << endl;
}

void cgi_sql::show_info()
{
	cout << mysql_get_client_info() << endl;
}

cgi_sql::~cgi_sql()
{
	close_connect();
	if(res)
	{
		free(res);
	}
}

#ifdef _DEBUG_

int main()
{
	string _sql_data[1024][5];
	string header[5];
	int curr_row = -1;
	const string _host = "0";
	const string _user = "root";
	const string _passwd = "";
	const string _db = "student";
	const string _data = "\"Amandine\", 18, \"Yale\", \"travel\"";
	cgi_sql con(_host, _user, _passwd, _db);
	con.begin_connect();
	con.select_sql(header, _sql_data, curr_row);
	con.insert_sql(_data);
	con.show_info();
	return 0;
}

#endif
