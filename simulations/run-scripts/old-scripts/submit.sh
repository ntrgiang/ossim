#!/bin/sh

# The following 2 parameters need to be entered manually
N_MEASUREMENT=3
N_ITERATION=30

for ((i=1; i<=N_MEASUREMENT; i++))
do
   NEW_FILE="so-sim-"$i".job"
   #echo $NEW_FILE
   rm $NEW_FILE*
   cp ./so-sim.job ./$NEW_FILE
   echo "./so-sim.sh $i $N_ITERATION" >> ./$NEW_FILE
   qsub $NEW_FILE
done

