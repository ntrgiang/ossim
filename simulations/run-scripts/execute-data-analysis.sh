#!/bin/bash

#echo "Hello Giang"

RESULT_FOLDER=../results
NEW_FOLDER=$RESULT_FOLDER/test_case

if [ -d $NEW_FOLDER ]; then
   echo "Folder $NEW_FOLDER exists, removing the folder ..."
   rm -rf $NEW_FOLDER
else
   echo "Folder $NEW_FOLDER doesn't exists."
fi

mkdir $NEW_FOLDER

cp $RESULT_FOLDER/*.sca $NEW_FOLDER/
cp $RESULT_FOLDER/*.vci $NEW_FOLDER/
cp $RESULT_FOLDER/*.vec $NEW_FOLDER/
