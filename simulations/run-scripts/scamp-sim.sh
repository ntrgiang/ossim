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
   CONFIG_FILE=Scamp.ini

# Delete all files in the results/ folder
# rm $RESULT_PATH/*.*

# Get the total number of runs
#N_RUN=`$EXEC_FILE -x $NETWORK $EXEC_SIM_PATH/$CONFIG_FILE | grep "Number of runs:" | cut -d' ' -f4`
#LAST_INDEX=$((N_RUN-1))

# Deviding the whole simulation into smaller "chunks"
#N_MEASUREMENT=$1 # This parameter is not really neccessary in this script
#N_ITERATION=$2

#INDEX_I=$3
#INDEX_J=$4

#RUN_INDEX=$((N_ITERATION*INDEX_I+INDEX_J))

#INDEX_LOW=$((N_ITERATION*(N_MEASUREMENT-1)))
#INDEX_HIGH=$((N_ITERATION*N_MEASUREMENT-1))

opp_runall -j1 \
	  $EXEC_FILE -u Cmdenv \
     -r 0 \
	  -c $NETWORK $EXEC_SIM_PATH/$CONFIG_FILE \
	  -n $EXEC_SIM_PATH:$EXEC_SRC_PATH:$INET_SRC_PATH \
	  -l $INET_SRC_PATH/inet 
     #-r $RUN_INDEX..$RUN_INDEX \

#opp_runall -j1 \
#   /nix/workspace/omnet422-inet118/so/src/so -u Cmdenv \
#   -r 0 \
#   -c Scamp_Homo_oneRouter_Network /nix/workspace/omnet422-inet118/so/simulations/ScampNetwork.ini \
#   -n /nix/workspace/omnet422-inet118/so/src:/nix/workspace/omnet422-inet118/so/simulations:/nix/workspace/omnet422-inet118/inet/src

#RESULT_PATH=$EXEC_SIM_PATH/results
#NEW_FOLDER="SCAMP_VALIDATION_PARTIALVIEW"
#mkdir $RESULT_PATH/$NEW_FOLDER
#for (( i=0; i<=LAST_INDEX; i++ ))
#for i in {0..LAST_INDEX}
#for i in `seq 0 $LAST_INDEX`
#do
#  cp $RESULT_PATH/$NETWORK-$i.* $RESULT_PATH/$NEW_FOLDER
#done

# ----------------
# Execute R script
# ----------------
#R_SCRIPT_FOLDER=$SCAMP_PATH/r-script

#Rscript $R_SCRIPT_FOLDER/draw-histogram.R

