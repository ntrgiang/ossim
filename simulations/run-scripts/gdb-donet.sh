#!/bin/sh

# Root path
ROOT_PATH=/home/giang/workspace/422-omnet-inet-donet

# Paths for Inet
INET_PATH=$ROOT_PATH/inet
INET_SRC_PATH=$INET_PATH/src

# More on the paths
EXEC_PATH=$ROOT_PATH/scamp
EXEC_SIM_PATH=$EXEC_PATH/simulations
EXEC_SRC_PATH=$EXEC_PATH/src

# Binary file to be executed
EXEC_FILE=$EXEC_SRC_PATH/scamp

# Specify the Network and the configuration file
NETWORK=Donet_Homo_oneRouter_Network
CONFIG_FILE=DonetNetworkConfig.ini

gdb  `$EXEC_FILE -u Cmdenv \
	  -r 0 \
	  -c $NETWORK \
	  -n $EXEC_SIM_PATH:$EXEC_SRC_PATH:$INET_SRC_PATH \
	  -l $INET_SRC_PATH/inet $EXEC_SIM_PATH/$CONFIG_FILE`

