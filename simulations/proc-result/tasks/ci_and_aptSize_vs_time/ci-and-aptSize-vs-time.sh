#!/bin/sh

# ------------ Experiment description ----------------
   PROTOCOL="DONET"
   SCHEDULING="RANDOM"
   N_NODE=2000
   DURATION=7200

# ------------ Metrics -------------
   METRIC1="CI"
   METRIC2="aptSize"

# ------------ Plot settings ----------------
   CONF_INT=0.9 # Confidence Interval

EXPERIMENT=$PROTOCOL"_"$SCHEDULING"_"$N_NODE"nodes_"$DURATION"s"
#echo $EXPERIMENT
if [ -d "$EXPERIMENT" ]; then
   #echo "Folder $EXPERIMENT exists, please check the parameters."
   #exit
   rm -rf $EXPERIMENT
fi

mkdir $EXPERIMENT

#/nix/workspace/omnet422-inet118/so/simulations/proc-result/tasks/ci_and_aptSize_vs_time
#PROJECT_DIR=/nix/workspace/omnet422-inet118/so
PROJECT_DIR=$PWD/../../../..
SCRIPT_DIR=$PROJECT_DIR/simulations/proc-result
ORI_RESULT_DIR=$PROJECT_DIR/simulations/results

OUTPUT_DIR=$PWD/$EXPERIMENT
PROCESS_SCRIPT=$SCRIPT_DIR/vector-extract.py

OUTFILE1=$OUTPUT_DIR/$METRIC1"_"$EXPERIMENT
OUTFILE2=$OUTPUT_DIR/$METRIC2"_"$EXPERIMENT

# --- Create a sub-folder for the experiment
RESULT_DIR=$ORI_RESULT_DIR/$EXPERIMENT

if [ -d "$RESULT_DIR" ]; then
   echo "Folder $RESULT_DIR exists, please check the parameters."
   #exit
   #rm -rf $RESULT_DIR
else
   mkdir $RESULT_DIR

   # --- Copy the data into that experiment folder
   cp $ORI_RESULT_DIR/*.sca $RESULT_DIR/
   cp $ORI_RESULT_DIR/*.vci $RESULT_DIR/
   cp $ORI_RESULT_DIR/*.vec $RESULT_DIR/
fi


# ------------ Data ---------------
VECTOR_FILE=$RESULT_DIR/*.vec
echo $VECTOR_FILE

# ------------ Extract data from vector files ----------------------------------
$PROCESS_SCRIPT $METRIC1:vector \
      $VECTOR_FILE \
      -r 0 $DURATION \
      -s 1.0 \
      -c $CONF_INT \
      -o $OUTFILE1

$PROCESS_SCRIPT $METRIC2:vector \
      $VECTOR_FILE \
      -r 0 $DURATION \
      -s 1.0 \
      -c $CONF_INT \
      -o $OUTFILE2

echo "Data files were analyzed!"

# ------------------------------------------------------------------------------
# GENERATE PLOT SCRIPT
# ------------------------------------------------------------------------------
# Plot the CI and the Number of joined nodes vs. time

GNUPLOT_FILE=$OUTPUT_DIR/plot.gpl

if [ -f "$GNUPLOT_FILE" ]; then
   echo "File $GNUPLOT_FILE exists, will be removed!"
   rm $GNUPLOT_FILE
fi

echo "\n" >> $GNUPLOT_FILE
echo "# --------- Output" >> $GNUPLOT_FILE
echo "\tset terminal postscript eps color" >> $GNUPLOT_FILE
echo "\tset output \"$OUTPUT_DIR/ci_aptSize_vs_time.eps\"" >> $GNUPLOT_FILE

echo "\n" >> $GNUPLOT_FILE
echo "#unset" >> $GNUPLOT_FILE
echo "unset log y2" >> $GNUPLOT_FILE

echo "\n" >> $GNUPLOT_FILE
echo "#set style data lines" >> $GNUPLOT_FILE
echo "set style data points" >> $GNUPLOT_FILE
echo "set style line 2 pt 1 lc 4" >> $GNUPLOT_FILE
echo "set style line 3 pt 2 lc 5" >> $GNUPLOT_FILE
echo "#set style line 1 pt 3 lc 4" >> $GNUPLOT_FILE
echo "#set style line 2 pt 4 lc 2" >> $GNUPLOT_FILE

echo "\n" >> $GNUPLOT_FILE
echo "set datafile separator \",\"" >> $GNUPLOT_FILE

echo "\n" >> $GNUPLOT_FILE
echo "# ---------- Log scale -----------" >> $GNUPLOT_FILE
echo "#set grid x y2" >> $GNUPLOT_FILE
echo "\tset key center top title \" \"" >> $GNUPLOT_FILE
echo "\t#set logscale xy" >> $GNUPLOT_FILE
echo "\t#set log x" >> $GNUPLOT_FILE
echo "\t#set log x2" >> $GNUPLOT_FILE

echo "\n" >> $GNUPLOT_FILE
echo "# ---------- Label -----------" >> $GNUPLOT_FILE
echo "\tset title \"Continuity Index\"" >> $GNUPLOT_FILE
echo "\tset xlabel \"Time (second)\"" >> $GNUPLOT_FILE
echo "\tset ylabel \"Continuitity Index\"" >> $GNUPLOT_FILE
echo "\tset y2label \"Number of active peers\"" >> $GNUPLOT_FILE

Y2MAX=`bc << EOF
$N_NODE * 1.1
EOF
`
#Y2MAX=$((N_NODE * RATIO)) # not works
#Y2MAX=`expr $N_NODE * $RATIO` # not works
#echo $Y2MAX
echo "\n" >> $GNUPLOT_FILE
echo "# ---------- Range -----------" >> $GNUPLOT_FILE
echo "\tset xrange [1.0 : $DURATION.0]" >> $GNUPLOT_FILE
echo "\tset x2range [1.0 : $DURATION.0]" >> $GNUPLOT_FILE
echo "\tset yrange [0.9 : 1.0]" >> $GNUPLOT_FILE
echo "\tset y2range [0.0 : $Y2MAX]" >> $GNUPLOT_FILE

echo "\n" >> $GNUPLOT_FILE
echo "# --------- Tic -------------" >> $GNUPLOT_FILE
echo "set ytics nomirror" >> $GNUPLOT_FILE
echo "set y2tics" >> $GNUPLOT_FILE
echo "set tics out" >> $GNUPLOT_FILE

echo "\n" >> $GNUPLOT_FILE
echo "# --------- Scale ------------" >> $GNUPLOT_FILE
echo "\t#set autoscale  y" >> $GNUPLOT_FILE
echo "\t#set autoscale y2" >> $GNUPLOT_FILE

echo "plot \"$OUTFILE1.dat\" using 1:2 title \"Continuity Index\" axes x1y1, \\" >> $GNUPLOT_FILE
echo "\"$OUTFILE2.dat\" using 1:2 title \"Number of Active Peers\" axes x2y2" >> $GNUPLOT_FILE

cat $GNUPLOT_FILE | gnuplot


