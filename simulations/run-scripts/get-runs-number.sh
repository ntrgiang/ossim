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
NETWORK=Scamp_Homo_oneRouter_Network
CONFIG_FILE=ScampNetwork.ini

# Number of configuration
# RUN_END_IDX=0

# execute the binary file
#opp_runall -j1 $SCAMP_PATH/src/scamp -r 0 -c $NETWORK -n $SCAMP_PATH/simulations:$SCAMP_PATH/src:$INET_PATH/src -l $INET_PATH/src/inet $SCAMP_PATH/simulations/$CONFIG_FILE -u Cmdenv

RETURN_STR=`$EXEC_FILE -x $NETWORK $EXEC_SIM_PATH/$CONFIG_FILE | grep "Number of runs:" | cut -d' ' -f4`

echo $RETURN_STR