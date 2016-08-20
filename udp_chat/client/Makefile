ROOT_PATH=$(shell pwd)
COMM_PATH=$(ROOT_PATH)/../comm
WINDOW_PATH=$(ROOT_PATH)/../window
JSON_PATH=$(ROOT_PATH)/../json_lib

INCLUDE=-I$(COMM_PATH) -I$(JSON_PATH)/include/json -I$(WINDOW_PATH)/
LIB_PATH=-L$(JSON_PATH)/libs
LIB=-ljsoncpp -lncurses -lpthread -static

BIN=udp_client
CC=g++

SRC=$(shell ls *.cpp)
SRC+=$(shell cd $(COMM_PATH); ls *.cpp)
SRC+=$(shell cd $(WINDOW_PATH); ls *.cpp)
OBJ=$(SRC:.cpp=.o)

$(BIN):$(OBJ)
	$(CC) -o $@ $^ $(LIB_PATH) $(LIB)
%.o:%.cpp
	$(CC) -c $< $(INCLUDE)
%.o:$(COMM_PATH)/%.cpp
	$(CC) -c $<
%.o:$(WINDOW_PATH)/%.cpp
	$(CC) -c $< $(INCLUDE)


.PHONY:clean
clean:
	rm -f $(BIN) *.o
