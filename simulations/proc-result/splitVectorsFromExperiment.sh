#!/bin/sh
#
# Split vectors from omnet 4.0 along all the replicas 
# arguments:
#  1. Experiment Name: the basename of the vectors file to be processed.
#  2. Vector Name: the vector name you want to extract
#  3. Vector Operation (optional): optional operation. See scavetool operations for more information
# 
# @author: Juan-Carlos Maureira
# INRIA 2009
# 

#if [ -z $OMNET_PATH ]
#then
#	echo "OMNET_PATH variable not present.\n"
#  echo "you need to set the variable OMNET_PATH indicating where the omnet 4.0 is installed"
#  echo "i.e: export OMNET_PATH=/opt/omnet40"
#  exit 1
#fi

#export PATH=$PATH:${OMNET_PATH}/bin

EXPERIMENT=$1
VECTOR_NAME=$2
VECTOR_OPERATION=$3

if [ -z "$EXPERIMENT" ] || [ -z "$VECTOR_NAME" ] 
then
  echo ""
  echo "usage: $0 myVector.vec \"My Vector Name\""
  echo ""
  exit 1
fi

#DATA_PATH="/nix/workspace/omnet422-inet118/so/simulations/results"
#REPLICA_FILES=`ls $DATA_PATH/$EXPERIMENT*.vec`

cd ../results
REPLICA_FILES=`ls $EXPERIMENT*.vec`
#echo $REPLICA_FILES

REPLICA_ID=`echo $REPLICA_FILES | cut -d- -f 2 | cut -d. -f 1`
echo $REPLICA_ID

#echo $VECTOR_NAME
DIR_NAME=`echo $VECTOR_NAME | cut -d: -f 1`
#echo $DIR_NAME

mkdir -p "$DIR_NAME"

echo "############"

for file in $REPLICA_FILES;
do
   export REPLICA_ID=`echo $file | cut -d- -f 2 | cut -d. -f 1`

   #scavetool vector -p "name(\"$VECTOR_NAME\")" -O output.out -F splitvec -V $file
   scavetool vector -p "name(\"$VECTOR_NAME\")" -O "$DIR_NAME/$EXPERIMENT-$REPLICA_ID-file" -F splitvec -V $file
done

##################################################################

#echo "Splings vectors along the replicas"
#mkdir -p "$VECTOR_NAME"

#for file in $REPLICA_FILES;
#do
#  export REPLICA_ID=`echo $file | cut -d- -f 2 | cut -d. -f 1`
#	echo "Extracting Vector $VECTOR_NAME from replica $REPLICA_ID. Experiment Name: $EXPERIMENT";

#  if [ -z "$VECTOR_OPERATION" ]
#  then
#		scavetool vector -p "name(\"$VECTOR_NAME\")" -O "$VECTOR_NAME/$EXPERIMENT-$REPLICA_ID-file" -F splitvec -V $file
#  else
#		scavetool vector -p "name(\"$VECTOR_NAME\")" -a $VECTOR_OPERATION -O "$VECTOR_NAME/$EXPERIMENT-$REPLICA_ID-file" -F splitvec -V $file
#  fi

#done
