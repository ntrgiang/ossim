#!/bin/bash

#
# copy all the configuration files and result files to a separate folder for backup
#

# syntax:
# add-license.sh $1 $2 $3 $4

FILE_DIR=$1
LICENSE=license.txt
TEMP_FILE=$FILE_DIR"temp"
FILE_SET=$FILE_DIR"*.*"

for f in $FILE_SET ; do
   #echo $f
   fname=`basename $f`
   old_fname=$f
   #echo $old_fname

   cat $LICENSE >> $TEMP_FILE

   echo "// -----------------------------------------------------------------------------" >> $TEMP_FILE
   echo "// "$fname >> $TEMP_FILE
   echo "// -----------------------------------------------------------------------------" >> $TEMP_FILE
   echo "// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors" >> $TEMP_FILE
   echo "//" >> $TEMP_FILE
   echo "// Contributors: Giang;" >> $TEMP_FILE
   echo "// Code Reviewers: -;" >> $TEMP_FILE
   echo "// ------------------------------------------------------------------------------" >> $TEMP_FILE
   echo "//" >> $TEMP_FILE
   echo "" >> $TEMP_FILE

   cat $f >> $TEMP_FILE

   rm $old_fname
   mv $TEMP_FILE $old_fname
done



