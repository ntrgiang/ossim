#!/bin/bash

# Clean-up
# Remove all log files created by the cluster for each job
rm so-sim-*.job.*
rm *.jobcore.*

# Check and delete the content of the jobs/ folder if neccessary
JOB_DIR=./jobs
#echo $JOB_DIR

# File which stores the completeness of the jobs
DONE_FILE=$JOB_DIR/jobs.done
CHECK_JOB_DONE_FILE=$JOB_DIR/check-job-done.job

if [ "$(ls -A $JOB_DIR)" ]; then
   echo "Take action $JOB_DIR is not Empty"
   ls $JOB_DIR/*.job
   rm $JOB_DIR/*.job
   rm $DONE_FILE
   #rm $CHECK_JOB_DONE_FILE
else
   echo "$JOB_DIR is Empty"
fi

# The following 2 parameters need to be entered manually
N_MEASUREMENT=2
N_ITERATION=3
# ------------------------------------------------------------------------------
# Create and submit simulation jobs
# ------------------------------------------------------------------------------
for((i=0; i<N_MEASUREMENT; i++))
do
   for((j=0; j<N_ITERATION; j++))
   do
      NEW_FILE=$JOB_DIR/"so-sim-"$i-$j".job"
      #echo $NEW_FILEq
      cp ./so-sim.jobcore ./$NEW_FILE
      echo "./so-sim.sh $N_MEASUREMENT $N_ITERATION $i $j" >> $NEW_FILE

      # Insert the shell script into the job file
      echo "echo \"job $i-$j ... DONE\" >> $DONE_FILE" >> $NEW_FILE

      #echo "job $i-$j ... DONE" >> $DONE_FILE # for testing only
      qsub $NEW_FILE
   done
done

#-------------------------------------------------------------------------------
# ------------------------------------------------------------------------------
# Create and submit simulation jobs
# ------------------------------------------------------------------------------
#for((i=1; i<59; i++))
#do
#      NEW_FILE=$JOB_DIR/"so-sim-"$i-$j".job"
#      #echo $NEW_FILEq
#      cp ./so-sim.jobcore ./$NEW_FILE
#      echo "./so-sim.sh $N_MEASUREMENT $N_ITERATION $i $j" >> $NEW_FILE
#
#      # Insert the shell script into the job file
#      echo "echo \"job $i-$j ... DONE\" >> $DONE_FILE" >> $NEW_FILE
#
#      #echo "job $i-$j ... DONE" >> $DONE_FILE # for testing only
#      qsub $NEW_FILE
#done


# ------------------------------------------------------------------------------
# Create and submit a single job to check the completeness of the sim. jobs
# ------------------------------------------------------------------------------
#CHECK_JOB_DONE_FILE=$JOB_DIR/check-job-done.jobcore
cp ./check-job-done.jobcore $CHECK_JOB_DONE_FILE

# Insert the shell script into the job file
echo "./check-job-done.sh $N_MEASUREMENT $N_ITERATION" >> $CHECK_JOB_DONE_FILE
qsub $CHECK_JOB_DONE_FILE

