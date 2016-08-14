ROOT_PATH=$(shell pwd)
CGI_PATH=$(PWD)/htdoc/cgi
BIN=httpd
SRC=httpd.c
OBJ=$(SRC:.c=.o)
CC=gcc
FLAGS=#-D_DEBUG_
LDFLAGS=-lpthread

.PHONY:all
all:$(BIN) cgi

$(BIN):$(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) 
%.o:%.c
	$(CC) -c $< $(FLAGS)

.PHONY:cgi
cgi:
	for name in `echo $(CGI_PATH)`;\
	do\
		cd $$name;\
			make;\
			cd -;\
	done

.PHONY:output
output:
	mkdir -p output/lib
	mkdir -p output/htdoc/cgi
	cp -f httpd output
	cp htdoc/cgi/math_cgi output/htdoc/cgi
	cp htdoc/cgi/register_cgi output/htdoc/cgi
	cp htdoc/cgi/student_show_cgi output/htdoc/cgi
	cp htdoc/index.html output/htdoc
	cp htdoc/register.html output/htdoc
	cp start.sh output
	cp lib/lib/* output/lib

.PHONY:clean
clean:
	rm -rf *.o $(BIN) output
	for name in `echo $(CGI_PATH)`;\
	do\
		cd $$name;\
		make clean;\
		cd -;\
	done

.PHONY:debug
debug:
	@echo $(ROOT_PATH)
	@echo $(BIN)
	@echo $(SRC)
	@echo $(OBJ)

