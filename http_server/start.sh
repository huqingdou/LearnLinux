#!/bin/sbin
ROOT_PATH=$(pwd)
export LD_LIBRARY_PATH=$ROOT_PATH/lib; ./httpd 0 8080
