#!/bin/sh
# Current directory should be at .../projectName/simulations/run-scripts/
# This script uses relative path

if [ $# -lt 2 ]; then
   echo "Wrong format! Should be:"
   echo "so-sim-all.sh <n_measurement> <n_iteration>"
   echo "\t- n_measurement: Number of different values of the parameter"
   echo "\t- n_iteration:   Number of iterations for each measurement"
   exit
fi

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
NETWORK=Donet_Homo_oneRouter_Network
CONFIG_FILE=DonetNetworkConfig.ini

# Delete all files in the results/ folder
# rm $RESULT_PATH/*.*

# Get the total number of runs
#N_RUN=`$EXEC_FILE -x $NETWORK $EXEC_SIM_PATH/$CONFIG_FILE | grep "Number of runs:" | cut -d' ' -f4`
#LAST_INDEX=$((N_RUN-1))

# Deviding the whole simulation into smaller "chunks"
N_MEASUREMENT=$1 # This parameter is not really neccessary in this script
N_ITERATION=$2

MAX_INDEX=$((N_MEASUREMENT*N_ITERATION-1))

#INDEX_I=$3
#INDEX_J=$4
#RUN_INDEX=$((N_ITERATION*INDEX_I+INDEX_J))

#INDEX_LOW=$((N_ITERATION*(N_MEASUREMENT-1)))
#INDEX_HIGH=$((N_ITERATION*N_MEASUREMENT-1))

opp_runall -j1 \
	  $EXEC_FILE -u Cmdenv \
     -r 0..$MAX_INDEX \
	  -c $NETWORK $EXEC_SIM_PATH/$CONFIG_FILE \
	  -n $EXEC_SIM_PATH:$EXEC_SRC_PATH:$INET_SRC_PATH \
	  -l $INET_SRC_PATH/inet 


