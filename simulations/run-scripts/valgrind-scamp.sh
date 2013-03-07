#!/bin/sh

# Project name
   PROJECT_NAME=so

# Paths for Inet
   INET_PATH=../../../inet
   INET_SRC_PATH=$INET_PATH/src

# More on the paths
   PROJECT_PATH=../../../$PROJECT_NAME
   EXEC_SIM_PATH=$PROJECT_PATH/simulations
   EXEC_SRC_PATH=$PROJECT_PATH/src
   #RESULT_PATH=$PROJECT_PATH/simulations/results

# Binary file to be executed
   EXEC_FILE=$EXEC_SRC_PATH/$PROJECT_NAME

# Specify the Network and the configuration file
   NETWORK=Scamp_Homo_oneRouter_Network
   CONFIG_FILE=ScampNetwork.ini

#valgrind
valgrind -v --log-file=valgrind.log --leak-check=full --show-reachable=yes --tool=memcheck --track-origins=yes \
	  $EXEC_FILE -u Cmdenv \
     -r 0 \
	  -c $NETWORK $EXEC_SIM_PATH/$CONFIG_FILE \
	  -n $EXEC_SIM_PATH:$EXEC_SRC_PATH:$INET_SRC_PATH \
	  -l $INET_SRC_PATH/inet 

