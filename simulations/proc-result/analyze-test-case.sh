#!/bin/bash

#VECTOR_FILES=$2
#EXPERIMENT=$5
#MODULE="peerNode[2]"

N_NODES=3

VECTOR_FILES=../results/Donet_Homo_oneRouter_Network-0.vec

#METRIC="localCI"
METRIC="nPartner"

VECTOR2="aptSize"

RANGE_LOWER=0
RANGE_UPPER=900

OUTPUT_DIR=../results/

#./vecplot-by-module.py localCI:vector ../results/Donet_Homo_oneRouter_Network-0.vec -r 0 900 -m "peerNode[2]"

for (( i=0; i<N_NODES; i++ ))
do
   ./vecplot-by-module.py $METRIC":vector" $VECTOR_FILES -r $RANGE_LOWER $RANGE_UPPER -m "peerNode[$i]" &
done

#./vector-extract-by-module.py $VECTOR1:vector $VECTOR_FILES -r RANGE_LOWER RANGE_UPPER -s 1.0 -o $OUTPUT1
#./vector-extract.py $VECTOR2:vector $VECTOR_FILES -r RANGE_LOWER RANGE_UPPER -s 1.0 -o $OUTPUT2



