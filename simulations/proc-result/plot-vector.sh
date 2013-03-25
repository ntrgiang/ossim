#!/bin/bash

#
# To run a script for producing a graphic file from vector data file
# .vec --> (.plot + .dat) --> .eps --> .pdf

METRIC=globalCI:vector
RESULT_FILE=../results/Donet_Homo_oneRouter_Network-0.vec

#OUTPUT_DIR=./test
OUTPUT_FILE=output
PLOT_FILE=$OUTPUT_FILE".plot"
EPS_FILE=$OUTPUT_FILE".eps"
PDF_FILE=$OUTPUT_FILE".pdf"

RANGE_LOWER=0
RANGE_UPPER=1500

./vector-extract.py $METRIC $RESULT_FILE -r $RANGE_LOWER $RANGE_UPPER -o $OUTPUT_FILE -c 0.9

gnuplot $PLOT_FILE

#evince $EPS_FILE

#epstopdf $EPS_FILE
#evince $PDF_FILE
