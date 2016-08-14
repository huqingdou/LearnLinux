FIND_PATH=-I/home/hm/Desktop/gitcodes/http_server/mysql_connect -I/home/hm/Desktop/gitcodes/http_server/lib/include -L/home/hm/Desktop/gitcodes/http_server/lib/lib -lmysqlclient

.PHONY:all
all:math_cgi student_show_cgi register_cgi

math_cgi:math_cgi.c
	gcc -o $@ $^
student_show_cgi:student_show_cgi.cpp /home/hm/Desktop/gitcodes/http_server/mysql_connect/cgi_sql.cpp
	g++ -o $@ $^ ${FIND_PATH}
register_cgi:register_cgi.cpp /home/hm/Desktop/gitcodes/http_server/mysql_connect/cgi_sql.cpp
	g++ -o $@ $^ ${FIND_PATH}

.PHONY:clean
clean:
	rm -f math_cgi student_show_cgi register_cgi
