#!/bin/bash

# Assuming that the file is executed within the run-script/ folder

# $1 = Configuration name
# $2 = INI file which stores the configuration

#config=Donet_oneRouter_underlayAttack 
#ini_file=../Donet-underlayAttack.ini

config=$1
ini_file=$2

rm /tmp/ossim-valgrind.log

valgrind -v --log-file=/tmp/ossim-valgrind.log --leak-check=full --show-reachable=yes --tool=memcheck --track-origins=yes \
	../../src/so -r 0 -u Cmdenv -c $config $ini_file -n ../../src:../../simulations:../../../inet/src
