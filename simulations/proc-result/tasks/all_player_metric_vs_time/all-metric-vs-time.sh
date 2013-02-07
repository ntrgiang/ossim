#!/bin/bash

# This script roughly give an overview on the metrics

# ------------ Experiment description ----------------
   PROTOCOL="DONET"
   SCHEDULING="RANDOM"
   N_NODE=2000
   DURATION=7200
   BITRATE=100kbps

EXEC_SCRIPT=../../vector-extract.py

EXPERIMENT=$PROTOCOL"_"$SCHEDULING"_"$N_NODE"_"$DURATION"_"$BITRATE"_2"

OUTPUT_FOLDER=$EXPERIMENT
echo $OUTPUT_FOLDER
mkdir $OUTPUT_FOLDER

# ------------ Range ---------------
LOWER=0
UPPER=1200

# ------------ Metrics -------------
METRIC1=CI
METRIC2=aptSize
METRIC3=SkipChunk
METRIC4=StallDuration
METRIC5=Rebuffering

OUTPUT1=$METRIC1"_vs_time"
OUTPUT2=$METRIC2"_vs_time"
OUTPUT3=$METRIC3"_vs_time"
OUTPUT4=$METRIC4"_vs_time"
OUTPUT5=$METRIC5"_vs_time"

RESULT_FILE=../../../results/Donet_Homo_oneRouter_Network-0.vec

$EXEC_SCRIPT $METRIC1":vector" $RESULT_FILE -r $LOWER $UPPER -o $OUTPUT1
mv "$OUTPUT1.dat" $OUTPUT_FOLDER
mv "$OUTPUT1.plot" $OUTPUT_FOLDER
cd $OUTPUT_FOLDER
cat "$OUTPUT1.plot" | gnuplot
cd ..

$EXEC_SCRIPT "$METRIC2:vector" $RESULT_FILE -r $LOWER $UPPER -o $OUTPUT2
mv "$OUTPUT2.dat" $OUTPUT_FOLDER
mv "$OUTPUT2.plot" $OUTPUT_FOLDER
cd $OUTPUT_FOLDER
cat "$OUTPUT2.plot" | gnuplot
cd ..

$EXEC_SCRIPT "$METRIC3:vector" $RESULT_FILE -r $LOWER $UPPER -o $OUTPUT3
mv "$OUTPUT3.dat" $OUTPUT_FOLDER
mv "$OUTPUT3.plot" $OUTPUT_FOLDER
cd $OUTPUT_FOLDER
cat "$OUTPUT3.plot" | gnuplot
cd ..

$EXEC_SCRIPT "$METRIC4:vector" $RESULT_FILE -r $LOWER $UPPER -o $OUTPUT4
mv "$OUTPUT4.dat" $OUTPUT_FOLDER
mv "$OUTPUT4.plot" $OUTPUT_FOLDER
cd $OUTPUT_FOLDER
cat "$OUTPUT4.plot" | gnuplot
cd ..

$EXEC_SCRIPT "$METRIC5:vector" $RESULT_FILE -r $LOWER $UPPER -o $OUTPUT5
mv "$OUTPUT5.dat" $OUTPUT_FOLDER
mv "$OUTPUT5.plot" $OUTPUT_FOLDER
cd $OUTPUT_FOLDER
cat "$OUTPUT5.plot" | gnuplot
cd ..


