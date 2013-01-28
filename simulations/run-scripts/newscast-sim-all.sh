#!/bin/bash
# Current directory should be at .../projectName/simulations/run-scripts/
# This script uses relative path

#if [ $# -lt 2 ]; then
#   echo "Wrong format! Should be:"
#   echo "so-sim-all.sh <n_measurement> <n_iteration>"
#   echo "\t- n_measurement: Number of different values of the parameter"
#   echo "\t- n_iteration:   Number of iterations for each measurement"
#   exit
#fi

# Configuration of the experiment
   PROTOCOL=NEWSCAST
   N_NODE=1000
   CACHE_SIZE=20

   TEXT_FILE_NAME=newscastConnections
   TEXT_FILE_EXT=csv
   TEXT_FILE=$TEXT_FILE_NAME"."$TEXT_FILE_EXT


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
   NETWORK=Newscast_Homo_oneRouter_Network
   CONFIG_FILE=NewscastNetwork.ini

# Delete all files in the results/ folder
# rm $RESULT_PATH/*.*

EXPERIMENT=$PROTOCOL"_"$N_NODE"nodes_"$CACHE_SIZE"cacheSize"
FOLDER_EXPERIMENT=$EXPERIMENT
   # Debug   
   # echo $FOLDER_EXPERIMENT
if [ -d "$EXPERIMENT" ]; then
   echo "Folder $FOLDER_EXPERIMENT exists, please check the parameters."
   exit
   #rm -rf $FOLDER_EXPERIMENT
fi

# Get the total number of runs
   N_RUN=`$EXEC_FILE -x $NETWORK $EXEC_SIM_PATH/$CONFIG_FILE | grep "Number of runs:" | cut -d' ' -f4`
      # Debug   
      echo $N_RUN # Debug
   LAST_INDEX=$((N_RUN-1))
      # Debug
      echo $LAST_INDEX 

# Deviding the whole simulation into smaller "chunks"
#N_MEASUREMENT=$1 # This parameter is not really neccessary in this script
#N_ITERATION=$2

#MAX_INDEX=$((N_MEASUREMENT*N_ITERATION-1))

#INDEX_I=$3
#INDEX_J=$4
#RUN_INDEX=$((N_ITERATION*INDEX_I+INDEX_J))

#INDEX_LOW=$((N_ITERATION*(N_MEASUREMENT-1)))
#INDEX_HIGH=$((N_ITERATION*N_MEASUREMENT-1))



for ((i=0; i<N_RUN; i++)) 
do

opp_runall -j1 \
	  $EXEC_FILE -u Cmdenv \
     -r 0..$i \
	  -c $NETWORK $EXEC_SIM_PATH/$CONFIG_FILE \
	  -n $EXEC_SIM_PATH:$EXEC_SRC_PATH:$INET_SRC_PATH \
	  -l $INET_SRC_PATH/inet 

   echo "Moving file $TEXT_FILE ..."

   cp $TEXT_FILE $FOLDER_EXPERIMENT/$TEXT_FILE_NAME"_"$i.$TEXT_FILE_EXT
done

