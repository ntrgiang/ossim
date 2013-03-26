#!/bin/sh

# Root path
ROOT_PATH=/home/giang/workspace/422-omnet-inet-donet

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
CONFIG_FILE=DonetNetworkConfig.ini

# Get the total number of runs
N_RUN=`$EXEC_FILE -x $NETWORK $EXEC_SIM_PATH/$CONFIG_FILE | grep "Number of runs:" | cut -d' ' -f4`
LAST_INDEX=$((N_RUN-1))

opp_runall -j1 \
	  $EXEC_FILE -u Cmdenv \
	  -r 0..$LAST_INDEX \
	  -c $NETWORK $EXEC_SIM_PATH/$CONFIG_FILE \
	  -n $EXEC_SIM_PATH:$EXEC_SRC_PATH:$INET_SRC_PATH \
	  -l $INET_SRC_PATH/inet 

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

