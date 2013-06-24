#!/bin/bash

# Assuming that the file is executed within the run-script/ folder

# $1 = Configuration name
# $2 = INI file which stores the configuration

#$1=Donet_oneRouter_overlayAttack 
#$2=../Donet-overlayAttack.ini
rm log.log
gdb --args ../../src/so -r 0 -u Cmdenv -c $1 $2 | tee log.log
