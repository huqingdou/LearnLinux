ROOT_PATH=$(shell pwd)
CGI_PATH=$(PWD)/htdoc/cgi
BIN=httpd
SRC=httpd.c
OBJ=$(SRC:.c=.o)
CC=gcc
FLAGS=-g #-D_DEBUG_
LDFLAGS=-g -lpthread

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
	mkdir -p output/htdoc/cgi
	cp httpd output
	cp -rf conf output
	cp start.sh output
	cp -rf htdoc/* output/htdoc
	for name in `echo $(CGI_PATH);`\
	do\
		cd $$name;\
		make output;\
		cd -;\
	done


.PHONY:clean
clean:
	rm -rf $(BIN) output
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

