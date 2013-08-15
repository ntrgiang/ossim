#!/bin/sh

config=$1
ini_file=$2

#valgrind
#valgrind -v --log-file=valgrind.log --leak-check=full --show-reachable=yes --tool=memcheck --track-origins=yes \
valgrind -v --log-file=/tmp/valgrind.log --tool=callgrind \
../../src/so -r 0 -u Cmdenv -c $config $ini_file -n ../:../../src:../../../inet/src
