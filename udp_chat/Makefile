ROOT_PATH=$(shell pwd)
BIN=$(ROOT_PATH)/server $(ROOT_PATH)/client


.PHONY:all
all:
	for name in `echo $(BIN)`;\
		do\
			cd $$name;\
			make;\
			cd -;\
		done


.PHONY:output
output:
	mkdir output
	cp $(ROOT_PATH)/server/udp_server output
	cp $(ROOT_PATH)/client/udp_server output
	cp -rf $(ROOT_PATH)/conf output
	cp -rf $(ROOT_PATH)/plugin/serverctl.sh output

.PHONY:clean
clean:
	rm -f output
	for name in `echo $(BIN)`;\
		do\
			cd $$name;\
			make clean;\
			cd -;\
		done


