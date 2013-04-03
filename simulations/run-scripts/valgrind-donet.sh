#!/bin/sh

# Root path
ROOT_PATH=/nix/workspace/omnet422-inet118

# Paths for Inet
INET_PATH=$ROOT_PATH/inet
INET_SRC_PATH=$INET_PATH/src

# More on the paths
EXEC_PATH=$ROOT_PATH/so
EXEC_SIM_PATH=$EXEC_PATH/simulations
EXEC_SRC_PATH=$EXEC_PATH/src

# Binary file to be executed
EXEC_FILE=$EXEC_SRC_PATH/so

# Specify the Network and the configuration file
NETWORK=Donet_Homo_oneRouter_Network
CONFIG_FILE=Donet.ini

#valgrind
#valgrind -v --log-file=valgrind.log --leak-check=full --show-reachable=yes --tool=memcheck --track-origins=yes \
valgrind -v --log-file=valgrind.log --tool=callgrind \
          $EXEC_FILE -u Cmdenv \
	  -r 0 \
	  -c $NETWORK \
	  -n $EXEC_SIM_PATH:$EXEC_SRC_PATH:$INET_SRC_PATH \
	  -l $INET_SRC_PATH/inet $EXEC_SIM_PATH/$CONFIG_FILE

