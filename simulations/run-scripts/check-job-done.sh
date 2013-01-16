#!/bin/sh

if [ $# -lt 2 ]; then
   echo "Wrong format! Should be:"
   echo "check-job-done.sh <n_measurement> <n_iteration>"
   echo "\t- n_measurement: Number of different values of the parameter"
   echo "\t- n_iteration: Number of iterations for each measurement"
   exit
fi

JOB_DONE_FILE=./jobs/jobs.done

N_MEASUREMENT=$1
N_ITERATION=$2
N_JOB=$((N_MEASUREMENT*N_ITERATION))
echo $N_JOB

while :
do
	echo "Press [CTRL+C] to stop.."

   N_JOB_DONE=`less $JOB_DONE_FILE | grep -c 'DONE'`
   #echo $N_JOB_DONE

	if [ "$N_JOB_DONE" -eq "$N_JOB" ]; then
      echo "All jobs have been done!"

      #sleep 300
      #break
   fi
   
   echo "Only $N_JOB_DONE have been completed so far, keep waiting ..."
   sleep 600
done

#./execute-data-analysis.sh


