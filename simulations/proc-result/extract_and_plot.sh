#!/bin/bash

VECTOR_FILES=$2
#OUTPUT1=$3
#OUTPUT2=$4
EXPERIMENT=$5

OUTPUT1="CI"
OUTPUT2="aptSize"

VECTOR1="CI"
VECTOR2="aptSize"

RANGE_LOWER=0
RANGE_UPPER=3600

OUTPUT_DIR=../results/intermediate/ci_aptSize_vs_time_$EXPERIMENT
mkdir $OUTPUT_DIR

#./vector-extract.py CI:vector ../results/Donet_Homo_oneRouter_Network-0.vec -r 50 10800 -s 1.0 -o ci_10800s_300nodes_noErrorBar

./vector-extract.py $VECTOR1:vector $VECTOR_FILES -r RANGE_LOWER RANGE_UPPER -s 1.0 -o $OUTPUT1
./vector-extract.py $VECTOR2:vector $VECTOR_FILES -r RANGE_LOWER RANGE_UPPER -s 1.0 -o $OUTPUT2

PLOT_SCRIPT=cat plot_ci_aptSize_vs_time.plot.file

PLOT_SCRIPT=$PLOT_SCRIPT"plot \"$OUTPUT_DIR/$OUTPUT1.dat" using 1:2 title "Continuity Index" axes x1y1,\
PLOT_SCRIPT=$PLOT_SCRIPT"plot \"$OUTPUT_DIR/$OUTPUT2.dat" using 1:2 title "Number of Active Peers" axes x2y2

#"aptSize.dat" using 1:2 title "Number of Active Peers" axes x2y2


