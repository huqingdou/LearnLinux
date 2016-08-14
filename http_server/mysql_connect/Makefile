ROOT_PATH=$(shell pwd)

CC=g++
INCLUDE_PATH=-I$(ROOT_PATH)/../lib/include
LIB_PATH=-L$(ROOT_PATH)/../lib/lib
LIB=-lmysqlclient

cgi_sql:cgi_sql.cpp
	$(CC) -o $@ $^ $(LIB_PATH) $(LIB) $(INCLUDE_PATH) #-D_DEBUG_

.PHONY:clean
clean:
	rm -f cgi_sql
