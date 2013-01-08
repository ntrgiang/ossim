#!/bin/bash

VECTOR_DIRNAME="$1"

if [ -d "$VECTOR_DIRNAME" ]
then
	find "$VECTOR_DIRNAME"/*.vec | while read file
  do
		export VECTOR_FILE_ID=`echo "$file" | cut -d- -f 3 | cut -d. -f 1`
      echo $VECTOR_FILE_ID

		export REPLICA_ID=`echo "$file" | cut -d- -f 2`
      echo $REPLICA_ID

		export EXPERIMENT=`echo "$file" | cut -d- -f 1 | cut -d\/ -f 2`
      echo $EXPERIMENT

		export VECTOR_NAME=`head -n 1 "$file" | cut -d\  -f 5 | cut -d: -f 1`
      echo $VECTOR_NAME

		export MODULE=`head -n 1 "$file" | cut -d' ' -f 4`
      echo $MODULE

		echo "procesing $file, Module: $MODULE VectorName: $VECTOR_NAME"

		OUTPUT_FILE="$VECTOR_NAME-$MODULE.data"
      echo $OUTPUT_FILE
	
		#if [ ! -f "$OUTPUT_FILE" ]
		#then
		#	echo "Experiment Replica Time Obs" > "$OUTPUT_FILE"
		#fi
		#tail -n +5 "$file" | gawk -F, '{print ENVIRON["EXPERIMENT"],ENVIRON["REPLICA_ID"],$1,$2}' >> "$OUTPUT_FILE"

      # tail -n +5 "$file"
      # Skip the first 5 lines

      #tail -n +5 "$file" | gawk -F, '{print ENVIRON["EXPERIMENT"],ENVIRON["REPLICA_ID"],$1,$2}'

      tail -n +5 "$file" >> "$OUTPUT_FILE"

  done
else
  echo "usage: $0 {vector_name}"
	echo "Vector name must be a directory containing one single vector file for each replica"
fi

